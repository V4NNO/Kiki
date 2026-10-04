#include "historyrecorder.h"

#include "vp8codec.h"

#include <QBuffer>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMap>
#include <QSqlError>
#include <QSqlQuery>

#include <algorithm>

namespace {
constexpr auto kConnectionName = "kiki_history";

// Force a VP8 keyframe this often within a sequence, so seeking to a frame
// only has to decode at most this many frames forward from a keyframe.
constexpr int kVideoKeyInterval = 10;

// More than this without any metadata push from a session (the subservice
// pushes at least every 10s) and the session counts as not reporting: the
// online range is closed and a new one opens on the next push.
constexpr qint64 kOnlineGapMs = 35 * 1000;

// "Idle HH:MM:SS" / "Locked HH:MM:SS" (activityprobe.cpp) -> kind + how long
// the state has lasted, in ms. Empty kind for anything else (active).
QPair<QString, qint64> parseIdleText(const QString &idleText)
{
    const QString trimmed = idleText.trimmed();
    QString kind;
    QString rest;
    if (trimmed.startsWith(QStringLiteral("Locked "))) {
        kind = QStringLiteral("lock");
        rest = trimmed.mid(7);
    } else if (trimmed.startsWith(QStringLiteral("Idle "))) {
        kind = QStringLiteral("idle");
        rest = trimmed.mid(5);
    } else {
        return {};
    }
    const QStringList parts = rest.split(QLatin1Char(':'));
    if (parts.size() != 3) {
        return {kind, 0};
    }
    const qint64 seconds = parts.at(0).toLongLong() * 3600 + parts.at(1).toLongLong() * 60
                           + parts.at(2).toLongLong();
    return {kind, qMax<qint64>(0, seconds) * 1000};
}

// FUN_14077f300: the chart granula (ms) -> the dpc level table it reads
// ("1m"/"5m"/"15m"/"1h", as seconds) and the bucket length S (seconds)
// obs_aligned_point() aligns to.
QPair<qint64, qint64> nodeLevelAndStep(qint64 granulaMs)
{
    if (granulaMs < 300000) {
        return {60, 60};
    }
    if (granulaMs < 600000) {
        return {300, 300};
    }
    if (granulaMs < 900000) {
        return {300, 600};
    }
    if (granulaMs < 1200000) {
        return {900, 900};
    }
    if (granulaMs < 1800000) {
        return {300, 1200};
    }
    if (granulaMs < 3600000) {
        return {900, 1800};
    }
    if (granulaMs < 7200000) {
        return {3600, 3600};
    }
    if (granulaMs < 14400000) {
        return {3600, 7200};
    }
    if (granulaMs < 28800000) {
        return {3600, 14400};
    }
    if (granulaMs < 43200000) {
        return {3600, 28800};
    }
    if (granulaMs < 86400000) {
        return {3600, 43200};
    }
    return {3600, 86400};
}

// obs_aligned_point(entity, granulaSec) = entity - MOD(EPOCH(entity),
// granulaSec) seconds (node.exe's embedded schema): aligned on the epoch,
// node timestamps being UTC -- not on local midnight.
qint64 alignedPoint(qint64 ms, qint64 stepMs)
{
    qint64 rem = ms % stepMs;
    if (rem < 0) {
        rem += stepMs;
    }
    return ms - rem;
}

qint64 alignUp(qint64 ms, qint64 stepMs)
{
    const qint64 down = alignedPoint(ms, stepMs);
    return down == ms ? ms : down + stepMs;
}

using Ranges = QList<QPair<qint64, qint64>>;

// Sorted, non-overlapping union of ranges.
Ranges mergeRanges(Ranges ranges)
{
    std::sort(ranges.begin(), ranges.end());
    Ranges merged;
    for (const auto &range : std::as_const(ranges)) {
        if (range.second <= range.first) {
            continue;
        }
        if (!merged.isEmpty() && range.first <= merged.last().second) {
            merged.last().second = qMax(merged.last().second, range.second);
        } else {
            merged.append(range);
        }
    }
    return merged;
}

// a \ b for merged range lists (FUN_14085eae0's set difference).
Ranges subtractRanges(const Ranges &a, const Ranges &b)
{
    Ranges result;
    int j = 0;
    for (const auto &range : a) {
        qint64 from = range.first;
        while (j < b.size() && b.at(j).second <= from) {
            ++j;
        }
        int k = j;
        while (k < b.size() && b.at(k).first < range.second) {
            if (b.at(k).first > from) {
                result.append({from, b.at(k).first});
            }
            from = qMax(from, b.at(k).second);
            ++k;
        }
        if (from < range.second) {
            result.append({from, range.second});
        }
    }
    return result;
}

// a n b for merged range lists.
Ranges intersectRanges(const Ranges &a, const Ranges &b)
{
    Ranges result;
    int i = 0;
    int j = 0;
    while (i < a.size() && j < b.size()) {
        const qint64 from = qMax(a.at(i).first, b.at(j).first);
        const qint64 to = qMin(a.at(i).second, b.at(j).second);
        if (from < to) {
            result.append({from, to});
        }
        if (a.at(i).second < b.at(j).second) {
            ++i;
        } else {
            ++j;
        }
    }
    return result;
}

// Adds every piece of [from, to) to the S-aligned bucket it falls in.
template <typename Add>
void forEachBucketPiece(qint64 from, qint64 to, qint64 stepMs, Add add)
{
    while (from < to) {
        const qint64 bucket = alignedPoint(from, stepMs);
        const qint64 pieceEnd = qMin(to, bucket + stepMs);
        add(bucket, pieceEnd - from);
        from = pieceEnd;
    }
}

// employee_to_application_rating.rating: 1 productive, 2 neutral,
// 3 nonProductive, 0 (or no rule) none.
int ratingIndex(const QString &category)
{
    if (category == QStringLiteral("productive")) {
        return 1;
    }
    if (category == QStringLiteral("neutral")) {
        return 2;
    }
    if (category == QStringLiteral("unproductive")) {
        return 3;
    }
    return 0;
}
}

// The VP8 sequence currently being written for one monitor stream. The
// encoder keeps libvpx's inter-frame state, so frames after the keyframe are
// coded as deltas against their predecessor.
struct HistoryRecorder::OpenVideoSequence {
    qint64 sequenceId = -1;
    std::unique_ptr<Vp8Encoder> encoder;
    int width = 0;
    int height = 0;
    qint64 lastMs = 0;
    int frameCount = 0;
};

// A warm decoder for reads: remembers which sequence it's in and the
// timestamp it has decoded up to, so consecutive forward reads only feed the
// new delta frames.
struct HistoryRecorder::VideoDecodeCache {
    qint64 sequenceId = -1;
    qint64 lastMs = -1;
    std::unique_ptr<Vp8Decoder> decoder;
};

