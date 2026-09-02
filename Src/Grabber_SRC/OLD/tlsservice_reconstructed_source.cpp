// Reconstructed source-style pseudocode for tlsservice.exe
// Basis: tlsservice_static_analysis_ghidra.txt / Ghidra MCP static analysis.
// This is NOT original source and is NOT expected to compile.
// It focuses on the observed focus/input/window metadata/capture behavior.

#include <windows.h>
#include <string>
#include <map>
#include <vector>

struct WindowMetadata {
    HWND hwnd;
    DWORD pid;
    std::wstring title;
    std::wstring className;
    std::wstring processPathLower;
    RECT rect;
};

struct InputTracker {
    HHOOK keyboardHook;     // approx object +0x78
    HHOOK mouseHook;        // approx object +0x80
    HWINEVENTHOOK winEventHook;
    HWND lastHwnd;          // observed object +0x90 in FUN_140040dd0
    BYTE keyState[256];
};

struct WindowTracker {
    std::map<HWND, WindowMetadata> windows; // observed QMap at object +0x68
};

struct CaptureContext {
    int printWindowFlags; // observed context +0x8c
    std::vector<unsigned char> pixels;
    int width;
    int height;
};

static InputTracker g_input{};
static WindowTracker g_windowTracker{};

// FUN_14003e8e0
bool install_low_level_input_hooks(InputTracker* tracker) {
    HMODULE module = GetModuleHandleA(nullptr);

    tracker->keyboardHook = SetWindowsHookExA(
        WH_KEYBOARD_LL,
        low_level_keyboard_mouse_callback,
        module,
        0);

    tracker->mouseHook = SetWindowsHookExA(
        WH_MOUSE_LL,
        low_level_keyboard_mouse_callback,
        module,
        0);

    return tracker->keyboardHook && tracker->mouseHook;
}

