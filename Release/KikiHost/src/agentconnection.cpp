#include "agentconnection.h"

#include "historyframereader.h"

#include <QBuffer>
#include <QElapsedTimer>
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

    // Deferred (SQL-heavy) history queries are drained one per event-loop
    // turn, and only while no frame read is outstanding -- a scrub must
    // never queue behind a chart query.
    static quint64 nextConnectionId = 0;
    m_connectionId = ++nextConnectionId;
    m_deferredTimer.setSingleShot(true);
    m_deferredTimer.setInterval(0);
    connect(&m_deferredTimer, &QTimer::timeout, this, &AgentConnection::processDeferredQueries);
    m_segmentTimer.setSingleShot(true);
    m_segmentTimer.setInterval(0);
    connect(&m_segmentTimer, &QTimer::timeout, this, [this] {
        if (!m_frameInFlight) {
            dispatchNextRead();
        }
    });
}

void AgentConnection::setHistoryFrameService(HistoryFrameService *service)
{
    if (m_frameService == service) {
        return;
    }
    if (m_frameService) {
        disconnect(m_frameService, nullptr, this, nullptr);
    }
    m_frameService = service;
    if (m_frameService) {
        connect(m_frameService, &HistoryFrameService::frameRead, this,
                &AgentConnection::onFrameRead);
        connect(m_frameService, &HistoryFrameService::segmentRead, this,
                &AgentConnection::onSegmentRead);
    }
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
                                         {QStringLiteral("height"), monitor.size.height()},
                                         {QStringLiteral("isWindow"), monitor.isWindow}}},
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

void AgentConnection::pushMetadata(quint32 streamId, const QString &application,
                                   const QString &idleText, quint32 activeMonitorStreamId)
{
    if (!m_authenticated) {
        return;
    }
    sendJson(ViewerProtocol::MessageType::Metadata, streamId,
            QJsonObject{{QStringLiteral("application"), application},
                        {QStringLiteral("idle"), idleText},
                        {QStringLiteral("activeMonitor"),
                         static_cast<qint64>(activeMonitorStreamId)}});
}

void AgentConnection::pushStreamClosed(quint32 streamId)
{
    if (!m_authenticated) {
        return;
    }
    m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::StreamClosed,
                                                  streamId, 0, QByteArray()));
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

namespace {
// Queries whose SQL is heavy enough to be felt as a stall if it runs while
// the user is scrubbing: they are queued and drained only when no frame read
// is outstanding, so the picture the user asked for always wins the race.
bool isDeferrableHistoryAction(const QString &action)
{
    return action == QStringLiteral("chartSeries")
        || action == QStringLiteral("listActivity")
        || action == QStringLiteral("listAppSegments")
        || action == QStringLiteral("listRunningApplications")
        || action == QStringLiteral("listWebVisits")
        || action == QStringLiteral("listWebPages")
        || action == QStringLiteral("listKeystrokes");
}

// A viewer that keeps changing its mind faster than the host can answer must
// not be able to grow this without bound.
constexpr int kMaxDeferredQueries = 32;

// Same idea for the prefetch queue.
constexpr int kMaxPendingSegments = 24;
} // namespace

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
                            {QStringLiteral("requestId"), request.value(QStringLiteral("requestId"))},
                            {QStringLiteral("error"), QStringLiteral("Istoricul nu este activat pe host.")}});
        return;
    }

    // The frame the user is looking at is served first and off this thread.
    if (action == QStringLiteral("getFrame")) {
        const qint64 timestampMs =
            static_cast<qint64>(request.value(QStringLiteral("timestampMs")).toDouble());
        const quint64 requestId =
            static_cast<quint64>(request.value(QStringLiteral("requestId")).toDouble());
        queueFrameRequest(monitorStreamId, timestampMs, requestId);
        return;
    }

    // Background prefetch of a whole stretch of video: queued behind any
    // frame the user is actually waiting for, ahead of the heavy queries.
    if (action == QStringLiteral("getSegment")) {
        queueSegmentRequest(
            monitorStreamId,
            static_cast<qint64>(request.value(QStringLiteral("sequenceId")).toDouble()),
            static_cast<qint64>(request.value(QStringLiteral("fromMs")).toDouble()),
            static_cast<qint64>(request.value(QStringLiteral("toMs")).toDouble()),
            static_cast<quint64>(request.value(QStringLiteral("requestId")).toDouble()));
        return;
    }

    if (isDeferrableHistoryAction(action)) {
        while (m_deferred.size() >= kMaxDeferredQueries) {
            m_deferred.dequeue();
        }
        m_deferred.enqueue(DeferredQuery{monitorStreamId, request});
        scheduleDeferredQueries();
        return;
    }

    runHistoryQuery(monitorStreamId, request);
}

