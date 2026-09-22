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
    void activityChanged(const QString &application, const QString &idleText);

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

        QString idleText;
        if (isSessionLocked()) {
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
            LASTINPUTINFO lastInput;
            lastInput.cbSize = sizeof(LASTINPUTINFO);
            if (GetLastInputInfo(&lastInput)) {
                const DWORD idleMs = GetTickCount() - lastInput.dwTime;
                const qint64 idleSeconds = idleMs / 1000;
                if (idleSeconds >= 60) {
                    idleText = QStringLiteral("Idle %1:%2:%3")
                                   .arg(idleSeconds / 3600, 2, 10, QLatin1Char('0'))
                                   .arg((idleSeconds % 3600) / 60, 2, 10, QLatin1Char('0'))
                                   .arg(idleSeconds % 60, 2, 10, QLatin1Char('0'));
                }
            }
        }

        if (application != m_lastApplication || idleText != m_lastIdleText) {
            m_lastApplication = application;
            m_lastIdleText = idleText;
            emit activityChanged(application, idleText);
        }
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
        emit activityChanged(QStringLiteral("unknown"), QString());
    }
signals:
    void activityChanged(const QString &application, const QString &idleText);
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
