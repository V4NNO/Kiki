#include "agentconnection.h"

#include <QBuffer>
#include <QDataStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHostAddress>

namespace {

enum class DiffResult { NoChange, Delta, Full };

// Cheap change detection: compare a small downscaled copy of the previous
// and current frame, then map the differing bounding box back to full
// resolution. Good enough to decide "send nothing / a patch / a full frame"
// without paying for a full-resolution pixel compare every tick.
DiffResult computeChangedRect(const QImage &prev, const QImage &curr, double fullFrameThreshold,
                              QRect *outRect)
{
    if (prev.isNull() || prev.size() != curr.size()) {
        return DiffResult::Full;
    }

    constexpr int SampleWidth = 200;
    const double scale = static_cast<double>(SampleWidth) / curr.width();
    const int sampleHeight = qMax(1, static_cast<int>(curr.height() * scale));
    const QImage prevSmall = prev.scaled(SampleWidth, sampleHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation)
                                  .convertToFormat(QImage::Format_Grayscale8);
    const QImage currSmall = curr.scaled(SampleWidth, sampleHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation)
                                  .convertToFormat(QImage::Format_Grayscale8);

    int minX = SampleWidth;
    int minY = sampleHeight;
    int maxX = -1;
    int maxY = -1;
    constexpr int NoiseTolerance = 8;
    for (int y = 0; y < sampleHeight; ++y) {
        const uchar *rowPrev = prevSmall.constScanLine(y);
        const uchar *rowCurr = currSmall.constScanLine(y);
        for (int x = 0; x < SampleWidth; ++x) {
            if (qAbs(int(rowPrev[x]) - int(rowCurr[x])) > NoiseTolerance) {
                minX = qMin(minX, x);
                minY = qMin(minY, y);
                maxX = qMax(maxX, x);
                maxY = qMax(maxY, y);
            }
        }
    }

    if (maxX < 0) {
        return DiffResult::NoChange;
    }

    const double invScaleX = static_cast<double>(curr.width()) / SampleWidth;
    const double invScaleY = static_cast<double>(curr.height()) / sampleHeight;
    constexpr int PaddingSamples = 2;
    QRect rect(static_cast<int>((minX - PaddingSamples) * invScaleX),
              static_cast<int>((minY - PaddingSamples) * invScaleY),
              static_cast<int>((maxX - minX + 1 + 2 * PaddingSamples) * invScaleX),
              static_cast<int>((maxY - minY + 1 + 2 * PaddingSamples) * invScaleY));
    rect = rect.intersected(curr.rect());
    if (rect.isEmpty()) {
        return DiffResult::NoChange;
    }

    const double area = static_cast<double>(rect.width()) * rect.height();
    const double totalArea = static_cast<double>(curr.width()) * curr.height();
    if (area / totalArea > fullFrameThreshold) {
        return DiffResult::Full;
    }
    *outRect = rect;
    return DiffResult::Delta;
}

QByteArray encodeImage(const QImage &image, int jpegQuality)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPEG", jpegQuality);
    return bytes;
}

} // namespace

AgentConnection::AgentConnection(QSslSocket *socket, AgentSettings settings, QObject *parent)
    : QObject(parent)
    , m_socket(socket)
    , m_settings(std::move(settings))
{
    m_socket->setParent(this);
    connect(m_socket, &QSslSocket::readyRead, this, &AgentConnection::onReadyRead);
    connect(m_socket, &QSslSocket::disconnected, this, &AgentConnection::onDisconnected);

    m_handshakeTimeout.setSingleShot(true);
    m_handshakeTimeout.setInterval(5000);
    connect(&m_handshakeTimeout, &QTimer::timeout, this, &AgentConnection::onHandshakeTimeout);
    m_handshakeTimeout.start();

    m_heartbeatTimer.setInterval(5000);
    connect(&m_heartbeatTimer, &QTimer::timeout, this, &AgentConnection::onHeartbeatTimer);
}

AgentConnection::~AgentConnection() = default;

