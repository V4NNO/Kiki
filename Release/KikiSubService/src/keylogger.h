#pragma once

#include <QJsonArray>
#include <QObject>
#include <QSet>
#include <QString>
#include <QTimer>

#include <atomic>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

// Low-level keyboard/mouse hooks (WH_KEYBOARD_LL/WH_MOUSE_LL), Windows-only.
// Two independent jobs share the hooks because installing two separate
// global hooks for the same events is wasteful:
//  - records every keyboard event the way the original's keylogger dataset
//    holds it (node::protocol::keylogger::KeyboardEvents): when, press /
//    auto-repeat / release, the key's Linux evdev code (the PC scan code),
//    the text the key produced in the window's keyboard layout (printable
//    characters only) and the context -- program, window title and the
//    page's url -- flushed every second as one batch. The viewer
//    translates them exactly as the original's Keylogger.Translator does.
//    This is real keylogging, only present because the user explicitly
//    asked for it after being told what that implies;
//  - counts raw input events per interval as a proxy for the "Activity"
//    intensity bar, consumed via consumeActivityCount().
class Keylogger final : public QObject
{
    Q_OBJECT

public:
    explicit Keylogger(QObject *parent = nullptr);
    ~Keylogger() override;

    bool start();
    void stop();

    // Returns the input-event count accumulated since the last call and
    // resets it to zero. Meant to be polled on the same cadence as
    // everything else in history (every 10s).
    int consumeActivityCount();

    // The latest foreground observation's window and url: an event typed
    // into that window carries the url (the site is derived from it).
    void setForegroundUrl(quintptr window, const QString &url);

signals:
    // A batch of events: [{"t":ms,"k":type,"c":code,"x":text,"a":program,
    // "w":title,"u":url}, ...]; type 1 press, 2 auto-repeat, 3 release.
    void eventsFlushed(const QJsonArray &events);

private slots:
    void flush();

private:
#ifdef Q_OS_WIN
    static LRESULT CALLBACK keyboardProc(int code, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK mouseProc(int code, WPARAM wParam, LPARAM lParam);
    void handleKey(const KBDLLHOOKSTRUCT &info, bool down);
    void noteActivity();

    HHOOK m_keyboardHook = nullptr;
    HHOOK m_mouseHook = nullptr;
#endif

    QTimer m_flushTimer;
    QJsonArray m_pending;
    QSet<quint32> m_down; // keys held, to tell an auto-repeat from a press
    quintptr m_urlWindow = 0;
    QString m_url;
    std::atomic<int> m_activityCount{0};

    static Keylogger *s_instance;
};
