#pragma once

#include "dxgicapture.h"

#include <QHash>
#include <QImage>
#include <QObject>
#include <QSet>
#include <QTimer>

#include <memory>

class QScreen;

struct MonitorInfo {
    quint32 streamId = 0;
    QString name;
    QSize size;
    // Filled in by PersonalHost when relaying a PersonalSubService's
    // monitors onward to real viewers; left at defaults everywhere a
    // monitor is only ever local to a single session (PersonalScreenAgent,
    // PersonalSubService).
    quint32 sessionId = 0;
    QString sessionUsername;
    QString sessionState;
};

// Grabs every connected screen on a timer and republishes the raw images.
// Does not know anything about the network protocol or about diffing;
// each AgentConnection keeps its own last-sent-frame cache and decides
// independently whether to forward a full or a delta frame.
class ScreenCaptureManager final : public QObject
{
    Q_OBJECT

public:
    explicit ScreenCaptureManager(QObject *parent = nullptr);
    ~ScreenCaptureManager() override;

    void start(int fps);
    void stop();
    // Changes the capture cadence without touching DXGI/GDI state (no
    // re-init, no dropped in-flight jobs); safe to call anytime, including
    // while running.
    void setFps(int fps);
    bool isRunning() const { return m_timer.isActive(); }

    QList<MonitorInfo> monitors() const;

signals:
    void frameCaptured(quint32 streamId, const QImage &image);
    void monitorListChanged(const QList<MonitorInfo> &monitors);
    // Emitted once per screen, right after the first DXGI init attempt for
    // it (success or fallback-to-GDI), purely for diagnosability.
    void captureBackendReady(const QString &screenName, bool usingDxgi, const QString &detail);

private slots:
    void captureTick();
    void refreshMonitorList();

private:
    quint32 idForScreen(QScreen *screen);
    void onCaptureFinished(const QString &name, quint32 id, bool ok, const QImage &image,
                           const QString &error);
    void onInitFinished(const QString &name, std::shared_ptr<DxgiOutputCapture> capture, bool ok,
                        const QString &error, bool firstAttempt);
    void onGdiCaptureFinished(quint32 id, const QString &name, const QImage &image);

    QTimer m_timer;
    QHash<QString, quint32> m_screenIds;
    quint32 m_nextId = 1;
    QList<MonitorInfo> m_monitors;
    // GPU-accelerated capture per screen (keyed by QScreen::name(), which
    // matches DXGI_OUTPUT_DESC.DeviceName -- see dxgicapture.h). Only ever
    // holds VALID captures -- a screen not present here either has never
    // been tried, or its last attempt failed and is on backoff (see
    // m_lastInitAttemptMs) before being retried, e.g. while the session is
    // locked and DXGI duplication is unavailable; captureTick() falls back
    // to QScreen::grabWindow() meanwhile. shared_ptr (not a raw/owning
    // pointer) so a capture job running on a QThreadPool worker (see
    // captureTick()) can keep its target alive even if this map entry is
    // replaced or erased before the job completes.
    QHash<QString, std::shared_ptr<DxgiOutputCapture>> m_dxgiCaptures;
    // Screens with a DXGI capture job currently running on a worker thread;
    // captureTick() skips a screen that's still in here rather than queuing
    // a second job behind it -- see the .cpp for why this thread must never
    // block on the actual GPU readback (AcquireNextFrame/CopyResource/Map).
    QSet<QString> m_capturesInFlight;
    // Screens with a DXGI *initialize* (device creation + DuplicateOutput)
    // job currently running on a worker thread -- same reasoning as
    // m_capturesInFlight, but for the (also blocking, also previously
    // synchronous-on-this-thread) setup call, which is what caused the
    // "mouse lags again after locking/unlocking the screen" bug: every
    // failed init used to retry synchronously on this thread, every tick,
    // for as long as DXGI stayed unavailable (i.e. the whole time the
    // screen was locked).
    QSet<QString> m_initsInFlight;
    // Throttles retries for a screen whose last init attempt failed, so a
    // sustained DXGI outage (secure desktop during lock, RDP, etc.) doesn't
    // turn into a tight retry loop -- still all on a worker thread, but no
    // reason to hammer it.
    QHash<QString, qint64> m_lastInitAttemptMs;
    QSet<QString> m_screensEverAttempted; // for captureBackendReady's "first attempt only" logging
    // Screens with a GDI fallback capture (BitBlt) currently running on a
    // worker thread -- same reasoning as m_capturesInFlight/m_initsInFlight:
    // whenever DXGI isn't available (typically briefly, right after
    // unlocking, until it reinitializes), this is what captureTick() used
    // to do synchronously, right here on the hook-owning thread.
    QSet<QString> m_gdiCapturesInFlight;
};
