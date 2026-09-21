#pragma once

#include <QObject>
#include <QString>
#include <QTimer>

#include <atomic>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

// Low-level keyboard/mouse hooks (WH_KEYBOARD_LL/WH_MOUSE_LL), Windows-only.
// Two independent jobs share the hooks because installing two separate
// global hooks for the same events is wasteful:
//  - accumulates typed text per foreground window, flushed periodically as
//    (windowTitle, text) chunks -- this is real keylogging, only present
//    because the user explicitly asked for it after being told what that
//    implies (see polished-singing-seahorse.md);
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

signals:
    void textFlushed(const QString &windowTitle, const QString &text);

private slots:
    void flush();

private:
#ifdef Q_OS_WIN
    static LRESULT CALLBACK keyboardProc(int code, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK mouseProc(int code, WPARAM wParam, LPARAM lParam);
    void handleKeyDown(DWORD vkCode);
    void noteActivity();

    HHOOK m_keyboardHook = nullptr;
    HHOOK m_mouseHook = nullptr;
#endif

    QTimer m_flushTimer;
    QString m_pendingWindowTitle;
    QString m_pendingText;
    std::atomic<int> m_activityCount{0};

    static Keylogger *s_instance;
};