void AgentConnection::runHistoryQuery(quint32 monitorStreamId, const QJsonObject &request)
{
    const QString action = request.value(QStringLiteral("action")).toString();
    // Day-scoped queries take an explicit [startMs, stopMs) range when the
    // viewer sends one (History works on multi-day / custom periods); else
    // the local day. "day" is echoed back either way, as the reply's key.
    const QString day = request.value(QStringLiteral("day")).toString();
    const auto [rangeStart, rangeStop] = [&request, &day]() -> QPair<qint64, qint64> {
        if (request.contains(QStringLiteral("startMs")) && request.contains(QStringLiteral("stopMs"))) {
            return {static_cast<qint64>(request.value(QStringLiteral("startMs")).toDouble()),
                    static_cast<qint64>(request.value(QStringLiteral("stopMs")).toDouble())};
        }
        return HistoryRecorder::dayBounds(day);
    }();

    if (!m_historyRecorder) {
        return;
    }

    if (action == QStringLiteral("listDays")) {
        const QStringList days = m_historyRecorder->listDays(monitorStreamId);
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("days"), QJsonArray::fromStringList(days)}});
    } else if (action == QStringLiteral("listFrames")) {
        const QList<qint64> timestamps = m_historyRecorder->listFrameTimestamps(monitorStreamId, rangeStart, rangeStop);
        QJsonArray array;
        for (qint64 timestamp : timestamps) {
            array.append(timestamp);
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("timestamps"), array}});
    } else if (action == QStringLiteral("listScreens")) {
        // The employee's screens that recorded anything in the period --
        // what History shows side by side, independent of what is plugged in
        // right now.
        QJsonArray array;
        for (const RecordedScreen &screen :
             m_historyRecorder->listRecordedScreens(monitorStreamId, rangeStart, rangeStop)) {
            array.append(QJsonObject{{QStringLiteral("streamId"), double(screen.streamId)},
                                     {QStringLiteral("name"), screen.name}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("screens"), array}});
    } else if (action == QStringLiteral("listSegments")) {
        // The recorded runs in the window. The viewer uses these to plan
        // which stretches of video to pull down and decode locally.
        QJsonArray array;
        for (const VideoSegmentInfo &info :
             m_historyRecorder->listVideoSegments(monitorStreamId, rangeStart, rangeStop)) {
            array.append(QJsonObject{{QStringLiteral("id"), double(info.sequenceId)},
                                     {QStringLiteral("beginMs"), info.beginMs},
                                     {QStringLiteral("endMs"), info.endMs},
                                     {QStringLiteral("width"), info.width},
                                     {QStringLiteral("height"), info.height}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("segments"), array}});
    } else if (action == QStringLiteral("listActivity")) {
        QJsonArray array;
        for (const ActivitySample &sample : m_historyRecorder->listActivity(monitorStreamId, rangeStart, rangeStop)) {
            array.append(QJsonObject{{QStringLiteral("timestampMs"), sample.timestampMs},
                                     {QStringLiteral("inputEvents"), sample.inputEvents}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("samples"), array}});
    } else if (action == QStringLiteral("listAppSegments")) {
        QJsonArray array;
        for (const AppSegment &segment : m_historyRecorder->listAppSegments(monitorStreamId, rangeStart, rangeStop)) {
            array.append(QJsonObject{{QStringLiteral("application"), segment.application},
                                     {QStringLiteral("startMs"), segment.startMs},
                                     {QStringLiteral("endMs"), segment.endMs},
                                     {QStringLiteral("title"), segment.title}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("segments"), array}});
    } else if (action == QStringLiteral("listRunningApplications")) {
        QJsonArray array;
        for (const AppUsage &usage : m_historyRecorder->listRunningApplications(monitorStreamId, rangeStart, rangeStop)) {
            array.append(QJsonObject{{QStringLiteral("application"), usage.application},
                                     {QStringLiteral("totalMs"), usage.totalMs},
                                     {QStringLiteral("category"), usage.category},
                                     {QStringLiteral("title"), usage.title}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("applications"), array}});
    } else if (action == QStringLiteral("listWebPages")) {
        QJsonArray array;
        for (const WebUsage &usage : m_historyRecorder->listWebPages(monitorStreamId, rangeStart, rangeStop)) {
            array.append(QJsonObject{{QStringLiteral("url"), usage.url},
                                     {QStringLiteral("totalMs"), usage.totalMs},
                                     {QStringLiteral("title"), usage.title}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("pages"), array}});
    } else if (action == QStringLiteral("listWebVisits")) {
        QJsonArray array;
        for (const WebVisit &visit : m_historyRecorder->listWebVisits(monitorStreamId, rangeStart, rangeStop)) {
            array.append(QJsonObject{{QStringLiteral("url"), visit.url},
                                     {QStringLiteral("startMs"), visit.startMs},
                                     {QStringLiteral("endMs"), visit.endMs},
                                     {QStringLiteral("title"), visit.title}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("visits"), array}});
    } else if (action == QStringLiteral("listCategories")) {
        QJsonArray array;
        const auto categories = m_historyRecorder->listCategories();
        for (const auto &entry : categories) {
            array.append(QJsonObject{{QStringLiteral("application"), entry.first},
                                     {QStringLiteral("category"), entry.second}});
        }
        // This screen's employee's own overrides, alongside the global ones.
        QJsonArray employeeArray;
        const QString username = m_historyRecorder->usernameForStream(monitorStreamId);
        for (const auto &entry : m_historyRecorder->listEmployeeCategories(username)) {
            employeeArray.append(QJsonObject{{QStringLiteral("application"), entry.first},
                                             {QStringLiteral("category"), entry.second}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("categories"), array},
                            {QStringLiteral("employeeCategories"), employeeArray}});
    } else if (action == QStringLiteral("setCategory")) {
        const QString application = request.value(QStringLiteral("application")).toString();
        const QString category = request.value(QStringLiteral("category")).toString();
        if (request.value(QStringLiteral("scope")).toString() == QStringLiteral("employee")) {
            m_historyRecorder->setEmployeeCategory(m_historyRecorder->usernameForStream(monitorStreamId),
                                                   application, category);
        } else {
            m_historyRecorder->setCategory(application, category);
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("application"), application},
                            {QStringLiteral("category"), category}});
    } else if (action == QStringLiteral("listKeystrokes")) {
        quint32 sessionId = 0;
        for (const MonitorInfo &monitor : std::as_const(m_monitors)) {
            if (monitor.streamId == monitorStreamId) {
                sessionId = monitor.sessionId;
                break;
            }
        }
        QJsonArray array;
        for (const KeystrokeEntry &entry : m_historyRecorder->listKeystrokes(sessionId, rangeStart, rangeStop)) {
            array.append(QJsonObject{{QStringLiteral("timestampMs"), entry.timestampMs},
                                     {QStringLiteral("windowTitle"), entry.windowTitle},
                                     {QStringLiteral("text"), entry.text}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("day"), day},
                            {QStringLiteral("entries"), array}});
    } else if (action == QStringLiteral("chartSeries")) {
        // ChartsModel.qml's Selector: kind "activity" (K_activity,
        // RF_serieInSessionsCombined) or "productivity" (K_byProductivity,
        // RF_multiSerieInSessionsCombinedSimple) over [startMs, stopMs) at
        // granulaMs. One serie per session marker -- this host records one
        // session (username) per employee.
        const QString kind = request.value(QStringLiteral("kind")).toString();
        const qint64 granulaMs = static_cast<qint64>(request.value(QStringLiteral("granulaMs")).toDouble());
        const bool productivity = kind == QStringLiteral("productivity");
        const QList<ChartPoint> points =
            productivity ? m_historyRecorder->chartProductivity(monitorStreamId, rangeStart, rangeStop, granulaMs)
                         : m_historyRecorder->chartActivity(monitorStreamId, rangeStart, rangeStop, granulaMs);
        QJsonArray moments;
        QJsonArray values;
        for (const ChartPoint &point : points) {
            moments.append(point.pointMs);
            if (productivity) {
                values.append(QJsonArray{point.volumes[0], point.volumes[1], point.volumes[2],
                                         point.volumes[3]});
            } else {
                values.append(point.value);
            }
        }
        QJsonArray series;
        if (!points.isEmpty()) {
            series.append(QJsonObject{
                {QStringLiteral("userName"), m_historyRecorder->usernameForStream(monitorStreamId)},
                {QStringLiteral("moment"), moments},
                {QStringLiteral("value"), values}});
        }
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), action},
                            {QStringLiteral("kind"), kind},
                            {QStringLiteral("tag"), request.value(QStringLiteral("tag"))},
                            {QStringLiteral("startMs"), rangeStart},
                            {QStringLiteral("stopMs"), rangeStop},
                            {QStringLiteral("granulaMs"), granulaMs},
                            {QStringLiteral("series"), series}});
    }
}

