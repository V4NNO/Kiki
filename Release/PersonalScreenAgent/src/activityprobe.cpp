#include "activityprobe.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <psapi.h>
#endif

ActivityProbe::ActivityProbe(QObject *parent)
    : QObject(parent)
{
    m_timer.setInterval(2000);
    connect(&m_timer, &QTimer::timeout, this, &ActivityProbe::poll);
}

void ActivityProbe::start()
{
    poll();
    m_timer.start();
}

void ActivityProbe::stop()
{
    m_timer.stop();
}

void ActivityProbe::poll()
{
#ifdef Q_OS_WIN
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

    LASTINPUTINFO lastInput;
    lastInput.cbSize = sizeof(LASTINPUTINFO);
    QString idleText;
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
#else
    const QString application = QStringLiteral("unknown");
    const QString idleText;
#endif

    if (application != m_lastApplication || idleText != m_lastIdleText) {
        m_lastApplication = application;
        m_lastIdleText = idleText;
        emit activityChanged(application, idleText);
    }
}