HistoryRecorder::HistoryRecorder(QString historyDir, QObject *parent)
    : QObject(parent)
    , m_historyDir(std::move(historyDir))
{
}

HistoryRecorder::~HistoryRecorder()
{
    // Close open sequences and decoders (free the libvpx contexts) before DB.
    m_openVideo.clear();
    m_decodeCache.clear();
    if (m_database.isOpen()) {
        m_database.close();
    }
    m_database = QSqlDatabase();
    QSqlDatabase::removeDatabase(QString::fromLatin1(kConnectionName));
}

void HistoryRecorder::setHistoryFps(double framesPerSecond)
{
    if (framesPerSecond > 0.0) {
        m_minIntervalMs = qMax(1, static_cast<int>(1000.0 / framesPerSecond + 0.5));
    }
}

bool HistoryRecorder::start(QString *error)
{
    if (!QDir().mkpath(m_historyDir)) {
        if (error) {
            *error = QStringLiteral("Nu pot crea directorul de istoric: %1").arg(m_historyDir);
        }
        return false;
    }

    m_database = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QString::fromLatin1(kConnectionName));
    m_database.setDatabaseName(QDir(m_historyDir).filePath(QStringLiteral("history.sqlite")));
    if (!m_database.open()) {
        if (error) {
            *error = QStringLiteral("Nu pot deschide history.sqlite: %1")
                         .arg(m_database.lastError().text());
        }
        return false;
    }

    QSqlQuery query(m_database);
    const bool created = query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS frames ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  session_username TEXT NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"
        "  monitor_name TEXT NOT NULL,"
        "  timestamp_ms INTEGER NOT NULL,"
        "  file_path TEXT NOT NULL,"
        "  width INTEGER NOT NULL,"
        "  height INTEGER NOT NULL"
        ")"));
    if (!created) {
        if (error) {
            *error = QStringLiteral("Nu pot crea tabela frames: %1").arg(query.lastError().text());
        }
        return false;
    }
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_frames_monitor_time ON frames(monitor_stream_id, timestamp_ms)"));

    // History video, stored the way the original Kickidler node does
    // (video_sequence / video_frame): a sequence is a continuous recording
    // run for one monitor that opens with a VP8 keyframe, each later frame
    // coded as a delta against the previous one; frames are BLOBs, not files
    // (thousands of tiny delta frames as separate files would waste the FS).
    // The legacy `frames` table (JPEG files on disk) is kept read-only so
    // history recorded before this change still views.
    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS video_sequence ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_username TEXT NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"
        "  monitor_name TEXT NOT NULL,"
        "  width INTEGER NOT NULL,"
        "  height INTEGER NOT NULL,"
        "  begin_ms INTEGER NOT NULL,"
        "  end_ms INTEGER NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_video_sequence_monitor_time "
        "ON video_sequence(monitor_stream_id, begin_ms)"));
    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS video_frame ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  sequence_id INTEGER NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"  // denormalized for range queries
        "  timestamp_ms INTEGER NOT NULL,"
        "  is_key INTEGER NOT NULL,"
        "  data BLOB NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_video_frame_monitor_time "
        "ON video_frame(monitor_stream_id, timestamp_ms)"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_video_frame_seq ON video_frame(sequence_id, timestamp_ms)"));

    // Input activity is a property of the whole session (the employee), not
    // of a single monitor -- the real Kickidler node stores it in
    // online_session_input_volume / online_session_idle keyed only by
    // online_session_id, never by a display. Keying it by session_username
    // keeps it independent of how many monitors are plugged in (or which one
    // the viewer happens to look at first). The legacy session_id/
    // monitor_stream_id columns stay for old databases but are written as 0.
    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS activity ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"
        "  timestamp_ms INTEGER NOT NULL,"
        "  input_count INTEGER NOT NULL,"
        "  session_username TEXT NOT NULL DEFAULT ''"
        ")"));
    // Migration for databases created before activity was session-scoped
    // (fails harmlessly when the column already exists).
    query.exec(QStringLiteral("ALTER TABLE activity ADD COLUMN session_username TEXT NOT NULL DEFAULT ''"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_activity_user_time ON activity(session_username, timestamp_ms)"));

    // Like activity, the foreground application is a property of the session
    // (the employee), not of a display -- the real node stores it in
    // online_session_program keyed only by online_session_id. Keyed by
    // session_username so Efficiency is independent of the monitor count /
    // which monitor the viewer looks at first. Legacy session_id/
    // monitor_stream_id columns stay for old databases but are written as 0.
    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS app_segments ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"
        "  application TEXT NOT NULL,"
        "  start_ms INTEGER NOT NULL,"
        "  end_ms INTEGER NOT NULL,"
        "  title TEXT NOT NULL DEFAULT '',"
        "  session_username TEXT NOT NULL DEFAULT ''"
        ")"));
    // Migrations for databases created before app_segments carried a window
    // title / was session-scoped (fail harmlessly when the column exists).
    query.exec(QStringLiteral("ALTER TABLE app_segments ADD COLUMN title TEXT NOT NULL DEFAULT ''"));
    query.exec(QStringLiteral("ALTER TABLE app_segments ADD COLUMN session_username TEXT NOT NULL DEFAULT ''"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_app_segments_user_time "
        "ON app_segments(session_username, start_ms)"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS web_visits ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"
        "  url TEXT NOT NULL,"
        "  start_ms INTEGER NOT NULL,"
        "  end_ms INTEGER NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_web_visits_monitor_time "
        "ON web_visits(monitor_stream_id, start_ms)"));

    // The node's online_session life (minus online_session_nodata) and its
    // online_session_lock / online_session_idle ranges, session-scoped by the
    // employee username: dpc volume_total is |online \ nodata| and
    // volume_activity |online \ nodata \ (lock u saver u idle)| per minute
    // (nodeDpc.cpp, FUN_140853600 / FUN_140856ee0 / FUN_140856e10).
    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS session_online ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_username TEXT NOT NULL,"
        "  begin_ms INTEGER NOT NULL,"
        "  end_ms INTEGER NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_session_online_user_time "
        "ON session_online(session_username, begin_ms)"));
    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS session_inactive ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_username TEXT NOT NULL,"
        "  kind TEXT NOT NULL,"
        "  begin_ms INTEGER NOT NULL,"
        "  end_ms INTEGER NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_session_inactive_user_time "
        "ON session_inactive(session_username, begin_ms)"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS keystrokes ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  timestamp_ms INTEGER NOT NULL,"
        "  window_title TEXT NOT NULL,"
        "  text TEXT NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_keystrokes_session_time ON keystrokes(session_id, timestamp_ms)"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS app_categories ("
        "  application TEXT PRIMARY KEY,"
        "  category TEXT NOT NULL"
        ")"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS app_categories_user ("
        "  username TEXT NOT NULL,"
        "  application TEXT NOT NULL,"
        "  category TEXT NOT NULL,"
        "  PRIMARY KEY (username, application)"
        ")"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS stream_ids ("
        "  username TEXT NOT NULL,"
        "  monitor_name TEXT NOT NULL,"
        "  stream_id INTEGER NOT NULL UNIQUE,"
        "  PRIMARY KEY (username, monitor_name)"
        ")"));

    emit logMessage(QStringLiteral("Istoric activat (VP8), ~%1 cadre/s per monitor, in %2")
                        .arg(1000.0 / m_minIntervalMs, 0, 'g', 3)
                        .arg(m_historyDir));
    return true;
}

