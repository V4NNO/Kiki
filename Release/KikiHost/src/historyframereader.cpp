#include "historyframereader.h"

#include "frameprotocol.h"
#include "vp8codec.h"

#include <QBuffer>
#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QImage>
#include <QSqlError>
#include <QSqlQuery>
#include <QThread>

namespace {
// A screen that was not being recorded around the asked-for moment stays
// empty rather than showing something from hours away (same rule the
// recorder's own reader used).
constexpr qint64 kMaxDistanceMs = 10 * 60 * 1000;

// Re-visiting a position the viewer has already been to (scrubbing back and
// forth over the same stretch) should cost nothing; 48 MB of JPEGs is a few
// hundred frames.
constexpr qint64 kJpegCacheBudgetBytes = 48 * 1024 * 1024;

// How much of a run one getSegment answer may carry. At ~9 KB per delta
// frame these bounds are a few seconds of screen each -- small enough that
// the first chunk lands quickly and the slider starts filling, large enough
// that a scrub stays inside one chunk most of the time.
constexpr int kMaxSegmentFrames = 300;
constexpr qint64 kMaxSegmentBytes = 4 * 1024 * 1024;

quint64 nextReaderIndex = 0;
} // namespace

HistoryFrameReader::HistoryFrameReader(QString historyDir, QObject *parent)
    : QObject(parent)
    , m_historyDir(std::move(historyDir))
    , m_connectionName(QStringLiteral("kiki_history_read_%1").arg(++nextReaderIndex))
{
}

HistoryFrameReader::~HistoryFrameReader()
{
    close();
}

void HistoryFrameReader::open()
{
    if (m_database.isOpen()) {
        return;
    }
    m_database = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), m_connectionName);
    m_database.setDatabaseName(QDir(m_historyDir).filePath(QStringLiteral("history.sqlite")));
    // Read-only: this connection must never take a write lock away from the
    // recorder. busy_timeout covers the moments the writer is checkpointing.
    m_database.setConnectOptions(QStringLiteral("QSQLITE_OPEN_READONLY;QSQLITE_BUSY_TIMEOUT=5000"));
    if (!m_database.open()) {
        return;
    }
}

void HistoryFrameReader::close()
{
    m_decodeCache.clear();
    m_jpegCache.clear();
    m_jpegOrder.clear();
    m_jpegCacheBytes = 0;
    if (m_database.isOpen()) {
        m_database.close();
    }
    m_database = QSqlDatabase();
    QSqlDatabase::removeDatabase(m_connectionName);
}

QByteArray HistoryFrameReader::cachedJpeg(quint32 monitorStreamId, qint64 storedMs) const
{
    return m_jpegCache.value(qMakePair(monitorStreamId, storedMs));
}

void HistoryFrameReader::cacheJpeg(quint32 monitorStreamId, qint64 storedMs, const QByteArray &jpeg)
{
    const QPair<quint32, qint64> key(monitorStreamId, storedMs);
    if (m_jpegCache.contains(key)) {
        return;
    }
    m_jpegCache.insert(key, jpeg);
    m_jpegOrder.append(key);
    m_jpegCacheBytes += jpeg.size();
    while (m_jpegCacheBytes > kJpegCacheBudgetBytes && !m_jpegOrder.isEmpty()) {
        const QPair<quint32, qint64> oldest = m_jpegOrder.takeFirst();
        m_jpegCacheBytes -= m_jpegCache.take(oldest).size();
    }
}

void HistoryFrameReader::readFrame(quint64 connectionId, quint32 monitorStreamId,
                                   qint64 timestampMs, quint64 requestId)
{
    QElapsedTimer timer;
    timer.start();
    const QByteArray jpeg = read(monitorStreamId, timestampMs);
    emit frameRead(connectionId, monitorStreamId, timestampMs, requestId, jpeg, timer.elapsed());
}


void HistoryFrameReader::readSegment(quint64 connectionId, quint32 monitorStreamId,
                                     qint64 sequenceId, qint64 fromMs, qint64 toMs,
                                     quint64 requestId)
{
    QElapsedTimer timer;
    timer.start();
    ViewerProtocol::HistorySegmentPayload segment;
    segment.sequenceId = sequenceId;

    if (!m_database.isOpen()) {
        open();
    }
    if (!m_database.isOpen() || toMs <= fromMs) {
        emit segmentRead(connectionId, monitorStreamId, requestId, {}, timer.elapsed());
        return;
    }

    // Resolve the run if the viewer did not name one: the first frame at or
    // after fromMs decides which sequence this chunk belongs to.
    if (sequenceId <= 0) {
        QSqlQuery find(m_database);
        find.prepare(QStringLiteral(
            "SELECT sequence_id FROM video_frame WHERE monitor_stream_id = ? "
            "AND timestamp_ms >= ? AND timestamp_ms < ? ORDER BY timestamp_ms ASC LIMIT 1"));
        find.addBindValue(monitorStreamId);
        find.addBindValue(fromMs);
        find.addBindValue(toMs);
        if (!find.exec() || !find.next()) {
            emit segmentRead(connectionId, monitorStreamId, requestId, {}, timer.elapsed());
            return;
        }
        sequenceId = find.value(0).toLongLong();
        segment.sequenceId = sequenceId;
    }

    QSqlQuery size(m_database);
    size.prepare(QStringLiteral("SELECT width, height FROM video_sequence WHERE id = ?"));
    size.addBindValue(sequenceId);
    if (size.exec() && size.next()) {
        segment.width = quint16(size.value(0).toInt());
        segment.height = quint16(size.value(1).toInt());
    }

    // Start at the keyframe at or before fromMs, so the chunk decodes on its
    // own without the viewer needing anything that came before it.
    qint64 startMs = fromMs;
    QSqlQuery key(m_database);
    key.prepare(QStringLiteral(
        "SELECT MAX(timestamp_ms) FROM video_frame WHERE sequence_id = ? AND is_key = 1 "
        "AND timestamp_ms <= ?"));
    key.addBindValue(sequenceId);
    key.addBindValue(fromMs);
    if (key.exec() && key.next() && !key.value(0).isNull()) {
        startMs = key.value(0).toLongLong();
    }

    QSqlQuery run(m_database);
    run.prepare(QStringLiteral(
        "SELECT timestamp_ms, is_key, data FROM video_frame WHERE sequence_id = ? "
        "AND timestamp_ms >= ? AND timestamp_ms < ? ORDER BY timestamp_ms ASC"));
    run.addBindValue(sequenceId);
    run.addBindValue(startMs);
    run.addBindValue(toMs);
    qint64 bytes = 0;
    qint64 lastMs = startMs;
    if (run.exec()) {
        while (run.next()) {
            if (segment.frames.size() >= kMaxSegmentFrames || bytes >= kMaxSegmentBytes) {
                segment.hasMore = true;
                break;
            }
            ViewerProtocol::SegmentFrame frame;
            frame.timestampMs = run.value(0).toLongLong();
            frame.isKey = run.value(1).toInt() != 0;
            frame.data = run.value(2).toByteArray();
            bytes += frame.data.size();
            lastMs = frame.timestampMs;
            segment.frames.append(frame);
        }
    }
    Q_UNUSED(lastMs)

    const QByteArray payload = segment.frames.isEmpty()
        ? QByteArray()
        : ViewerProtocol::encodeHistorySegment(segment);
    emit segmentRead(connectionId, monitorStreamId, requestId, payload, timer.elapsed());
}

