// =============================================================================
// testGrabb_Claude.cpp
// Reconstructed grabber diagnostic tool
// Basis: static analysis of tlsservice.exe, tlshost.exe, tssubservice.exe
//        via Ghidra MCP (reports dated 2026-07-15 / 2026-07-16).
//
// Purpose: reproduce and expose the bug where the grabber stays locked on a
//          previous process/tab (e.g. Chrome) and reports maximum activity
//          even though the employee switched to a different application
//          (e.g. AutoCAD) and is actually inactive.
//
// Architecture observed:
//   tlsservice.exe   -> focus detection, Win32 hooks, GDI capture
//   tlshost.exe      -> video pipeline, QImage ring-buffer, delta encoding, IPC
//   tssubservice.exe -> user-session subagent: UI/tray, clipboard, HTTP, RPC
//
// Build:
//   cl /EHsc /W4 /DUNICODE /D_UNICODE testGrabb_Claude.cpp
//      /link user32.lib gdi32.lib advapi32.lib kernel32.lib psapi.lib
// =============================================================================

#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <psapi.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cassert>
#include <cstring>

// ---------------------------------------------------------------------------
// Runtime constants (approximated from binary analysis)
// ---------------------------------------------------------------------------
constexpr UINT  kPollIntervalMs      = 500;
constexpr DWORD kIdleThresholdMs     = 60  * 1000;
constexpr DWORD kAwayThresholdMs     = 5   * 60000;
constexpr DWORD kHeartbeatMs         = 5000;
constexpr DWORD kPipeTimeoutMs       = 2000;
constexpr int   kPipePacketSize      = 0x50;    // observed TransactNamedPipe size
constexpr DWORD kDesktopAccessMask   = GENERIC_ALL;
constexpr DWORD kPipeAccessMask      = 0x103;   // GENERIC_READ|GENERIC_WRITE|...
constexpr DWORD kPipeFlags           = 0x110000;
constexpr DWORD kOpenProcessFlags    = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
constexpr DWORD kOpenProcessLight    = PROCESS_QUERY_LIMITED_INFORMATION;
constexpr int   kQImageFormat_RGB555 = 12;      // QImage::Format_RGB555 asserted in sequencer
constexpr int   kWinEventMin         = 0x0020;  // EVENT_OBJECT_FOCUS
constexpr int   kWinEventMax         = 0x8005;  // range observed in FUN_14003ee60
constexpr int   kRingBufferCapacity  = 8;       // default from FUN_14028e700
constexpr ULONG kMouseInjectedFlag   = 0x00000001;
constexpr int   kWindowTitleMax      = 0x1000;
constexpr int   kProcessPathMax      = 0x1000;
constexpr int   kStringTruncLen      = 0x1b8;  // truncation observed in FUN_140046190

// ---------------------------------------------------------------------------
// Forward declarations
// ---------------------------------------------------------------------------
struct InputTracker;
struct WindowMetadata;
struct WindowTracker;
struct CaptureContext;
struct VideoSequencer;
struct PipeHandshakeState;

static void CALLBACK WinEventCallback(
    HWINEVENTHOOK hook, DWORD event, HWND hwnd,
    LONG objectId, LONG childId, DWORD eventThread, DWORD eventTime);
