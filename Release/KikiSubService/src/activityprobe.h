#pragma once

#include "foregroundobserver.h"

#include <QMetaType>
#include <QObject>
#include <QString>
#include <QThread>

// Polls the foreground window (executable, title and browser url, as ONE
// observation -- see foregroundobserver.h) and idle/locked time so the
// viewer can show the same "active app / locked" hints the original
// Kickidler tile UI has, and History can record which program, title and
// page were in front. Windows-only; on other platforms it reports "unknown"
// and never idle.
//
// Runs on its own dedicated thread, same reasoning as WindowListCapture:
// OpenInputDesktop() (used to detect the locked state) and the browser's UI
// Automation tree (BrowserUrlReader) can each block, and this process also
// owns Keylogger's low-level input hooks on its main thread, where such a
// call must never run -- see subservicehost.h's class comment for why. The
// url is read here, in the same poll as the executable and title, instead of
// by a separate probe on its own timer: two independent timers plus a title
// read at push time meant one metadata push combined values read at
// different moments.
class ActivityProbe final : public QObject
{
    Q_OBJECT

public:
    explicit ActivityProbe(QObject *parent = nullptr);
    ~ActivityProbe() override;

    void start();
    void stop();

signals:
    void activityChanged(const ForegroundObservation &foreground, const QString &idleText,
                         double idleSeconds, bool screensaver);
    // Diagnostic: a single poll (UI Automation included) that took long.
    void logMessage(const QString &message);

private:
    class Worker;
    QThread m_thread;
    Worker *m_worker = nullptr;
};

Q_DECLARE_METATYPE(ForegroundObservation)
