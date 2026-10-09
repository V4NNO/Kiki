#include "activityprobe.h"

#include <QDateTime>
#include <QElapsedTimer>
#include <QTimer>

#include <memory>

#ifdef Q_OS_WIN
#include <qt_windows.h>

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

    ~Worker() override
    {
        // The UI Automation objects must go before COM is torn down.
        m_source.reset();
        if (m_comInitialized) {
            CoUninitialize();
        }
    }

public slots:
    void init()
    {
        // MTA, not STA: UIA client calls can block on a provider (e.g. a
        // browser tab building its accessibility tree for the first time),
        // and MTA means that block only ever affects this thread's own
        // queued work, never anyone else's.
        m_comInitialized = SUCCEEDED(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
        m_source = std::make_unique<DesktopForegroundSource>();

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
    void activityChanged(const ForegroundObservation &foreground, const QString &idleText,
                         double idleSeconds, bool screensaver);
    void logMessage(const QString &message);

private slots:
    void poll()
    {
        QElapsedTimer stopwatch;
        stopwatch.start();
        const ForegroundObservation foreground = observeForeground(*m_source);
        const qint64 elapsedMs = stopwatch.elapsed();
        // BrowserUrlReader deliberately waits 350 ms for the address bar to
        // settle after a navigation; only time beyond that is worth a line.
        if (elapsedMs > 500) {
            emit logMessage(QStringLiteral("[foreground] observatia pentru %1 a durat %2 ms")
                                .arg(foreground.application)
                                .arg(elapsedMs));
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
        emit activityChanged(foreground, idleText, idleSeconds, screensaver);
    }

private:
    bool m_comInitialized = false;
    std::unique_ptr<DesktopForegroundSource> m_source;
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
        ForegroundObservation foreground;
        foreground.application = QStringLiteral("unknown");
        emit activityChanged(foreground, QString(), 0.0, false);
    }
signals:
    void activityChanged(const ForegroundObservation &foreground, const QString &idleText,
                         double idleSeconds, bool screensaver);
    void logMessage(const QString &message);
};

#endif

ActivityProbe::ActivityProbe(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<ForegroundObservation>();
    m_worker = new Worker();
    m_worker->moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, m_worker, &Worker::init);
    connect(&m_thread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &Worker::activityChanged, this, &ActivityProbe::activityChanged);
    connect(m_worker, &Worker::logMessage, this, &ActivityProbe::logMessage);
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