void AgentConnection::queueFrameRequest(quint32 monitorStreamId, qint64 timestampMs,
                                       quint64 requestId)
{
    if (!m_frameService) {
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), QStringLiteral("getFrame")},
                            {QStringLiteral("requestId"), static_cast<double>(requestId)},
                            {QStringLiteral("error"), QStringLiteral("Cadru negasit.")}});
        return;
    }
    // Only the newest position per screen survives: while the slider is
    // being dragged every superseded position is dropped here instead of
    // being read and decoded for nothing.
    if (!m_pendingFrames.contains(monitorStreamId)) {
        m_frameOrder.append(monitorStreamId);
    }
    m_pendingFrames.insert(monitorStreamId, PendingFrame{timestampMs, requestId});
    if (!m_frameInFlight) {
        dispatchNextRead();
    }
}

void AgentConnection::queueSegmentRequest(quint32 monitorStreamId, qint64 sequenceId,
                                         qint64 fromMs, qint64 toMs, quint64 requestId)
{
    if (!m_frameService) {
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), QStringLiteral("getSegment")},
                            {QStringLiteral("requestId"), static_cast<double>(requestId)},
                            {QStringLiteral("error"), QStringLiteral("Segment negasit.")}});
        return;
    }
    // Prefetch is best-effort: a viewer that keeps asking faster than the
    // disk can answer loses its oldest requests rather than growing a queue.
    while (m_pendingSegments.size() >= kMaxPendingSegments) {
        m_pendingSegments.dequeue();
    }
    m_pendingSegments.enqueue(PendingSegment{monitorStreamId, sequenceId, fromMs, toMs, requestId});
    // Deliberately not dispatched here: a viewer that is scrubbing sends the
    // frame it is waiting for alongside its prefetch, and both land in the
    // same read burst. Yielding a turn lets that frame be queued first, so
    // the picture never waits behind a background download.
    if (!m_frameInFlight && !m_segmentTimer.isActive()) {
        m_segmentTimer.start();
    }
}

