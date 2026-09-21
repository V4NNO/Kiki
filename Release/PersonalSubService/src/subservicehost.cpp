#include "subservicehost.h"

#include <QBuffer>
#include <QJsonDocument>
#include <QJsonObject>

SubServiceHost::SubServiceHost(QString pipeName, QString pipeSecret, quint32 sessionId,
                               QString sessionUsername, QObject *parent)
    : QObject(parent)
    , m_pipeName(std::move(pipeName))
    , m_pipeSecret(std::move(pipeSecret))
    , m_sessionId(sessionId)
    , m_sessionUsername(std::move(sessionUsername))
{
    connect(&m_server, &QLocalServer::newConnection, this, &SubServiceHost::onNewConnection);
    m_heartbeatTimer.setInterval(5000);
    connect(&m_heartbeatTimer, &QTimer::timeout, this, &SubServiceHost::onHeartbeatTimer);
}

bool SubServiceHost::start(QString *error)
{
    QLocalServer::removeServer(m_pipeName);
    if (!m_server.listen(m_pipeName)) {
        if (error) {
            *error = m_server.errorString();
        }
        return false;
    }
    return true;
}

void SubServiceHost::onNewConnection()
{
    QLocalSocket *incoming = m_server.nextPendingConnection();
    if (m_socket) {
        // Only PersonalHost is expected to connect; anyone else is rejected.
        incoming->abort();
        incoming->deleteLater();
        return;
    }
    m_socket = incoming;
    m_socket->setParent(this);
    connect(m_socket, &QLocalSocket::readyRead, this, &SubServiceHost::onReadyRead);
    connect(m_socket, &QLocalSocket::disconnected, this, &SubServiceHost::onDisconnected);
    sendHostHello();
    m_heartbeatTimer.start();
    emit logMessage(QStringLiteral("PersonalHost conectat pe pipe-ul de sesiune."));
    emit hostConnected();
}

void SubServiceHost::sendHostHello()
{
    sendJson(ViewerProtocol::MessageType::AgentHello, 0,
            QJsonObject{{QStringLiteral("agentName"), QStringLiteral("PersonalSubService")},
                        {QStringLiteral("pipeSecret"), m_pipeSecret},
                        {QStringLiteral("sessionId"), static_cast<qint64>(m_sessionId)},
                        {QStringLiteral("sessionUsername"), m_sessionUsername}});
}

void SubServiceHost::pushMonitorList(const QList<MonitorInfo> &monitors)
{
    if (!m_socket) {
        return;
    }
    for (const MonitorInfo &monitor : monitors) {
        sendJson(ViewerProtocol::MessageType::AgentHello, monitor.streamId,
                QJsonObject{{QStringLiteral("monitor"),
                             QJsonObject{{QStringLiteral("name"), monitor.name},
                                         {QStringLiteral("width"), monitor.size.width()},
                                         {QStringLiteral("height"), monitor.size.height()}}}});
    }
}

void SubServiceHost::pushFrame(quint32 streamId, const QImage &image)
{
    if (!m_socket || image.isNull()) {
        return;
    }
    QByteArray encoded;
    QBuffer buffer(&encoded);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    quint64 &sequence = m_sequences[streamId];
    m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::FullFrame, streamId,
                                                   ++sequence, encoded));
}

void SubServiceHost::pushMetadata(quint32 streamId, const QString &application,
                                  const QString &idleText, int inputEvents)
{
    if (!m_socket) {
        return;
    }
    sendJson(ViewerProtocol::MessageType::Metadata, streamId,
            QJsonObject{{QStringLiteral("application"), application},
                        {QStringLiteral("idle"), idleText},
                        {QStringLiteral("inputEvents"), inputEvents}});
}

void SubServiceHost::pushKeystroke(const QString &windowTitle, const QString &text)
{
    if (!m_socket) {
        return;
    }
    sendJson(ViewerProtocol::MessageType::Keystroke, 0,
            QJsonObject{{QStringLiteral("windowTitle"), windowTitle},
                        {QStringLiteral("text"), text}});
}

void SubServiceHost::sendJson(ViewerProtocol::MessageType type, quint32 streamId,
                              const QJsonObject &object)
{
    if (!m_socket) {
        return;
    }
    const QByteArray payload = QJsonDocument(object).toJson(QJsonDocument::Compact);
    m_socket->write(ViewerProtocol::encodeMessage(type, streamId, 0, payload));
}

void SubServiceHost::onReadyRead()
{
    m_frameReader.append(m_socket->readAll());
    while (true) {
        ViewerProtocol::Header header;
        QByteArray payload;
        QString error;
        const auto result = m_frameReader.next(&header, &payload, &error);
        if (result == ViewerProtocol::PsvFrameReader::Result::NeedMoreData) {
            return;
        }
        if (result == ViewerProtocol::PsvFrameReader::Result::Error) {
            emit logMessage(QStringLiteral("Eroare de protocol pe pipe: %1").arg(error));
            m_socket->abort();
            return;
        }
        if (header.type == ViewerProtocol::MessageType::Goodbye) {
            m_socket->disconnectFromServer();
        }
        // Heartbeat and any other inbound type from the host are ignored;
        // this channel only needs to carry frames outward.
    }
}

void SubServiceHost::onHeartbeatTimer()
{
    if (m_socket) {
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::Heartbeat, 0, 0,
                                                       QByteArray()));
    }
}

void SubServiceHost::onDisconnected()
{
    m_heartbeatTimer.stop();
    m_frameReader.clear();
    if (m_socket) {
        m_socket->deleteLater();
        m_socket = nullptr;
    }
    emit hostDisconnected();
}