quint32 HistoryRecorder::stableStreamId(const QString &username, const QString &monitorName)
{
    if (!m_database.isOpen()) {
        return 0;
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT stream_id FROM stream_ids WHERE username = ? AND monitor_name = ?"));
    query.addBindValue(username);
    query.addBindValue(monitorName);
    if (query.exec() && query.next()) {
        return query.value(0).toUInt();
    }

    // First time this monitor is seen since stream_ids existed: adopt the id
    // its most recent recorded history used, so that history stays visible,
    // unless another monitor already claimed it.
    quint32 streamId = 0;
    QSqlQuery previous(m_database);
    previous.prepare(QStringLiteral(
        "SELECT monitor_stream_id FROM frames WHERE session_username = ? AND monitor_name = ? "
        "AND monitor_stream_id NOT IN (SELECT stream_id FROM stream_ids) "
        "ORDER BY timestamp_ms DESC LIMIT 1"));
    previous.addBindValue(username);
    previous.addBindValue(monitorName);
    if (previous.exec() && previous.next()) {
        streamId = previous.value(0).toUInt();
    }
    if (streamId == 0) {
        QSqlQuery next(m_database);
        next.exec(QStringLiteral(
            "SELECT MAX(COALESCE((SELECT MAX(stream_id) FROM stream_ids), 0),"
            " COALESCE((SELECT MAX(monitor_stream_id) FROM frames), 0)) + 1"));
        streamId = next.next() ? next.value(0).toUInt() : 1;
    }

    QSqlQuery insert(m_database);
    insert.prepare(QStringLiteral(
        "INSERT INTO stream_ids (username, monitor_name, stream_id) VALUES (?, ?, ?)"));
    insert.addBindValue(username);
    insert.addBindValue(monitorName);
    insert.addBindValue(streamId);
    if (!insert.exec()) {
        emit logMessage(QStringLiteral("Nu am putut salva id-ul stabil pentru %1/%2: %3")
                            .arg(username, monitorName, insert.lastError().text()));
        return 0;
    }
    return streamId;
}

void HistoryRecorder::recordFrame(quint32 sessionId, const QString &sessionUsername,
                                  quint32 monitorStreamId, const QString &monitorName,
                                  const QImage &image)
{
    if (image.isNull()) {
        return;
    }
    Q_UNUSED(sessionId);
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 &lastRecorded = m_lastRecordedMs[monitorStreamId];
    if (lastRecorded != 0 && now - lastRecorded < m_minIntervalMs) {
        return;
    }

    const int w = image.width() & ~1; // VP8 needs even dimensions
    const int h = image.height() & ~1;
    if (w <= 0 || h <= 0) {
        return;
    }
    // Start a new sequence when nothing is open, the resolution changed, or
    // there's been a long gap (which, since DXGI only emits on change, is
    // normal) -- a new sequence just means the next frame is a keyframe.
    const qint64 seqGapMs = qMax<qint64>(static_cast<qint64>(m_minIntervalMs) * 5, 60 * 1000);
    std::unique_ptr<OpenVideoSequence> &slot = m_openVideo[monitorStreamId];
    OpenVideoSequence *seq = slot.get();
    const bool needNew = !seq || !seq->encoder || !seq->encoder->isOpen() || seq->width != w
                         || seq->height != h || (now - seq->lastMs) > seqGapMs;
    if (needNew) {
        QSqlQuery insertSeq(m_database);
        insertSeq.prepare(QStringLiteral(
            "INSERT INTO video_sequence (session_username, monitor_stream_id, monitor_name, "
            "width, height, begin_ms, end_ms) VALUES (?, ?, ?, ?, ?, ?, ?)"));
        insertSeq.addBindValue(sessionUsername);
        insertSeq.addBindValue(monitorStreamId);
        insertSeq.addBindValue(monitorName);
        insertSeq.addBindValue(w);
        insertSeq.addBindValue(h);
        insertSeq.addBindValue(now);
        insertSeq.addBindValue(now);
        if (!insertSeq.exec()) {
            emit logMessage(QStringLiteral("Nu am putut deschide secventa video: %1")
                                .arg(insertSeq.lastError().text()));
            return;
        }
        auto fresh = std::make_unique<OpenVideoSequence>();
        fresh->sequenceId = insertSeq.lastInsertId().toLongLong();
        fresh->encoder = std::make_unique<Vp8Encoder>();
        if (!fresh->encoder->begin(w, h)) {
            emit logMessage(QStringLiteral("Nu am putut porni encoderul VP8 (%1x%2).").arg(w).arg(h));
            return;
        }
        fresh->width = w;
        fresh->height = h;
        fresh->frameCount = 0;
        slot = std::move(fresh);
        seq = slot.get();
    }

    // The sequence's first frame, and then one every kVideoKeyInterval, is a
    // keyframe -- bounds how far a seek must decode forward.
    const bool forceKey = (seq->frameCount % kVideoKeyInterval) == 0;
    bool isKey = false;
    const QByteArray data = seq->encoder->encode(image, forceKey, &isKey);
    if (data.isEmpty()) {
        emit logMessage(QStringLiteral("Encodarea VP8 a esuat pentru monitorul %1.").arg(monitorStreamId));
        return;
    }

    QSqlQuery insertFrame(m_database);
    insertFrame.prepare(QStringLiteral(
        "INSERT INTO video_frame (sequence_id, monitor_stream_id, timestamp_ms, is_key, data) "
        "VALUES (?, ?, ?, ?, ?)"));
    insertFrame.addBindValue(seq->sequenceId);
    insertFrame.addBindValue(monitorStreamId);
    insertFrame.addBindValue(now);
    insertFrame.addBindValue(isKey ? 1 : 0);
    insertFrame.addBindValue(data);
    if (!insertFrame.exec()) {
        emit logMessage(QStringLiteral("Nu am putut scrie cadrul video: %1")
                            .arg(insertFrame.lastError().text()));
        return;
    }
    QSqlQuery updateSeq(m_database);
    updateSeq.prepare(QStringLiteral("UPDATE video_sequence SET end_ms = ? WHERE id = ?"));
    updateSeq.addBindValue(now);
    updateSeq.addBindValue(seq->sequenceId);
    updateSeq.exec();

    lastRecorded = now;
    seq->lastMs = now;
    ++seq->frameCount;
}

QStringList HistoryRecorder::listDays(quint32 monitorStreamId) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    // 'localtime' matches how the day folders themselves were named:
    // recordFrame() derives them from QDateTime::fromMSecsSinceEpoch(),
    // which is local time by default.
    // Union of the VP8 video frames and the legacy JPEG frames table, so a
    // database that has both (recorded before and after the VP8 switch) lists
    // every day.
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT DISTINCT day FROM ("
        "  SELECT strftime('%Y%m%d', timestamp_ms / 1000, 'unixepoch', 'localtime') AS day "
        "  FROM video_frame WHERE monitor_stream_id = ?"
        "  UNION "
        "  SELECT strftime('%Y%m%d', timestamp_ms / 1000, 'unixepoch', 'localtime') AS day "
        "  FROM frames WHERE monitor_stream_id = ?"
        ") ORDER BY day DESC"));
    query.addBindValue(monitorStreamId);
    query.addBindValue(monitorStreamId);
    QStringList days;
    if (query.exec()) {
        while (query.next()) {
            days.append(query.value(0).toString());
        }
    }
    return days;
}

