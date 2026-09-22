#pragma once

#include "frameprotocol.h"

#include <QHash>
#include <QImage>
#include <QLocalSocket>
#include <QObject>
#include <QString>
#include <QTimer>

// One QLocalSocket connection to a single PersonalSubService's pipe. Reads
// raw PSV1 framing and republishes it upward with plain signals using the
// LOCAL (per-session) streamId. SessionManager is the one that knows how to
// turn (sessionId, localStreamId) into the globally unique streamId real
// viewers see, so all the ID bookkeeping lives there, not here.
//
// The sub-service sends full-vs-delta JPEG frames (see subservicehost.cpp);
// DeltaFrame patches are composited onto a per-streamId cached image here,
// the same way ViewerConnection::processDeltaFrame() does on the
// Host->Viewer leg, so everything above this class still only ever sees
// complete images via frameReady() -- SessionManager, AgentServer/history
// recording etc. are unaware the wire format has full/delta frames at all.
class SessionIngest final : public QObject
{
    Q_OBJECT

public:
    SessionIngest(QString pipeName, QString expectedSecret, quint32 sessionId,
                  QObject *parent = nullptr);

    void start();
    void requestShutdown();
    quint32 sessionId() const { return m_sessionId; }
    // Tells this session's sub-service how many authenticated viewers are
    // currently connected, so it can throttle its own capture FPS -- see
    // frameprotocol.h's ViewerCount. No-op if the pipe isn't connected yet;
    // SessionManager sends the current count again once it is (see
    // onMonitorDiscovered()).
    void sendViewerCount(int count);

signals:
    void monitorDiscovered(quint32 sessionId, quint32 localStreamId, const QString &name,
                           const QSize &size, bool isWindow);
    void frameReady(quint32 sessionId, quint32 localStreamId, const QImage &image);
    void metadataChanged(quint32 sessionId, quint32 localStreamId, const QString &application,
                         const QString &idleText, int inputEvents, const QString &url);
    void keystrokeReceived(quint32 sessionId, const QString &windowTitle, const QString &text);
    void ingestDisconnected(quint32 sessionId);
    // Emitted once the connect retries are exhausted; SessionManager treats
    // this as "the sub-service never came up" and gives up on the session.
    void connectFailed(quint32 sessionId);
    void logMessage(const QString &message);

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();
    void onErrorOccurred();
    void retryConnect();

private:
    void processMessage(const ViewerProtocol::Header &header, const QByteArray &payload);

    QLocalSocket m_socket;
    ViewerProtocol::PsvFrameReader m_frameReader;
    QTimer m_retryTimer;
    QString m_pipeName;
    QString m_expectedSecret;
    quint32 m_sessionId = 0;
    bool m_secretVerified = false;
    bool m_everConnected = false;
    int m_connectAttempts = 0;
    // Per-localStreamId state for reconstructing full images from
    // full/delta frames -- see the class comment above.
    QHash<quint32, QImage> m_lastImages;
    QHash<quint32, quint64> m_lastSequences;
};
