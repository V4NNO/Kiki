#include "screencapture.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QGuiApplication>
#include <QPixmap>
#include <QPointer>
#include <QScreen>
#include <QThreadPool>

#ifdef Q_OS_WIN
#include <QtGui/qscreen_platform.h>
#include <qt_windows.h>
#endif

namespace {
#ifdef Q_OS_WIN
// The one identifier DXGI and Qt actually agree on -- see dxgicapture.h for
// why QScreen::name() can't be used for this matching.
void *nativeMonitorHandle(QScreen *screen)
{
    if (auto *windowsScreen = screen->nativeInterface<QNativeInterface::QWindowsScreen>()) {
        return static_cast<void *>(windowsScreen->handle());
    }
    return nullptr;
}

// Same technique as WindowListCapture's/ActivityProbe's lock detection.
// DXGI duplication is unavailable for the whole time the session is locked
// (the secure desktop isn't accessible to it), which used to make every
// tick fall through to the GDI fallback below -- screen->grabWindow(0), a
// synchronous BitBlt on this thread, i.e. exactly the original mouse-lag
// bug, just re-triggered specifically during lock/unlock instead of being
// permanently absent. There's nothing useful to capture while locked
// anyway, so skip the tick entirely instead.
bool isSessionLocked()
{
    HDESK desktop = OpenInputDesktop(0, FALSE, DESKTOP_READOBJECTS);
    if (!desktop) {
        return true;
    }
    CloseDesktop(desktop);
    return false;
}

// Raw Win32 BitBlt capture (no Qt GUI calls, so safe to run on a worker
// thread) for the fallback path used while a screen doesn't have a working
// DXGI capture -- typically only for the brief window right after
// unlocking, until DXGI reinitializes (see onInitFinished()). Same pixel
// layout reasoning as dxgicapture.cpp/windowlistcapture.cpp: BI_RGB 32bpp
// matches QImage::Format_ARGB32 exactly.
bool captureMonitorGdi(HMONITOR monitor, QImage *outImage)
{
    MONITORINFO mi{};
    mi.cbSize = sizeof(MONITORINFO);
    if (!GetMonitorInfoW(monitor, &mi)) {
        return false;
    }
    const int width = mi.rcMonitor.right - mi.rcMonitor.left;
    const int height = mi.rcMonitor.bottom - mi.rcMonitor.top;
    if (width <= 0 || height <= 0) {
        return false;
    }

    HDC screenDc = GetDC(nullptr);
    if (!screenDc) {
        return false;
    }
    HDC memDc = CreateCompatibleDC(screenDc);
    HBITMAP bitmap = memDc ? CreateCompatibleBitmap(screenDc, width, height) : nullptr;
    if (!memDc || !bitmap) {
        if (bitmap) {
            DeleteObject(bitmap);
        }
        if (memDc) {
            DeleteDC(memDc);
        }
        ReleaseDC(nullptr, screenDc);
        return false;
    }
    HGDIOBJ oldBitmap = SelectObject(memDc, bitmap);
    const BOOL blitOk = BitBlt(memDc, 0, 0, width, height, screenDc, mi.rcMonitor.left, mi.rcMonitor.top,
                               SRCCOPY);
    ReleaseDC(nullptr, screenDc);

    bool success = false;
    if (blitOk) {
        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // negative = top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        QImage buffer(width, height, QImage::Format_ARGB32);
        if (GetDIBits(memDc, bitmap, 0, static_cast<UINT>(height), buffer.bits(), &bmi,
                     DIB_RGB_COLORS) > 0) {
            *outImage = buffer;
            success = true;
        }
    }

    SelectObject(memDc, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(memDc);
    return success;
}
#endif
}

ScreenCaptureManager::ScreenCaptureManager(QObject *parent)
    : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &ScreenCaptureManager::captureTick);
    connect(qApp, &QGuiApplication::screenAdded, this, &ScreenCaptureManager::refreshMonitorList);
    connect(qApp, &QGuiApplication::screenRemoved, this, &ScreenCaptureManager::refreshMonitorList);
    refreshMonitorList();
}

ScreenCaptureManager::~ScreenCaptureManager()
{
    // Just drop our references -- a capture job still running on a
    // QThreadPool worker holds its own shared_ptr copy and keeps the
    // DxgiOutputCapture alive until it finishes; onCaptureFinished() is
    // guarded against this object being gone by then (see captureTick()).
    m_dxgiCaptures.clear();
}

