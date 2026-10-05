#pragma once

#include <QByteArray>
#include <QHash>
#include <QList>
#include <QObject>
#include <QPair>
#include <QSqlDatabase>
#include <QString>
#include <QThread>

#include <memory>
#include <unordered_map>

class Vp8Decoder;

// Reads (and VP8-decodes) one history frame per request on a worker thread,
// so a seek never blocks the host's event loop -- the live PSV1 streams, the
// heartbeat and every other viewer keep running while a far-away seek walks
// a sequence back to its keyframe.
//
// It owns its own read-only SQLite connection to history.sqlite (SQLite is
// happy with a second connection from another thread as long as the file is
// in WAL mode, which HistoryRecorder::start() switches it to), its own warm
// VP8 decoder per monitor, and a small LRU of already-encoded JPEGs so
// re-visiting a position the viewer has been to is immediate.
class HistoryFrameReader final : public QObject
{
    Q_OBJECT

public:
    explicit HistoryFrameReader(QString historyDir, QObject *parent = nullptr);
    ~HistoryFrameReader() override;

public slots:
    // Runs on the worker thread. Always answers (empty jpeg = nothing
    // recorded near that moment), so the caller's in-flight slot is never
    // left hanging.
    void open();
    void close();
    void readFrame(quint64 connectionId, quint32 monitorStreamId, qint64 timestampMs,
                   quint64 requestId);
    // One keyframe-aligned chunk of a recorded run, for the viewer to decode
    // locally: the frames from the keyframe at or before fromMs up to toMs,
    // capped so a single answer stays a reasonable size. No decoding happens
    // here -- the VP8 packets go out as they are stored.
    void readSegment(quint64 connectionId, quint32 monitorStreamId, qint64 sequenceId,
                     qint64 fromMs, qint64 toMs, quint64 requestId);

signals:
    // decodeMs: how long the read+decode itself took, for the timing
    // instrumentation (see AgentConnection::onFrameRead).
    void frameRead(quint64 connectionId, quint32 monitorStreamId, qint64 timestampMs,
                   quint64 requestId, const QByteArray &jpeg, qint64 decodeMs);
    // payload is an already-encoded ViewerProtocol::HistorySegment body;
    // empty means nothing was recorded there.
    void segmentRead(quint64 connectionId, quint32 monitorStreamId, quint64 requestId,
                     const QByteArray &payload, qint64 readMs);

private:
    // The warm decoder for one monitor: which sequence it is inside and the
    // timestamp it has decoded up to, so a forward step feeds only the new
    // delta frames instead of replaying from the keyframe.
    struct DecodeCache {
        qint64 sequenceId = -1;
        qint64 lastMs = -1;
        std::unique_ptr<Vp8Decoder> decoder;
    };

    QByteArray read(quint32 monitorStreamId, qint64 timestampMs);
    QByteArray readLegacyJpeg(quint32 monitorStreamId, qint64 timestampMs);
    QByteArray cachedJpeg(quint32 monitorStreamId, qint64 storedMs) const;
    void cacheJpeg(quint32 monitorStreamId, qint64 storedMs, const QByteArray &jpeg);

    QString m_historyDir;
    QString m_connectionName;
    QSqlDatabase m_database;
    std::unordered_map<quint32, std::unique_ptr<DecodeCache>> m_decodeCache;
    // Already-encoded JPEGs, keyed by (monitor, the stored frame's own
    // timestamp), oldest first; trimmed to kJpegCacheBudgetBytes.
    QHash<QPair<quint32, qint64>, QByteArray> m_jpegCache;
    QList<QPair<quint32, qint64>> m_jpegOrder;
    qint64 m_jpegCacheBytes = 0;
};

// Owns the reader's thread and marshals requests onto it. One per host.
class HistoryFrameService final : public QObject
{
    Q_OBJECT

public:
    explicit HistoryFrameService(const QString &historyDir, QObject *parent = nullptr);
    ~HistoryFrameService() override;

    // Queued onto the worker thread; the answer comes back on frameRead.
    void request(quint64 connectionId, quint32 monitorStreamId, qint64 timestampMs,
                 quint64 requestId);
    void requestSegment(quint64 connectionId, quint32 monitorStreamId, qint64 sequenceId,
                        qint64 fromMs, qint64 toMs, quint64 requestId);

signals:
    void frameRead(quint64 connectionId, quint32 monitorStreamId, qint64 timestampMs,
                   quint64 requestId, const QByteArray &jpeg, qint64 decodeMs);
    void segmentRead(quint64 connectionId, quint32 monitorStreamId, quint64 requestId,
                     const QByteArray &payload, qint64 readMs);

private:
    QThread m_thread;
    HistoryFrameReader *m_reader = nullptr;
};
