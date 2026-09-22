#pragma once

#include <QImage>
#include <QObject>
#include <QThread>

// Captures EVERY top-level window's own content (not just the foreground
// one) via PrintWindow, matching the reference product's confirmed
// PrintWindow-based per-window capture technique (see
// screenShooterImpl_win.cpp in the decompiled grabber source) generalized
// from "one caller-supplied HWND" to "every currently open window" per
// explicit user request -- Kickidler itself only confirmed doing this for
// the single foreground window, this is our own extension built on the
// same real API.
//
// Runs on its own dedicated thread: enumerating and PrintWindow-capturing
// potentially many windows every tick is real, synchronous GDI work, and
// this process also owns Keylogger's low-level input hooks on its main
// thread -- doing this work there would reintroduce the exact system-wide
// input lag already fixed for screen capture (see subservicehost.h's class
// comment).
class WindowListCapture final : public QObject
{
    Q_OBJECT

public:
    // Reserved streamId range, outside anything ScreenCaptureManager::
    // idForScreen() ever assigns (starts at 1, grows by one per monitor).
    static constexpr quint32 kStreamBase = 0xFFFF0000;

    explicit WindowListCapture(QObject *parent = nullptr);
    ~WindowListCapture() override;

    void start();
    void stop();

signals:
    // Emitted once, the first time a given window is seen (its streamId
    // stays stable for as long as the window stays open).
    void windowDiscovered(quint32 streamId, const QString &title);
    void frameCaptured(quint32 streamId, const QImage &image);
    // Temporary diagnostic: which window a capture attempt was for and
    // whether/why it failed, to tell apart "PrintWindow genuinely can't
    // render this app" from a bug in our own capture/delivery code.
    void logMessage(const QString &message);

private:
    class Worker;
    QThread m_thread;
    Worker *m_worker = nullptr;
};
