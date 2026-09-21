#include "viewerconnection.h"

#include <QCryptographicHash>
#include <QDataStream>
#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QSslCertificate>

namespace {
QString normalizedFingerprint(QString value)
{
    value.remove(QLatin1Char(':'));
    value.remove(QLatin1Char(' '));
    return value.trimmed().toLower();
}
}

ViewerConnection::ViewerConnection(QObject *parent)
    : QObject(parent)
{
    connect(&m_socket, &QSslSocket::connected, this, &ViewerConnection::onSocketConnected);
    connect(&m_socket, &QSslSocket::encrypted, this, &ViewerConnection::onSocketEncrypted);
    connect(&m_socket, &QSslSocket::readyRead, this, &ViewerConnection::onReadyRead);
    connect(&m_socket, &QSslSocket::errorOccurred, this, &ViewerConnection::onSocketError);
    connect(&m_socket, &QSslSocket::sslErrors, this, &ViewerConnection::onSslErrors);
    connect(&m_demoTimer, &QTimer::timeout, this, &ViewerConnection::renderDemoFrame);
    m_demoTimer.setInterval(100);
}

void ViewerConnection::connectToAgent(const QString &host, quint16 port,
                                      const QString &token, bool useTls,
                                      const QString &certificateSha256)
{
    stopDemo();
    m_socket.abort();
    m_host = host.trimmed();
    m_token = token;
    m_useTls = useTls;
    m_certificateSha256 = normalizedFingerprint(certificateSha256);
    m_frameReader.clear();
    m_streams.clear();

    if (m_host.isEmpty() || port == 0) {
        emit protocolError(QStringLiteral("Adresa si portul sunt obligatorii."));
        return;
    }
    if (!m_useTls && !m_token.isEmpty() && !isSafePlainTextTarget()) {
        emit protocolError(QStringLiteral(
            "Tokenul nu este trimis necriptat catre o adresa externa. Activati TLS."));
        return;
    }

    emit statusChanged(QStringLiteral("Conectare la %1:%2...").arg(m_host).arg(port), false);
    if (m_useTls) {
        m_socket.connectToHostEncrypted(m_host, port);
    } else {
        m_socket.connectToHost(m_host, port);
    }
}

void ViewerConnection::disconnectFromAgent()
{
    if (m_socket.state() != QAbstractSocket::UnconnectedState) {
        m_socket.disconnectFromHost();
    }
}

void ViewerConnection::startDemo()
{
    disconnectFromAgent();
    m_demoMode = true;
    m_demoSequence = 0;
    m_streams.clear();
    emit agentIdentified(QStringLiteral("Demo organization"), QStringLiteral("Tracker grid"));
    const QStringList names{
        QStringLiteral("Alex Morgan Operations Manager (offline since 09:14)"),
        QStringLiteral("Andrei Popescu Field Support Specialist"),
        QStringLiteral("Bianca Ionescu Business Development Specialist"),
        QStringLiteral("Carmen Radu Accounts Payable Specialist"),
        QStringLiteral("Cristian Marin Construction Manager"),
        QStringLiteral("Dan Munteanu Business Development Specialist"),
        QStringLiteral("Diana Pavel Design Engineer"),
        QStringLiteral("Elena Stoica Project Manager"),
        QStringLiteral("Florin Matei Vendor Acquisition"),
        QStringLiteral("Gabriel Ene Team Coordinator (offline)"),
        QStringLiteral("Ioana Dumitru Production Analyst"),
        QStringLiteral("Marian Tudor Fleet Specialist"),
        QStringLiteral("Nicoleta Stan Compliance Coordinator"),
        QStringLiteral("Ovidiu Ilie Application Support (offline)"),
        QStringLiteral("Roxana Vasile HR & Payroll Specialist")};
    for (int index = 0; index < names.size(); ++index) {
        emit monitorDiscovered(static_cast<quint32>(index + 1), names.at(index),
                               QSize(1600, 900), 0, QString(), QString());
    }
    emit statusChanged(QStringLiteral("Mod demonstrativ"), true);
    m_demoTimer.start();
    renderDemoFrame();
}