void AgentConnection::dispatchNextRead()
{
    // A frame the user is looking at always goes before background prefetch.
    while (!m_frameOrder.isEmpty()) {
        const quint32 streamId = m_frameOrder.takeFirst();
        const auto it = m_pendingFrames.find(streamId);
        if (it == m_pendingFrames.end()) {
            continue;
        }
        const PendingFrame pending = it.value();
        m_pendingFrames.erase(it);
        m_frameInFlight = true;
        m_frameService->request(m_connectionId, streamId, pending.timestampMs, pending.requestId);
        return;
    }
    if (!m_pendingSegments.isEmpty()) {
        const PendingSegment pending = m_pendingSegments.dequeue();
        m_frameInFlight = true;
        m_frameService->requestSegment(m_connectionId, pending.streamId, pending.sequenceId,
                                       pending.fromMs, pending.toMs, pending.requestId);
        return;
    }
    m_frameInFlight = false;
    // Nothing left to read: let the charts/keystrokes through.
    scheduleDeferredQueries();
}

void AgentConnection::onSegmentRead(quint64 connectionId, quint32 monitorStreamId,
                                    quint64 requestId, const QByteArray &payload, qint64 readMs)
{
    if (connectionId != m_connectionId) {
        return;
    }
    if (payload.isEmpty()) {
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), QStringLiteral("getSegment")},
                            {QStringLiteral("requestId"), static_cast<double>(requestId)},
                            {QStringLiteral("error"), QStringLiteral("Segment negasit.")}});
    } else {
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::HistorySegment,
                                                       monitorStreamId, requestId, payload));
    }
    if (readMs >= 500) {
        emit logMessage(QStringLiteral("History: segmentul pentru ecranul %1 a durat %2 ms.")
                            .arg(monitorStreamId)
                            .arg(readMs));
    }
    dispatchNextRead();
}

