#include "windowlistcapture.h"

#include <QDateTime>
#include <QHash>
#include <QSet>
#include <QTimer>

#ifdef Q_OS_WIN
#include <qt_windows.h>

#ifndef PW_RENDERFULLCONTENT
#define PW_RENDERFULLCONTENT 0x00000002
#endif

namespace {
QString windowTitle(HWND hwnd)
{
    wchar_t buffer[256] = {0};
    GetWindowTextW(hwnd, buffer, 256);
    return QString::fromWCharArray(buffer);
}

// While the session is locked, target windows generally stop pumping their
// message loops -- PrintWindow sends WM_PRINT/WM_PRINTCLIENT internally and
// blocks until the target processes it, so calling it (sequentially, for
// potentially many windows) during a lock/unlock transition can stall this
// thread for a long time. Skipping the whole tick during that window is
// what actually fixed the lag reported after lock/unlock -- there's nothing
// useful to capture then anyway. OpenInputDesktop() failing is the
// standard way to detect "not on the interactive desktop right now" (locked
// screen, UAC secure desktop, etc.) -- same technique already used
// elsewhere in this codebase for desktop-switch detection.
bool isSessionLocked()
{
    HDESK desktop = OpenInputDesktop(0, FALSE, DESKTOP_READOBJECTS);
    if (!desktop) {
        return true;
    }
    CloseDesktop(desktop);
    return false;
}

// Same PrintWindow-based capture as the reference grabber's
// screenShooterImpl_win.cpp, applied to one arbitrary HWND at a time.
// outError, if set on a false return, says which step failed -- purely
// diagnostic, to tell apart "this app just doesn't support PrintWindow"
// from a bug in this function.
bool captureWindow(HWND hwnd, QImage *outImage, QString *outError)
{
    RECT rect{};
    if (!GetWindowRect(hwnd, &rect)) {
        if (outError) {
            *outError = QStringLiteral("GetWindowRect a esuat");
        }
        return false;
    }
    const int width = rect.right - rect.left;
    const int height = rect.bottom - rect.top;
    if (width <= 0 || height <= 0 || width > 8192 || height > 8192) {
        if (outError) {
            *outError = QStringLiteral("dimensiune invalida (%1x%2)").arg(width).arg(height);
        }
        return false;
    }

    HDC screenDc = GetDC(nullptr);
    if (!screenDc) {
        if (outError) {
            *outError = QStringLiteral("GetDC a esuat");
        }
        return false;
    }
    HDC memDc = CreateCompatibleDC(screenDc);
    HBITMAP bitmap = memDc ? CreateCompatibleBitmap(screenDc, width, height) : nullptr;
    ReleaseDC(nullptr, screenDc);
    if (!memDc || !bitmap) {
        if (outError) {
            *outError = QStringLiteral("CreateCompatibleDC/Bitmap a esuat");
        }
        if (bitmap) {
            DeleteObject(bitmap);
        }
        if (memDc) {
            DeleteDC(memDc);
        }
        return false;
    }
    HGDIOBJ oldBitmap = SelectObject(memDc, bitmap);

    const BOOL ok = PrintWindow(hwnd, memDc, PW_RENDERFULLCONTENT);
    bool success = false;
    if (!ok) {
        if (outError) {
            *outError = QStringLiteral("PrintWindow a returnat FALSE (eroare %1)")
                            .arg(GetLastError());
        }
    } else {
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
            if (outError) {
                // Cheap "did PrintWindow actually render anything" check --
                // a known PrintWindow failure mode is returning TRUE while
                // leaving the target DC untouched (all one color), which
                // some GPU-composited apps do instead of failing outright.
                bool looksBlank = true;
                const QPoint samples[] = {{0, 0},
                                          {width - 1, 0},
                                          {0, height - 1},
                                          {width - 1, height - 1},
                                          {width / 2, height / 2}};
                const QRgb first = buffer.pixel(0, 0);
                for (const QPoint &p : samples) {
                    if (buffer.pixel(p) != first) {
                        looksBlank = false;
                        break;
                    }
                }
                if (looksBlank) {
                    *outError = QStringLiteral("PrintWindow a reusit dar imaginea pare goala/uniforma");
                }
            }
        } else if (outError) {
            *outError = QStringLiteral("GetDIBits a esuat");
        }
    }

    SelectObject(memDc, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(memDc);
    return success;
}
} // namespace

class WindowListCapture::Worker : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

public slots:
    void init()
    {
        auto *timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer, &QTimer::timeout, this, &Worker::tick);
        timer->start();
    }

signals:
    void windowDiscovered(quint32 streamId, const QString &title);
    void frameCaptured(quint32 streamId, const QImage &image);
    void logMessage(const QString &message);

private slots:
    void tick()
    {
        if (isSessionLocked()) {
            return;
        }
        m_seenThisTick.clear();
        m_foregroundHwnd = GetForegroundWindow();
        EnumWindows(&Worker::enumProc, reinterpret_cast<LPARAM>(this));
        // Anything tracked from a previous tick but not seen this time was
        // closed -- just stop tracking it. SubServiceHost naturally stops
        // receiving frames for it; the viewer prunes streams that go quiet
        // (see MainWindow's stale-window-stream timer).
        for (auto it = m_streamIds.begin(); it != m_streamIds.end();) {
            if (!m_seenThisTick.contains(it.key())) {
                m_lastCapturedMs.remove(it.key());
                m_loggedFailures.remove(it.key());
                it = m_streamIds.erase(it);
            } else {
                ++it;
            }
        }
    }