// FUN_14003ee60
DWORD input_and_winevent_thread(void* arg) {
    InputTracker* tracker = static_cast<InputTracker*>(arg);

    install_low_level_input_hooks(tracker);

    HMODULE module = GetModuleHandleA(nullptr);
    tracker->winEventHook = SetWinEventHook(
        0x20,       // observed eventMin
        0x8005,     // observed eventMax
        module,
        win_event_callback,
        0,
        0,
        WINEVENT_OUTOFCONTEXT);

    MSG msg{};
    while (GetMessageA(&msg, nullptr, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    if (tracker->keyboardHook) {
        UnhookWindowsHookEx(tracker->keyboardHook);
    }
    if (tracker->mouseHook) {
        UnhookWindowsHookEx(tracker->mouseHook);
    }
    if (tracker->winEventHook) {
        UnhookWinEvent(tracker->winEventHook);
    }

    return 0;
}

// FUN_140040dd0
void CALLBACK win_event_callback(HWINEVENTHOOK hook,
                                 DWORD event,
                                 HWND hwnd,
                                 LONG objectId,
                                 LONG childId,
                                 DWORD eventThread,
                                 DWORD eventTime) {
    (void)hook;
    (void)objectId;
    (void)childId;
    (void)eventThread;
    (void)eventTime;

    if (event == 0x8005) {
        GUITHREADINFO info{};
        info.cbSize = sizeof(info);
        if (GetGUIThreadInfo(0, &info) && info.hwndActive) {
            g_input.lastHwnd = info.hwndActive;
        }
    } else if (event == 0x20) {
        g_input.lastHwnd = hwnd;
    }

    if (g_input.lastHwnd) {
        update_window_metadata(g_input.lastHwnd);
        notify_active_window_changed(g_input.lastHwnd);
    }
}

// FUN_1400405d0
LRESULT CALLBACK low_level_keyboard_mouse_callback(int code, WPARAM wParam, LPARAM lParam) {
    if (code < 0) {
        return CallNextHookEx(nullptr, code, wParam, lParam);
    }

    HWND hwnd = g_input.lastHwnd;
    if (!hwnd) {
        hwnd = GetForegroundWindow();
    }

    if (is_keyboard_message(wParam)) {
        DWORD pid = 0;
        DWORD threadId = GetWindowThreadProcessId(hwnd, &pid);
        HKL layout = GetKeyboardLayout(threadId);

        refresh_key_state(g_input.keyState);

        WCHAR chars[8]{};
        translate_key_with_ToUnicodeEx(lParam, g_input.keyState, layout, chars);

        send_keyboard_activity(hwnd, pid, chars);
    } else if (is_mouse_message(wParam)) {
        POINT pt = extract_mouse_point(lParam);
        send_mouse_activity(hwnd, pt);
    }

    return CallNextHookEx(nullptr, code, wParam, lParam);
}

// FUN_140047d80
HWND query_foreground_window_on_input_desktop() {
    HDESK oldDesktop = GetThreadDesktop(GetCurrentThreadId());
    HDESK inputDesktop = OpenInputDesktop(0, FALSE, GENERIC_ALL);
    if (!inputDesktop) {
        return nullptr;
    }

    HWND hwnd = nullptr;
    if (SetThreadDesktop(inputDesktop)) {
        hwnd = GetForegroundWindow();
        SetThreadDesktop(oldDesktop);
    }

    CloseDesktop(inputDesktop);
    return hwnd;
}

// FUN_1400478b0
void enumerate_desktop_windows(const char* desktopName) {
    HDESK desktop = OpenDesktopA(desktopName, 0, FALSE, GENERIC_READ | GENERIC_WRITE);
    if (!desktop) {
        return;
    }

    EnumDesktopWindows(desktop, enum_desktop_window_callback, reinterpret_cast<LPARAM>(&g_windowTracker));
    CloseDesktop(desktop);
}

// FUN_1400471d0
bool collect_window_metadata(HWND hwnd, WindowMetadata& out) {
    if (!IsWindowVisible(hwnd)) {
        return false;
    }

    // Original filters owned/tool windows using GetWindow/GetWindowLongPtrW/GetWindowInfo.
    if (GetWindow(hwnd, GW_OWNER) != nullptr) {
        return false;
    }

    WINDOWINFO wi{};
    wi.cbSize = sizeof(wi);
    GetWindowInfo(hwnd, &wi);

    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);

    wchar_t title[0x1000]{};
    GetWindowTextW(hwnd, title, 0x1000);

    wchar_t className[512]{};
    GetClassNameW(hwnd, className, 512);

    wchar_t processPath[0x1000]{};
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (process) {
        GetProcessImageFileNameW(process, processPath, 0x1000);
        CloseHandle(process);
    }

    RECT rect{};
    GetWindowRect(hwnd, &rect);

    out.hwnd = hwnd;
    out.pid = pid;
    out.title = title;
    out.className = className;
    out.processPathLower = to_lower(processPath);
    out.rect = rect;
    return true;
}

// FUN_140046190
void store_window_metadata(WindowTracker* tracker, const WindowMetadata& meta) {
    // Original stores fields in a QMap keyed by HWND:
    // +0x20 HWND
    // +0x28 title
    // +0x30 PID string
    // +0x38 process/path lowercase
    // +0x40 extra string
    // +0x48..+0x54 rect/dimensions
    tracker->windows[meta.hwnd] = meta;
}

void update_window_metadata(HWND hwnd) {
    WindowMetadata meta{};
    if (collect_window_metadata(hwnd, meta)) {
        store_window_metadata(&g_windowTracker, meta);
        send_window_metadata_to_host(meta);
    }
}

// FUN_1400515a0
bool capture_window_wrapper(CaptureContext* ctx, HWND hwnd) {
    RECT rect{};
    if (!GetWindowRect(hwnd, &rect)) {
        return false;
    }

    HDC windowDc = GetWindowDC(hwnd);
    if (!windowDc) {
        return false;
    }

    // Observed behavior: if foreground window equals target, pass hwnd=0
    // to the low-level capturer, causing BitBlt path instead of PrintWindow.
    HWND printWindowTarget = (GetForegroundWindow() == hwnd) ? nullptr : hwnd;

    bool ok = capture_gdi_to_pixels(ctx, windowDc, printWindowTarget, rect);
    ReleaseDC(hwnd, windowDc);
    return ok;
}

// FUN_1400501d0
bool capture_gdi_to_pixels(CaptureContext* ctx, HDC sourceDc, HWND hwndOrNull, RECT rect) {
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    if (width <= 0 || height <= 0) {
        return false;
    }

    HBITMAP bitmap = CreateCompatibleBitmap(sourceDc, width, height);
    HDC memoryDc = CreateCompatibleDC(sourceDc);
    if (!bitmap || !memoryDc) {
        if (bitmap) DeleteObject(bitmap);
        if (memoryDc) DeleteDC(memoryDc);
        return false;
    }

    HGDIOBJ old = SelectObject(memoryDc, bitmap);

    BOOL copied = FALSE;
    if (hwndOrNull == nullptr) {
        copied = BitBlt(memoryDc, 0, 0, width, height,
                        sourceDc, rect.left, rect.top, SRCCOPY);
    } else {
        copied = PrintWindow(hwndOrNull, memoryDc, ctx->printWindowFlags);
    }

    if (copied) {
        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        ctx->pixels.resize(width * height * 4);
        ctx->width = width;
        ctx->height = height;

        GetDIBits(memoryDc, bitmap, 0, height, ctx->pixels.data(), &bmi, DIB_RGB_COLORS);
        package_or_convert_captured_pixels(ctx);
    }

    SelectObject(memoryDc, old);
    DeleteDC(memoryDc);
    DeleteObject(bitmap);
    return !!copied;
}

// FUN_14016ace0
void create_agent_shared_memory_channel(const char* key, size_t size) {
    HANDLE mutex = CreateMutexA(nullptr, FALSE, key);
    if (!mutex) {
        return;
    }

    WaitForSingleObject(mutex, INFINITE);

    // Original uses QSharedMemory::setNativeKey/create/data.
    // It zeroes the region and writes a global counter at offset 0
    // and QCoreApplication::applicationPid() at offset 8.

    ReleaseMutex(mutex);
    CloseHandle(mutex);
}

// FUN_1400c37f0 / FUN_1400c38d0 / FUN_1400c3960 / FUN_1400c3af0
bool named_pipe_client_handshake(const wchar_t* pipeName) {
    HANDLE pipe = CreateFileW(pipeName, 0x103, 0, nullptr, OPEN_EXISTING, 0x110000, nullptr);
    if (pipe == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_PIPE_BUSY) {
            WaitNamedPipeW(pipeName, 2000);
        }
        return false;
    }

    DWORD mode = 2;
    SetNamedPipeHandleState(pipe, &mode, nullptr, nullptr);

    unsigned char request[0x50]{};
    unsigned char response[0x50]{};
    DWORD transferred = 0;

    *reinterpret_cast<int*>(request) = 1;
    *reinterpret_cast<DWORD*>(request + 4) = GetCurrentProcessId();

    BOOL ok = TransactNamedPipe(pipe, request, sizeof(request),
                                response, sizeof(response),
                                &transferred, nullptr);
    if (ok) {
        unsigned char ack[0x50]{};
        *reinterpret_cast<int*>(ack) = 3;
        WriteFile(pipe, ack, sizeof(ack), &transferred, nullptr);
    }

    CloseHandle(pipe);
    return !!ok;
}