void AgentConnection::onFrameRead(quint64 connectionId, quint32 monitorStreamId, qint64 timestampMs,
                                  quint64 requestId, const QByteArray &jpeg, qint64 decodeMs)
{
    if (connectionId != m_connectionId) {
        return; // another viewer's frame (the reader is shared)
    }
    if (jpeg.isEmpty()) {
        // The request id goes back with the miss too, so the viewer can tell
        // "the position you are on now has nothing" from a stale answer to a
        // position it has already left.
        sendJson(ViewerProtocol::MessageType::HistoryQuery, monitorStreamId,
                QJsonObject{{QStringLiteral("action"), QStringLiteral("getFrame")},
                            {QStringLiteral("requestId"), static_cast<double>(requestId)},
                            {QStringLiteral("timestampMs"), timestampMs},
                            {QStringLiteral("error"), QStringLiteral("Cadru negasit.")}});
    } else {
        // sequence carries the request id: HistoryFrame is a binary message,
        // and this is what lets the viewer drop an out-of-order answer.
        m_socket->write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::HistoryFrame,
                                                       monitorStreamId, requestId, jpeg,
                                                       timestampMs));
    }
    if (decodeMs >= 250) {
        emit logMessage(QStringLiteral("History: cadrul %1 (ecran %2) a durat %3 ms de citit.")
                            .arg(timestampMs)
                            .arg(monitorStreamId)
                            .arg(decodeMs));
    }
    dispatchNextRead();
}

void AgentConnection::scheduleDeferredQueries()
{
    if (m_deferred.isEmpty() || m_frameInFlight || m_deferredTimer.isActive()) {
        return;
    }
    m_deferredTimer.start();
}

void AgentConnection::processDeferredQueries()
{
    if (m_frameInFlight || m_deferred.isEmpty()) {
        return;
    }
    const DeferredQuery query = m_deferred.dequeue();
    runHistoryQuery(query.streamId, query.request);
    scheduleDeferredQueries();
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