QPair<qint64, qint64> HistoryRecorder::dayBounds(const QString &day)
{
    // Local midnight to the next local midnight, so a DST day is 23/25h.
    const QDate date = QDate::fromString(day, QStringLiteral("yyyyMMdd"));
    if (!date.isValid()) {
        return {0, 0};
    }
    return {QDateTime(date, QTime(0, 0)).toMSecsSinceEpoch(),
            QDateTime(date.addDays(1), QTime(0, 0)).toMSecsSinceEpoch()};
}

QList<qint64> HistoryRecorder::listFrameTimestamps(quint32 monitorStreamId, qint64 startMs,
                                                   qint64 stopMs) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT timestamp_ms FROM ("
        "  SELECT timestamp_ms FROM video_frame WHERE monitor_stream_id = ? "
        "    AND timestamp_ms >= ? AND timestamp_ms < ?"
        "  UNION "
        "  SELECT timestamp_ms FROM frames WHERE monitor_stream_id = ? "
        "    AND timestamp_ms >= ? AND timestamp_ms < ?"
        ") ORDER BY timestamp_ms ASC"));
    query.addBindValue(monitorStreamId);
    query.addBindValue(startMs);
    query.addBindValue(stopMs);
    query.addBindValue(monitorStreamId);
    query.addBindValue(startMs);
    query.addBindValue(stopMs);
    QList<qint64> timestamps;
    if (query.exec()) {
        while (query.next()) {
            timestamps.append(query.value(0).toLongLong());
        }
    }
    return timestamps;
}

QByteArray HistoryRecorder::readFrame(quint32 monitorStreamId, qint64 timestampMs)
{
    if (!m_database.isOpen()) {
        return {};
    }
    // Each monitor is recorded on its own clock (see recordFrame), so the
    // viewer asking every screen of a device for monitor 1's timestamps
    // almost never hits an exact match on the others. Serve what that
    // screen showed at that moment instead: its latest frame at or before
    // timestampMs, else the first one after -- within kMaxDistanceMs, so a
    // screen that wasn't being recorded then stays empty rather than
    // showing something from hours away.
    constexpr qint64 kMaxDistanceMs = 10 * 60 * 1000;

    // VP8 first: find the nearest stored frame, then decode its sequence from
    // the last keyframe at/before it, and hand the viewer a plain JPEG -- so
    // the wire protocol and the viewer stay unchanged.
    qint64 targetMs = -1;
    qint64 sequenceId = -1;
    QSqlQuery frame(m_database);
    frame.prepare(QStringLiteral(
        "SELECT timestamp_ms, sequence_id FROM video_frame WHERE monitor_stream_id = ? "
        "AND timestamp_ms <= ? AND timestamp_ms >= ? ORDER BY timestamp_ms DESC LIMIT 1"));
    frame.addBindValue(monitorStreamId);
    frame.addBindValue(timestampMs);
    frame.addBindValue(timestampMs - kMaxDistanceMs);
    if (frame.exec() && frame.next()) {
        targetMs = frame.value(0).toLongLong();
        sequenceId = frame.value(1).toLongLong();
    } else {
        QSqlQuery after(m_database);
        after.prepare(QStringLiteral(
            "SELECT timestamp_ms, sequence_id FROM video_frame WHERE monitor_stream_id = ? "
            "AND timestamp_ms > ? AND timestamp_ms <= ? ORDER BY timestamp_ms ASC LIMIT 1"));
        after.addBindValue(monitorStreamId);
        after.addBindValue(timestampMs);
        after.addBindValue(timestampMs + kMaxDistanceMs);
        if (after.exec() && after.next()) {
            targetMs = after.value(0).toLongLong();
            sequenceId = after.value(1).toLongLong();
        }
    }

    if (sequenceId >= 0) {
        std::unique_ptr<VideoDecodeCache> &cachePtr = m_decodeCache[monitorStreamId];
        if (!cachePtr) {
            cachePtr = std::make_unique<VideoDecodeCache>();
        }
        VideoDecodeCache &cache = *cachePtr;

        // Fast path: same sequence, stepping forward -- feed only the frames
        // after what the warm decoder has already seen.
        qint64 fromMs = 0;
        const bool warm = cache.decoder && cache.decoder->isOpen()
                          && cache.sequenceId == sequenceId && targetMs > cache.lastMs;
        if (warm) {
            fromMs = cache.lastMs; // exclusive lower bound below
        } else {
            // Cold path: (re)start from the keyframe at/before the target.
            qint64 keyMs = targetMs;
            QSqlQuery key(m_database);
            key.prepare(QStringLiteral(
                "SELECT MAX(timestamp_ms) FROM video_frame WHERE sequence_id = ? AND is_key = 1 "
                "AND timestamp_ms <= ?"));
            key.addBindValue(sequenceId);
            key.addBindValue(targetMs);
            if (key.exec() && key.next() && !key.value(0).isNull()) {
                keyMs = key.value(0).toLongLong();
            }
            if (!cache.decoder) {
                cache.decoder = std::make_unique<Vp8Decoder>();
            }
            if (!cache.decoder->begin()) {
                return {};
            }
            cache.sequenceId = sequenceId;
            fromMs = keyMs - 1; // include the keyframe (>= keyMs)
        }

        QSqlQuery run(m_database);
        run.prepare(QStringLiteral(
            "SELECT timestamp_ms, data FROM video_frame WHERE sequence_id = ? AND timestamp_ms > ? "
            "AND timestamp_ms <= ? ORDER BY timestamp_ms ASC"));
        run.addBindValue(sequenceId);
        run.addBindValue(fromMs);
        run.addBindValue(targetMs);
        QImage decoded;
        if (run.exec()) {
            while (run.next()) {
                decoded = cache.decoder->decode(run.value(1).toByteArray());
                cache.lastMs = run.value(0).toLongLong();
            }
        }
        if (!decoded.isNull()) {
            QByteArray jpeg;
            QBuffer buffer(&jpeg);
            buffer.open(QIODevice::WriteOnly);
            if (decoded.save(&buffer, "JPEG", 80)) {
                return jpeg;
            }
        }
        // Decode hiccup: drop the warm state so the next read restarts clean.
        cache.sequenceId = -1;
        cache.lastMs = -1;
        return {};
    }

    // Fallback: legacy JPEG frames recorded before the VP8 switch.
    QString relativePath;
    QSqlQuery before(m_database);
    before.prepare(QStringLiteral(
        "SELECT file_path FROM frames WHERE monitor_stream_id = ? AND timestamp_ms <= ? "
        "AND timestamp_ms >= ? ORDER BY timestamp_ms DESC LIMIT 1"));
    before.addBindValue(monitorStreamId);
    before.addBindValue(timestampMs);
    before.addBindValue(timestampMs - kMaxDistanceMs);
    if (before.exec() && before.next()) {
        relativePath = before.value(0).toString();
    } else {
        QSqlQuery after(m_database);
        after.prepare(QStringLiteral(
            "SELECT file_path FROM frames WHERE monitor_stream_id = ? AND timestamp_ms > ? "
            "AND timestamp_ms <= ? ORDER BY timestamp_ms ASC LIMIT 1"));
        after.addBindValue(monitorStreamId);
        after.addBindValue(timestampMs);
        after.addBindValue(timestampMs + kMaxDistanceMs);
        if (!after.exec() || !after.next()) {
            return {};
        }
        relativePath = after.value(0).toString();
    }
    QFile file(QDir(m_historyDir).filePath(relativePath));
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    return file.readAll();
}

