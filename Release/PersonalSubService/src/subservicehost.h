#pragma once

#include "frameprotocol.h"
#include "screencapture.h"

#include <QHash>
#include <QImage>
#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QObject>
#include <QTimer>

// Speaks PSV1 framing over a local named pipe to PersonalHost. There is
// exactly one legitimate peer (the host that spawned this process and
// therefore knows the pipe name and secret); anyone else that connects is
// rejected immediately. Always sends full (PNG, lossless) frames -- the
// pipe is local and fast, and per-viewer full/delta decisions are already
// handled independently by AgentConnection on the host side, so diffing
// again here would only add CPU cost without a matching bandwidth benefit.
class SubServiceHost final : public QObject
{
    Q_OBJECT

public:
    SubServiceHost(QString pipeName, QString pipeSecret, quint32 sessionId,
                   QString sessionUsername, QObject *parent = nullptr);

    bool start(QString *error);

    void pushMonitorList(const QList<MonitorInfo> &monitors);
    void pushFrame(quint32 streamId, const QImage &image);
    void pushMetadata(quint32 streamId, const QString &application, const QString &idleText,
                      int inputEvents);
    // streamId 0: keystrokes aren't tied to a specific monitor, only to the
    // session (matches how Keylogger observes the foreground window
    // regardless of which physical screen it's on).
    void pushKeystroke(const QString &windowTitle, const QString &text);

signals:
    void hostConnected();
    void hostDisconnected();
    void logMessage(const QString &message);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
    void onHeartbeatTimer();

private:
    void sendJson(ViewerProtocol::MessageType type, quint32 streamId, const QJsonObject &object);
    void sendHostHello();

    QLocalServer m_server;
    QLocalSocket *m_socket = nullptr;
    ViewerProtocol::PsvFrameReader m_frameReader;
    QHash<quint32, quint64> m_sequences;
    QTimer m_heartbeatTimer;
    QString m_pipeName;
    QString m_pipeSecret;
    quint32 m_sessionId = 0;
    QString m_sessionUsername;
};
