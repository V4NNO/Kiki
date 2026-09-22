#include "sessionmanager.h"

#include <QRandomGenerator>
#include <QSet>

#ifdef Q_OS_WIN
#include <userenv.h>
#include <wtsapi32.h>
#endif

namespace {
#ifdef Q_OS_WIN
QString wtsStateToString(WTS_CONNECTSTATE_CLASS state)
{
    switch (state) {
    case WTSActive: return QStringLiteral("active");
    case WTSConnected: return QStringLiteral("connected");
    case WTSDisconnected: return QStringLiteral("disconnected");
    case WTSIdle: return QStringLiteral("idle");
    default: return QStringLiteral("other");
    }
}
#endif

QString generatePipeName(quint32 sessionId)
{
    return QStringLiteral("PersonalScreenAgent-%1-%2")
        .arg(sessionId)
        .arg(QRandomGenerator::global()->generate(), 8, 16, QLatin1Char('0'));
}

QString generateSecret()
{
    return QStringLiteral("%1%2")
        .arg(QRandomGenerator::global()->generate64(), 16, 16, QLatin1Char('0'))
        .arg(QRandomGenerator::global()->generate64(), 16, 16, QLatin1Char('0'));
}
}

SessionManager::SessionManager(AgentServer *server, QString subServicePath, QObject *parent)
    : QObject(parent)
    , m_server(server)
    , m_subServicePath(std::move(subServicePath))
{
    m_pollTimer.setInterval(5000);
    connect(&m_pollTimer, &QTimer::timeout, this, &SessionManager::pollSessions);
    if (m_server) {
        connect(m_server, &AgentServer::connectionCountChanged, this,
                &SessionManager::onViewerCountChanged);
    }
}

SessionManager::~SessionManager()
{
    stopAll();
}

bool SessionManager::start(QString *error)
{
#ifndef Q_OS_WIN
    if (error) {
        *error = QStringLiteral("Enumerarea sesiunilor Windows nu e disponibila pe aceasta platforma.");
    }
    return false;
#else
    pollSessions();
    m_pollTimer.start();
    Q_UNUSED(error)
    return true;
#endif
}

void SessionManager::stopAll()
{
    m_pollTimer.stop();
    for (auto it = m_sessions.begin(); it != m_sessions.end(); ++it) {
        terminateSession(it.key(), it.value());
    }
    m_sessions.clear();
    m_globalMonitors.clear();
}

void SessionManager::pollSessions()
{
#ifdef Q_OS_WIN
    PWTS_SESSION_INFOW sessionInfo = nullptr;
    DWORD sessionCount = 0;
    if (!WTSEnumerateSessionsW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &sessionInfo, &sessionCount)) {
        emit logMessage(QStringLiteral("WTSEnumerateSessionsW a esuat (%1).").arg(GetLastError()));
        return;
    }

    QSet<quint32> seen;
    for (DWORD i = 0; i < sessionCount; ++i) {
        const WTS_SESSION_INFOW &info = sessionInfo[i];
        if (info.SessionId == 0 || info.State != WTSActive) {
            continue;
        }
        LPWSTR nameBuffer = nullptr;
        DWORD nameBytes = 0;
        QString username;
        if (WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, info.SessionId, WTSUserName,
                                        &nameBuffer, &nameBytes)) {
            username = QString::fromWCharArray(nameBuffer);
            WTSFreeMemory(nameBuffer);
        }
        if (username.isEmpty()) {
            continue; // Not an interactive user session (e.g. a listener slot).
        }

        seen.insert(info.SessionId);
        if (!m_sessions.contains(info.SessionId)) {
            launchSubService(info.SessionId, username, wtsStateToString(info.State));
        }
    }
    WTSFreeMemory(sessionInfo);

    const QList<quint32> tracked = m_sessions.keys();
    for (quint32 sessionId : tracked) {
        if (!seen.contains(sessionId)) {
            SessionEntry entry = m_sessions.take(sessionId);
            terminateSession(sessionId, entry);
            rebuildAndBroadcastMonitors();
        }
    }
#endif
}

