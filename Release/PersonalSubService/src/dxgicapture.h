#pragma once

#include <QImage>
#include <QString>

// GPU-accelerated screen capture via the DXGI Desktop Duplication API
// (D3D11 + IDXGIOutputDuplication), matching what the real grabber uses as
// its primary capture path (see tlsservice_ANALIZA.txt section 8 -- "Fluxul
// prefera captura DXGI", GDI BitBlt only as a fallback). GDI-based capture
// (QScreen::grabWindow on Windows, our previous only path) is a synchronous
// desktop-compositor operation that visibly stutters the whole system at
// capture-loop frequencies -- this is why the app "lags when it starts
// taking screenshots": DXGI capture doesn't have that effect, it reads
// already-composited frames the GPU produced anyway.
//
// One instance wraps exactly one physical output (monitor). Matched to a
// QScreen by its HMONITOR handle (via
// screen->nativeInterface<QNativeInterface::QWindowsScreen>()->handle()),
// NOT by QScreen::name() -- on real hardware, Qt returns an EDID-derived
// friendly name for some monitors ("LF24T35 (2)") and the raw device path
// for others ("\\\\.\\DISPLAY1") on the very same machine, so string
// matching against DXGI_OUTPUT_DESC.DeviceName silently fails for whichever
// monitors got the friendly-name treatment. HMONITOR is the one identifier
// both Qt and DXGI agree on.
class DxgiOutputCapture
{
public:
    DxgiOutputCapture();
    ~DxgiOutputCapture();

    DxgiOutputCapture(const DxgiOutputCapture &) = delete;
    DxgiOutputCapture &operator=(const DxgiOutputCapture &) = delete;

    // targetMonitor is an HMONITOR, typed as void* here so this header
    // doesn't need <windef.h>; cast at the call site on Windows only.
    // Enumerates all adapters/outputs looking for the one whose Monitor
    // handle matches. Safe to call again after release()/failure to retry
    // (e.g. after a mode change invalidated the previous session).
    bool initialize(void *targetMonitor, const QString &debugName, QString *error);
    void release();
    bool isValid() const;

    // Blocks up to timeoutMs waiting for a new frame. Returns false (not an
    // error) when the desktop was static and nothing changed -- the caller
    // should keep showing the previous frame. Returns false with *error set
    // on a real failure; the caller should then call release() and retry
    // initialize() later (this happens on display mode changes, secure
    // desktop transitions, etc. -- DXGI_ERROR_ACCESS_LOST).
    bool captureFrame(int timeoutMs, QImage *image, QString *error);

private:
    struct Impl;
    Impl *m_impl = nullptr;
};
