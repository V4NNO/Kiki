#pragma once

#include "agentserver.h"
#include "historyrecorder.h"
#include "screencapture.h"
#include "sessioningest.h"

#include <QHash>
#include <QObject>
#include <QTimer>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

// Owns the whole "one capture per Windows session" story: enumerates
// sessions, launches a KikiSubService helper in each interactive one,
// ingests its frames, and is the single place that turns a
// (sessionId, localStreamId) pair into the globally unique streamId real
// viewers see (AgentServer/AgentConnection only ever deal in that global
// space, unchanged from the single-session KikiAgent).
class SessionManager final : public QObject
{
    Q_OBJECT

public:
    SessionManager(AgentServer *server, QString subServicePath, QObject *parent = nullptr);
    ~SessionManager() override;

    // Kicks off the first enumeration and starts the periodic rescan.
    // Returns false with *error set if the very first enumeration fails
    // outright (e.g. WTS not available at all).
    bool start(QString *error);
    void stopAll();

    // Optional; when set, every captured frame is also offered to the
    // recorder (which internally rate-limits to its own low-frequency
    // history timeline, independent of the live viewer stream).
    void setHistoryRecorder(HistoryRecorder *recorder) { m_historyRecorder = recorder; }

signals:
    void logMessage(const QString &message);

private slots:
    void pollSessions();
    void onMonitorDiscovered(quint32 sessionId, quint32 localStreamId, const QString &name,
                             const QSize &size, bool isWindow);
    void onFrameReady(quint32 sessionId, quint32 localStreamId, const QImage &image);
    void onMetadataChanged(quint32 sessionId, quint32 localStreamId, const QString &application,
                           const QString &idleText, int inputEvents, const QString &url,
                           quint32 activeMonitorLocalStreamId, const QString &windowTitle);
    void onKeystrokeReceived(quint32 sessionId, const QString &windowTitle, const QString &text);
    // See MessageType::StreamClosed / SessionIngest::streamClosed.
    void onStreamClosed(quint32 sessionId, quint32 localStreamId);
    void onIngestDisconnected(quint32 sessionId);
    void onViewerCountChanged(int count);

private:
    struct SessionEntry {
        QString username;
        QString state;
        bool ownProcessFallback = false;
#ifdef Q_OS_WIN
        PROCESS_INFORMATION process{};
#endif
        SessionIngest *ingest = nullptr;
        QHash<quint32, quint32> localToGlobal;
    };

    bool launchSubService(quint32 sessionId, const QString &username, const QString &state);
    void terminateSession(quint32 sessionId, SessionEntry &entry);
    void rebuildAndBroadcastMonitors();
    quint32 allocateGlobalStreamId();

    AgentServer *m_server = nullptr;
    HistoryRecorder *m_historyRecorder = nullptr;
    QString m_subServicePath;
    QTimer m_pollTimer;
    QHash<quint32, SessionEntry> m_sessions; // key: Windows session id
    QHash<quint32, MonitorInfo> m_globalMonitors; // key: global streamId
    // Transient ids (window streams, or no history database) start far above
    // the persisted per-monitor ids so the two can never collide.
    quint32 m_nextGlobalStreamId = 1000000;
    // Last count broadcast by AgentServer; re-sent to a session's
    // sub-service as soon as it's discovered so a session that starts up
    // after viewers are already connected doesn't sit at idle FPS.
    int m_viewerCount = 0;
};