bool SessionManager::launchSubService(quint32 sessionId, const QString &username,
                                      const QString &state)
{
#ifdef Q_OS_WIN
    const QString pipeName = generatePipeName(sessionId);
    const QString secret = generateSecret();

    const QString commandLine = QStringLiteral(
                                     "\"%1\" --pipe-name=%2 --pipe-secret=%3 --session-id=%4 "
                                     "--session-username=\"%5\"")
                                     .arg(m_subServicePath, pipeName, secret)
                                     .arg(sessionId)
                                     .arg(username);
    // CreateProcess(AsUser)W is documented to potentially write into
    // lpCommandLine, so this must be a private, mutable buffer -- never a
    // pointer into QString's (possibly shared) internal storage.
    std::wstring commandLineBuffer = commandLine.toStdWString();

    STARTUPINFOW startupInfo{};
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.lpDesktop = const_cast<LPWSTR>(L"winsta0\\default");
    PROCESS_INFORMATION processInfo{};

    bool launched = false;
    bool ownProcessFallback = false;

    // CreateProcessAsUserW additionally needs SE_ASSIGNPRIMARYTOKEN_NAME and
    // SE_INCREASE_QUOTA_NAME *enabled* in the caller's token, which even an
    // elevated Administrator does not have by default -- only LocalSystem
    // does. So for the session this process itself already runs in, skip
    // the impersonation dance entirely and just launch a normal child
    // process; that always works and needs no special privileges. Any
    // other session genuinely requires running as the SYSTEM service.
    DWORD ownSessionId = 0;
    ProcessIdToSessionId(GetCurrentProcessId(), &ownSessionId);

    if (ownSessionId == sessionId) {
        ownProcessFallback = true;
        launched = CreateProcessW(nullptr, commandLineBuffer.data(), nullptr, nullptr, FALSE,
                                  CREATE_NEW_CONSOLE, nullptr, nullptr, &startupInfo, &processInfo);
    } else {
        HANDLE userToken = nullptr;
        if (WTSQueryUserToken(sessionId, &userToken)) {
            HANDLE primaryToken = nullptr;
            LPVOID environment = nullptr;
            if (DuplicateTokenEx(userToken, MAXIMUM_ALLOWED, nullptr, SecurityIdentification,
                                 TokenPrimary, &primaryToken)) {
                CreateEnvironmentBlock(&environment, primaryToken, FALSE);
                launched = CreateProcessAsUserW(
                    primaryToken, nullptr, commandLineBuffer.data(), nullptr, nullptr, FALSE,
                    CREATE_UNICODE_ENVIRONMENT | CREATE_NEW_CONSOLE, environment, nullptr,
                    &startupInfo, &processInfo);
                if (environment) {
                    DestroyEnvironmentBlock(environment);
                }
                CloseHandle(primaryToken);
            } else {
                emit logMessage(QStringLiteral("DuplicateTokenEx a esuat (%1).").arg(GetLastError()));
            }
            CloseHandle(userToken);
        } else {
            emit logMessage(QStringLiteral(
                "Sesiunea %1 (%2) necesita rulare ca serviciu Windows (SYSTEM) pentru captura; "
                "sarita in modul curent de test.").arg(sessionId).arg(username));
            return false;
        }
    }

    if (!launched) {
        emit logMessage(QStringLiteral("Nu am putut porni PersonalSubService pentru sesiunea %1 (%2).")
                            .arg(sessionId)
                            .arg(GetLastError()));
        return false;
    }
    CloseHandle(processInfo.hThread);

    SessionEntry entry;
    entry.username = username;
    entry.state = state;
    entry.ownProcessFallback = ownProcessFallback;
    entry.process = processInfo;
    entry.ingest = new SessionIngest(pipeName, secret, sessionId, this);
    connect(entry.ingest, &SessionIngest::monitorDiscovered, this,
            &SessionManager::onMonitorDiscovered);
    connect(entry.ingest, &SessionIngest::frameReady, this, &SessionManager::onFrameReady);
    connect(entry.ingest, &SessionIngest::metadataChanged, this,
            &SessionManager::onMetadataChanged);
    connect(entry.ingest, &SessionIngest::keystrokeReceived, this,
            &SessionManager::onKeystrokeReceived);
    connect(entry.ingest, &SessionIngest::ingestDisconnected, this,
            &SessionManager::onIngestDisconnected);
    connect(entry.ingest, &SessionIngest::connectFailed, this,
            &SessionManager::onIngestDisconnected);
    connect(entry.ingest, &SessionIngest::logMessage, this, &SessionManager::logMessage);

    m_sessions.insert(sessionId, entry);
    entry.ingest->start();
    emit logMessage(QStringLiteral("PersonalSubService pornit pentru sesiunea %1 (%2)%3.")
                        .arg(sessionId)
                        .arg(username)
                        .arg(ownProcessFallback
                                 ? QStringLiteral(" [test local, fara elevare]")
                                 : QString()));
    return true;
#else
    Q_UNUSED(sessionId)
    Q_UNUSED(username)
    Q_UNUSED(state)
    return false;
#endif
}

