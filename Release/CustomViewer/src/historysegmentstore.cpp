#include "historysegmentstore.h"

#include "vp8decoder.h"

#include <QCryptographicHash>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QUuid>
#include <QVariant>

#include <algorithm>

HistorySegmentStore::HistorySegmentStore() = default;

HistorySegmentStore::~HistorySegmentStore()
{
    close();
}

void HistorySegmentStore::clear()
{
    m_streams.clear();
    m_bytes = 0;
}

QString HistorySegmentStore::cacheDirectory()
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
        + QStringLiteral("/historyCache");
}

QString HistorySegmentStore::cacheFileForTag(const QString &tag)
{
    const QByteArray hash = QCryptographicHash::hash(tag.toUtf8(), QCryptographicHash::Sha1).toHex().left(12);
    return cacheDirectory() + QStringLiteral("/vhdb.") + QString::fromLatin1(hash) + QStringLiteral(".sqlite");
}

bool HistorySegmentStore::open(const QString &databasePath)
{
    close();
    clear();
    const QString name = QStringLiteral("historyCache-") + QUuid::createUuid().toString(QUuid::WithoutBraces);
    {
        QSqlDatabase database = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), name);
        database.setDatabaseName(databasePath);
        // The export reads the same file from its own thread meanwhile.
        database.setConnectOptions(QStringLiteral("QSQLITE_BUSY_TIMEOUT=2000"));
        if (!database.open()) {
            database = QSqlDatabase();
            QSqlDatabase::removeDatabase(name);
            return false;
        }
        QSqlQuery query(database);
        // FUN_14025f120: synchronous=OFF, journal_mode=OFF -- a cache, not
        // a record; a torn file is simply downloaded again.
        query.exec(QStringLiteral("PRAGMA synchronous=OFF"));
        query.exec(QStringLiteral("PRAGMA journal_mode=OFF"));
        query.exec(QStringLiteral(
            "CREATE TABLE IF NOT EXISTS segment (id INTEGER PRIMARY KEY, stream INTEGER NOT NULL, "
            "sequence INTEGER NOT NULL, first_ms INTEGER NOT NULL, last_ms INTEGER NOT NULL, "
            "data BLOB NOT NULL)"));
        query.exec(QStringLiteral("CREATE INDEX IF NOT EXISTS segment_stream_first ON segment(stream, first_ms)"));
        // Only whole runs are ever stored (one reply = one complete run), so
        // everything here is usable as is.
        if (query.exec(QStringLiteral(
                "SELECT id, stream, sequence, first_ms, last_ms, length(data) FROM segment ORDER BY stream, first_ms"))) {
            while (query.next()) {
                Chunk chunk;
                chunk.rowId = query.value(0).toLongLong();
                chunk.sequenceId = query.value(2).toLongLong();
                chunk.firstMs = query.value(3).toLongLong();
                chunk.lastMs = query.value(4).toLongLong();
                Stream &stream = m_streams[query.value(1).toUInt()];
                const int held = chunkAt(stream.chunks, chunk.firstMs);
                if (held < 0 || stream.chunks.at(held).lastMs < chunk.lastMs) {
                    const auto at = std::lower_bound(
                        stream.chunks.cbegin(), stream.chunks.cend(), chunk.firstMs,
                        [](const Chunk &existing, qint64 value) { return existing.firstMs < value; });
                    stream.chunks.insert(int(at - stream.chunks.cbegin()), std::move(chunk));
                }
            }
        }
    }
    m_connectionName = name;
    return true;
}

void HistorySegmentStore::close()
{
    if (m_connectionName.isEmpty()) {
        return;
    }
    clear();
    {
        QSqlDatabase database = QSqlDatabase::database(m_connectionName, false);
        database.close();
    }
    QSqlDatabase::removeDatabase(m_connectionName);
    m_connectionName.clear();
}