QString AgentConnection::peerLabel() const
{
    return QStringLiteral("%1:%2").arg(m_socket->peerAddress().toString()).arg(m_socket->peerPort());
}

void AgentConnection::setMonitors(const QList<MonitorInfo> &monitors)
{
    m_monitors = monitors;
    if (m_authenticated) {
        sendAgentHellos();
    }
}

void AgentConnection::onReadyRead()
{
    m_receiveBuffer.append(m_socket->readAll());
    if (m_receiveBuffer.size() > static_cast<qsizetype>(ViewerProtocol::MaxPayloadSize)
                                    + ViewerProtocol::HeaderSize) {
        fail(QStringLiteral("Bufferul de intrare a depasit limita admisa."));
        return;
    }
    parseAvailableMessages();
}

void AgentConnection::parseAvailableMessages()
{
    while (m_receiveBuffer.size() >= ViewerProtocol::HeaderSize) {
        ViewerProtocol::Header header;
        QString error;
        if (!ViewerProtocol::decodeHeader(m_receiveBuffer, &header, &error)) {
            fail(error);
            return;
        }
        const qsizetype totalSize = ViewerProtocol::HeaderSize
            + static_cast<qsizetype>(header.payloadSize);
        if (m_receiveBuffer.size() < totalSize) {
            return;
        }
        const QByteArray payload = m_receiveBuffer.mid(ViewerProtocol::HeaderSize,
                                                       header.payloadSize);
        m_receiveBuffer.remove(0, totalSize);
        processMessage(header, payload);
    }
}

void AgentConnection::processMessage(const ViewerProtocol::Header &header, const QByteArray &payload)
{
    using ViewerProtocol::MessageType;
    switch (header.type) {
    case MessageType::ClientHello:
        handleClientHello(payload);
        break;
    case MessageType::Heartbeat:
        break;
    case MessageType::Goodbye:
        m_socket->disconnectFromHost();
        break;
    default:
        fail(QStringLiteral("Viewer-ul a trimis un tip de mesaj neasteptat."));
        break;
    }
}

void AgentConnection::handleClientHello(const QByteArray &payload)
{
    if (m_authenticated) {
        fail(QStringLiteral("ClientHello duplicat."));
        return;
    }
    m_handshakeTimeout.stop();

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        fail(QStringLiteral("ClientHello invalid."));
        return;
    }
    const QJsonObject object = document.object();
    const QString token = object.value(QStringLiteral("token")).toString();
    const QJsonObject capabilities = object.value(QStringLiteral("capabilities")).toObject();

    const bool disallowedCapabilityRequested =
        capabilities.value(QStringLiteral("remoteControl")).toBool()
        || capabilities.value(QStringLiteral("audio")).toBool()
        || capabilities.value(QStringLiteral("clipboard")).toBool()
        || capabilities.value(QStringLiteral("fileTransfer")).toBool()
        || capabilities.value(QStringLiteral("keylogging")).toBool();

    if (m_settings.token.isEmpty() || token != m_settings.token) {
        sendJson(ViewerProtocol::MessageType::AuthResult, 0,
                QJsonObject{{QStringLiteral("accepted"), false},
                            {QStringLiteral("reason"), QStringLiteral("Token invalid.")}});
        emit logMessage(QStringLiteral("Autentificare refuzata pentru %1 (token invalid).")
                            .arg(peerLabel()));
        m_socket->disconnectFromHost();
        return;
    }
    if (disallowedCapabilityRequested) {
        sendJson(ViewerProtocol::MessageType::AuthResult, 0,
                QJsonObject{{QStringLiteral("accepted"), false},
                            {QStringLiteral("reason"),
                             QStringLiteral("Capabilitati nepermise (control/audio/fisiere).")}});
        emit logMessage(QStringLiteral("Conexiune refuzata pentru %1: solicita capabilitati nepermise.")
                            .arg(peerLabel()));
        m_socket->disconnectFromHost();
        return;
    }

    m_authenticated = true;
    sendJson(ViewerProtocol::MessageType::AuthResult, 0, QJsonObject{{QStringLiteral("accepted"), true}});
    m_heartbeatTimer.start();
    emit logMessage(QStringLiteral("Viewer autentificat: %1").arg(peerLabel()));
    emit authenticated(this);
}

