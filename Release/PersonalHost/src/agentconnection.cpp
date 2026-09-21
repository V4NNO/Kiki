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
    m_frameReader.append(m_socket->readAll());
    if (m_frameReader.bufferedSize() > static_cast<qsizetype>(ViewerProtocol::MaxPayloadSize)
                                          + ViewerProtocol::HeaderSize) {
        fail(QStringLiteral("Bufferul de intrare a depasit limita admisa."));
        return;
    }
    parseAvailableMessages();
}

void AgentConnection::parseAvailableMessages()
{
    using ViewerProtocol::PsvFrameReader;
    while (true) {
        ViewerProtocol::Header header;
        QByteArray payload;
        QString error;
        const PsvFrameReader::Result result = m_frameReader.next(&header, &payload, &error);
        if (result == PsvFrameReader::Result::NeedMoreData) {
            return;
        }
        if (result == PsvFrameReader::Result::Error) {
            fail(error);
            return;
        }
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
    case MessageType::HistoryQuery:
        handleHistoryQuery(header, payload);
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
                                         {QStringLiteral("height"), monitor.size.height()}}},
                            {QStringLiteral("session"),
                             QJsonObject{{QStringLiteral("id"),
                                          static_cast<qint64>(monitor.sessionId)},
                                         {QStringLiteral("username"), monitor.sessionUsername},
                                         {QStringLiteral("state"), monitor.sessionState}}}});
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

void AgentConnection::handleHistoryQuery(const ViewerProtocol::Header &header, const QByteArray &payload)
{
    if (!m_authenticated) {
        return;
    }
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        return;
    }
    const QJsonObject request = document.object();
    const QString action = request.value(QStringLiteral("action")).toString();
    const quint32 monitorStreamId = header.streamId;

    if (!m_historyRecorder) {
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("error"), QStringLiteral("Istoricul nu este activat pe host.")}});
        return;
    }

    if (action == QStringLiteral("listDays")) {
        const QStringList days = m_historyRecorder->listDays(monitorStreamId);
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("days"), QJsonArray::fromStringList(days)}});
    } else if (action == QStringLiteral("listFrames")) {
        const QString day = request.value(QStringLiteral("day")).toString();
        const QList<qint64> timestamps = m_historyRecorder->listFrameTimestamps(monitorStreamId, day);
        QJsonArray array;
        for (qint64 timestamp : timestamps) {
            array.append(timestamp);
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("timestamps"), array}});
    } else if (action == QStringLiteral("getFrame")) {
        const qint64 timestampMs = static_cast<qint64>(
            request.value(QStringLiteral("timestampMs")).toDouble());
        const QByteArray jpeg = m_historyRecorder->readFrame(monitorStreamId, timestampMs);
        if (jpeg.isEmpty()) {
            sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                    QJsonObject{{QStringLiteral("action"), action},
                                {QStringLiteral("error"), QStringLiteral("Cadru negasit.")}});
            return;
        }
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::HistoryFrame,
                                                       monitorStreamId, 0, jpeg, timestampMs));
    } else if (action == QStringLiteral("listActivity")) {
        const QString day = request.value(QStringLiteral("day")).toString();
        QJsonArray array;
        for (const ActivitySample &sample : m_historyRecorder->listActivity(monitorStreamId, day)) {
            array.append(QJsonObject{{QStringLiteral("timestampMs"), sample.timestampMs},
                                     {QStringLiteral("inputEvents"), sample.inputEvents}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("samples"), array}});
    } else if (action == QStringLiteral("listAppSegments")) {
        const QString day = request.value(QStringLiteral("day")).toString();
        QJsonArray array;
        for (const AppSegment &segment : m_historyRecorder->listAppSegments(monitorStreamId, day)) {
            array.append(QJsonObject{{QStringLiteral("application"), segment.application},
                                     {QStringLiteral("startMs"), segment.startMs},
                                     {QStringLiteral("endMs"), segment.endMs}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("segments"), array}});
    } else if (action == QStringLiteral("listRunningApplications")) {
        const QString day = request.value(QStringLiteral("day")).toString();
        QJsonArray array;
        for (const AppUsage &usage : m_historyRecorder->listRunningApplications(monitorStreamId, day)) {
            array.append(QJsonObject{{QStringLiteral("application"), usage.application},
                                     {QStringLiteral("totalMs"), usage.totalMs},
                                     {QStringLiteral("category"), usage.category}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("applications"), array}});
    } else if (action == QStringLiteral("listCategories")) {
        QJsonArray array;
        const auto categories = m_historyRecorder->listCategories();
        for (const auto &entry : categories) {
            array.append(QJsonObject{{QStringLiteral("application"), entry.first},
                                     {QStringLiteral("category"), entry.second}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("categories"), array}});
    } else if (action == QStringLiteral("setCategory")) {
        const QString application = request.value(QStringLiteral("application")).toString();
        const QString category = request.value(QStringLiteral("category")).toString();
        m_historyRecorder->setCategory(application, category);
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("application"), application},
                            {QStringLiteral("category"), category}});
    } else if (action == QStringLiteral("listKeystrokes")) {
        const QString day = request.value(QStringLiteral("day")).toString();
        quint32 sessionId = 0;
        for (const MonitorInfo &monitor : std::as_const(m_monitors)) {
            if (monitor.streamId == monitorStreamId) {
                sessionId = monitor.sessionId;
                break;
            }
        }
        QJsonArray array;
        for (const KeystrokeEntry &entry : m_historyRecorder->listKeystrokes(sessionId, day)) {
            array.append(QJsonObject{{QStringLiteral("timestampMs"), entry.timestampMs},
                                     {QStringLiteral("windowTitle"), entry.windowTitle},
                                     {QStringLiteral("text"), entry.text}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("entries"), array}});
    }
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