void ViewerConnection::stopDemo()
{
    if (!m_demoMode) {
        return;
    }
    m_demoMode = false;
    m_demoTimer.stop();
}

bool ViewerConnection::isConnected() const
{
    return m_demoMode || m_socket.state() == QAbstractSocket::ConnectedState;
}

void ViewerConnection::onSocketConnected()
{
    if (!m_useTls) {
        sendClientHello();
        emit statusChanged(QStringLiteral("Conectat; autentificare in curs..."), false);
    }
}

void ViewerConnection::onSocketEncrypted()
{
    sendClientHello();
    emit statusChanged(QStringLiteral("TLS activ; autentificare in curs..."), false);
}

void ViewerConnection::onReadyRead()
{
    m_frameReader.append(m_socket.readAll());
    if (m_frameReader.bufferedSize() > static_cast<qsizetype>(ViewerProtocol::MaxPayloadSize)
                                          + ViewerProtocol::HeaderSize) {
        failProtocol(QStringLiteral("Bufferul de intrare a depasit limita admisa."));
        return;
    }
    parseAvailableMessages();
}

void ViewerConnection::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    if (!m_demoMode) {
        emit statusChanged(QStringLiteral("Deconectat"), false);
        emit protocolError(m_socket.errorString());
    }
}

void ViewerConnection::onSslErrors(const QList<QSslError> &errors)
{
    const QSslCertificate peer = m_socket.peerCertificate();
    const QString actual = QString::fromLatin1(peer.digest(QCryptographicHash::Sha256).toHex());
    if (!m_certificateSha256.isEmpty()
        && normalizedFingerprint(actual) == m_certificateSha256) {
        m_socket.ignoreSslErrors(errors);
        return;
    }

    QStringList messages;
    for (const QSslError &error : errors) {
        messages.append(error.errorString());
    }
    emit protocolError(QStringLiteral("Certificatul TLS nu este de incredere: %1")
                           .arg(messages.join(QStringLiteral("; "))));
}

void ViewerConnection::sendClientHello()
{
    QJsonObject object{
        {QStringLiteral("client"), QStringLiteral("PersonalScreenViewer")},
        {QStringLiteral("version"), QStringLiteral("0.1.0")},
        {QStringLiteral("token"), m_token},
        {QStringLiteral("capabilities"), QJsonObject{
             {QStringLiteral("video"), true},
             {QStringLiteral("deltaFrames"), true},
             {QStringLiteral("audio"), false},
             {QStringLiteral("remoteControl"), false},
             {QStringLiteral("clipboard"), false},
             {QStringLiteral("fileTransfer"), false},
             {QStringLiteral("keylogging"), false}}}
    };
    const QByteArray payload = QJsonDocument(object).toJson(QJsonDocument::Compact);
    m_socket.write(ViewerProtocol::encodeMessage(
        ViewerProtocol::MessageType::ClientHello, 0, 0, payload));
}

void ViewerConnection::parseAvailableMessages()
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
            failProtocol(error);
            return;
        }
        processMessage(header, payload);
    }
}

void ViewerConnection::processMessage(const ViewerProtocol::Header &header,
                                      const QByteArray &payload)
{
    using ViewerProtocol::MessageType;
    switch (header.type) {
    case MessageType::AgentHello:
    case MessageType::Metadata:
    case MessageType::AuthResult:
        processJsonMessage(header, payload);
        break;
    case MessageType::FullFrame:
        processFullFrame(header, payload);
        break;
    case MessageType::DeltaFrame:
        processDeltaFrame(header, payload);
        break;
    case MessageType::HistoryQuery:
        processHistoryQuery(header, payload);
        break;
    case MessageType::HistoryFrame:
        processHistoryFrame(header, payload);
        break;
    case MessageType::Heartbeat:
        break;
    case MessageType::Goodbye:
        emit statusChanged(QStringLiteral("Agentul a inchis sesiunea"), false);
        m_socket.disconnectFromHost();
        break;
    case MessageType::ClientHello:
        failProtocol(QStringLiteral("Agentul a trimis un tip de mesaj rezervat viewer-ului."));
        break;
    default:
        failProtocol(QStringLiteral("Tip de mesaj necunoscut."));
        break;
    }
}