// FUN_14004a6b0 / FUN_140085410
void prepare_user_session_and_desktop_access() {
    // Observed helpers use:
    // WTSQuerySessionInformationA, WTSQueryUserToken, GetTokenInformation,
    // ConvertSidToStringSidW, registry HKU/SID,
    // OpenWindowStationA("WinSta0"), SetProcessWindowStation,
    // OpenDesktopA("Default"), SetThreadDesktop.
}

int main_tlsservice() {
    prepare_user_session_and_desktop_access();
    create_agent_shared_memory_channel("grabber-agent-channel", 0x10000);

    CreateThread(nullptr, 0, input_and_winevent_thread, &g_input, 0, nullptr);

    while (service_is_running()) {
        HWND hwnd = query_foreground_window_on_input_desktop();
        if (hwnd) {
            update_window_metadata(hwnd);
        }

        // Capture is separate from active-application metadata.
        for (auto& kv : g_windowTracker.windows) {
            CaptureContext ctx{};
            capture_window_wrapper(&ctx, kv.first);
        }

        process_ipc_and_transport();
    }

    return 0;
}

// Stub helpers used only to make the reconstruction readable.
LRESULT CALLBACK low_level_keyboard_mouse_callback(int, WPARAM, LPARAM);
void CALLBACK win_event_callback(HWINEVENTHOOK, DWORD, HWND, LONG, LONG, DWORD, DWORD);
BOOL CALLBACK enum_desktop_window_callback(HWND hwnd, LPARAM) { update_window_metadata(hwnd); return TRUE; }
bool is_keyboard_message(WPARAM) { return true; }
bool is_mouse_message(WPARAM) { return false; }
void refresh_key_state(BYTE state[256]) { for (int i = 0; i < 256; ++i) state[i] = (GetKeyState(i) & 0x80) ? 0x80 : 0; }
void translate_key_with_ToUnicodeEx(LPARAM, BYTE*, HKL, WCHAR*) {}
POINT extract_mouse_point(LPARAM) { return {}; }
void send_keyboard_activity(HWND, DWORD, const WCHAR*) {}
void send_mouse_activity(HWND, POINT) {}
std::wstring to_lower(const std::wstring& s) { return s; }
void send_window_metadata_to_host(const WindowMetadata&) {}
void notify_active_window_changed(HWND) {}
void package_or_convert_captured_pixels(CaptureContext*) {}
bool service_is_running() { return false; }
void process_ipc_and_transport() {}