void ScreenCaptureManager::start(int fps)
{
    refreshMonitorList();
    setFps(fps);
    m_timer.start();
}

void ScreenCaptureManager::setFps(int fps)
{
    const int clampedFps = qBound(1, fps, 30);
    m_timer.setInterval(1000 / clampedFps);
}

void ScreenCaptureManager::stop()
{
    m_timer.stop();
}

QList<MonitorInfo> ScreenCaptureManager::monitors() const
{
    return m_monitors;
}

quint32 ScreenCaptureManager::idForScreen(QScreen *screen)
{
    const QString key = screen->name();
    auto it = m_screenIds.constFind(key);
    if (it != m_screenIds.constEnd()) {
        return it.value();
    }
    const quint32 id = m_nextId++;
    m_screenIds.insert(key, id);
    return id;
}

void ScreenCaptureManager::refreshMonitorList()
{
    QList<MonitorInfo> monitors;
    const QList<QScreen *> screens = QGuiApplication::screens();
    for (QScreen *screen : screens) {
        MonitorInfo info;
        info.streamId = idForScreen(screen);
        info.name = screen->name();
        // geometry() is in logical (DPI-scaled) pixels, but grabWindow()
        // below captures at the physical device pixel ratio; report the
        // size viewers will actually receive in FullFrame, or a DPI-scaled
        // monitor ends up announced smaller than the images sent for it.
        info.size = screen->geometry().size() * screen->devicePixelRatio();
        monitors.append(info);
    }
    m_monitors = monitors;
    emit monitorListChanged(m_monitors);
}

void ScreenCaptureManager::captureTick()
{
#ifdef Q_OS_WIN
    if (isSessionLocked()) {
        return;
    }
#endif
    const QList<QScreen *> screens = QGuiApplication::screens();
    if (screens.size() != m_monitors.size()) {
        refreshMonitorList();
    }
    constexpr qint64 InitRetryBackoffMs = 5000;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    for (QScreen *screen : screens) {
        const quint32 id = idForScreen(screen);
        const QString name = screen->name();

        std::shared_ptr<DxgiOutputCapture> dxgi = m_dxgiCaptures.value(name);
        if (!dxgi) {
            const qint64 lastAttempt = m_lastInitAttemptMs.value(name, 0);
            if (!m_initsInFlight.contains(name)
                && (lastAttempt == 0 || now - lastAttempt >= InitRetryBackoffMs)) {
                m_initsInFlight.insert(name);
                m_lastInitAttemptMs.insert(name, now);
                const bool firstAttempt = !m_screensEverAttempted.contains(name);
                m_screensEverAttempted.insert(name);

                // Device creation + DuplicateOutput is itself a real,
                // synchronous, potentially slow call (and fails, then gets
                // retried, for as long as DXGI stays unavailable -- e.g.
                // the entire time the session is locked) -- this used to
                // run right here on captureTick()'s thread, which is what
                // caused the "mouse lags again after locking/unlocking"
                // bug: this is the SAME thread Keylogger's low-level hooks
                // live on (see subservicehost.h's class comment for why
                // that thread must never block). Only the steady-state
                // per-frame readback had been moved off this thread before;
                // now the (also blocking) setup call is too.
                void *handle = nullptr;
#ifdef Q_OS_WIN
                handle = nativeMonitorHandle(screen);
#endif
                QPointer<ScreenCaptureManager> self(this);
                QThreadPool::globalInstance()->start([self, name, handle, firstAttempt] {
                    auto capture = std::make_shared<DxgiOutputCapture>();
                    QString initError;
                    bool ok = false;
#ifdef Q_OS_WIN
                    if (handle) {
                        ok = capture->initialize(handle, name, &initError);
                    } else {
                        initError = QStringLiteral("Handle HMONITOR indisponibil.");
                    }
#else
                    initError = QStringLiteral("DXGI disponibil doar pe Windows.");
#endif
                    QMetaObject::invokeMethod(
                        qApp,
                        [self, name, capture, ok, initError, firstAttempt] {
                            if (self) {
                                self->onInitFinished(name, capture, ok, initError, firstAttempt);
                            }
                        },
                        Qt::QueuedConnection);
                });
            }

            // No valid DXGI capture yet (never tried, still initializing,
            // or on backoff after a failure) -- GDI fallback for this tick,
            // dispatched to a worker thread the same way the DXGI paths are
            // (see captureMonitorGdi()): screen->grabWindow(0) is a
            // synchronous BitBlt, and this is the exact path that's active
            // for the brief window right after unlocking, before DXGI
            // reinitializes -- letting it block this thread would be the
            // original mouse-lag bug all over again, just for a shorter
            // window instead of the whole lock duration.
            if (!m_gdiCapturesInFlight.contains(name)) {
#ifdef Q_OS_WIN
                if (void *monitorHandle = nativeMonitorHandle(screen)) {
                    m_gdiCapturesInFlight.insert(name);
                    QPointer<ScreenCaptureManager> self(this);
                    HMONITOR hmon = static_cast<HMONITOR>(monitorHandle);
                    QThreadPool::globalInstance()->start([self, hmon, name, id] {
                        QImage image;
                        const bool ok = captureMonitorGdi(hmon, &image);
                        QMetaObject::invokeMethod(
                            qApp,
                            [self, id, name, ok, image] {
                                if (self) {
                                    self->onGdiCaptureFinished(id, name, ok ? image : QImage());
                                }
                            },
                            Qt::QueuedConnection);
                    });
                }
#else
                const QPixmap pixmap = screen->grabWindow(0);
                if (!pixmap.isNull()) {
                    emit frameCaptured(id, pixmap.toImage());
                }
#endif
            }
            continue;
        }

        // m_dxgiCaptures only ever holds successfully-initialized captures
        // (see onInitFinished()), so dxgi is guaranteed valid here.
        //
        // The actual GPU readback (AcquireNextFrame/CopyResource/Map) can
        // block for real time once the desktop is actively redrawing --
        // and this timer fires on the same thread that owns Keylogger's
        // WH_MOUSE_LL/WH_KEYBOARD_LL hooks, which Windows only dispatches
        // (for every process, system-wide) while that thread is pumping
        // messages. Doing the readback here was the cause of the mouse lag
        // that only appeared once the screen was actually changing. Run it
        // on a worker thread instead; skip this tick for the screen if its
        // previous capture job hasn't finished yet rather than queuing a
        // second one behind it.
        if (m_capturesInFlight.contains(name)) {
            continue;
        }
        m_capturesInFlight.insert(name);

        QPointer<ScreenCaptureManager> self(this);
        std::shared_ptr<DxgiOutputCapture> job = dxgi;
        QThreadPool::globalInstance()->start([self, job, name, id] {
            QImage image;
            QString captureError;
            const bool ok = job->captureFrame(50, &image, &captureError);
            QMetaObject::invokeMethod(
                qApp,
                [self, name, id, ok, image, captureError] {
                    if (self) {
                        self->onCaptureFinished(name, id, ok, image, captureError);
                    }
                },
                Qt::QueuedConnection);
        });
    }
}