void ViewerConnection::processJsonMessage(const ViewerProtocol::Header &header,
                                          const QByteArray &payload)
{
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        failProtocol(QStringLiteral("Mesaj JSON invalid."));
        return;
    }
    const QJsonObject object = document.object();

    if (header.type == ViewerProtocol::MessageType::AuthResult) {
        if (!object.value(QStringLiteral("accepted")).toBool()) {
            failProtocol(QStringLiteral("Autentificarea a fost refuzata de agent."));
            return;
        }
        emit statusChanged(QStringLiteral("Conectat si autentificat"), true);
        return;
    }
    if (header.type == ViewerProtocol::MessageType::AgentHello) {
        emit agentIdentified(object.value(QStringLiteral("agentName")).toString(
                                 QStringLiteral("Agent necunoscut")),
                             object.value(QStringLiteral("sessionName")).toString());
        const QJsonObject monitor = object.value(QStringLiteral("monitor")).toObject();
        if (header.streamId != 0) {
            StreamState &state = m_streams[header.streamId];
            state.name = monitor.value(QStringLiteral("name")).toString(
                QStringLiteral("Monitor %1").arg(header.streamId));
            const QJsonObject session = object.value(QStringLiteral("session")).toObject();
            emit monitorDiscovered(header.streamId, state.name,
                                   QSize(monitor.value(QStringLiteral("width")).toInt(),
                                         monitor.value(QStringLiteral("height")).toInt()),
                                   static_cast<quint32>(session.value(QStringLiteral("id")).toInt()),
                                   session.value(QStringLiteral("username")).toString(),
                                   session.value(QStringLiteral("state")).toString());
        }
        return;
    }
    emit metadataChanged(header.streamId,
                         object.value(QStringLiteral("application")).toString(),
                         object.value(QStringLiteral("idle")).toString());
}

void ViewerConnection::processFullFrame(const ViewerProtocol::Header &header,
                                        const QByteArray &payload)
{
    QImage image;
    if (!image.loadFromData(payload) || image.isNull()
        || image.width() > 16384 || image.height() > 16384) {
        failProtocol(QStringLiteral("Cadru complet invalid pentru fluxul %1.")
                         .arg(header.streamId));
        return;
    }
    StreamState &state = m_streams[header.streamId];
    if (state.name.isEmpty()) {
        state.name = QStringLiteral("Monitor %1").arg(header.streamId);
        emit monitorDiscovered(header.streamId, state.name, image.size(), 0, QString(), QString());
    }
    state.image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    state.sequence = header.sequence;
    const qint64 latency = qMax<qint64>(0, QDateTime::currentMSecsSinceEpoch()
                                              - header.timestampMs);
    emit frameReady(header.streamId, state.image, header.sequence, latency);
}