bool HistorySegmentStore::ensureResident(Chunk &chunk)
{
    if (!chunk.frames.isEmpty()) {
        return true;
    }
    if (chunk.rowId == 0 || m_connectionName.isEmpty()) {
        return false;
    }
    QSqlQuery query(QSqlDatabase::database(m_connectionName, false));
    query.prepare(QStringLiteral("SELECT data FROM segment WHERE id = ?"));
    query.addBindValue(chunk.rowId);
    ViewerProtocol::HistorySegmentPayload payload;
    if (!query.exec() || !query.next()
        || !ViewerProtocol::decodeHistorySegment(query.value(0).toByteArray(), &payload)
        || payload.frames.isEmpty()) {
        return false;
    }
    chunk.frames = payload.frames;
    std::sort(chunk.frames.begin(), chunk.frames.end(),
              [](const ViewerProtocol::SegmentFrame &a, const ViewerProtocol::SegmentFrame &b) {
                  return a.timestampMs < b.timestampMs;
              });
    chunk.bytes = 0;
    for (const ViewerProtocol::SegmentFrame &frame : std::as_const(chunk.frames)) {
        chunk.bytes += frame.data.size();
    }
    m_bytes += chunk.bytes;
    return true;
}

int HistorySegmentStore::chunkAt(const QList<Chunk> &chunks, qint64 timestampMs)
{
    // Chunks are kept sorted and non-overlapping, so this is a binary search.
    const auto after = std::upper_bound(
        chunks.cbegin(), chunks.cend(), timestampMs,
        [](qint64 value, const Chunk &chunk) { return value < chunk.firstMs; });
    if (after == chunks.cbegin()) {
        return -1;
    }
    // Chunks may overlap by a keyframe interval (a getSegment answer starts
    // at the keyframe at or before the gap it was asked for): the latest one
    // holding the moment wins, an earlier one still counts.
    for (int index = int(after - chunks.cbegin()) - 1, tries = 0; index >= 0 && tries < 3; --index, ++tries) {
        if (timestampMs <= chunks.at(index).lastMs) {
            return index;
        }
    }
    return -1;
}

int HistorySegmentStore::frameAtOrBefore(const Chunk &chunk, qint64 timestampMs)
{
    const auto after = std::upper_bound(
        chunk.frames.cbegin(), chunk.frames.cend(), timestampMs,
        [](qint64 value, const ViewerProtocol::SegmentFrame &frame) {
            return value < frame.timestampMs;
        });
    return after == chunk.frames.cbegin() ? -1 : int(after - chunk.frames.cbegin()) - 1;
}

bool HistorySegmentStore::addSegment(quint32 streamId,
                                     const ViewerProtocol::HistorySegmentPayload &payload)
{
    if (payload.frames.isEmpty()) {
        return false;
    }
    Chunk chunk;
    chunk.sequenceId = payload.sequenceId;
    chunk.frames = payload.frames;
    std::sort(chunk.frames.begin(), chunk.frames.end(),
              [](const ViewerProtocol::SegmentFrame &a, const ViewerProtocol::SegmentFrame &b) {
                  return a.timestampMs < b.timestampMs;
              });
    // A run only decodes standalone if it opens with a keyframe; the host
    // always starts one there, but never trust the wire.
    if (!chunk.frames.first().isKey) {
        return false;
    }
    chunk.firstMs = chunk.frames.first().timestampMs;
    chunk.lastMs = chunk.frames.last().timestampMs;
    for (const ViewerProtocol::SegmentFrame &frame : std::as_const(chunk.frames)) {
        chunk.bytes += frame.data.size();
    }

    Stream &stream = m_streams[streamId];
    // Already held whole (a request that raced another for the same
    // stretch) -- keep the one we have, its decoder may be warm on it.
    {
        const int held = chunkAt(stream.chunks, chunk.firstMs);
        if (held >= 0 && stream.chunks.at(held).lastMs >= chunk.lastMs) {
            return false;
        }
    }
    const auto at = std::lower_bound(
        stream.chunks.cbegin(), stream.chunks.cend(), chunk.firstMs,
        [](const Chunk &existing, qint64 value) { return existing.firstMs < value; });
    const int index = int(at - stream.chunks.cbegin());
    // Inserting before an existing chunk invalidates the warm decoder's
    // index into the list.
    if (stream.warm.chunkIndex >= index) {
        stream.warm.chunkIndex = -1;
        stream.warm.frameIndex = -1;
    }
    if (!m_connectionName.isEmpty()) {
        // Stored as it arrived (FUN_140259110 binds the packet unchanged).
        QSqlQuery insert(QSqlDatabase::database(m_connectionName, false));
        insert.prepare(QStringLiteral(
            "INSERT INTO segment (stream, sequence, first_ms, last_ms, data) VALUES (?, ?, ?, ?, ?)"));
        insert.addBindValue(streamId);
        insert.addBindValue(chunk.sequenceId);
        insert.addBindValue(chunk.firstMs);
        insert.addBindValue(chunk.lastMs);
        insert.addBindValue(ViewerProtocol::encodeHistorySegment(payload));
        if (insert.exec()) {
            chunk.rowId = insert.lastInsertId().toLongLong();
        }
    }
    m_bytes += chunk.bytes;
    stream.chunks.insert(index, std::move(chunk));
    return true;
}

