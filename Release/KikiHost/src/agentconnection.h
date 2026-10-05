#pragma once

#include "frameprotocol.h"
#include "historyrecorder.h"
#include "screencapture.h"

#include <QElapsedTimer>
#include <QHash>
#include <QImage>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QQueue>
#include <QSslSocket>
#include <QTimer>

class HistoryFrameService;

struct AgentSettings {
    QString token;
    QString agentName = QStringLiteral("Kiki Agent");
    QString sessionName;
    int jpegQuality = 70;
    // A changed region larger than this fraction of the monitor area is sent
    // as a full frame instead of a delta, because encoding/transmitting a
    // near-whole-screen patch is not worth the bookkeeping.
    double fullFrameThreshold = 0.55;
};

// Owns one authenticated (or authenticating) viewer socket. Keeps its own
// per-stream "last frame sent to this viewer" cache so every connection can
// be served independently and always starts a stream with a FullFrame.
class AgentConnection final : public QObject
{
    Q_OBJECT

public:
    AgentConnection(QSslSocket *socket, AgentSettings settings, QObject *parent = nullptr);
    ~AgentConnection() override;

    void setMonitors(const QList<MonitorInfo> &monitors);
    void pushFrame(quint32 streamId, const QImage &image);
    void pushMetadata(quint32 streamId, const QString &application, const QString &idleText,
                      quint32 activeMonitorStreamId);
    // See MessageType::StreamClosed -- tells this viewer to drop the stream
    // outright, instead of waiting to notice it stopped getting frames
    // (which, for a WindowListCapture window stream, never happens on its
    // own once it's backgrounded -- see SessionManager::onStreamClosed).
    void pushStreamClosed(quint32 streamId);
    void sendGoodbyeAndClose();

    // Optional; when unset, HistoryQuery requests just get an error JSON
    // response instead of a crash -- KikiAgent (no history
    // support at all) never sets this.
    void setHistoryRecorder(HistoryRecorder *recorder) { m_historyRecorder = recorder; }

    // Where getFrame requests are actually served from (its own thread, see
    // historyframereader.h). Without it getFrame answers "not available"
    // rather than decoding VP8 on the event loop.
    void setHistoryFrameService(HistoryFrameService *service);

    QString peerLabel() const;
    bool isAuthenticated() const { return m_authenticated; }

signals:
    void authenticated(AgentConnection *self);
    void closed(AgentConnection *self);
    void logMessage(const QString &message);

private slots:
    void onReadyRead();
    void onDisconnected();
    void onHandshakeTimeout();
    void onHeartbeatTimer();
    // The worker thread answered a getFrame; ships it and releases the
    // in-flight slot (which is also what un-blocks the deferred queries).
    void onFrameRead(quint64 connectionId, quint32 monitorStreamId, qint64 timestampMs,
                     quint64 requestId, const QByteArray &jpeg, qint64 decodeMs);
    void onSegmentRead(quint64 connectionId, quint32 monitorStreamId, quint64 requestId,
                       const QByteArray &payload, qint64 readMs);
    void processDeferredQueries();

private:
    struct StreamState {
        QImage lastImage;
        quint64 sequence = 0;
        bool primed = false;
    };

    void parseAvailableMessages();
    void processMessage(const ViewerProtocol::Header &header, const QByteArray &payload);
    void handleClientHello(const QByteArray &payload);
    void sendJson(ViewerProtocol::MessageType type, quint32 streamId, const QJsonObject &object);
    void sendAgentHellos();
    void handleHistoryQuery(const ViewerProtocol::Header &header, const QByteArray &payload);
    // Everything but getFrame; run either straight away (cheap actions) or
    // out of the deferred queue (the SQL-heavy chart/segment/keystroke ones).
    void runHistoryQuery(quint32 monitorStreamId, const QJsonObject &request);
    // Keeps only the newest wanted position per screen: while the user drags
    // the slider, superseded positions are dropped instead of decoded.
    void queueFrameRequest(quint32 monitorStreamId, qint64 timestampMs, quint64 requestId);
    void queueSegmentRequest(quint32 monitorStreamId, qint64 sequenceId, qint64 fromMs,
                             qint64 toMs, quint64 requestId);
    // Frames the user is waiting on go first, then background segment
    // prefetch, and only when neither is outstanding do the heavy queries run.
    void dispatchNextRead();
    void scheduleDeferredQueries();
    void fail(const QString &reason);

    // The newest position the viewer wants for one screen, and the request
    // id it asked under (echoed back so a late answer can be recognised).
    struct PendingFrame {
        qint64 timestampMs = 0;
        quint64 requestId = 0;
    };
    struct PendingSegment {
        quint32 streamId = 0;
        qint64 sequenceId = 0;
        qint64 fromMs = 0;
        qint64 toMs = 0;
        quint64 requestId = 0;
    };
    struct DeferredQuery {
        quint32 streamId = 0;
        QJsonObject request;
    };

    QSslSocket *m_socket = nullptr;
    AgentSettings m_settings;
    HistoryRecorder *m_historyRecorder = nullptr;
    HistoryFrameService *m_frameService = nullptr;
    // Identifies this connection in the shared reader's replies.
    quint64 m_connectionId = 0;
    QHash<quint32, PendingFrame> m_pendingFrames; // streamId -> newest wanted
    QList<quint32> m_frameOrder;                  // streams waiting, in order
    QQueue<PendingSegment> m_pendingSegments;     // background prefetch
    bool m_frameInFlight = false;
    // Charts/segments/keystrokes wait here while a frame the user is
    // actually looking at is being read.
    QQueue<DeferredQuery> m_deferred;
    QTimer m_deferredTimer;
    // Prefetch always yields one event-loop turn before starting, so a
    // getFrame that arrived in the same read burst is queued first and wins.
    QTimer m_segmentTimer;
    ViewerProtocol::PsvFrameReader m_frameReader;
    QHash<quint32, StreamState> m_streams;
    QList<MonitorInfo> m_monitors;
    QTimer m_handshakeTimeout;
    QTimer m_heartbeatTimer;
    bool m_authenticated = false;
};