QByteArray HistoryFrameReader::read(quint32 monitorStreamId, qint64 timestampMs)
{
    if (!m_database.isOpen()) {
        open();
    }
    if (!m_database.isOpen()) {
        return {};
    }
    // Each monitor is recorded on its own clock, so the viewer asking every
    // screen of a device for monitor 1's timestamps almost never hits an
    // exact match on the others: serve what that screen showed at that
    // moment -- its latest frame at or before timestampMs, else the first
    // one after it.
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

    if (sequenceId < 0) {
        return readLegacyJpeg(monitorStreamId, timestampMs);
    }

    // Already decoded once (scrubbing back over ground we have seen).
    const QByteArray cached = cachedJpeg(monitorStreamId, targetMs);
    if (!cached.isEmpty()) {
        return cached;
    }

    std::unique_ptr<DecodeCache> &cachePtr = m_decodeCache[monitorStreamId];
    if (!cachePtr) {
        cachePtr = std::make_unique<DecodeCache>();
    }
    DecodeCache &cache = *cachePtr;

    // Fast path: same sequence, stepping forward -- feed only the frames the
    // warm decoder has not seen yet.
    qint64 fromMs = 0;
    const bool warm = cache.decoder && cache.decoder->isOpen() && cache.sequenceId == sequenceId
                      && targetMs > cache.lastMs;
    if (warm) {
        fromMs = cache.lastMs; // exclusive lower bound below
    } else {
        // Cold path (a backward or far-away seek): restart from the keyframe
        // at or before the target, so at most one keyframe interval of
        // deltas is replayed.
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
            cacheJpeg(monitorStreamId, targetMs, jpeg);
            return jpeg;
        }
    }
    // Decode hiccup: drop the warm state so the next read restarts clean.
    cache.sequenceId = -1;
    cache.lastMs = -1;
    return {};
}

QByteArray HistoryFrameReader::readLegacyJpeg(quint32 monitorStreamId, qint64 timestampMs)
{
    // History recorded before the VP8 switch: JPEG files on disk.
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

HistoryFrameService::HistoryFrameService(const QString &historyDir, QObject *parent)
    : QObject(parent)
    , m_reader(new HistoryFrameReader(historyDir))
{
    m_reader->moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, m_reader, &HistoryFrameReader::open);
    connect(&m_thread, &QThread::finished, m_reader, &QObject::deleteLater);
    connect(m_reader, &HistoryFrameReader::frameRead, this, &HistoryFrameService::frameRead);
    connect(m_reader, &HistoryFrameReader::segmentRead, this, &HistoryFrameService::segmentRead);
    m_thread.setObjectName(QStringLiteral("KikiHistoryFrames"));
    m_thread.start();
}

HistoryFrameService::~HistoryFrameService()
{
    QMetaObject::invokeMethod(m_reader, &HistoryFrameReader::close, Qt::BlockingQueuedConnection);
    m_thread.quit();
    m_thread.wait();
}

void HistoryFrameService::request(quint64 connectionId, quint32 monitorStreamId, qint64 timestampMs,
                                  quint64 requestId)
{
    QMetaObject::invokeMethod(m_reader, "readFrame", Qt::QueuedConnection,
                              Q_ARG(quint64, connectionId), Q_ARG(quint32, monitorStreamId),
                              Q_ARG(qint64, timestampMs), Q_ARG(quint64, requestId));
}

void HistoryFrameService::requestSegment(quint64 connectionId, quint32 monitorStreamId,
                                         qint64 sequenceId, qint64 fromMs, qint64 toMs,
                                         quint64 requestId)
{
    QMetaObject::invokeMethod(m_reader, "readSegment", Qt::QueuedConnection,
                              Q_ARG(quint64, connectionId), Q_ARG(quint32, monitorStreamId),
                              Q_ARG(qint64, sequenceId), Q_ARG(qint64, fromMs),
                              Q_ARG(qint64, toMs), Q_ARG(quint64, requestId));
}
