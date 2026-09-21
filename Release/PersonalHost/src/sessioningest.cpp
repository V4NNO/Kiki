#include "sessioningest.h"

#include <QJsonDocument>
#include <QJsonObject>

SessionIngest::SessionIngest(QString pipeName, QString expectedSecret, quint32 sessionId,
                             QObject *parent)
    : QObject(parent)
    , m_pipeName(std::move(pipeName))
    , m_expectedSecret(std::move(expectedSecret))
    , m_sessionId(sessionId)
{
    connect(&m_socket, &QLocalSocket::connected, this, &SessionIngest::onConnected);
    connect(&m_socket, &QLocalSocket::readyRead, this, &SessionIngest::onReadyRead);
    connect(&m_socket, &QLocalSocket::disconnected, this, &SessionIngest::onDisconnected);
    connect(&m_socket, &QLocalSocket::errorOccurred, this, &SessionIngest::onErrorOccurred);
    m_retryTimer.setInterval(300);
    m_retryTimer.setSingleShot(true);
    connect(&m_retryTimer, &QTimer::timeout, this, &SessionIngest::retryConnect);
}

void SessionIngest::start()
{
    m_connectAttempts = 0;
    m_socket.connectToServer(m_pipeName);
}

void SessionIngest::retryConnect()
{
    if (m_everConnected) {
        return;
    }
    constexpr int MaxAttempts = 20; // ~6s of retries while the sub-service boots
    if (m_connectAttempts >= MaxAttempts) {
        emit logMessage(QStringLiteral("Sub-service-ul sesiunii %1 nu a pornit la timp.")
                            .arg(m_sessionId));
        emit connectFailed(m_sessionId);
        return;
    }
    ++m_connectAttempts;
    m_socket.connectToServer(m_pipeName);
}

void SessionIngest::requestShutdown()
{
    if (m_socket.state() == QLocalSocket::ConnectedState) {
        m_socket.write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::Goodbye, 0, 0,
                                                      QByteArray()));
        m_socket.flush();
    }
}

void SessionIngest::onConnected()
{
    m_everConnected = true;
    m_retryTimer.stop();
    emit logMessage(QStringLiteral("Conectat la PersonalSubService (sesiune %1).").arg(m_sessionId));
}

void SessionIngest::onReadyRead()
{
    m_frameReader.append(m_socket.readAll());
    while (true) {
        ViewerProtocol::Header header;
        QByteArray payload;
        QString error;
        const auto result = m_frameReader.next(&header, &payload, &error);
        if (result == ViewerProtocol::PsvFrameReader::Result::NeedMoreData) {
            return;
        }
        if (result == ViewerProtocol::PsvFrameReader::Result::Error) {
            emit logMessage(QStringLiteral("Eroare de protocol IPC (sesiune %1): %2")
                                .arg(m_sessionId)
                                .arg(error));
            m_socket.abort();
            return;
        }
        processMessage(header, payload);
    }
}

void SessionIngest::processMessage(const ViewerProtocol::Header &header, const QByteArray &payload)
{
    using ViewerProtocol::MessageType;
    switch (header.type) {
    case MessageType::AgentHello: {
        QJsonParseError parseError;
        const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);
        if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
            return;
        }
        const QJsonObject object = document.object();
        if (header.streamId == 0) {
            const QString secret = object.value(QStringLiteral("pipeSecret")).toString();
            m_secretVerified = !m_expectedSecret.isEmpty() && secret == m_expectedSecret;
            if (!m_secretVerified) {
                emit logMessage(QStringLiteral("Secret invalid pe pipe-ul sesiunii %1; inchid.")
                                    .arg(m_sessionId));
                m_socket.abort();
            }
            return;
        }
        if (!m_secretVerified) {
            return;
        }
        const QJsonObject monitor = object.value(QStringLiteral("monitor")).toObject();
        emit monitorDiscovered(m_sessionId, header.streamId,
                               monitor.value(QStringLiteral("name")).toString(),
                               QSize(monitor.value(QStringLiteral("width")).toInt(),
                                     monitor.value(QStringLiteral("height")).toInt()));
        break;
    }
    case MessageType::FullFrame: {
        if (!m_secretVerified) {
            return;
        }
        QImage image;
        if (image.loadFromData(payload) && !image.isNull()) {
            emit frameReady(m_sessionId, header.streamId, image);
        }
        break;
    }
    case MessageType::Metadata: {
        if (!m_secretVerified) {
            return;
        }
        QJsonParseError parseError;
        const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);
        if (parseError.error == QJsonParseError::NoError && document.isObject()) {
            const QJsonObject object = document.object();
            emit metadataChanged(m_sessionId, header.streamId,
                                 object.value(QStringLiteral("application")).toString(),
                                 object.value(QStringLiteral("idle")).toString(),
                                 object.value(QStringLiteral("inputEvents")).toInt());
        }
        break;
    }
    case MessageType::Keystroke: {
        if (!m_secretVerified) {
            return;
        }
        QJsonParseError parseError;
        const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);
        if (parseError.error == QJsonParseError::NoError && document.isObject()) {
            const QJsonObject object = document.object();
            emit keystrokeReceived(m_sessionId, object.value(QStringLiteral("windowTitle")).toString(),
                                   object.value(QStringLiteral("text")).toString());
        }
        break;
    }
    case MessageType::Heartbeat:
    default:
        break;
    }
}

void SessionIngest::onDisconnected()
{
    m_frameReader.clear();
    m_secretVerified = false;
    emit ingestDisconnected(m_sessionId);
}

void SessionIngest::onErrorOccurred()
{
    if (!m_everConnected) {
        m_retryTimer.start();
        return;
    }
    emit logMessage(QStringLiteral("Eroare pipe (sesiune %1): %2")
                        .arg(m_sessionId)
                        .arg(m_socket.errorString()));
}