void HistoryRecorder::recordActivity(const QString &sessionUsername, int inputEvents)
{
    if (!m_database.isOpen()) {
        return;
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO activity (session_id, monitor_stream_id, timestamp_ms, input_count, session_username) "
        "VALUES (0, 0, ?, ?, ?)"));
    query.addBindValue(QDateTime::currentMSecsSinceEpoch());
    query.addBindValue(inputEvents);
    query.addBindValue(sessionUsername);
    if (!query.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut scrie randul de activitate: %1").arg(query.lastError().text()));
    }
}

void HistoryRecorder::noteApplication(quint32 sessionId, const QString &sessionUsername,
                                      const QString &application, const QString &title)
{
    if (!m_database.isOpen() || application.isEmpty()) {
        return;
    }
    // One open segment per session, not per monitor: the subservice sends the
    // same foreground app for every monitor each tick, so keying by session
    // collapses those into a single extend instead of one segment per screen.
    const quint64 key = sessionId;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    auto it = m_openSegments.find(key);

    if (it != m_openSegments.end() && it->application == application) {
        // Same app still in front: extend the segment and keep its title
        // current (the window title changes while the app stays the same).
        QSqlQuery update(m_database);
        update.prepare(QStringLiteral("UPDATE app_segments SET end_ms = ?, title = ? WHERE id = ?"));
        update.addBindValue(now);
        update.addBindValue(title);
        update.addBindValue(it->rowId);
        update.exec();
        return;
    }

    QSqlQuery insert(m_database);
    insert.prepare(QStringLiteral(
        "INSERT INTO app_segments (session_id, monitor_stream_id, application, start_ms, end_ms, title, session_username) "
        "VALUES (0, 0, ?, ?, ?, ?, ?)"));
    insert.addBindValue(application);
    insert.addBindValue(now);
    insert.addBindValue(now);
    insert.addBindValue(title);
    insert.addBindValue(sessionUsername);
    if (!insert.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut scrie segmentul de aplicatie: %1").arg(insert.lastError().text()));
        return;
    }
    m_openSegments[key] = OpenSegment{application, insert.lastInsertId().toLongLong()};
}

void HistoryRecorder::noteUrl(quint32 sessionId, quint32 monitorStreamId, const QString &url)
{
    if (!m_database.isOpen()) {
        return;
    }
    const quint64 key = (static_cast<quint64>(sessionId) << 32) | monitorStreamId;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    auto it = m_openWebSegments.find(key);

    if (url.isEmpty()) {
        // Not on a (recognized) browser right now -- just close whatever
        // was open, if anything. No blank row for "not browsing".
        if (it != m_openWebSegments.end()) {
            QSqlQuery update(m_database);
            update.prepare(QStringLiteral("UPDATE web_visits SET end_ms = ? WHERE id = ?"));
            update.addBindValue(now);
            update.addBindValue(it->rowId);
            update.exec();
            m_openWebSegments.erase(it);
        }
        return;
    }

    if (it != m_openWebSegments.end() && it->url == url) {
        QSqlQuery update(m_database);
        update.prepare(QStringLiteral("UPDATE web_visits SET end_ms = ? WHERE id = ?"));
        update.addBindValue(now);
        update.addBindValue(it->rowId);
        update.exec();
        return;
    }

    QSqlQuery insert(m_database);
    insert.prepare(QStringLiteral(
        "INSERT INTO web_visits (session_id, monitor_stream_id, url, start_ms, end_ms) "
        "VALUES (?, ?, ?, ?, ?)"));
    insert.addBindValue(sessionId);
    insert.addBindValue(monitorStreamId);
    insert.addBindValue(url);
    insert.addBindValue(now);
    insert.addBindValue(now);
    if (!insert.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut scrie vizita web: %1").arg(insert.lastError().text()));
        return;
    }
    m_openWebSegments[key] = OpenWebSegment{url, insert.lastInsertId().toLongLong()};
}