private:
    static BOOL CALLBACK enumProc(HWND hwnd, LPARAM lParam)
    {
        reinterpret_cast<Worker *>(lParam)->handleWindow(hwnd);
        return TRUE;
    }

    void handleWindow(HWND hwnd)
    {
        if (!IsWindowVisible(hwnd)) {
            return;
        }
        // Same "alt-tab eligible" heuristic confirmed present in the
        // reference grabber's window-metadata extractor (an owner-window
        // check plus WS_EX_TOOLWINDOW/WS_EX_APPWINDOW bits): a plain
        // IsWindowVisible() pass, by itself, also matches a large number of
        // owned dialogs/tool windows and invisible-content helper windows
        // that aren't things a user would call an "open program".
        if (GetWindow(hwnd, GW_OWNER) != nullptr) {
            return;
        }
        const LONG_PTR exStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
        if (exStyle & WS_EX_TOOLWINDOW) {
            return;
        }

        DWORD pid = 0;
        GetWindowThreadProcessId(hwnd, &pid);
        if (pid == GetCurrentProcessId()) {
            // Skip our own windows (IndicatorOverlay etc.) -- capturing our
            // own monitoring UI as a "program" would just be noise.
            return;
        }

        const QString title = windowTitle(hwnd);
        if (title.isEmpty()) {
            // A real, user-facing top-level window essentially always has
            // one; the empty-title survivors that got this far are still
            // almost always leftover ghost/utility windows, confirmed by
            // testing (every blank "(fara titlu)" tile never rendered any
            // content).
            return;
        }

        m_seenThisTick.insert(hwnd);
        auto it = m_streamIds.constFind(hwnd);
        quint32 streamId;
        if (it == m_streamIds.constEnd()) {
            streamId = kStreamBase + (++m_nextOffset);
            m_streamIds.insert(hwnd, streamId);
            emit windowDiscovered(streamId, title);
        } else {
            streamId = it.value();
        }

        // Live capture only for the window the user is actually
        // interacting with; every other window is captured once (on
        // discovery) and then left as a static snapshot forever, unless it
        // becomes the foreground window at some point (at which point it
        // starts getting live updates again for as long as it stays
        // foreground). Deliberately NOT re-attempting backgrounded windows
        // periodically: GPU-composited apps (Teams, Outlook, WebView2/
        // Electron, Windows 11's File Explorer) only render correctly via
        // PrintWindow while actually in the foreground being composited by
        // DWM -- a periodic background retry would frequently overwrite a
        // perfectly good snapshot (captured while it briefly had focus)
        // with a blank one, for zero benefit. This also matches what was
        // asked for directly: minimal resource use, live only for the
        // active window, static image otherwise.
        const bool isForeground = (hwnd == m_foregroundHwnd);
        const bool everCaptured = m_lastCapturedMs.contains(hwnd);
        if (!isForeground && everCaptured) {
            return;
        }
        const qint64 now = QDateTime::currentMSecsSinceEpoch();

        QImage image;
        QString error;
        const bool ok = captureWindow(hwnd, &image, &error);
        if (!error.isEmpty() && !m_loggedFailures.contains(hwnd)) {
            m_loggedFailures.insert(hwnd);
            emit logMessage(QStringLiteral("[windowlist] '%1': %2").arg(title, error));
        }
        // Marked as "attempted" whether or not it actually produced an
        // image -- a hard failure (e.g. an elevated window we're not
        // allowed to capture across the UAC boundary) will never succeed no
        // matter how often it's retried while backgrounded, so there's no
        // point hammering it every tick; it'll get one more try if/when it
        // becomes the foreground window.
        m_lastCapturedMs.insert(hwnd, now);
        if (ok && !image.isNull()) {
            emit frameCaptured(streamId, image);
        }
    }

    QHash<HWND, quint32> m_streamIds;
    QHash<HWND, qint64> m_lastCapturedMs;
    QSet<HWND> m_seenThisTick;
    QSet<HWND> m_loggedFailures;
    HWND m_foregroundHwnd = nullptr;
    quint32 m_nextOffset = 0;
};

#else

class WindowListCapture::Worker : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
public slots:
    void init() {}
signals:
    void windowDiscovered(quint32 streamId, const QString &title);
    void frameCaptured(quint32 streamId, const QImage &image);
    void logMessage(const QString &message);
};

#endif

WindowListCapture::WindowListCapture(QObject *parent)
    : QObject(parent)
{
    m_worker = new Worker();
    m_worker->moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, m_worker, &Worker::init);
    connect(&m_thread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &Worker::windowDiscovered, this, &WindowListCapture::windowDiscovered);
    connect(m_worker, &Worker::frameCaptured, this, &WindowListCapture::frameCaptured);
    connect(m_worker, &Worker::logMessage, this, &WindowListCapture::logMessage);
}

WindowListCapture::~WindowListCapture()
{
    stop();
}

void WindowListCapture::start()
{
    if (!m_thread.isRunning()) {
        m_thread.start();
    }
}

void WindowListCapture::stop()
{
    if (m_thread.isRunning()) {
        m_thread.quit();
        m_thread.wait();
    }
}

#include "windowlistcapture.moc"