bool HistorySegmentStore::covers(quint32 streamId, qint64 timestampMs) const
{
    const auto it = m_streams.find(streamId);
    return it != m_streams.end() && chunkAt(it->second.chunks, timestampMs) >= 0;
}

qint64 HistorySegmentStore::frameTimeAt(quint32 streamId, qint64 timestampMs) const
{
    const auto it = m_streams.find(streamId);
    if (it == m_streams.end()) {
        return -1;
    }
    const int index = chunkAt(it->second.chunks, timestampMs);
    if (index < 0) {
        return -1;
    }
    const Chunk &chunk = it->second.chunks.at(index);
    if (chunk.frames.isEmpty()) {
        return timestampMs; // paged out: some frame at or before it exists
    }
    const int frame = frameAtOrBefore(chunk, timestampMs);
    return frame < 0 ? -1 : chunk.frames.at(frame).timestampMs;
}

int HistorySegmentStore::keyAtOrBefore(const Chunk &chunk, int frameIndex)
{
    for (int i = frameIndex; i > 0; --i) {
        if (chunk.frames.at(i).isKey) {
            return i;
        }
    }
    return 0;
}

QImage HistorySegmentStore::frameAt(quint32 streamId, qint64 timestampMs)
{
    const auto it = m_streams.find(streamId);
    if (it == m_streams.end()) {
        return {};
    }
    Stream &stream = it->second;
    const int index = chunkAt(stream.chunks, timestampMs);
    if (index < 0) {
        return {};
    }
    if (!ensureResident(stream.chunks[index])) {
        return {};
    }
    const Chunk &chunk = stream.chunks.at(index);
    const int target = frameAtOrBefore(chunk, timestampMs);
    if (target < 0) {
        return {};
    }
    Warm &warm = stream.warm;
    // Exactly where we already are: hand back the image we decoded last.
    if (warm.chunkIndex == index && warm.frameIndex == target && !warm.image.isNull()) {
        return warm.image;
    }
    if (!warm.decoder) {
        warm.decoder = std::make_unique<Vp8Decoder>();
    }
    if (!warm.decoder->isOpen() && !warm.decoder->begin()) {
        warm.chunkIndex = -1;
        warm.frameIndex = -1;
        return {};
    }

    // Where decoding has to start. A keyframe resets every reference buffer,
    // so the latest one at or before the target is always a valid starting
    // point -- at most one keyframe interval (10 frames) to replay, whatever
    // the direction or the distance of the seek. Stepping forward inside the
    // same keyframe interval is cheaper still: only the new deltas.
    const int key = keyAtOrBefore(chunk, target);
    const bool canStepForward = warm.chunkIndex == index && warm.frameIndex >= key
                                && target > warm.frameIndex;
    const int from = canStepForward ? warm.frameIndex + 1 : key;

    // Feed everything up to the target, but convert only the target itself
    // to RGB: the intermediate frames are never shown, and a full-screen
    // colour conversion per frame was most of a drag's cost.
    warm.chunkIndex = index;
    for (int i = from; i <= target; ++i) {
        if (!warm.decoder->feed(chunk.frames.at(i).data)) {
            // Decode hiccup: drop the warm state so the next look restarts
            // clean from a keyframe.
            warm.chunkIndex = -1;
            warm.frameIndex = -1;
            warm.image = QImage();
            return {};
        }
        warm.frameIndex = i;
    }
    warm.image = warm.decoder->image();
    return warm.image;
}

