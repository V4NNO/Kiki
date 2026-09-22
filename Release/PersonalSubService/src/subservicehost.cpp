#include "subservicehost.h"

#include <QBuffer>
#include <QCoreApplication>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPointer>
#include <QThreadPool>

namespace {

enum class DiffResult { NoChange, Delta, Full };

// Identical technique to computeChangedRect() in agentconnection.cpp (Host
// -> Viewer leg): compare cheap downscaled copies of the previous and
// current frame, then map the differing bounding box back to full
// resolution, instead of a full-resolution pixel compare every tick.
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

constexpr double kFullFrameThreshold = 0.55;

} // namespace

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

void SubServiceHost::pushWindowAnnounce(quint32 streamId, const QString &title)
{
    if (!m_socket) {
        return;
    }
    sendJson(ViewerProtocol::MessageType::AgentHello, streamId,
            QJsonObject{{QStringLiteral("monitor"),
                         QJsonObject{{QStringLiteral("name"), title},
                                     {QStringLiteral("width"), 0},
                                     {QStringLiteral("height"), 0},
                                     {QStringLiteral("isWindow"), true}}}});
}

void SubServiceHost::pushFrame(quint32 streamId, const QImage &image)
{
    if (!m_socket || image.isNull()) {
        return;
    }
    // Drop this tick's frame for the stream rather than queuing a second
    // encode job behind it -- see the class comment in subservicehost.h for
    // why this thread must never build up a backlog of encode work.
    if (m_streamsEncoding.contains(streamId)) {
        return;
    }

    StreamState &state = m_streamStates[streamId];
    QRect changedRect;
    const DiffResult result = state.primed
        ? computeChangedRect(state.lastSentImage, image, kFullFrameThreshold, &changedRect)
        : DiffResult::Full;
    if (result == DiffResult::NoChange) {
        return;
    }
    const bool isFull = (result == DiffResult::Full);
    const QImage toEncode = isFull ? image : image.copy(changedRect);

    m_streamsEncoding.insert(streamId);
    QPointer<SubServiceHost> self(this);
    QThreadPool::globalInstance()->start([self, streamId, toEncode, isFull, changedRect, image] {
        QByteArray encoded;
        QBuffer buffer(&encoded);
        buffer.open(QIODevice::WriteOnly);
        toEncode.save(&buffer, "JPEG", 75);
        QMetaObject::invokeMethod(
            qApp, [self, streamId, encoded, isFull, changedRect, image] {
                if (self) {
                    self->writeEncodedFrame(streamId, encoded, isFull, changedRect, image);
                }
            },
            Qt::QueuedConnection);
    });
}

void SubServiceHost::writeEncodedFrame(quint32 streamId, const QByteArray &encoded, bool isFull,
                                       const QRect &rect, const QImage &fullImage)
{
    m_streamsEncoding.remove(streamId);
    if (!m_socket) {
        return;
    }
    quint64 &sequence = m_sequences[streamId];
    ++sequence;
    if (isFull) {
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::FullFrame,
                                                       streamId, sequence, encoded));
    } else {
        QByteArray payload;
        payload.reserve(16 + encoded.size());
        QDataStream stream(&payload, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::BigEndian);
        stream << static_cast<qint32>(rect.x()) << static_cast<qint32>(rect.y())
               << static_cast<qint32>(rect.width()) << static_cast<qint32>(rect.height());
        payload.append(encoded);
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::DeltaFrame,
                                                       streamId, sequence, payload));
    }
    StreamState &state = m_streamStates[streamId];
    state.lastSentImage = fullImage;
    state.primed = true;
}

void SubServiceHost::pushMetadata(quint32 streamId, const QString &application,
                                  const QString &idleText, int inputEvents, const QString &url)
{
    if (!m_socket) {
        return;
    }
    sendJson(ViewerProtocol::MessageType::Metadata, streamId,
            QJsonObject{{QStringLiteral("application"), application},
                        {QStringLiteral("idle"), idleText},
                        {QStringLiteral("inputEvents"), inputEvents},
                        {QStringLiteral("url"), url}});
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
        } else if (header.type == ViewerProtocol::MessageType::ViewerCount) {
            const QJsonDocument document = QJsonDocument::fromJson(payload);
            if (document.isObject()) {
                emit viewerCountChanged(document.object().value(QStringLiteral("count")).toInt());
            }
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
