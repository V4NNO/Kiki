#pragma once

#include "frameprotocol.h"

#include <QImage>
#include <QLocalSocket>
#include <QObject>
#include <QString>
#include <QTimer>

// One QLocalSocket connection to a single PersonalSubService's pipe. Reads
// raw PSV1 framing (the sub-service always sends FullFrame, PNG-encoded,
// no delta -- see subservicehost.cpp) and republishes it upward with plain
// signals using the LOCAL (per-session) streamId. SessionManager is the
// one that knows how to turn (sessionId, localStreamId) into the globally
// unique streamId real viewers see, so all the ID bookkeeping lives there,
// not here.
class SessionIngest final : public QObject
{
    Q_OBJECT

public:
    SessionIngest(QString pipeName, QString expectedSecret, quint32 sessionId,
                  QObject *parent = nullptr);

    void start();
    void requestShutdown();
    quint32 sessionId() const { return m_sessionId; }

signals:
    void monitorDiscovered(quint32 sessionId, quint32 localStreamId, const QString &name,
                           const QSize &size);
    void frameReady(quint32 sessionId, quint32 localStreamId, const QImage &image);
    void metadataChanged(quint32 sessionId, quint32 localStreamId, const QString &application,
                         const QString &idleText, int inputEvents);
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
};