static LRESULT CALLBACK LowLevelKeyboardCallback(int code, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK LowLevelMouseCallback(int code, WPARAM wParam, LPARAM lParam);
static BOOL    CALLBACK EnumDesktopWindowCallback(HWND hwnd, LPARAM lParam);
static DWORD   WINAPI   InputAndWinEventThread(LPVOID arg);
static DWORD   WINAPI   PollThread(LPVOID arg);

// ---------------------------------------------------------------------------
// Utility helpers
// ---------------------------------------------------------------------------
static std::wstring ToWStr(DWORD v) { std::wstringstream s; s << v; return s.str(); }

static std::wstring HexW(UINT_PTR v)
{
    std::wstringstream s;
    s << L"0x" << std::hex << v << std::dec;
    return s.str();
}

static std::wstring BoolW(bool v) { return v ? L"yes" : L"no"; }

static std::wstring FormatDuration(DWORD ms)
{
    DWORD s  = ms / 1000, h = s / 3600, m = (s % 3600) / 60, sc = s % 60;
    std::wstringstream ss;
    ss << (h  < 10 ? L"0" : L"") << h  << L":"
       << (m  < 10 ? L"0" : L"") << m  << L":"
       << (sc < 10 ? L"0" : L"") << sc;
    return ss.str();
}

static std::wstring IdleState(DWORD ms)
{
    if (ms >= kAwayThresholdMs) return L"AWAY";
    if (ms >= kIdleThresholdMs) return L"IDLE";
    return L"ACTIVE";
}

static std::wstring ToLowerW(std::wstring s)
{
    std::transform(s.begin(), s.end(), s.begin(),
        [](wchar_t c) { return static_cast<wchar_t>(::towlower(c)); });
    return s;
}

// ---------------------------------------------------------------------------
// ===  TLSSERVICE.EXE LAYER  =================================================
//
// Structures inferred from FUN_140040dd0, FUN_1400405d0, FUN_140046190,
// FUN_1400501d0 and their callers.
// ---------------------------------------------------------------------------

// Object layout from FUN_14003ee60 / FUN_140040dd0 / FUN_1400405d0.
// Confirmed offsets in Ghidra pseudocode comments in the report.
struct InputTracker
{
    HHOOK         keyboardHook  = nullptr;  // object +0x78
    HHOOK         mouseHook     = nullptr;  // object +0x80
    HWINEVENTHOOK winEventHook  = nullptr;  // object +0x88
    volatile HWND lastHwnd      = nullptr;  // object +0x90  <-- THE STICKY HWND (root of bug)
    BYTE          keyState[256] = {};       // key-state table for ToUnicodeEx (0x100 entries)
    DWORD         deadKeyFlag   = 0;        // object +0x98  dead-key accumulation flag
};

// Window metadata node in QMap (FUN_140046190).
// Field offsets in original: +0x20 hwnd, +0x28 title, +0x30 pid-string,
//                             +0x38 process-path-lower, +0x40 extra, +0x48..+0x54 rect.
struct WindowMetadata
{
    HWND         hwnd            = nullptr; // +0x20
    std::wstring title;                     // +0x28
    std::wstring pidStr;                    // +0x30  (stored as QString in original)
    std::wstring processPathLower;          // +0x38  toLower applied
    std::wstring extraStr;                  // +0x40
    RECT         rect            = {};      // +0x48..+0x54
    DWORD        pid             = 0;
    DWORD        windowThreadId  = 0;
};

// QMap container at param_1+0x68 in FUN_140046190.
struct WindowTracker
{
    std::map<HWND, WindowMetadata> windows;
    HWND                           currentHwnd = nullptr;
};

// Capture context (FUN_1400501d0 / FUN_1400515a0).
// context+0x8c   = PrintWindow flags
// context+0x10+0x418 = pixel buffer (GetDIBits destination)
// context+0x18   = buffer capacity; context+0x20 = required size
struct CaptureContext
{
    DWORD              printWindowFlags = 0;    // +0x8c  PW_RENDERFULLCONTENT=2
    std::vector<UINT8> pixels;                  // +0x10+0x418
    int                width            = 0;
    int                height           = 0;
    SIZE_T             bufferCapacity   = 0;    // +0x18
    SIZE_T             requiredSize     = 0;    // +0x20
    BOOL               lastCaptureOk   = FALSE;
};

// Named-pipe handshake state (FUN_1400c3af0 / FUN_1400323a0).
// Reply fields written to param_1+0x40/+0x48/+0x50/+0x58.
struct PipeHandshakeState
{
    UINT64 field40 = 0;
    UINT64 field48 = 0;
    UINT64 field50 = 0;
    UINT64 field58 = 0;  // status
};

// Shared-memory header (FUN_14016ace0 / FUN_1402a3ab0).
// offset 0: global counter (_DAT_14037028c); offset 8: applicationPid.
struct AgentChannelHeader
{
    volatile LONG counter  = 0;
    DWORD         ownerPid = 0;
    DWORD         pad      = 0;
};

// ---------------------------------------------------------------------------
// Global state (mirrors globals visible in tlsservice.exe)
// ---------------------------------------------------------------------------
static InputTracker  g_input;
static WindowTracker g_windowTracker;
static HANDLE        g_stopEvent   = nullptr;
static HANDLE        g_pollThread  = nullptr;
static HANDLE        g_inputThread = nullptr;
static CRITICAL_SECTION g_printLock;
static CRITICAL_SECTION g_trackerLock;

static volatile LONG g_keyboardEvents      = 0;
static volatile LONG g_mouseEvents         = 0;
static volatile LONG g_injectedKbdEvents   = 0;
static volatile LONG g_injectedMouseEvents = 0;
static volatile LONG g_lastPhysicalTick    = 0;
static volatile LONG g_foregroundEventSeq  = 0;
static volatile LONG g_lastEventThreadId   = 0;
static volatile LONG g_lastEventTime       = 0;

// ---------------------------------------------------------------------------
// FUN_14003e8e0  --  Install low-level keyboard + mouse hooks
// "SetWindowsHookExA(0xd, &DAT_1400088d7, module, 0) -> param_1+0x78"
// "SetWindowsHookExA(0xe, &DAT_1400088d7, module, ..) -> param_1+0x80"
// ---------------------------------------------------------------------------
static bool InstallLowLevelInputHooks(InputTracker* t)
{
    HMODULE mod = GetModuleHandleW(nullptr);

    // WH_KEYBOARD_LL = 13 = 0xD
    t->keyboardHook = SetWindowsHookExA(
        WH_KEYBOARD_LL, LowLevelKeyboardCallback, mod, 0);

    // WH_MOUSE_LL = 14 = 0xE
    t->mouseHook = SetWindowsHookExA(
        WH_MOUSE_LL, LowLevelMouseCallback, mod, 0);

    if (!t->keyboardHook || !t->mouseHook) {
        DWORD err = GetLastError();
        std::wcerr << L"[ERROR] SetWindowsHookExA failed, code=" << err << L"\n";
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// FUN_1400405d0  --  Low-level keyboard hook callback  (WH_KEYBOARD_LL)
// "uses HWND from object+0x90 or GetForegroundWindow()"
// "GetWindowThreadProcessId -> GetKeyboardLayout -> ToUnicodeEx"
// "sends event via thunk_FUN_14003d5d0"
// ---------------------------------------------------------------------------
static LRESULT CALLBACK LowLevelKeyboardCallback(int code, WPARAM wParam, LPARAM lParam)
{
    if (code < 0)
        return CallNextHookEx(g_input.keyboardHook, code, wParam, lParam);

    InterlockedIncrement(&g_keyboardEvents);

    const KBDLLHOOKSTRUCT* kbd = reinterpret_cast<const KBDLLHOOKSTRUCT*>(lParam);

    if (kbd->flags & LLKHF_INJECTED) {
        InterlockedIncrement(&g_injectedKbdEvents);
    } else {
        InterlockedExchange(&g_lastPhysicalTick, static_cast<LONG>(GetTickCount()));
    }

    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
    {
        // "for key events, uses HWND from object+0x90 or GetForegroundWindow()"
        HWND hwnd = static_cast<HWND>(
            InterlockedCompareExchangePointer(
                reinterpret_cast<volatile PVOID*>(&g_input.lastHwnd),
                nullptr, nullptr));
        if (!hwnd) hwnd = GetForegroundWindow();

        if (hwnd)
        {
            DWORD pid = 0;
            DWORD tid = GetWindowThreadProcessId(hwnd, &pid);
            HKL   layout = GetKeyboardLayout(tid);

            // Refresh 256-entry key state table (observed: 0x100 VK codes)
            for (int i = 0; i < 256; ++i)
                g_input.keyState[i] = static_cast<BYTE>(GetKeyState(i) & 0xFF);

            WCHAR charBuf[8] = {};
            int result = ToUnicodeEx(
                kbd->vkCode, kbd->scanCode,
                g_input.keyState, charBuf, 8,
                g_input.deadKeyFlag, layout);

            // result < 0 -> dead key; > 0 -> printable characters
            if (result < 0) g_input.deadKeyFlag = 1;
            else            g_input.deadKeyFlag = 0;
            // Original sends via thunk_FUN_14003d5d0 (IPC/activity channel).
            (void)pid;
        }
    }

    return CallNextHookEx(g_input.keyboardHook, code, wParam, lParam);
}

// Low-level mouse hook (WH_MOUSE_LL = 0xE).
// "for mouse events sends coordinates via thunk_FUN_14003d760"
static LRESULT CALLBACK LowLevelMouseCallback(int code, WPARAM wParam, LPARAM lParam)
{
    if (code < 0)
        return CallNextHookEx(g_input.mouseHook, code, wParam, lParam);

    InterlockedIncrement(&g_mouseEvents);

    const MSLLHOOKSTRUCT* mll = reinterpret_cast<const MSLLHOOKSTRUCT*>(lParam);
    if (mll->flags & kMouseInjectedFlag)
        InterlockedIncrement(&g_injectedMouseEvents);
    else
        InterlockedExchange(&g_lastPhysicalTick, static_cast<LONG>(GetTickCount()));
    // Original: forwards via thunk_FUN_14003d760.
    (void)wParam;

    return CallNextHookEx(g_input.mouseHook, code, wParam, lParam);
}

// ---------------------------------------------------------------------------
// FUN_140040dd0  --  WinEvent callback (focus / foreground changes)
// "if param_2 == 0x8005: GetGUIThreadInfo, save hwndActive to object+0x90"
// "if param_2 == 0x20:   save param_3 (hwnd) to object+0x90"
// "unhooks low-level hooks and re-initialises via FUN_14003fa20"
// ---------------------------------------------------------------------------
static void CALLBACK WinEventCallback(
    HWINEVENTHOOK /*hook*/, DWORD event, HWND hwnd,
    LONG /*objectId*/, LONG /*childId*/,
    DWORD eventThread, DWORD eventTime)
{
    HWND newHwnd = nullptr;

    if (event == static_cast<DWORD>(kWinEventMax)) // 0x8005
    {
        GUITHREADINFO gui{};
        gui.cbSize = sizeof(gui);
        if (GetGUIThreadInfo(0, &gui) && gui.hwndActive)
            newHwnd = gui.hwndActive;
        else
            newHwnd = hwnd;
    }
    else if (event == static_cast<DWORD>(kWinEventMin)) // 0x0020
    {
        newHwnd = hwnd;
    }
    else if (event == EVENT_SYSTEM_FOREGROUND)
    {
        newHwnd = hwnd;
    }

    if (newHwnd)
    {
        // BUG RISK #1: object+0x90 updated only here.
        // If this callback never fires for AutoCAD (blocked message loop,
        // lost hook, or wrong desktop), lastHwnd stays on Chrome.
        // Diagnostic: "All HWNDs agree" field in snapshot output.
        InterlockedExchangePointer(
            reinterpret_cast<volatile PVOID*>(&g_input.lastHwnd),
            static_cast<PVOID>(newHwnd));
        // Original also calls: unhook LL hooks + FUN_14003fa20 (re-init).
    }

    InterlockedExchange(&g_lastEventThreadId, static_cast<LONG>(eventThread));
    InterlockedExchange(&g_lastEventTime,     static_cast<LONG>(eventTime));
    InterlockedIncrement(&g_foregroundEventSeq);
}

// ---------------------------------------------------------------------------
// FUN_140047d80  --  Query foreground HWND from the input desktop
// "OpenInputDesktop -> verify name -> SetThreadDesktop ->
//  GetForegroundWindow -> restore -> CloseDesktop"
// ---------------------------------------------------------------------------
static HWND QueryForegroundOnInputDesktop()
{
    HDESK oldDesktop   = GetThreadDesktop(GetCurrentThreadId());
    HDESK inputDesktop = OpenInputDesktop(0, FALSE, kDesktopAccessMask);
    if (!inputDesktop) return nullptr;

    char desktopName[256] = {}; DWORD needed = 0;
    GetUserObjectInformationA(inputDesktop, UOI_NAME, desktopName, sizeof(desktopName), &needed);
    // Original checks name and may skip non-interactive desktops.

    HWND hwnd = nullptr;
    if (SetThreadDesktop(inputDesktop))
    {
        // BUG RISK #2 / #5: if AutoCAD is elevated or on a different
        // window-station, GetForegroundWindow may return NULL or old HWND.
        hwnd = GetForegroundWindow();
        SetThreadDesktop(oldDesktop);
    }

    CloseDesktop(inputDesktop);
    return hwnd;
}

// ---------------------------------------------------------------------------
// FUN_1400478b0  --  Enumerate windows on a named desktop
// "OpenDesktopA -> EnumDesktopWindows(&UNK_1400034db) -> CloseDesktop"
// ---------------------------------------------------------------------------
static void EnumerateDesktopWindows(const char* desktopName, WindowTracker* tracker)
{
    HDESK desktop = OpenDesktopA(const_cast<LPSTR>(desktopName), 0, FALSE, GENERIC_READ | GENERIC_WRITE);
    if (!desktop) return;
    EnumDesktopWindows(desktop, EnumDesktopWindowCallback,
                       reinterpret_cast<LPARAM>(tracker));
    CloseDesktop(desktop);
}

// ---------------------------------------------------------------------------
// FUN_1400471d0  --  Collect window metadata (HWND/PID/title/path)
// "IsWindowVisible -> GetWindow(owner) -> GetWindowInfo ->
//  GetWindowThreadProcessId -> OpenProcess -> GetProcessImageFileNameW ->
//  GetWindowTextW -> builds struct -> passes to FUN_140046190"
// ---------------------------------------------------------------------------
static bool CollectWindowMetadata(HWND hwnd, WindowMetadata& out)
{
    if (!IsWindowVisible(hwnd)) return false;

    // Original filters owned windows via GetWindow + GetWindowLongPtrW/GetWindowInfo.
    if (GetWindow(hwnd, GW_OWNER) != nullptr) return false;

    WINDOWINFO wi{}; wi.cbSize = sizeof(wi);
    if (!GetWindowInfo(hwnd, &wi)) return false;

    DWORD pid = 0;
    DWORD tid = GetWindowThreadProcessId(hwnd, &pid);

    wchar_t title[kWindowTitleMax] = {};
    GetWindowTextW(hwnd, title, kWindowTitleMax);
    title[kStringTruncLen] = L'\0'; // truncate to 0x1b8 as in FUN_140046190

    wchar_t processPath[kProcessPathMax] = {};
    HANDLE hProc = OpenProcess(kOpenProcessFlags, FALSE, pid);
    if (hProc) {
        GetProcessImageFileNameW(hProc, processPath, kProcessPathMax);
        CloseHandle(hProc);
    }

    RECT rect{}; GetWindowRect(hwnd, &rect);

    out.hwnd             = hwnd;
    out.pid              = pid;
    out.windowThreadId   = tid;
    out.title            = title;
    out.pidStr           = ToWStr(pid);
    out.processPathLower = ToLowerW(processPath);  // toLower applied in FUN_140046190
    out.rect             = rect;
    return true;
}

// ---------------------------------------------------------------------------
// FUN_140046190  --  Normalise and store metadata in QMap keyed by HWND
// "truncate to 0x1b8, toLower, search QMap at param_1+0x68 by HWND;
//  if found: update fields; if not: insert new node"
// ---------------------------------------------------------------------------
static void StoreWindowMetadata(WindowTracker* tracker, const WindowMetadata& meta)
{
    // BUG RISK #3: QMap keeps every HWND ever inserted.
    // When lastHwnd (object+0x90) is still Chrome due to bug #1,
    // the pipeline queries the QMap with Chrome's HWND and gets stale metadata,
    // even though the employee is in AutoCAD.
    EnterCriticalSection(&g_trackerLock);
    tracker->windows[meta.hwnd] = meta;
    tracker->currentHwnd        = meta.hwnd;
    LeaveCriticalSection(&g_trackerLock);
}

static void UpdateWindowMetadata(HWND hwnd)
{
    WindowMetadata meta{};
    if (CollectWindowMetadata(hwnd, meta))
        StoreWindowMetadata(&g_windowTracker, meta);
}

static BOOL CALLBACK EnumDesktopWindowCallback(HWND hwnd, LPARAM lParam)
{
    auto* tracker = reinterpret_cast<WindowTracker*>(lParam);
    WindowMetadata meta{};
    if (CollectWindowMetadata(hwnd, meta))
        StoreWindowMetadata(tracker, meta);
    return TRUE;
}

// ---------------------------------------------------------------------------
// FUN_1400501d0  --  GDI capture (PrintWindow / BitBlt / GetDIBits)
// "if hwndOrZero == 0: BitBlt(memDC,...,hdc,...,SRCCOPY)"
// "else: PrintWindow(hwndOrZero, memDC, flags from context+0x8c)"
// "GetDIBits into context+0x10+0x418"
// ---------------------------------------------------------------------------
static bool CaptureGdiToPixels(CaptureContext* ctx, HDC sourceDc,
                                HWND hwndOrNull, RECT rect)
{
    int width  = rect.right  - rect.left;
    int height = rect.bottom - rect.top;
    if (width <= 0 || height <= 0) return false;

    HBITMAP bitmap = CreateCompatibleBitmap(sourceDc, width, height);
    HDC     memDc  = CreateCompatibleDC(sourceDc);
    if (!bitmap || !memDc) {
        if (bitmap) DeleteObject(bitmap);
        if (memDc)  DeleteDC(memDc);
        return false;
    }

    HGDIOBJ old = SelectObject(memDc, bitmap);

    BOOL copied = FALSE;
    if (!hwndOrNull)
    {
        // Foreground window path: BitBlt from window/screen DC.
        copied = BitBlt(memDc, 0, 0, width, height,
                        sourceDc, rect.left, rect.top, SRCCOPY);
    }
    else
    {
        // Non-foreground path: PrintWindow.
        // BUG RISK #4: AutoCAD (GPU/DirectX) renders to a surface that does
        // not respond to GDI PrintWindow without PW_RENDERFULLCONTENT (flag=2).
        // When PrintWindow returns FALSE or a blank bitmap, the pixel buffer is
        // NOT updated -> the PREVIOUS valid Chrome frame is kept and forwarded.
        // Fix: set ctx->printWindowFlags = 2 (PW_RENDERFULLCONTENT).
        copied = PrintWindow(hwndOrNull, memDc, ctx->printWindowFlags);
    }

    if (copied)
    {
        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth       = width;
        bmi.bmiHeader.biHeight      = -height;  // top-down DIB
        bmi.bmiHeader.biPlanes      = 1;
        bmi.bmiHeader.biBitCount    = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        SIZE_T needed = static_cast<SIZE_T>(width) * height * 4;

        if (needed > ctx->bufferCapacity)
        {
            ctx->requiredSize = needed;   // context+0x20: signal caller to reallocate
        }
        else
        {
            ctx->pixels.resize(needed);
            ctx->width  = width;
            ctx->height = height;
            // First call: query scanline count (null data ptr, as observed in report).
            GetDIBits(memDc, bitmap, 0, static_cast<UINT>(height),
                      nullptr, &bmi, DIB_RGB_COLORS);
            // Second call: retrieve actual pixels into context+0x10+0x418.
            int rows = GetDIBits(memDc, bitmap, 0, static_cast<UINT>(height),
                                 ctx->pixels.data(), &bmi, DIB_RGB_COLORS);
            copied = (rows > 0);
            // Original calls thunk_FUN_14004dcc0 for packing/conversion.
        }
    }

    ctx->lastCaptureOk = copied;

    SelectObject(memDc, old);
    DeleteDC(memDc);
    DeleteObject(bitmap);
    return !!copied;
}

// ---------------------------------------------------------------------------
// FUN_1400515a0  --  Window capture wrapper
// "GetWindowRect -> normalise -> GetWindowDC ->
//  if foreground==hwnd pass hwnd=0 else hwnd -> FUN_1400501d0 -> ReleaseDC"
// ---------------------------------------------------------------------------
static bool CaptureWindowWrapper(CaptureContext* ctx, HWND hwnd)
{
    RECT rect{};
    if (!GetWindowRect(hwnd, &rect)) return false;

    // "normalise rect to (0,0,width,height)"
    int w = rect.right - rect.left, h = rect.bottom - rect.top;
    RECT normalized{ 0, 0, w, h };

    HDC windowDc = GetWindowDC(hwnd);
    if (!windowDc) return false;

    // "if GetForegroundWindow() == hwnd, pass hwnd=0 -> BitBlt path"
    HWND printTarget = (GetForegroundWindow() == hwnd) ? nullptr : hwnd;

    bool ok = CaptureGdiToPixels(ctx, windowDc, printTarget, normalized);
    ReleaseDC(hwnd, windowDc);
    return ok;
}

// ---------------------------------------------------------------------------
// FUN_14016ace0 / FUN_1402a3ab0  --  Shared memory agent channel
// "CreateMutexA(key) -> WaitForSingleObject(INFINITE) ->
//  QSharedMemory::create -> zero -> write counter@0 + appPid@8 ->
//  ReleaseMutex"
// ---------------------------------------------------------------------------
static HANDLE CreateAgentSharedMemoryChannel(const char* key, DWORD size)
{
    HANDLE mutex = CreateMutexA(nullptr, FALSE, key);
    if (!mutex) return nullptr;

    WaitForSingleObject(mutex, INFINITE);

    std::string mapKey = std::string(key) + "_shm";
    HANDLE hMap = CreateFileMappingA(
        INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, size, mapKey.c_str());
    if (hMap) {
        void* view = MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, size);
        if (view) {
            ZeroMemory(view, size);
            auto* hdr    = static_cast<AgentChannelHeader*>(view);
            hdr->counter  = 0;
            hdr->ownerPid = GetCurrentProcessId();
            UnmapViewOfFile(view);
        }
        CloseHandle(hMap);
    }

    ReleaseMutex(mutex);
    return mutex;
}

// ---------------------------------------------------------------------------
// FUN_1400c37f0 + _c38d0 + _c3960 + _c3af0  --  Named-pipe client helpers
// ---------------------------------------------------------------------------
static HANDLE OpenNamedPipeWithRetry(const wchar_t* pipeName,
                                     DWORD access = kPipeAccessMask,
                                     DWORD flags  = kPipeFlags)
{
    for (int attempt = 0; attempt < 2; ++attempt) {
        HANDLE h = CreateFileW(pipeName, access, 0, nullptr, OPEN_EXISTING, flags, nullptr);
        if (h != INVALID_HANDLE_VALUE) return h;
        if (GetLastError() != ERROR_PIPE_BUSY) break;
        WaitNamedPipeW(pipeName, kPipeTimeoutMs);
    }
    return nullptr;
}

static HANDLE OpenMessageModePipe(const wchar_t* pipeName)
{
    HANDLE h = OpenNamedPipeWithRetry(pipeName);
    if (!h) return nullptr;
    DWORD mode = PIPE_READMODE_MESSAGE;
    if (!SetNamedPipeHandleState(h, &mode, nullptr, nullptr)) { CloseHandle(h); return nullptr; }
    return h;
}

static HANDLE OpenPipeAndDuplicateToProcess(const wchar_t* pipeName, HANDLE targetProcess)
{
    HANDLE local = OpenNamedPipeWithRetry(pipeName);
    if (!local) return INVALID_HANDLE_VALUE;
    HANDLE remote = INVALID_HANDLE_VALUE;
    DuplicateHandle(GetCurrentProcess(), local, targetProcess, &remote, 0, FALSE, DUPLICATE_SAME_ACCESS);
    CloseHandle(local);
    return remote;
}

// FUN_1400c3af0 / FUN_1400323a0
// "request.type=1, pid=GetCurrentProcessId; reply.type==2, ack.type=3"
// "stores reply fields to param_1+0x40/+0x48/+0x50/+0x58"
static bool PerformPipeHandshake(HANDLE pipe, PipeHandshakeState* state)
{
    BYTE request[kPipePacketSize]  = {};
    BYTE response[kPipePacketSize] = {};
    DWORD transferred = 0;

    *reinterpret_cast<int*>  (request + 0x00) = 1;
    *reinterpret_cast<DWORD*>(request + 0x04) = GetCurrentProcessId();

    BOOL ok = TransactNamedPipe(pipe, request, sizeof(request),
                                response, sizeof(response), &transferred, nullptr);
    if (!ok) return false;

    int   replyType   = *reinterpret_cast<int*>  (response + 0x00);
    DWORD replyStatus = *reinterpret_cast<DWORD*>(response + 0x04);
    if (replyType != 2 || replyStatus == 0) return false;

    UINT64 ptr1 = *reinterpret_cast<UINT64*>(response + 0x08);
    UINT64 ptr2 = *reinterpret_cast<UINT64*>(response + 0x10);
    if (!ptr1 || !ptr2) return false;

    BYTE ack[kPipePacketSize] = {};
    *reinterpret_cast<int*>(ack + 0x00) = 3;
    WriteFile(pipe, ack, sizeof(ack), &transferred, nullptr);

    state->field40 = *reinterpret_cast<UINT64*>(response + 0x18);
    state->field48 = *reinterpret_cast<UINT64*>(response + 0x20);
    state->field50 = *reinterpret_cast<UINT64*>(response + 0x28);
    state->field58 = replyStatus;
    return true;
}

// ---------------------------------------------------------------------------
// ===  TLSHOST.EXE LAYER  ====================================================
//
// Video pipeline: QImage ring-buffer, delta encoding, frame serialization.
// From FUN_14028e700, FUN_14028ee20, FUN_14028fcd0, FUN_14028c570.
// ---------------------------------------------------------------------------

struct ImageFrame
{
    int                width  = 0;
    int                height = 0;
    int                format = 0;  // expected: kQImageFormat_RGB555 = 12
    std::vector<UINT8> bytes;
    bool IsNull() const { return bytes.empty(); }
};

// FUN_14028e700 init / constructor.
// Confirmed offsets:
//   +0x18 capacity, +0x28 readIndex, +0x2c availableCount, +0x30 paused,
//   +0x38/+0x40 lastSent ts/QImage, +0x60/+0x68 compare ts/QImage.
struct VideoSequencer
{
    int                    capacity       = kRingBufferCapacity;
    std::vector<ImageFrame> ringBuffer;
    int                    readIndex      = 0;               // +0x28
    int                    availableCount = 0;               // +0x2c
    bool                   paused         = false;           // +0x30
    ULONGLONG              lastSentTs     = 0;               // +0x38
    ImageFrame             lastSentFrame;                    // +0x40
    ULONGLONG              compareTs      = 0;               // +0x60
    ImageFrame             compareFrame;                     // +0x68
};

static void InitVideoSequencer(VideoSequencer& seq, int cap)
{
    seq.capacity       = cap;
    seq.ringBuffer.assign(static_cast<size_t>(cap), ImageFrame{});
    seq.readIndex      = 0;
    seq.availableCount = 0;
    seq.paused         = false;
    seq.lastSentFrame  = {};
    seq.compareFrame   = {};
}

static void PushFrameToSequencer(VideoSequencer& seq, const ImageFrame& frame)
{
    if (seq.availableCount >= seq.capacity) return;
    int writeIdx = (seq.readIndex + seq.availableCount) % seq.capacity;
    seq.ringBuffer[static_cast<size_t>(writeIdx)] = frame;
    ++seq.availableCount;
}

static ImageFrame PopNextFrame(VideoSequencer& seq)
{
    if (seq.availableCount <= 0) return {};
    ImageFrame f = seq.ringBuffer[static_cast<size_t>(seq.readIndex)];
    seq.readIndex = (seq.readIndex + 1) % seq.capacity;
    --seq.availableCount;
    return f;
}

// FUN_14028fcd0 -- serialise a frame (or rect sub-region) into a byte packet.
struct FramePacket
{
    int                x = 0, y = 0, width = 0, height = 0;
    int                format = 0;
    std::vector<UINT8> pixels;
};

static FramePacket SerializeFrameRect(const ImageFrame& frame, int x, int y, int w, int h)
{
    FramePacket pkt;
    pkt.x = x; pkt.y = y; pkt.width = w; pkt.height = h;
    pkt.format = frame.format;

    if (x == 0 && y == 0 && w == frame.width && h == frame.height) {
        pkt.pixels = frame.bytes;   // full frame – no copy
    } else {
        // Sub-rect: equivalent to QImage::copy(rect).constBits().
        int bpp = 2;  // RGB555 = 2 bytes per pixel
        pkt.pixels.reserve(static_cast<size_t>(w) * h * bpp);
        for (int row = y; row < y + h; ++row) {
            const UINT8* src = frame.bytes.data()
                + static_cast<size_t>(row) * frame.width * bpp
                + static_cast<size_t>(x) * bpp;
            pkt.pixels.insert(pkt.pixels.end(), src, src + static_cast<size_t>(w) * bpp);
        }
    }
    return pkt;
}

static bool FrameRegionChanged(const ImageFrame& prev, const ImageFrame& curr,
                               int x, int y, int w, int h)
{
    if (prev.width != curr.width || prev.height != curr.height) return true;
    if (prev.bytes.size() != curr.bytes.size()) return true;
    int bpp = 2;
    for (int row = y; row < y + h; ++row) {
        size_t off = static_cast<size_t>(row) * curr.width * bpp + static_cast<size_t>(x) * bpp;
        if (memcmp(prev.bytes.data() + off, curr.bytes.data() + off,
                   static_cast<size_t>(w) * bpp) != 0) return true;
    }
    return false;
}

// FUN_14028ee20 -- ring-buffer consumer: build delta or full-frame packets.
// "if paused: return null" / "if availableCount==0: return null"
// "assert format == QImage::Format_RGB555"
// "if no previous or size changed: full rect"
// "else: compute changed rects; if efficient send rects, else full frame"
static std::vector<FramePacket> BuildVideoDeltaPackets(VideoSequencer& seq)
{
    std::vector<FramePacket> packets;

    if (seq.paused) return packets;          // +0x30 set: no frames emitted
    if (seq.availableCount == 0) return packets; // +0x2c == 0: stuck on last frame

    ImageFrame current = PopNextFrame(seq);
    if (current.IsNull()) return packets;

    // Source-level assert from videoStreamSequencer.cpp path:
    // "image.format() == QImage::Format_RGB555"
    assert(current.format == kQImageFormat_RGB555);

    bool sendFull = seq.lastSentFrame.IsNull() ||
                    seq.lastSentFrame.width  != current.width  ||
                    seq.lastSentFrame.height != current.height;

    if (sendFull) {
        packets.push_back(SerializeFrameRect(current, 0, 0, current.width, current.height));
    } else {
        int tileW = 64, tileH = 64;
        std::vector<FramePacket> changed;
        for (int ry = 0; ry < current.height; ry += tileH) {
            for (int rx = 0; rx < current.width; rx += tileW) {
                int tw = std::min(tileW, current.width  - rx);
                int th = std::min(tileH, current.height - ry);
                if (FrameRegionChanged(seq.lastSentFrame, current, rx, ry, tw, th))
                    changed.push_back(SerializeFrameRect(current, rx, ry, tw, th));
            }
        }
        if (changed.empty()) return packets;

        SIZE_T deltaBytes = 0;
        for (const auto& p : changed) deltaBytes += p.pixels.size();
        SIZE_T fullBytes  = current.bytes.size();

        if (deltaBytes < fullBytes / 2) packets = std::move(changed);
        else packets.push_back(SerializeFrameRect(current, 0, 0, current.width, current.height));
    }

    seq.lastSentFrame = current;
    seq.lastSentTs    = static_cast<ULONGLONG>(GetTickCount());
    return packets;
}

// FUN_14028c570 -- send frame if stream active and time negotiated.
static void SendVideoFrameIfReady(VideoSequencer& seq, bool streamActive, bool timeNegotiated)
{
    if (!streamActive) return;
    auto packets = BuildVideoDeltaPackets(seq);
    if (packets.empty()) return;

    if (!timeNegotiated) {
        // "trying to send frame while time is not negotiated" -> drop all
        seq.availableCount = 0;
        return;
    }
    for (const auto& pkt : packets) { (void)pkt; }  // send_packet_to_transport(pkt)
}

// FUN_140089540 -- map component profile.
struct ComponentNames {
    std::wstring hostExe, serviceExe, subserviceExe;
    std::wstring hostName, serviceName, subserviceName;
    std::wstring family, displayName;
};

static ComponentNames MapComponentProfile(const std::wstring& exeName)
{
    ComponentNames n;
    if (exeName == L"tlshost.exe"      ||
        exeName == L"tlsservice.exe"   ||
        exeName == L"tlssubservice.exe")
    {
        n.hostExe        = L"tlshost.exe";
        n.serviceExe     = L"tlsservice.exe";
        n.subserviceExe  = L"tlssubservice.exe";
        n.hostName       = L"tlshost";
        n.serviceName    = L"tlsservice";
        n.subserviceName = L"tlssubservice";
        n.family         = L"tls";
        n.displayName    = L"Telemetry Service Host";
    }
    return n;
}

// FUN_140127170 -- OpenGL/GPU telemetry ("KickidlerTelemtry").
static void CollectOpenGLTelemetry()
{
    HMODULE gl = LoadLibraryA("opengl32.dll");
    if (!gl) return;
    // Original: creates 1x1 hidden window, wglCreateContext,
    // glGetString(GL_VENDOR/GL_RENDERER/GL_VERSION/GL_EXTENSIONS).
    auto p1 = GetProcAddress(gl, "wglCreateContext");
    auto p2 = GetProcAddress(gl, "wglMakeCurrent");
    auto p3 = GetProcAddress(gl, "wglDeleteContext");
    auto p4 = GetProcAddress(gl, "glGetString");
    (void)p1; (void)p2; (void)p3; (void)p4;
    FreeLibrary(gl);
}

// ---------------------------------------------------------------------------
// ===  TSSUBSERVICE.EXE LAYER  ===============================================
//
// Subagent launched with "-ipc-name <name>" in the user session.
// FUN_140017280 main logic.
// ---------------------------------------------------------------------------
struct SubAgentArgs
{
    std::string ipcName;
    std::string loggingPostfix;
    DWORD       sessionId = 0;
};

static SubAgentArgs ParseSubAgentArgs(int argc, char** argv)
{
    SubAgentArgs a;
    ProcessIdToSessionId(GetCurrentProcessId(), &a.sessionId);
    for (int i = 1; i < argc - 1; ++i) {
        if (std::string(argv[i]) == "-ipc-name")        a.ipcName        = argv[i + 1];
        if (std::string(argv[i]) == "-logging-postfix") a.loggingPostfix = argv[i + 1];
    }
    return a;
}

// ---------------------------------------------------------------------------
// ===  DIAGNOSTIC OUTPUT LAYER  ==============================================
// Surfaces all five bug-risk conditions simultaneously.
// ---------------------------------------------------------------------------
struct DiagnosticSnapshot
{
    HWND         lastHwnd      = nullptr;   // object+0x90 (cached)
    HWND         fgHwnd        = nullptr;   // GetForegroundWindow()
    HWND         desktopHwnd   = nullptr;   // QueryForegroundOnInputDesktop()
    bool         hwndsAgree    = false;
    std::wstring cachedTitle, cachedPath;
    DWORD        cachedPid     = 0;
    std::wstring liveTitle, livePath;
    DWORD        livePid       = 0;
    LONG         kbEvents      = 0;
    LONG         mouseEvts     = 0;
    LONG         injKbd        = 0;
    LONG         injMouse      = 0;
    DWORD        physIdleMs    = 0;
    DWORD        winIdleMs     = 0;
    bool         winIdleOk     = false;
    std::wstring inputDesktop;
    DWORD        sessionId     = 0;
    DWORD        tick          = 0;
};

static DiagnosticSnapshot BuildDiagnosticSnapshot()
{
    DiagnosticSnapshot d;
    d.tick = GetTickCount();

    d.lastHwnd    = static_cast<HWND>(
        InterlockedCompareExchangePointer(
            reinterpret_cast<volatile PVOID*>(&g_input.lastHwnd), nullptr, nullptr));
    d.fgHwnd      = GetForegroundWindow();
    d.desktopHwnd = QueryForegroundOnInputDesktop();
    d.hwndsAgree  = (d.lastHwnd == d.fgHwnd) && (d.fgHwnd == d.desktopHwnd);

    EnterCriticalSection(&g_trackerLock);
    auto it = g_windowTracker.windows.find(d.lastHwnd);
    if (it != g_windowTracker.windows.end()) {
        d.cachedTitle = it->second.title;
        d.cachedPath  = it->second.processPathLower;
        d.cachedPid   = it->second.pid;
    }
    LeaveCriticalSection(&g_trackerLock);

    if (d.fgHwnd) {
        wchar_t buf[kWindowTitleMax] = {};
        GetWindowTextW(d.fgHwnd, buf, kWindowTitleMax);
        d.liveTitle = buf;
        DWORD lpid = 0;
        GetWindowThreadProcessId(d.fgHwnd, &lpid);
        d.livePid = lpid;
        wchar_t pbuf[kProcessPathMax] = {};
        HANDLE hp = OpenProcess(kOpenProcessLight, FALSE, lpid);
        if (hp) { GetProcessImageFileNameW(hp, pbuf, kProcessPathMax); CloseHandle(hp); }
        d.livePath = pbuf;
    }

    d.kbEvents   = InterlockedCompareExchange(&g_keyboardEvents,      0, 0);
    d.mouseEvts  = InterlockedCompareExchange(&g_mouseEvents,         0, 0);
    d.injKbd     = InterlockedCompareExchange(&g_injectedKbdEvents,   0, 0);
    d.injMouse   = InterlockedCompareExchange(&g_injectedMouseEvents,  0, 0);

    LONG physTick = InterlockedCompareExchange(&g_lastPhysicalTick, 0, 0);
    d.physIdleMs = physTick ? (d.tick - static_cast<DWORD>(physTick)) : 0;

    LASTINPUTINFO lii{}; lii.cbSize = sizeof(lii);
    if (GetLastInputInfo(&lii)) { d.winIdleOk = true; d.winIdleMs = d.tick - lii.dwTime; }

    HDESK desk = OpenInputDesktop(0, FALSE, DESKTOP_READOBJECTS);
    if (desk) {
        char name[256] = {}; DWORD needed = 0;
        GetUserObjectInformationA(desk, UOI_NAME, name, sizeof(name), &needed);
        int wlen = MultiByteToWideChar(CP_ACP, 0, name, -1, nullptr, 0);
        if (wlen > 0) {
            std::wstring ws(static_cast<size_t>(wlen), L'\0');
            MultiByteToWideChar(CP_ACP, 0, name, -1, &ws[0], wlen);
            if (!ws.empty() && ws.back() == L'\0') ws.pop_back();
            d.inputDesktop = ws;
        }
        CloseDesktop(desk);
    }
    ProcessIdToSessionId(GetCurrentProcessId(), &d.sessionId);
    return d;
}

static void PrintDiagnosticSnapshot(const DiagnosticSnapshot& d)
{
    EnterCriticalSection(&g_printLock);

    DWORD effIdle = d.winIdleOk
        ? std::min(d.winIdleMs, d.physIdleMs > 0 ? d.physIdleMs : MAXDWORD)
        : d.physIdleMs;

    std::wcout
        << L"\n+--------------------------------------------------------------+\n"
        << L"|  GRABBER SNAPSHOT  tick=" << d.tick << L"\n"
        << L"+--------------------------------------------------------------+\n"
        << L"| FOCUS / HWND\n"
        << L"|  lastHwnd (obj+0x90): " << HexW(reinterpret_cast<UINT_PTR>(d.lastHwnd)) << L"\n"
        << L"|  GetForegroundWnd(): " << HexW(reinterpret_cast<UINT_PTR>(d.fgHwnd)) << L"\n"
        << L"|  DesktopQuery HWND:  " << HexW(reinterpret_cast<UINT_PTR>(d.desktopHwnd)) << L"\n"
        << L"|  All HWNDs agree:    " << BoolW(d.hwndsAgree) << L"\n";

    if (!d.hwndsAgree)
        std::wcout << L"|  *** MISMATCH - object+0x90 stale! BUG RISK #1 / #2 ***\n";

    std::wcout
        << L"+--------------------------------------------------------------+\n"
        << L"| QMap CACHED (lastHwnd key, param_1+0x68)\n"
        << L"|  Cached title: " << d.cachedTitle << L"\n"
        << L"|  Cached PID:   " << d.cachedPid   << L"\n"
        << L"|  Cached path:  " << d.cachedPath  << L"\n"
        << L"+--------------------------------------------------------------+\n"
        << L"| LIVE (fresh GetForegroundWindow)\n"
        << L"|  Live title:   " << d.liveTitle   << L"\n"
        << L"|  Live PID:     " << d.livePid     << L"\n"
        << L"|  Live path:    " << d.livePath    << L"\n";

    if (d.cachedPid != d.livePid && d.cachedPid != 0 && d.livePid != 0)
        std::wcout << L"|  *** METADATA STALE - QMap has old PID! BUG RISK #3 ***\n";

    std::wcout
        << L"+--------------------------------------------------------------+\n"
        << L"| ACTIVITY / IDLE\n"
        << L"|  State:          " << IdleState(effIdle) << L"\n"
        << L"|  Windows idle:   " << (d.winIdleOk ? FormatDuration(d.winIdleMs) : L"unavail") << L"\n"
        << L"|  Physical idle:  " << (d.physIdleMs > 0 ? FormatDuration(d.physIdleMs) : L"no physical input yet") << L"\n"
        << L"|  Effective idle: " << FormatDuration(effIdle) << L"\n"
        << L"|  KB events:      " << d.kbEvents   << L"\n"
        << L"|  Mouse events:   " << d.mouseEvts  << L"\n"
        << L"|  Injected KB:    " << d.injKbd     << L"\n"
        << L"|  Injected mouse: " << d.injMouse   << L"\n";

    if (d.injKbd > 0 || d.injMouse > 0)
        std::wcout << L"|  *** INJECTED INPUT detected! ***\n";

    std::wcout
        << L"+--------------------------------------------------------------+\n"
        << L"| ENVIRONMENT\n"
        << L"|  Input desktop:  " << d.inputDesktop << L"\n"
        << L"|  Session ID:     " << d.sessionId << L"\n"
        << L"+--------------------------------------------------------------+\n"
        << std::flush;

    LeaveCriticalSection(&g_printLock);
}

// ---------------------------------------------------------------------------
// InputAndWinEventThread  (FUN_14003ee60)
// ---------------------------------------------------------------------------
static DWORD WINAPI InputAndWinEventThread(LPVOID arg)
{
    InputTracker* t = static_cast<InputTracker*>(arg);

    InstallLowLevelInputHooks(t);

    HMODULE mod = GetModuleHandleW(nullptr);
    // "SetWinEventHook(0x20, 0x8005, module, &UNK_140010979, ...)"
    t->winEventHook = SetWinEventHook(
        static_cast<DWORD>(kWinEventMin), static_cast<DWORD>(kWinEventMax),
        mod, WinEventCallback, 0, 0, WINEVENT_OUTOFCONTEXT);

    HWINEVENTHOOK fgHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND,
        nullptr, WinEventCallback, 0, 0, WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    // BUG RISK #1: if this message loop stalls or the thread exits,
    // WinEvent callbacks stop firing -> lastHwnd stale forever.
    MSG msg{};
    while (GetMessageA(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    if (t->keyboardHook) { UnhookWindowsHookEx(t->keyboardHook); t->keyboardHook = nullptr; }
    if (t->mouseHook)    { UnhookWindowsHookEx(t->mouseHook);    t->mouseHook    = nullptr; }
    if (t->winEventHook) { UnhookWinEvent(t->winEventHook);      t->winEventHook = nullptr; }
    if (fgHook) UnhookWinEvent(fgHook);
    return 0;
}

// Poll thread
static DWORD WINAPI PollThread(LPVOID /*arg*/)
{
    LONG lastSeq = 0;
    while (WaitForSingleObject(g_stopEvent, kPollIntervalMs) == WAIT_TIMEOUT)
    {
        HWND fg = GetForegroundWindow();
        if (fg) UpdateWindowMetadata(fg);
        HWND dq = QueryForegroundOnInputDesktop();
        if (dq && dq != fg) UpdateWindowMetadata(dq);
        EnumerateDesktopWindows("Default", &g_windowTracker);

        LONG seq = InterlockedCompareExchange(&g_foregroundEventSeq, 0, 0);
        if (seq != lastSeq) {
            lastSeq = seq;
            PrintDiagnosticSnapshot(BuildDiagnosticSnapshot());
        }
    }
    return 0;
}

// ---------------------------------------------------------------------------
// Startup banner
// ---------------------------------------------------------------------------
static void PrintBugHypotheses()
{
    std::wcout
        << L"\n"
        << L"================================================================\n"
        << L"  testGrabb_Claude.cpp  --  Kickidler grabber reconstruction\n"
        << L"  Basis: Ghidra MCP static analysis of\n"
        << L"         tlsservice.exe / tlshost.exe / tssubservice.exe\n"
        << L"================================================================\n"
        << L"BUG: grabber stays locked on a previous window (e.g. Chrome)\n"
        << L"     and shows MAX ACTIVITY even though the employee switched\n"
        << L"     to another app (e.g. AutoCAD) and is actually idle.\n\n"
        << L"Reconstructed hypotheses from static analysis:\n\n"
        << L"  #1  object+0x90 (lastHwnd) not updated when AutoCAD gets focus.\n"
        << L"      Cause: WinEventHook callback not firing (thread starved,\n"
        << L"             hook lost, or message loop blocked).\n"
        << L"      Diagnostic: watch 'All HWNDs agree' in snapshot output.\n\n"
        << L"  #2  FUN_140047d80 (OpenInputDesktop/SetThreadDesktop) stays on\n"
        << L"      old/UAC desktop. GetForegroundWindow returns previous HWND.\n"
        << L"      Diagnostic: watch 'DesktopQuery HWND' vs 'GetForegroundWnd()'.\n\n"
        << L"  #3  QMap at param_1+0x68 keeps stale Chrome entry; AutoCAD entry\n"
        << L"      missing because IsWindowVisible/GetWindowInfo filters reject it.\n"
        << L"      Diagnostic: watch 'METADATA STALE' warning.\n\n"
        << L"  #4  PrintWindow/BitBlt returns FALSE or black bitmap for AutoCAD\n"
        << L"      (GPU/DirectX surface). Pipeline re-forwards last valid Chrome frame.\n"
        << L"      Fix: set printWindowFlags = PW_RENDERFULLCONTENT (flag=2),\n"
        << L"           or add a DXGI Desktop Duplication capture path.\n\n"
        << L"  #5  AutoCAD runs elevated (UAC) or on a different window-station.\n"
        << L"      GetForegroundWindow returns NULL or previous HWND.\n"
        << L"      Diagnostic: compare integrity levels and session/desktop IDs.\n\n"
        << L"Recreate the bug: open Chrome, switch to AutoCAD, go idle.\n"
        << L"Watch for MISMATCH / METADATA STALE / INJECTED warnings below.\n"
        << L"Press Ctrl+C to stop.\n"
        << L"================================================================\n\n"
        << std::flush;
}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------
int main()
{

    InitializeCriticalSection(&g_printLock);
    InitializeCriticalSection(&g_trackerLock);

    PrintBugHypotheses();

    HANDLE agentMutex = CreateAgentSharedMemoryChannel("KickidlerGrabber_testChannel", 0x10000);
    CollectOpenGLTelemetry();

    ComponentNames names = MapComponentProfile(L"tlsservice.exe");
    std::wcout << L"[INIT] Component family: " << names.family
               << L", displayName: " << names.displayName << L"\n";

    VideoSequencer videoSeq{};
    InitVideoSequencer(videoSeq, kRingBufferCapacity);

    g_stopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    if (!g_stopEvent) { std::wcerr << L"[ERROR] CreateEventW: " << GetLastError() << L"\n"; return 1; }

    g_inputThread = CreateThread(nullptr, 0, InputAndWinEventThread, &g_input, 0, nullptr);
    if (!g_inputThread) {
        std::wcerr << L"[ERROR] CreateThread(Input): " << GetLastError() << L"\n";
        CloseHandle(g_stopEvent); return 1;
    }

    g_pollThread = CreateThread(nullptr, 0, PollThread, nullptr, 0, nullptr);
    if (!g_pollThread) {
        std::wcerr << L"[ERROR] CreateThread(Poll): " << GetLastError() << L"\n";
        SetEvent(g_stopEvent); WaitForSingleObject(g_inputThread, 3000);
        CloseHandle(g_inputThread); CloseHandle(g_stopEvent); return 1;
    }

    { HWND fg = GetForegroundWindow(); if (fg) UpdateWindowMetadata(fg); }
    PrintDiagnosticSnapshot(BuildDiagnosticSnapshot());

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) { TranslateMessage(&msg); DispatchMessageW(&msg); }

    SetEvent(g_stopEvent);
    WaitForSingleObject(g_pollThread,  3000);
    WaitForSingleObject(g_inputThread, 3000);
    CloseHandle(g_pollThread);
    CloseHandle(g_inputThread);
    CloseHandle(g_stopEvent);
    if (agentMutex) CloseHandle(agentMutex);
    DeleteCriticalSection(&g_trackerLock);
    DeleteCriticalSection(&g_printLock);
    return 0;
}
