#pragma once

#include "frameprotocol.h"
#include "screencapture.h"

#include <QElapsedTimer>
#include <QHash>
#include <QImage>
#include <QJsonObject>
#include <QObject>
#include <QSslSocket>
#include <QTimer>

struct AgentSettings {
    QString token;
    QString agentName = QStringLiteral("Personal Screen Agent");
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
    void pushMetadata(quint32 streamId, const QString &application, const QString &idleText);
    void sendGoodbyeAndClose();

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
    void fail(const QString &reason);

    QSslSocket *m_socket = nullptr;
    AgentSettings m_settings;
    ViewerProtocol::PsvFrameReader m_frameReader;
    QHash<quint32, StreamState> m_streams;
    QList<MonitorInfo> m_monitors;
    QTimer m_handshakeTimeout;
    QTimer m_heartbeatTimer;
    bool m_authenticated = false;
};