QList<QPair<qint64, qint64>> HistorySegmentStore::spans(quint32 streamId) const
{
    QList<QPair<qint64, qint64>> out;
    const auto it = m_streams.find(streamId);
    if (it == m_streams.end()) {
        return out;
    }
    for (const Chunk &chunk : it->second.chunks) {
        out.append({chunk.firstMs, chunk.lastMs});
    }
    return out;
}

bool HistorySegmentStore::nextGap(quint32 streamId, qint64 fromMs, qint64 toMs, qint64 aroundMs,
                                  qint64 *gapStartMs) const
{
    if (toMs <= fromMs) {
        return false;
    }
    const auto it = m_streams.find(streamId);
    const QList<Chunk> empty;
    const QList<Chunk> &chunks = it == m_streams.end() ? empty : it->second.chunks;

    // Search outward from where the user is, so what they are about to scrub
    // through is downloaded before the far ends of the period.
    const auto firstGapFrom = [&](qint64 start) -> qint64 {
        qint64 at = qBound(fromMs, start, toMs - 1);
        while (at < toMs) {
            const int index = chunkAt(chunks, at);
            if (index < 0) {
                return at;
            }
            at = chunks.at(index).lastMs + 1;
        }
        return -1;
    };
    const qint64 ahead = firstGapFrom(qBound(fromMs, aroundMs, toMs - 1));
    if (ahead >= 0) {
        *gapStartMs = ahead;
        return true;
    }
    // Everything after the cursor is held; fill in behind it.
    const qint64 behind = firstGapFrom(fromMs);
    if (behind >= 0) {
        *gapStartMs = behind;
        return true;
    }
    return false;
}

void HistorySegmentStore::trim(qint64 aroundMs)
{
    const bool pageOut = !m_connectionName.isEmpty();
    while (m_bytes > m_budgetBytes) {
        quint32 worstStream = 0;
        int worstIndex = -1;
        qint64 worstDistance = -1;
        for (auto it = m_streams.begin(); it != m_streams.end(); ++it) {
            const QList<Chunk> &chunks = it->second.chunks;
            for (int i = 0; i < chunks.size(); ++i) {
                const Chunk &chunk = chunks.at(i);
                if (chunk.frames.isEmpty()) {
                    continue; // already paged out
                }
                const qint64 distance = aroundMs < chunk.firstMs ? chunk.firstMs - aroundMs
                    : aroundMs > chunk.lastMs                    ? aroundMs - chunk.lastMs
                                                                 : 0;
                if (distance > worstDistance) {
                    worstDistance = distance;
                    worstStream = it->first;
                    worstIndex = i;
                }
            }
        }
        if (worstIndex < 0) {
            return;
        }
        Stream &stream = m_streams[worstStream];
        Chunk &worst = stream.chunks[worstIndex];
        m_bytes -= worst.bytes;
        if (pageOut && worst.rowId != 0) {
            // Still downloaded (and blue): only its frames leave memory.
            worst.frames.clear();
            worst.frames.squeeze();
            worst.bytes = 0;
        } else {
            stream.chunks.removeAt(worstIndex);
        }
        // The warm decoder's index into the list just shifted.
        stream.warm.chunkIndex = -1;
        stream.warm.frameIndex = -1;
        stream.warm.image = QImage();
    }
}