void HistoryRecorder::noteSessionState(const QString &sessionUsername, const QString &idleText,
                                       double idleSeconds, bool screensaver)
{
    if (!m_database.isOpen() || sessionUsername.isEmpty()) {
        return;
    }
    const qint64 now = QDateTime::currentMSecsSinceEpoch();

    // Online: one range while pushes keep coming; a longer silence is "no
    // data" and splits it (online_session minus online_session_nodata).
    auto online = m_openOnline.find(sessionUsername);
    if (online != m_openOnline.end() && now - online->endMs <= kOnlineGapMs) {
        if (now > online->endMs) {
            QSqlQuery update(m_database);
            update.prepare(QStringLiteral("UPDATE session_online SET end_ms = ? WHERE id = ?"));
            update.addBindValue(now);
            update.addBindValue(online->rowId);
            update.exec();
            online->endMs = now;
        }
    } else {
        QSqlQuery insert(m_database);
        insert.prepare(QStringLiteral(
            "INSERT INTO session_online (session_username, begin_ms, end_ms) VALUES (?, ?, ?)"));
        insert.addBindValue(sessionUsername);
        insert.addBindValue(now);
        insert.addBindValue(now);
        if (!insert.exec()) {
            emit logMessage(QStringLiteral("Nu am putut scrie intervalul online: %1")
                                .arg(insert.lastError().text()));
            return;
        }
        m_openOnline[sessionUsername] = OpenRange{insert.lastInsertId().toLongLong(), now, now};
        // Whatever inactive ranges were open belonged to the previous online
        // range; they ended with it.
        for (const QString &kind :
             {QStringLiteral("lock"), QStringLiteral("idle"), QStringLiteral("saver")}) {
            m_openInactive.remove(sessionUsername + QLatin1Char('\x1f') + kind);
        }
    }

    // The three independent inactive kinds, each its own range (they overlap
    // freely; volume_activity subtracts their union). lock comes from the
    // "Locked HH:MM:SS" text (dated back by its elapsed seconds); idle from
    // the raw GetLastInputInfo seconds, threshold 1s, dated back like the
    // node (FUN_1402f7960); saver from the screensaver flag (no backdating
    // info, so it starts when first seen).
    const auto [textKind, textElapsedMs] = parseIdleText(idleText);
    const bool locked = textKind == QStringLiteral("lock");
    updateInactiveRange(sessionUsername, QStringLiteral("lock"), locked, now - textElapsedMs, now);
    updateInactiveRange(sessionUsername, QStringLiteral("idle"),
                        !locked && idleSeconds >= 1.0,
                        now - static_cast<qint64>(idleSeconds * 1000.0), now);
    updateInactiveRange(sessionUsername, QStringLiteral("saver"), !locked && screensaver, now, now);
}

void HistoryRecorder::updateInactiveRange(const QString &username, const QString &kind, bool active,
                                          qint64 wantBeginMs, qint64 nowMs)
{
    const QString key = username + QLatin1Char('\x1f') + kind;
    auto open = m_openInactive.find(key);
    if (!active) {
        m_openInactive.remove(key);
        return;
    }
    if (open != m_openInactive.end()) {
        if (nowMs > open->endMs) {
            QSqlQuery update(m_database);
            update.prepare(QStringLiteral("UPDATE session_inactive SET end_ms = ? WHERE id = ?"));
            update.addBindValue(nowMs);
            update.addBindValue(open->rowId);
            update.exec();
            open->endMs = nowMs;
        }
        return;
    }
    // Opening a new range: never reach before the current online range begins.
    qint64 beginMs = qMin(wantBeginMs, nowMs);
    const OpenRange online = m_openOnline.value(username);
    beginMs = qMax(beginMs, online.beginMs);
    QSqlQuery insert(m_database);
    insert.prepare(QStringLiteral(
        "INSERT INTO session_inactive (session_username, kind, begin_ms, end_ms) VALUES (?, ?, ?, ?)"));
    insert.addBindValue(username);
    insert.addBindValue(kind);
    insert.addBindValue(beginMs);
    insert.addBindValue(nowMs);
    if (!insert.exec()) {
        emit logMessage(QStringLiteral("Nu am putut scrie intervalul inactiv: %1")
                            .arg(insert.lastError().text()));
        return;
    }
    m_openInactive[key] = OpenRange{insert.lastInsertId().toLongLong(), beginMs, nowMs};
}

void HistoryRecorder::recordKeystroke(quint32 sessionId, const QString &windowTitle,
                                      const QString &text)
{
    if (!m_database.isOpen() || text.isEmpty()) {
        return;
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO keystrokes (session_id, timestamp_ms, window_title, text) VALUES (?, ?, ?, ?)"));
    query.addBindValue(sessionId);
    query.addBindValue(QDateTime::currentMSecsSinceEpoch());
    query.addBindValue(windowTitle);
    query.addBindValue(text);
    if (!query.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut scrie textul tastat: %1").arg(query.lastError().text()));
    }
}

void HistoryRecorder::setCategory(const QString &application, const QString &category)
{
    if (!m_database.isOpen() || application.isEmpty()) {
        return;
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO app_categories (application, category) VALUES (?, ?) "
        "ON CONFLICT(application) DO UPDATE SET category = excluded.category"));
    query.addBindValue(application);
    query.addBindValue(category);
    if (!query.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut salva categoria: %1").arg(query.lastError().text()));
    }
}

void HistoryRecorder::setEmployeeCategory(const QString &username, const QString &application,
                                          const QString &category)
{
    if (!m_database.isOpen() || username.isEmpty() || application.isEmpty()) {
        return;
    }
    QSqlQuery query(m_database);
    if (category == QStringLiteral("none")) {
        query.prepare(QStringLiteral(
            "DELETE FROM app_categories_user WHERE username = ? AND application = ?"));
        query.addBindValue(username);
        query.addBindValue(application);
    } else {
        query.prepare(QStringLiteral(
            "INSERT INTO app_categories_user (username, application, category) VALUES (?, ?, ?) "
            "ON CONFLICT(username, application) DO UPDATE SET category = excluded.category"));
        query.addBindValue(username);
        query.addBindValue(application);
        query.addBindValue(category);
    }
    if (!query.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut salva categoria angajatului: %1").arg(query.lastError().text()));
    }
}

QList<QPair<QString, QString>> HistoryRecorder::listEmployeeCategories(const QString &username) const
{
    QList<QPair<QString, QString>> categories;
    if (!m_database.isOpen() || username.isEmpty()) {
        return categories;
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT application, category FROM app_categories_user WHERE username = ?"));
    query.addBindValue(username);
    if (query.exec()) {
        while (query.next()) {
            categories.append({query.value(0).toString(), query.value(1).toString()});
        }
    }
    return categories;
}

QString HistoryRecorder::usernameForStream(quint32 streamId) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral("SELECT username FROM stream_ids WHERE stream_id = ?"));
    query.addBindValue(streamId);
    return query.exec() && query.next() ? query.value(0).toString() : QString();
}

QString HistoryRecorder::category(const QString &application) const
{
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral("SELECT category FROM app_categories WHERE application = ?"));
    query.addBindValue(application);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    // Apps with no saved category are uncategorized ("none"), not "neutral" --
    // "none" is the uncategorized token the whole system uses (white text, no
    // efficiency band), so new/unknown apps stay uncategorized until the user
    // rates them.
    return QStringLiteral("none");
}

