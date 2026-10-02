#include "activityprobe.h"

#include <QDateTime>
#include <QTimer>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <psapi.h>

namespace {
// Same technique as WindowListCapture's lock detection (see
// windowlistcapture.cpp): OpenInputDesktop() failing is the standard way to
// tell "not on the interactive desktop right now" (locked, UAC secure
// desktop, etc.) apart from merely idle.
bool isSessionLocked()
{
    HDESK desktop = OpenInputDesktop(0, FALSE, DESKTOP_READOBJECTS);
    if (!desktop) {
        return true;
    }
    CloseDesktop(desktop);
    return false;
}

// The real grabber's sessionStateShooter reports inSaver via
// SystemParametersInfoA(SPI_GETSCREENSAVERRUNNING) on a timer
// (tlsservice.exe FUN_14005cab0); the node stores it in
// online_session_saver, which -- like lock and idle -- subtracts from
// volume_activity.
bool isScreensaverRunning()
{
    BOOL running = FALSE;
    if (!SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &running, 0)) {
        return false;
    }
    return running != FALSE;
}

// Seconds since the last keyboard/mouse input, as the grabber's InputTracker
// reports inputIdlesSec (tlsservice.exe): GetLastInputInfo is the combined
// last-input clock, i.e. the "both devices idle" value the node turns into
// an online_session_idle range (threshold 1s, FUN_140597870). Unlike the
// human-facing idleText below, this is reported continuously, with no 60s
// floor, so History's Activity can subtract short idle gaps the way the
// original does.
double idleSecondsNow()
{
    LASTINPUTINFO lastInput;
    lastInput.cbSize = sizeof(LASTINPUTINFO);
    if (!GetLastInputInfo(&lastInput)) {
        return 0.0;
    }
    return static_cast<double>(GetTickCount() - lastInput.dwTime) / 1000.0;
}
}

class ActivityProbe::Worker : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

public slots:
    void init()
    {
        auto *timer = new QTimer(this);
        timer->setInterval(2000);
        connect(timer, &QTimer::timeout, this, &Worker::poll);
        timer->start();
        poll();
    }

signals:
    // idleSeconds / screensaver are the raw session-state the node turns into
    // online_session_idle / online_session_saver ranges; idleText is only the
    // human-facing "Idle/Locked HH:MM:SS" the tile shows (60s floor kept).
    void activityChanged(const QString &application, const QString &idleText, double idleSeconds,
                         bool screensaver);

private slots:
    void poll()
    {
        QString application = QStringLiteral("desktop");
        HWND foreground = GetForegroundWindow();
        if (foreground) {
            DWORD processId = 0;
            GetWindowThreadProcessId(foreground, &processId);
            if (processId != 0) {
                HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
                if (process) {
                    wchar_t path[MAX_PATH] = {0};
                    DWORD size = MAX_PATH;
                    if (QueryFullProcessImageNameW(process, 0, path, &size)) {
                        const QString fullPath = QString::fromWCharArray(path, static_cast<int>(size));
                        const int slash = fullPath.lastIndexOf(QLatin1Char('\\'));
                        application = slash >= 0 ? fullPath.mid(slash + 1) : fullPath;
                    }
                    CloseHandle(process);
                }
            }
        }

        const bool locked = isSessionLocked();
        // Raw idle seconds and screensaver state for history; the tile text is
        // derived from them but keeps its 60s idle floor.
        const double idleSeconds = locked ? 0.0 : idleSecondsNow();
        const bool screensaver = locked ? false : isScreensaverRunning();

        QString idleText;
        if (locked) {
            const qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
            if (m_lockStartMs == 0) {
                m_lockStartMs = nowMs;
            }
            const qint64 lockedSeconds = (nowMs - m_lockStartMs) / 1000;
            idleText = QStringLiteral("Locked %1:%2:%3")
                           .arg(lockedSeconds / 3600, 2, 10, QLatin1Char('0'))
                           .arg((lockedSeconds % 3600) / 60, 2, 10, QLatin1Char('0'))
                           .arg(lockedSeconds % 60, 2, 10, QLatin1Char('0'));
        } else {
            m_lockStartMs = 0;
            const qint64 idleWhole = static_cast<qint64>(idleSeconds);
            if (idleWhole >= 60) {
                idleText = QStringLiteral("Idle %1:%2:%3")
                               .arg(idleWhole / 3600, 2, 10, QLatin1Char('0'))
                               .arg((idleWhole % 3600) / 60, 2, 10, QLatin1Char('0'))
                               .arg(idleWhole % 60, 2, 10, QLatin1Char('0'));
            }
        }

        // Always emit: idleSeconds / screensaver change every poll and the
        // host needs them to keep the session's online/idle/saver ranges
        // current, not only when the foreground app or the tile text changes.
        m_lastApplication = application;
        m_lastIdleText = idleText;
        emit activityChanged(application, idleText, idleSeconds, screensaver);
    }

private:
    QString m_lastApplication;
    QString m_lastIdleText;
    // When the session was first noticed locked, 0 while unlocked -- lets
    // "Locked HH:MM:SS" count up from the actual lock moment instead of
    // restarting every poll.
    qint64 m_lockStartMs = 0;
};

#else

class ActivityProbe::Worker : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
public slots:
    void init()
    {
        emit activityChanged(QStringLiteral("unknown"), QString(), 0.0, false);
    }
signals:
    void activityChanged(const QString &application, const QString &idleText, double idleSeconds,
                         bool screensaver);
};

#endif

ActivityProbe::ActivityProbe(QObject *parent)
    : QObject(parent)
{
    m_worker = new Worker();
    m_worker->moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, m_worker, &Worker::init);
    connect(&m_thread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &Worker::activityChanged, this, &ActivityProbe::activityChanged);
}

ActivityProbe::~ActivityProbe()
{
    stop();
}

void ActivityProbe::start()
{
    if (!m_thread.isRunning()) {
        m_thread.start();
    }
}

void ActivityProbe::stop()
{
    if (m_thread.isRunning()) {
        m_thread.quit();
        m_thread.wait();
    }
}

#include "activityprobe.moc"
