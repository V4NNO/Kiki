#pragma once

#include <QObject>
#include <QString>
#include <QThread>

// Polls the foreground application and idle/locked time so the viewer can
// show the same "active app / locked" hints the original Kickidler tile UI
// has. Windows-only; on other platforms it reports "unknown" and never
// idle.
//
// Runs on its own dedicated thread, same reasoning as BrowserUrlProbe/
// WindowListCapture: OpenInputDesktop() (used to detect the locked state)
// and the other Win32 calls here are normally fast, but this process also
// owns Keylogger's low-level input hooks on its main thread, and even an
// occasional slow call right at a lock/unlock transition (when desktop
// objects are actively being created/torn down by the OS) is exactly the
// kind of thing that must never run on that thread -- see
// subservicehost.h's class comment for why.
class ActivityProbe final : public QObject
{
    Q_OBJECT

public:
    explicit ActivityProbe(QObject *parent = nullptr);
    ~ActivityProbe() override;

    void start();
    void stop();

signals:
    void activityChanged(const QString &application, const QString &idleText);

private:
    class Worker;
    QThread m_thread;
    Worker *m_worker = nullptr;
};
