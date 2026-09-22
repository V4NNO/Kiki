#pragma once

#include <QObject>
#include <QString>
#include <QThread>

// Polls the foreground window's browser address bar via UI Automation, on
// its own dedicated thread. FindFirst() over a browser's UI Automation tree
// can block for a long time -- Chrome only builds its full accessibility
// tree (which includes the page's DOM content, not just chrome UI) lazily,
// the first time something queries it, so a single call can stall for
// hundreds of ms on a complex page. This process also owns Keylogger's
// low-level input hooks on its main thread; doing this work there would
// reintroduce the exact kind of system-wide input lag already fixed for
// screen capture (see subservicehost.h's class comment). Runs on the same
// ~2s cadence as ActivityProbe, independently of it, and only emits when
// the extracted URL actually changes.
class BrowserUrlProbe final : public QObject
{
    Q_OBJECT

public:
    explicit BrowserUrlProbe(QObject *parent = nullptr);
    ~BrowserUrlProbe() override;

    void start();
    void stop();

signals:
    // Empty url means "foreground app isn't a recognized browser, or
    // extraction failed" -- callers should treat that the same as "no web
    // page currently open".
    void urlChanged(const QString &url);
    // Temporary diagnostic: fires when a single poll (UIA query included)
    // takes longer than a threshold, to help tell apart "one slow UIA call"
    // from general system-wide CPU contention as a lag cause.
    void logMessage(const QString &message);

private:
    class Worker;
    QThread m_thread;
    Worker *m_worker = nullptr;
};