QList<ActivitySample> HistoryRecorder::listActivity(quint32 monitorStreamId, qint64 startMs,
                                                   qint64 stopMs) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    // Activity is session-scoped (see recordActivity): resolve whatever
    // monitor stream the viewer asked for to its session's employee, then
    // return that whole session's activity. Any monitor of the device --
    // even one whose screen has since been unplugged -- resolves to the same
    // username, so the chart no longer depends on which monitor is "first".
    const QString username = usernameForStream(monitorStreamId);
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT timestamp_ms, input_count FROM activity WHERE session_username = ? "
        "AND timestamp_ms >= ? AND timestamp_ms < ? ORDER BY timestamp_ms ASC"));
    query.addBindValue(username);
    query.addBindValue(startMs);
    query.addBindValue(stopMs);
    QList<ActivitySample> samples;
    if (query.exec()) {
        while (query.next()) {
            samples.append(ActivitySample{query.value(0).toLongLong(), query.value(1).toInt()});
        }
    }
    return samples;
}

QList<AppSegment> HistoryRecorder::listAppSegments(quint32 monitorStreamId, qint64 startMs,
                                                   qint64 stopMs) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    // Session-scoped like activity: resolve the requested monitor stream to
    // its employee and return the whole session's segments, so Efficiency no
    // longer depends on which monitor is asked for (or is still plugged in).
    // Every segment overlapping [startMs, stopMs) -- one that began before
    // midnight still belongs to the next day for the part it lasted into it.
    const QString username = usernameForStream(monitorStreamId);
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT application, start_ms, end_ms, title FROM app_segments WHERE session_username = ? "
        "AND start_ms < ? AND end_ms > ? ORDER BY start_ms ASC"));
    query.addBindValue(username);
    query.addBindValue(stopMs);
    query.addBindValue(startMs);
    QList<AppSegment> segments;
    if (query.exec()) {
        while (query.next()) {
            segments.append(AppSegment{query.value(0).toString(), query.value(1).toLongLong(),
                                       query.value(2).toLongLong(), query.value(3).toString()});
        }
    }
    return segments;
}

QList<AppUsage> HistoryRecorder::listRunningApplications(quint32 monitorStreamId, qint64 startMs,
                                                          qint64 stopMs) const
{
    QHash<QString, qint64> totals;
    QHash<QString, QString> latestTitle;
    // Segments come back oldest-first, so the last non-empty title seen for
    // an app is its most recent window title.
    for (const AppSegment &segment : listAppSegments(monitorStreamId, startMs, stopMs)) {
        totals[segment.application] +=
            qMax<qint64>(0, qMin(segment.endMs, stopMs) - qMax(segment.startMs, startMs));
        if (!segment.title.isEmpty()) {
            latestTitle[segment.application] = segment.title;
        }
    }
    QList<AppUsage> usage;
    for (auto it = totals.cbegin(); it != totals.cend(); ++it) {
        usage.append(AppUsage{it.key(), it.value(), category(it.key()), latestTitle.value(it.key())});
    }
    std::sort(usage.begin(), usage.end(),
             [](const AppUsage &a, const AppUsage &b) { return a.totalMs > b.totalMs; });
    return usage;
}

QList<WebVisit> HistoryRecorder::listWebVisits(quint32 monitorStreamId, qint64 startMs,
                                               qint64 stopMs) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT url, start_ms, end_ms FROM web_visits WHERE monitor_stream_id = ? "
        "AND start_ms < ? AND end_ms > ? ORDER BY start_ms ASC"));
    query.addBindValue(monitorStreamId);
    query.addBindValue(stopMs);
    query.addBindValue(startMs);
    QList<WebVisit> visits;
    if (query.exec()) {
        while (query.next()) {
            visits.append(WebVisit{query.value(0).toString(), query.value(1).toLongLong(),
                                   query.value(2).toLongLong()});
        }
    }
    return visits;
}

QList<WebUsage> HistoryRecorder::listWebPages(quint32 monitorStreamId, qint64 startMs,
                                              qint64 stopMs) const
{
    QHash<QString, qint64> totals;
    for (const WebVisit &visit : listWebVisits(monitorStreamId, startMs, stopMs)) {
        totals[visit.url] += qMax<qint64>(0, qMin(visit.endMs, stopMs) - qMax(visit.startMs, startMs));
    }
    QList<WebUsage> usage;
    for (auto it = totals.cbegin(); it != totals.cend(); ++it) {
        usage.append(WebUsage{it.key(), it.value()});
    }
    std::sort(usage.begin(), usage.end(),
             [](const WebUsage &a, const WebUsage &b) { return a.totalMs > b.totalMs; });
    return usage;
}

QList<QPair<QString, QString>> HistoryRecorder::listCategories() const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    QList<QPair<QString, QString>> categories;
    if (query.exec(QStringLiteral("SELECT application, category FROM app_categories"))) {
        while (query.next()) {
            categories.append({query.value(0).toString(), query.value(1).toString()});
        }
    }
    return categories;
}

QList<KeystrokeEntry> HistoryRecorder::listKeystrokes(quint32 sessionId, qint64 startMs,
                                                      qint64 stopMs) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT timestamp_ms, window_title, text FROM keystrokes WHERE session_id = ? "
        "AND timestamp_ms >= ? AND timestamp_ms < ? ORDER BY timestamp_ms ASC"));
    query.addBindValue(sessionId);
    query.addBindValue(startMs);
    query.addBindValue(stopMs);
    QList<KeystrokeEntry> entries;
    if (query.exec()) {
        while (query.next()) {
            entries.append(KeystrokeEntry{query.value(0).toLongLong(), query.value(1).toString(),
                                          query.value(2).toString()});
        }
    }
    return entries;
}

QList<QPair<qint64, qint64>> HistoryRecorder::loadRanges(const QString &table, const QString &username,
                                                         qint64 startMs, qint64 stopMs) const
{
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral("SELECT begin_ms, end_ms FROM %1 WHERE session_username = ? "
                                 "AND begin_ms < ? AND end_ms > ?")
                      .arg(table));
    query.addBindValue(username);
    query.addBindValue(stopMs);
    query.addBindValue(startMs);
    Ranges ranges;
    if (query.exec()) {
        while (query.next()) {
            ranges.append({qMax(startMs, query.value(0).toLongLong()),
                           qMin(stopMs, query.value(1).toLongLong())});
        }
    }
    return mergeRanges(ranges);
}