void AgentConnection::sendAgentHellos()
{
    sendJson(ViewerProtocol::MessageType::AgentHello, 0,
            QJsonObject{{QStringLiteral("agentName"), m_settings.agentName},
                        {QStringLiteral("sessionName"), m_settings.sessionName}});
    for (const MonitorInfo &monitor : std::as_const(m_monitors)) {
        sendJson(ViewerProtocol::MessageType::AgentHello, monitor.streamId,
                QJsonObject{{QStringLiteral("agentName"), m_settings.agentName},
                            {QStringLiteral("sessionName"), m_settings.sessionName},
                            {QStringLiteral("monitor"),
                             QJsonObject{{QStringLiteral("name"), monitor.name},
                                         {QStringLiteral("width"), monitor.size.width()},
                                         {QStringLiteral("height"), monitor.size.height()}}}});
    }
}

void AgentConnection::pushFrame(quint32 streamId, const QImage &image)
{
    if (!m_authenticated || image.isNull()) {
        return;
    }
    StreamState &state = m_streams[streamId];

    QRect changedRect;
    DiffResult result = state.primed
        ? computeChangedRect(state.lastImage, image, m_settings.fullFrameThreshold, &changedRect)
        : DiffResult::Full;

    if (result == DiffResult::NoChange) {
        return;
    }

    ++state.sequence;
    if (result == DiffResult::Full) {
        const QByteArray encoded = encodeImage(image, m_settings.jpegQuality);
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::FullFrame,
                                                       streamId, state.sequence, encoded));
    } else {
        const QImage patch = image.copy(changedRect);
        const QByteArray encodedPatch = encodeImage(patch, m_settings.jpegQuality);
        QByteArray payload;
        payload.reserve(16 + encodedPatch.size());
        QDataStream stream(&payload, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << static_cast<qint32>(changedRect.x()) << static_cast<qint32>(changedRect.y())
               << static_cast<qint32>(changedRect.width())
               << static_cast<qint32>(changedRect.height());
        payload.append(encodedPatch);
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::DeltaFrame,
                                                       streamId, state.sequence, payload));
    }
    state.lastImage = image;
    state.primed = true;
}

void AgentConnection::pushMetadata(quint32 streamId, const QString &application, const QString &idleText)
{
    if (!m_authenticated) {
        return;
    }
    sendJson(ViewerProtocol::MessageType::Metadata, streamId,
            QJsonObject{{QStringLiteral("application"), application},
                        {QStringLiteral("idle"), idleText}});
}

void AgentConnection::sendJson(ViewerProtocol::MessageType type, quint32 streamId,
                               const QJsonObject &object)
{
    const QByteArray payload = QJsonDocument(object).toJson(QJsonDocument::Compact);
    m_socket->write(ViewerProtocol::encodeMessage(type, streamId, 0, payload));
}

void AgentConnection::sendGoodbyeAndClose()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::Goodbye, 0, 0,
                                                       QByteArray()));
        m_socket->flush();
    }
    m_socket->disconnectFromHost();
}

void AgentConnection::onHandshakeTimeout()
{
    fail(QStringLiteral("ClientHello nu a sosit la timp."));
}

void AgentConnection::onHeartbeatTimer()
{
    if (m_authenticated) {
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::Heartbeat, 0, 0,
                                                       QByteArray()));
    }
}

void AgentConnection::fail(const QString &reason)
{
    emit logMessage(QStringLiteral("Conexiune inchisa (%1): %2").arg(peerLabel(), reason));
    m_socket->abort();
}

void AgentConnection::onDisconnected()
{
    m_handshakeTimeout.stop();
    m_heartbeatTimer.stop();
    emit closed(this);
}