void ViewerConnection::processDeltaFrame(const ViewerProtocol::Header &header,
                                         const QByteArray &payload)
{
    StreamState &state = m_streams[header.streamId];
    if (state.image.isNull() || payload.size() < 16
        || header.sequence != state.sequence + 1) {
        return;
    }
    QByteArray coordinates = payload.first(16);
    QDataStream stream(&coordinates, QIODevice::ReadOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    qint32 x = 0;
    qint32 y = 0;
    qint32 width = 0;
    qint32 height = 0;
    stream >> x >> y >> width >> height;
    const QRect target(x, y, width, height);
    if (stream.status() != QDataStream::Ok || width <= 0 || height <= 0
        || !state.image.rect().contains(target)) {
        return;
    }
    QImage patch;
    if (!patch.loadFromData(payload.mid(16)) || patch.size() != target.size()) {
        return;
    }
    QPainter painter(&state.image);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(target.topLeft(), patch);
    painter.end();
    state.sequence = header.sequence;
    const qint64 latency = qMax<qint64>(0, QDateTime::currentMSecsSinceEpoch()
                                              - header.timestampMs);
    emit frameReady(header.streamId, state.image, header.sequence, latency);
}

void ViewerConnection::requestHistoryDays(quint32 streamId)
{
    sendHistoryQuery(streamId, QJsonObject{{QStringLiteral("action"), QStringLiteral("listDays")}});
}

void ViewerConnection::requestHistoryFrames(quint32 streamId, const QString &day)
{
    sendHistoryQuery(streamId, QJsonObject{{QStringLiteral("action"), QStringLiteral("listFrames")},
                                           {QStringLiteral("day"), day}});
}

void ViewerConnection::requestHistoryFrame(quint32 streamId, qint64 timestampMs)
{
    sendHistoryQuery(streamId, QJsonObject{{QStringLiteral("action"), QStringLiteral("getFrame")},
                                           {QStringLiteral("timestampMs"), timestampMs}});
}

void ViewerConnection::requestHistoryActivity(quint32 streamId, const QString &day)
{
    sendHistoryQuery(streamId, QJsonObject{{QStringLiteral("action"), QStringLiteral("listActivity")},
                                           {QStringLiteral("day"), day}});
}

void ViewerConnection::requestHistoryAppSegments(quint32 streamId, const QString &day)
{
    sendHistoryQuery(streamId,
                     QJsonObject{{QStringLiteral("action"), QStringLiteral("listAppSegments")},
                                 {QStringLiteral("day"), day}});
}

void ViewerConnection::requestRunningApplications(quint32 streamId, const QString &day)
{
    sendHistoryQuery(streamId,
                     QJsonObject{{QStringLiteral("action"), QStringLiteral("listRunningApplications")},
                                 {QStringLiteral("day"), day}});
}

void ViewerConnection::requestCategories(quint32 streamId)
{
    sendHistoryQuery(streamId, QJsonObject{{QStringLiteral("action"), QStringLiteral("listCategories")}});
}

void ViewerConnection::setAppCategory(quint32 streamId, const QString &application,
                                      const QString &category)
{
    sendHistoryQuery(streamId, QJsonObject{{QStringLiteral("action"), QStringLiteral("setCategory")},
                                           {QStringLiteral("application"), application},
                                           {QStringLiteral("category"), category}});
}

void ViewerConnection::requestKeystrokes(quint32 streamId, const QString &day)
{
    sendHistoryQuery(streamId, QJsonObject{{QStringLiteral("action"), QStringLiteral("listKeystrokes")},
                                           {QStringLiteral("day"), day}});
}

void ViewerConnection::sendHistoryQuery(quint32 streamId, const QJsonObject &object)
{
    const QByteArray payload = QJsonDocument(object).toJson(QJsonDocument::Compact);
    m_socket.write(ViewerProtocol::encodeMessage(ViewerProtocol::MessageType::HistoryQuery, streamId,
                                                 0, payload));
}

void ViewerConnection::processHistoryQuery(const ViewerProtocol::Header &header,
                                           const QByteArray &payload)
{
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(payload, &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        return;
    }
    const QJsonObject object = document.object();
    if (object.contains(QStringLiteral("error"))) {
        emit historyError(header.streamId, object.value(QStringLiteral("error")).toString());
        return;
    }
    const QString action = object.value(QStringLiteral("action")).toString();
    if (action == QStringLiteral("listDays")) {
        QStringList days;
        for (const QJsonValue &value : object.value(QStringLiteral("days")).toArray()) {
            days.append(value.toString());
        }
        emit historyDaysReceived(header.streamId, days);
    } else if (action == QStringLiteral("listFrames")) {
        QList<qint64> timestamps;
        for (const QJsonValue &value : object.value(QStringLiteral("timestamps")).toArray()) {
            timestamps.append(static_cast<qint64>(value.toDouble()));
        }
        emit historyFramesReceived(header.streamId, object.value(QStringLiteral("day")).toString(),
                                   timestamps);
    } else if (action == QStringLiteral("listActivity")) {
        QList<HistoryActivitySample> samples;
        for (const QJsonValue &value : object.value(QStringLiteral("samples")).toArray()) {
            const QJsonObject entry = value.toObject();
            samples.append(HistoryActivitySample{
                static_cast<qint64>(entry.value(QStringLiteral("timestampMs")).toDouble()),
                entry.value(QStringLiteral("inputEvents")).toInt()});
        }
        emit historyActivityReceived(header.streamId, object.value(QStringLiteral("day")).toString(),
                                     samples);
    } else if (action == QStringLiteral("listAppSegments")) {
        QList<HistoryAppSegment> segments;
        for (const QJsonValue &value : object.value(QStringLiteral("segments")).toArray()) {
            const QJsonObject entry = value.toObject();
            segments.append(HistoryAppSegment{
                entry.value(QStringLiteral("application")).toString(),
                static_cast<qint64>(entry.value(QStringLiteral("startMs")).toDouble()),
                static_cast<qint64>(entry.value(QStringLiteral("endMs")).toDouble())});
        }
        emit historyAppSegmentsReceived(header.streamId, object.value(QStringLiteral("day")).toString(),
                                        segments);
    } else if (action == QStringLiteral("listRunningApplications")) {
        QList<HistoryAppUsage> applications;
        for (const QJsonValue &value : object.value(QStringLiteral("applications")).toArray()) {
            const QJsonObject entry = value.toObject();
            applications.append(HistoryAppUsage{
                entry.value(QStringLiteral("application")).toString(),
                static_cast<qint64>(entry.value(QStringLiteral("totalMs")).toDouble()),
                entry.value(QStringLiteral("category")).toString()});
        }
        emit historyRunningApplicationsReceived(
            header.streamId, object.value(QStringLiteral("day")).toString(), applications);
    } else if (action == QStringLiteral("listCategories")) {
        QHash<QString, QString> categories;
        for (const QJsonValue &value : object.value(QStringLiteral("categories")).toArray()) {
            const QJsonObject entry = value.toObject();
            categories.insert(entry.value(QStringLiteral("application")).toString(),
                              entry.value(QStringLiteral("category")).toString());
        }
        emit historyCategoriesReceived(header.streamId, categories);
    } else if (action == QStringLiteral("setCategory")) {
        // Ack-only; the Running Applications panel already updated itself
        // optimistically, nothing further to do here.
    } else if (action == QStringLiteral("listKeystrokes")) {
        QList<HistoryKeystrokeEntry> entries;
        for (const QJsonValue &value : object.value(QStringLiteral("entries")).toArray()) {
            const QJsonObject entry = value.toObject();
            entries.append(HistoryKeystrokeEntry{
                static_cast<qint64>(entry.value(QStringLiteral("timestampMs")).toDouble()),
                entry.value(QStringLiteral("windowTitle")).toString(),
                entry.value(QStringLiteral("text")).toString()});
        }
        emit historyKeystrokesReceived(header.streamId, object.value(QStringLiteral("day")).toString(),
                                       entries);
    }
}

void ViewerConnection::processHistoryFrame(const ViewerProtocol::Header &header,
                                           const QByteArray &payload)
{
    QImage image;
    if (!image.loadFromData(payload) || image.isNull()) {
        emit historyError(header.streamId, QStringLiteral("Cadru de istoric invalid."));
        return;
    }
    emit historyFrameReceived(header.streamId, header.timestampMs, image);
}

void ViewerConnection::failProtocol(const QString &message)
{
    emit protocolError(message);
    emit statusChanged(QStringLiteral("Eroare de protocol"), false);
    m_socket.abort();
    m_frameReader.clear();
}

bool ViewerConnection::isSafePlainTextTarget() const
{
    const QHostAddress address(m_host);
    return m_host.compare(QStringLiteral("localhost"), Qt::CaseInsensitive) == 0
        || address.isLoopback();
}

void ViewerConnection::renderDemoFrame()
{
    ++m_demoSequence;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    const QList<QPair<quint32, QSize>> displays{
        {2, QSize(1600, 900)}, {3, QSize(1600, 900)}, {5, QSize(1600, 900)},
        {6, QSize(1600, 900)}, {8, QSize(1600, 900)}, {9, QSize(1600, 900)},
        {11, QSize(1600, 900)}, {13, QSize(1600, 900)}};

    for (const auto &display : displays) {
        QImage image(display.second, QImage::Format_ARGB32_Premultiplied);
        const QList<QColor> backgrounds{
            QColor(26, 34, 48), QColor(230, 235, 239), QColor(238, 221, 167),
            QColor(28, 31, 38), QColor(238, 240, 243), QColor(20, 24, 31)};
        image.fill(backgrounds.at(static_cast<int>(display.first % backgrounds.size())));
        QPainter painter(&image);
        painter.setRenderHint(QPainter::Antialiasing);
        const int offset = static_cast<int>((m_demoSequence * 7) % image.width());
        QLinearGradient gradient(0, 0, image.width(), image.height());
        gradient.setColorAt(0.0, QColor(37, 99, 235));
        gradient.setColorAt(1.0, QColor(124, 58, 237));
        painter.setBrush(gradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(QRect(offset - 260, image.height() / 3, 420, 240), 28, 28);
        painter.setPen(display.first % 3 == 0 ? QColor(30, 38, 48) : QColor(240, 244, 255));
        QFont titleFont(QStringLiteral("Segoe UI"), 34, QFont::DemiBold);
        painter.setFont(titleFont);
        painter.drawText(QRect(60, 60, image.width() - 120, 100),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         display.first % 2 == 0
                             ? QStringLiteral("Project workspace")
                             : QStringLiteral("Business dashboard"));
        painter.setFont(QFont(QStringLiteral("Segoe UI"), 20));
        painter.setPen(QColor(165, 180, 252));
        painter.drawText(QRect(60, 160, image.width() - 120, 80),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         QStringLiteral("Authorized demo stream • Station %1").arg(display.first));
        painter.setPen(QColor(203, 213, 225));
        painter.drawText(QRect(60, image.height() - 120, image.width() - 120, 60),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         QDateTime::currentDateTime().toString(QStringLiteral("dd.MM.yyyy  HH:mm:ss")));
        painter.end();
        emit frameReady(display.first, image, m_demoSequence,
                        qMax<qint64>(0, QDateTime::currentMSecsSinceEpoch() - now));
        emit metadataChanged(display.first,
                             display.first % 3 == 0 ? QStringLiteral("ms-teams.exe")
                                                    : QStringLiteral("chrome.exe"),
                             QStringLiteral("Browser is active"));
    }

    emit metadataChanged(4, QStringLiteral("microsoft.lockapp"),
                         QStringLiteral("Locked 00:10:15"));
    emit metadataChanged(7, QStringLiteral("microsoft.lockapp"),
                         QStringLiteral("Locked 00:06:34"));
    emit metadataChanged(12, QStringLiteral("microsoft.lockapp"),
                         QStringLiteral("Locked 00:36:52"));
    emit metadataChanged(15, QStringLiteral("microsoft.lockapp"),
                         QStringLiteral("Locked 00:01:31"));
}