// Selector K_activity, node.exe FUN_140781c40:
//   SELECT obs_aligned_point(session.point, S) AS point,
//          sum(session.volume_activity)/S AS volume, <session marker>
//   FROM dpc.session_<level> AS session ...
//   WHERE session.point >= :start AND session.point < :stop
//   GROUP BY point, <marker> ORDER BY point
// with (level, S) from the granula (FUN_14077f300). A dpc row exists for
// every level point the session was online in; its volume_activity is the
// online time there minus lock/saver/idle.
QList<ChartPoint> HistoryRecorder::chartActivity(quint32 monitorStreamId, qint64 startMs,
                                                 qint64 stopMs, qint64 granulaMs) const
{
    const QString username = usernameForStream(monitorStreamId);
    if (!m_database.isOpen() || username.isEmpty() || stopMs <= startMs || granulaMs <= 0) {
        return {};
    }
    const auto [levelSec, stepSec] = nodeLevelAndStep(granulaMs);
    const qint64 stepMs = stepSec * 1000;
    // A level point L passes "L >= start AND L < stop" exactly when the time
    // it stands for lies in [alignUp(start), alignUp(stop)).
    const qint64 from = alignUp(startMs, levelSec * 1000);
    const qint64 to = alignUp(stopMs, levelSec * 1000);
    const Ranges online = loadRanges(QStringLiteral("session_online"), username, from, to);
    const Ranges active =
        subtractRanges(online, loadRanges(QStringLiteral("session_inactive"), username, from, to));

    QMap<qint64, qint64> onlineMs;
    QMap<qint64, qint64> activeMs;
    for (const auto &range : online) {
        forEachBucketPiece(range.first, range.second, stepMs,
                           [&](qint64 bucket, qint64 ms) { onlineMs[bucket] += ms; });
    }
    for (const auto &range : active) {
        forEachBucketPiece(range.first, range.second, stepMs,
                           [&](qint64 bucket, qint64 ms) { activeMs[bucket] += ms; });
    }
    QList<ChartPoint> points;
    for (auto it = onlineMs.cbegin(); it != onlineMs.cend(); ++it) {
        ChartPoint point;
        point.pointMs = it.key();
        point.value = activeMs.value(it.key()) / 1000.0 / stepSec;
        points.append(point);
    }
    return points;
}

// Selector K_byProductivity, node.exe FUN_1407838d0:
//   SELECT obs_aligned_point(session.point, S) AS point,
//     sum(CASE WHEN COALESCE(rating_s.rating, rating_p.rating, 0)=0
//         THEN program.volume_total ELSE 0 END) AS volume0, ... volume1..3
//   FROM dpc.program_<level> AS program JOIN dpc.session_<level> ...
//   LEFT JOIN rating_p ON executable_mask = program.executable
//   LEFT JOIN rating_s ON site_mask = program.site
// program.volume_total is the foreground time of (executable, site) minus
// nodata -- idle time included, it is not volume_activity.
QList<ChartPoint> HistoryRecorder::chartProductivity(quint32 monitorStreamId, qint64 startMs,
                                                     qint64 stopMs, qint64 granulaMs) const
{
    const QString username = usernameForStream(monitorStreamId);
    if (!m_database.isOpen() || username.isEmpty() || stopMs <= startMs || granulaMs <= 0) {
        return {};
    }
    const auto [levelSec, stepSec] = nodeLevelAndStep(granulaMs);
    const qint64 stepMs = stepSec * 1000;
    const qint64 from = alignUp(startMs, levelSec * 1000);
    const qint64 to = alignUp(stopMs, levelSec * 1000);
    const Ranges online = loadRanges(QStringLiteral("session_online"), username, from, to);

    // The employee's rating rule for an executable or site: their own
    // override, else the global one; none when neither exists.
    QHash<QString, QString> rules;
    for (const auto &entry : listCategories()) {
        rules.insert(entry.first, entry.second);
    }
    for (const auto &entry : listEmployeeCategories(username)) {
        rules.insert(entry.first, entry.second);
    }
    const auto hasRule = [&rules](const QString &key) {
        return !key.isEmpty() && rules.contains(key) && rules.value(key) != QStringLiteral("none");
    };

    // Sites the employee was on (every monitor's copy of a visit merges).
    QHash<QString, Ranges> siteRanges;
    {
        QSqlQuery query(m_database);
        query.prepare(QStringLiteral(
            "SELECT url, start_ms, end_ms FROM web_visits WHERE monitor_stream_id IN "
            "(SELECT stream_id FROM stream_ids WHERE username = ?) AND start_ms < ? AND end_ms > ?"));
        query.addBindValue(username);
        query.addBindValue(to);
        query.addBindValue(from);
        if (query.exec()) {
            while (query.next()) {
                siteRanges[query.value(0).toString()].append(
                    {qMax(from, query.value(1).toLongLong()), qMin(to, query.value(2).toLongLong())});
            }
        }
        for (auto it = siteRanges.begin(); it != siteRanges.end(); ++it) {
            it.value() = mergeRanges(it.value());
        }
    }

    QMap<qint64, ChartPoint> buckets;
    const auto addVolume = [&](const Ranges &ranges, int rating) {
        for (const auto &range : ranges) {
            forEachBucketPiece(range.first, range.second, stepMs, [&](qint64 bucket, qint64 ms) {
                ChartPoint &point = buckets[bucket];
                point.pointMs = bucket;
                point.volumes[rating] += ms / 1000.0;
            });
        }
    };

    QSqlQuery segments(m_database);
    segments.prepare(QStringLiteral(
        "SELECT application, start_ms, end_ms FROM app_segments WHERE session_username = ? "
        "AND application != '' AND start_ms < ? AND end_ms > ?"));
    segments.addBindValue(username);
    segments.addBindValue(to);
    segments.addBindValue(from);
    if (!segments.exec()) {
        return {};
    }
    while (segments.next()) {
        const QString executable = segments.value(0).toString();
        const Ranges segment = {{qMax(from, segments.value(1).toLongLong()),
                                 qMin(to, segments.value(2).toLongLong())}};
        Ranges remaining = intersectRanges(mergeRanges(segment), online);
        const int programRating = hasRule(executable) ? ratingIndex(rules.value(executable)) : 0;
        // COALESCE(rating_s.rating, rating_p.rating, 0): while on a site, its
        // rule decides; the executable's rule only when the site has none.
        for (auto it = siteRanges.cbegin(); it != siteRanges.cend() && !remaining.isEmpty(); ++it) {
            const Ranges onSite = intersectRanges(remaining, it.value());
            if (onSite.isEmpty()) {
                continue;
            }
            addVolume(onSite, hasRule(it.key()) ? ratingIndex(rules.value(it.key())) : programRating);
            remaining = subtractRanges(remaining, it.value());
        }
        addVolume(remaining, programRating);
    }
    return buckets.values();
}