void SessionManager::terminateSession(quint32 sessionId, SessionEntry &entry)
{
    if (entry.ingest) {
        entry.ingest->requestShutdown();
        entry.ingest->deleteLater();
        entry.ingest = nullptr;
    }
#ifdef Q_OS_WIN
    if (entry.process.hProcess) {
        WaitForSingleObject(entry.process.hProcess, 2000);
        TerminateProcess(entry.process.hProcess, 0);
        CloseHandle(entry.process.hProcess);
        entry.process.hProcess = nullptr;
    }
#endif
    for (quint32 globalId : std::as_const(entry.localToGlobal)) {
        m_globalMonitors.remove(globalId);
    }
    emit logMessage(QStringLiteral("Sesiunea %1 (%2) a fost inchisa.")
                        .arg(sessionId)
                        .arg(entry.username));
}

quint32 SessionManager::allocateGlobalStreamId()
{
    return m_nextGlobalStreamId++;
}

void SessionManager::onMonitorDiscovered(quint32 sessionId, quint32 localStreamId,
                                         const QString &name, const QSize &size, bool isWindow)
{
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end()) {
        return;
    }
    SessionEntry &entry = it.value();
    quint32 globalId = entry.localToGlobal.value(localStreamId, 0);
    if (globalId == 0) {
        globalId = allocateGlobalStreamId();
        entry.localToGlobal.insert(localStreamId, globalId);
        // First monitor discovered for this session means its pipe just
        // came up; bring it up to speed on the current viewer count right
        // away instead of leaving it at idle FPS until the count next
        // changes (which might be never, if viewers connected before this
        // session started).
        entry.ingest->sendViewerCount(m_viewerCount);
    }

    MonitorInfo info;
    info.streamId = globalId;
    info.name = name;
    info.size = size;
    info.sessionId = sessionId;
    info.sessionUsername = entry.username;
    info.sessionState = entry.state;
    info.isWindow = isWindow;
    m_globalMonitors.insert(globalId, info);
    rebuildAndBroadcastMonitors();
}

void SessionManager::onFrameReady(quint32 sessionId, quint32 localStreamId, const QImage &image)
{
    auto it = m_sessions.constFind(sessionId);
    if (it == m_sessions.constEnd()) {
        return;
    }
    const quint32 globalId = it.value().localToGlobal.value(localStreamId, 0);
    if (globalId == 0) {
        return;
    }
    if (m_server) {
        m_server->broadcastFrame(globalId, image);
    }
    const MonitorInfo info = m_globalMonitors.value(globalId);
    // The ActiveWindowCapture live-preview stream isn't a monitor and
    // updates at its own 1fps regardless of viewer activity -- recording it
    // into history would just fill the timeline with window screenshots
    // nobody asked to keep.
    if (m_historyRecorder && !info.isWindow) {
        m_historyRecorder->recordFrame(sessionId, info.sessionUsername, globalId, info.name, image);
    }
}

void SessionManager::onMetadataChanged(quint32 sessionId, quint32 localStreamId,
                                       const QString &application, const QString &idleText,
                                       int inputEvents, const QString &url)
{
    auto it = m_sessions.constFind(sessionId);
    if (it == m_sessions.constEnd()) {
        return;
    }
    const quint32 globalId = it.value().localToGlobal.value(localStreamId, 0);
    if (globalId == 0) {
        return;
    }
    if (m_server) {
        m_server->broadcastMetadata(globalId, application, idleText);
    }
    if (m_historyRecorder) {
        m_historyRecorder->noteApplication(sessionId, globalId, application);
        m_historyRecorder->noteUrl(sessionId, globalId, url);
        // inputEvents is only meaningful on the periodic 10s sample tick
        // (see PersonalSubService main.cpp); app-change-triggered metadata
        // pushes always carry 0, so this naturally only records real
        // samples instead of a row per app switch too.
        if (inputEvents > 0) {
            m_historyRecorder->recordActivity(sessionId, globalId, inputEvents);
        }
    }
}

void SessionManager::onKeystrokeReceived(quint32 sessionId, const QString &windowTitle,
                                         const QString &text)
{
    if (m_historyRecorder) {
        m_historyRecorder->recordKeystroke(sessionId, windowTitle, text);
    }
}

void SessionManager::onIngestDisconnected(quint32 sessionId)
{
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end()) {
        return;
    }
    SessionEntry entry = m_sessions.take(sessionId);
    terminateSession(sessionId, entry);
    rebuildAndBroadcastMonitors();
}

void SessionManager::onViewerCountChanged(int count)
{
    m_viewerCount = count;
    for (SessionEntry &entry : m_sessions) {
        if (entry.ingest) {
            entry.ingest->sendViewerCount(count);
        }
    }
}

void SessionManager::rebuildAndBroadcastMonitors()
{
    if (!m_server) {
        return;
    }
    m_server->broadcastMonitorList(m_globalMonitors.values());
}