void ScreenCaptureManager::onCaptureFinished(const QString &name, quint32 id, bool ok,
                                             const QImage &image, const QString &error)
{
    m_capturesInFlight.remove(name);
    if (ok) {
        emit frameCaptured(id, image);
        return;
    }
    if (!error.isEmpty()) {
        // A real failure (not just "no new frame yet"), e.g.
        // DXGI_ERROR_ACCESS_LOST from a mode change or the secure desktop --
        // drop this session so the next tick creates a fresh one and
        // retries initialize(), since these are expected to be transient.
        // The map might already hold a different (newer) entry for `name`
        // in theory, but only this method ever erases entries and only one
        // capture per name is ever in flight, so this is always still the
        // one that just failed.
        m_dxgiCaptures.remove(name);
    }
}

void ScreenCaptureManager::onInitFinished(const QString &name, std::shared_ptr<DxgiOutputCapture> capture,
                                          bool ok, const QString &error, bool firstAttempt)
{
    m_initsInFlight.remove(name);
    if (ok) {
        m_dxgiCaptures.insert(name, capture);
    }
    // Only log every screen's very first outcome, plus any successful
    // (re)connection -- a sustained outage (e.g. the whole time the screen
    // stays locked) would otherwise spam a log line every retry.
    if (firstAttempt || ok) {
        emit captureBackendReady(name, ok, ok ? QString() : error);
    }
}

void ScreenCaptureManager::onGdiCaptureFinished(quint32 id, const QString &name, const QImage &image)
{
    m_gdiCapturesInFlight.remove(name);
    if (!image.isNull()) {
        emit frameCaptured(id, image);
    }
}
