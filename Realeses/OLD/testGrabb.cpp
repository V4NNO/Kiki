#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr UINT kPollIntervalMs = 500;
constexpr DWORD kIdleThresholdMs = 60 * 1000;
constexpr DWORD kAwayThresholdMs = 5 * 60 * 1000;
constexpr DWORD kHeartbeatMs = 5000;
constexpr ULONG_PTR kMouseInjectedFlag = 0x00000001;

HWINEVENTHOOK g_foregroundHook = nullptr;
HHOOK g_keyboardHook = nullptr;
HHOOK g_mouseHook = nullptr;
HANDLE g_stopEvent = nullptr;
HANDLE g_pollThread = nullptr;
CRITICAL_SECTION g_outputLock;
volatile LONG g_lastPhysicalInputTick = 0;
volatile LONG g_keyboardEvents = 0;
volatile LONG g_mouseEvents = 0;
volatile LONG g_injectedKeyboardEvents = 0;
volatile LONG g_injectedMouseEvents = 0;
volatile PVOID g_lastEventWindow = nullptr;
volatile LONG g_lastEventThreadId = 0;
volatile LONG g_lastEventTime = 0;
volatile LONG g_foregroundEventSequence = 0;

struct InputIdleStatus {
    bool available = false;
    DWORD error = ERROR_SUCCESS;
    DWORD currentTick = 0;
    DWORD lastInputTick = 0;
    DWORD idleMs = 0;
};

struct WindowSnapshot {
    HWND hwnd = nullptr;
    DWORD pid = 0;
    DWORD windowThreadId = 0;
    DWORD guiThreadId = 0;
    std::wstring title;
    std::wstring className;
    std::wstring processPath;
    RECT rect{};
    bool visible = false;
    bool minimized = false;
    bool hasGuiFocus = false;
    HWND guiFocus = nullptr;
    HWND guiActive = nullptr;
    InputIdleStatus windowsIdle;
    DWORD physicalIdleMs = 0;
    bool physicalInputSeen = false;
    ULONGLONG keyboardEvents = 0;
    ULONGLONG mouseEvents = 0;
    ULONGLONG injectedKeyboardEvents = 0;
    ULONGLONG injectedMouseEvents = 0;
    std::wstring desktopName;
    DWORD sessionId = 0;
    LONG_PTR style = 0;
    LONG_PTR exStyle = 0;
};

WindowSnapshot g_lastPrinted;
DWORD g_lastPrintTick = 0;
DWORD g_lastLiveSecond = MAXDWORD;

std::wstring ToWideString(DWORD value)
{
    std::wstringstream stream;
    stream << value;
    return stream.str();
}

std::wstring HexValue(UINT_PTR value)
{
    std::wstringstream stream;
    stream << L"0x" << std::hex << value << std::dec;
    return stream.str();
}

std::wstring BoolText(bool value)
{
    return value ? L"yes" : L"no";
}

std::wstring FormatDuration(DWORD milliseconds)
{
    DWORD totalSeconds = milliseconds / 1000;
    DWORD hours = totalSeconds / 3600;
    DWORD minutes = (totalSeconds % 3600) / 60;
    DWORD seconds = totalSeconds % 60;

    std::wstringstream stream;
    stream
        << (hours < 10 ? L"0" : L"") << hours << L":"
        << (minutes < 10 ? L"0" : L"") << minutes << L":"
        << (seconds < 10 ? L"0" : L"") << seconds;
    return stream.str();
}

std::wstring IdleState(DWORD idleMs)
{
    if (idleMs >= kAwayThresholdMs)
        return L"AWAY";
    if (idleMs >= kIdleThresholdMs)
        return L"IDLE";
    return L"ACTIVE";
}

std::wstring GetWindowTitle(HWND hwnd)
{
    std::vector<wchar_t> buffer(4096, L'\0');
    int copied = GetWindowTextW(
        hwnd,
        buffer.data(),
        static_cast<int>(buffer.size())
    );
    if (copied <= 0)
        return L"";

    return std::wstring(buffer.data(), static_cast<size_t>(copied));
}

std::wstring GetWindowClass(HWND hwnd)
{
    std::vector<wchar_t> buffer(512);
    int copied = GetClassNameW(hwnd, buffer.data(), static_cast<int>(buffer.size()));
    if (copied <= 0)
        return L"";
    return std::wstring(buffer.data(), static_cast<size_t>(copied));
}

std::wstring GetCurrentInputDesktopName()
{
    HDESK desktop = OpenInputDesktop(0, FALSE, DESKTOP_READOBJECTS);
    if (desktop == nullptr)
        return L"<OpenInputDesktop failed: " + ToWideString(GetLastError()) + L">";

    char name[256] = {};
    DWORD needed = 0;
    BOOL ok = GetUserObjectInformationA(desktop, UOI_NAME, name, sizeof(name), &needed);
    DWORD error = ok ? ERROR_SUCCESS : GetLastError();
    CloseDesktop(desktop);

    if (!ok)
        return L"<GetUserObjectInformationA failed: " + ToWideString(error) + L">";

    int wideLen = MultiByteToWideChar(CP_ACP, 0, name, -1, nullptr, 0);
    if (wideLen <= 0)
        return L"";

    std::wstring wide(static_cast<size_t>(wideLen), L'\0');
    MultiByteToWideChar(CP_ACP, 0, name, -1, &wide[0], wideLen);
    if (!wide.empty() && wide.back() == L'\0')
        wide.pop_back();
    return wide;
}

InputIdleStatus GetWindowsIdleStatus()
{
    InputIdleStatus status;
    LASTINPUTINFO info{};
    info.cbSize = sizeof(info);
    SetLastError(ERROR_SUCCESS);

    if (!GetLastInputInfo(&info)) {
        status.error = GetLastError();
        return status;
    }

    status.available = true;
    status.currentTick = GetTickCount();
    status.lastInputTick = info.dwTime;
    status.idleMs = status.currentTick - status.lastInputTick;
    return status;
}

std::wstring GetProcessPath(DWORD processId)
{
    using QueryFullProcessImageNameWFn = BOOL (WINAPI *)(HANDLE, DWORD, LPWSTR, PDWORD);

    HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");
    QueryFullProcessImageNameWFn queryFullProcessImageNameW =
        kernel32 == nullptr
            ? nullptr
            : reinterpret_cast<QueryFullProcessImageNameWFn>(
                GetProcAddress(kernel32, "QueryFullProcessImageNameW")
            );

    if (queryFullProcessImageNameW == nullptr)
        return L"<QueryFullProcessImageNameW unavailable>";

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
    if (process == nullptr)
        return L"<OpenProcess failed: " + ToWideString(GetLastError()) + L">";

    std::vector<wchar_t> buffer(32768);
    DWORD size = static_cast<DWORD>(buffer.size());
    BOOL success = queryFullProcessImageNameW(process, 0, buffer.data(), &size);
    DWORD error = success ? ERROR_SUCCESS : GetLastError();

    CloseHandle(process);

    if (!success)
        return L"<QueryFullProcessImageName failed: " + ToWideString(error) + L">";

    return std::wstring(buffer.data(), size);
}

WindowSnapshot BuildSnapshot(HWND hwnd)
{
    WindowSnapshot snapshot;
    DWORD lastPhysicalInputTick = static_cast<DWORD>(
        InterlockedCompareExchange(&g_lastPhysicalInputTick, 0, 0)
    );

    snapshot.hwnd = hwnd;
    snapshot.windowsIdle = GetWindowsIdleStatus();
    snapshot.physicalInputSeen = lastPhysicalInputTick != 0;
    snapshot.physicalIdleMs = snapshot.physicalInputSeen
        ? GetTickCount() - lastPhysicalInputTick
        : 0;
    snapshot.keyboardEvents = static_cast<DWORD>(
        InterlockedCompareExchange(&g_keyboardEvents, 0, 0)
    );
    snapshot.mouseEvents = static_cast<DWORD>(
        InterlockedCompareExchange(&g_mouseEvents, 0, 0)
    );
    snapshot.injectedKeyboardEvents = static_cast<DWORD>(
        InterlockedCompareExchange(&g_injectedKeyboardEvents, 0, 0)
    );
    snapshot.injectedMouseEvents = static_cast<DWORD>(
        InterlockedCompareExchange(&g_injectedMouseEvents, 0, 0)
    );
    snapshot.desktopName = GetCurrentInputDesktopName();
    ProcessIdToSessionId(GetCurrentProcessId(), &snapshot.sessionId);

    if (hwnd == nullptr)
        return snapshot;

    snapshot.windowThreadId = GetWindowThreadProcessId(hwnd, &snapshot.pid);
    snapshot.title = GetWindowTitle(hwnd);
    snapshot.className = GetWindowClass(hwnd);
    snapshot.processPath = snapshot.pid == 0 ? L"" : GetProcessPath(snapshot.pid);
    snapshot.visible = IsWindowVisible(hwnd) != FALSE;
    snapshot.minimized = IsIconic(hwnd) != FALSE;
    snapshot.style = GetWindowLongPtrW(hwnd, GWL_STYLE);
    snapshot.exStyle = GetWindowLongPtrW(hwnd, GWL_EXSTYLE);
    GetWindowRect(hwnd, &snapshot.rect);

    GUITHREADINFO gui{};
    gui.cbSize = sizeof(gui);
    if (snapshot.windowThreadId != 0 && GetGUIThreadInfo(snapshot.windowThreadId, &gui)) {
        snapshot.guiThreadId = snapshot.windowThreadId;
        snapshot.guiFocus = gui.hwndFocus;
        snapshot.guiActive = gui.hwndActive;
        snapshot.hasGuiFocus = gui.hwndFocus != nullptr || gui.hwndActive != nullptr;
    }

    return snapshot;
}

DWORD EffectiveIdleMilliseconds(const WindowSnapshot& snapshot)
{
    if (snapshot.windowsIdle.available && snapshot.physicalInputSeen) {
        return snapshot.windowsIdle.idleMs < snapshot.physicalIdleMs
            ? snapshot.windowsIdle.idleMs
            : snapshot.physicalIdleMs;
    }

    if (snapshot.windowsIdle.available)
        return snapshot.windowsIdle.idleMs;

    return snapshot.physicalInputSeen ? snapshot.physicalIdleMs : 0;
}

bool SnapshotChanged(const WindowSnapshot& a, const WindowSnapshot& b)
{
    DWORD aIdle = EffectiveIdleMilliseconds(a);
    DWORD bIdle = EffectiveIdleMilliseconds(b);

    return a.hwnd != b.hwnd ||
           a.pid != b.pid ||
           a.windowThreadId != b.windowThreadId ||
           a.title != b.title ||
           a.className != b.className ||
           IdleState(aIdle) != IdleState(bIdle) ||
           a.desktopName != b.desktopName;
}

void PrintSnapshot(const wchar_t *source, const WindowSnapshot& s, DWORD eventThreadId, DWORD eventTime)
{
    EnterCriticalSection(&g_outputLock);

    DWORD now = GetTickCount();
    bool heartbeatDue = now - g_lastPrintTick >= kHeartbeatMs;

    if (!SnapshotChanged(s, g_lastPrinted) && !heartbeatDue) {
        LeaveCriticalSection(&g_outputLock);
        return;
    }

    g_lastPrinted = s;
    g_lastPrintTick = now;

    DWORD effectiveIdleMs = EffectiveIdleMilliseconds(s);
    const wchar_t *idleSource =
        s.windowsIdle.available && s.physicalInputSeen
            ? L"most recent of Windows/hooks"
            : (s.physicalInputSeen ? L"physical hooks" : L"GetLastInputInfo");

    std::wcout
        << L"\n[" << source << L"] foreground snapshot"
        << L"\n  State:         " << IdleState(effectiveIdleMs)
        << L"\n  State source:  " << idleSource
        << L"\n  Effective idle:" << L" " << effectiveIdleMs << L" ms"
        << L"\n  Windows idle:  ";

    if (s.windowsIdle.available) {
        std::wcout
            << s.windowsIdle.idleMs << L" ms"
            << L" (now=" << s.windowsIdle.currentTick
            << L", last=" << s.windowsIdle.lastInputTick << L")";
    } else {
        std::wcout << L"unavailable, error=" << s.windowsIdle.error;
    }

    std::wcout
        << L"\n  Physical idle:"
        << L" "
        << (s.physicalInputSeen
            ? ToWideString(s.physicalIdleMs) + L" ms"
            : L"no physical input observed yet")
        << L"\n  Key events:    " << s.keyboardEvents
        << L"\n  Mouse events:  " << s.mouseEvents
        << L"\n  Injected keys: " << s.injectedKeyboardEvents
        << L"\n  Injected mouse:" << L" " << s.injectedMouseEvents
        << L"\n  HWND:          " << HexValue(reinterpret_cast<UINT_PTR>(s.hwnd))
        << L"\n  PID:           " << s.pid
        << L"\n  Window TID:    " << s.windowThreadId
        << L"\n  Event TID:     " << eventThreadId
        << L"\n  Event time:    " << eventTime
        << L"\n  Session ID:    " << s.sessionId
        << L"\n  Desktop:       " << s.desktopName
        << L"\n  Visible:       " << BoolText(s.visible)
        << L"\n  Minimized:     " << BoolText(s.minimized)
        << L"\n  GUI focus:     " << HexValue(reinterpret_cast<UINT_PTR>(s.guiFocus))
        << L"\n  GUI active:    " << HexValue(reinterpret_cast<UINT_PTR>(s.guiActive))
        << L"\n  GUI available: " << BoolText(s.hasGuiFocus)
        << L"\n  Rect:          "
        << s.rect.left << L"," << s.rect.top << L" - "
        << s.rect.right << L"," << s.rect.bottom
        << L"\n  Style:         " << HexValue(static_cast<UINT_PTR>(s.style))
        << L"\n  ExStyle:       " << HexValue(static_cast<UINT_PTR>(s.exStyle))
        << L"\n  Class:         " << s.className
        << L"\n  Title:         " << s.title
        << L"\n  Path:          " << s.processPath
        << L"\n"
        << std::flush;

    LeaveCriticalSection(&g_outputLock);
}

void PrintLiveStatus(const WindowSnapshot& snapshot)
{
    EnterCriticalSection(&g_outputLock);

    DWORD currentSecond = GetTickCount() / 1000;
    if (currentSecond == g_lastLiveSecond) {
        LeaveCriticalSection(&g_outputLock);
        return;
    }
    g_lastLiveSecond = currentSecond;

    DWORD effectiveIdleMs = EffectiveIdleMilliseconds(snapshot);

    std::wstring displayTitle = snapshot.title.empty()
        ? snapshot.className
        : snapshot.title;
    if (displayTitle.size() > 70)
        displayTitle = displayTitle.substr(0, 67) + L"...";

    std::wcout
        << L"\n[LIVE] " << IdleState(effectiveIdleMs)
        << L" | effective=" << FormatDuration(effectiveIdleMs)
        << L" | windows="
        << (snapshot.windowsIdle.available
            ? FormatDuration(snapshot.windowsIdle.idleMs)
            : L"unavailable")
        << L" | physical="
        << (snapshot.physicalInputSeen
            ? FormatDuration(snapshot.physicalIdleMs)
            : L"not-seen")
        << L" | PID " << snapshot.pid
        << L" | " << displayTitle
        << std::flush;

    LeaveCriticalSection(&g_outputLock);
}

LRESULT CALLBACK KeyboardHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
    (void)wParam;

    if (code == HC_ACTION) {
        InterlockedIncrement(&g_keyboardEvents);

        const KBDLLHOOKSTRUCT *data =
            reinterpret_cast<const KBDLLHOOKSTRUCT *>(lParam);
        if ((data->flags & LLKHF_INJECTED) != 0)
            InterlockedIncrement(&g_injectedKeyboardEvents);
        else
            InterlockedExchange(
                &g_lastPhysicalInputTick,
                static_cast<LONG>(GetTickCount())
            );
    }

    return CallNextHookEx(g_keyboardHook, code, wParam, lParam);
}

LRESULT CALLBACK MouseHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
    (void)wParam;

    if (code == HC_ACTION) {
        InterlockedIncrement(&g_mouseEvents);

        const MSLLHOOKSTRUCT *data =
            reinterpret_cast<const MSLLHOOKSTRUCT *>(lParam);
        if ((data->flags & kMouseInjectedFlag) != 0)
            InterlockedIncrement(&g_injectedMouseEvents);
        else
            InterlockedExchange(
                &g_lastPhysicalInputTick,
                static_cast<LONG>(GetTickCount())
            );
    }

    return CallNextHookEx(g_mouseHook, code, wParam, lParam);
}

void PollForeground(const wchar_t *source, DWORD eventThreadId = 0, DWORD eventTime = 0)
{
    HWND hwnd = GetForegroundWindow();
    WindowSnapshot snapshot = BuildSnapshot(hwnd);
    PrintSnapshot(source, snapshot, eventThreadId, eventTime);
    PrintLiveStatus(snapshot);
}

DWORD WINAPI PollThreadProc(LPVOID parameter)
{
    (void)parameter;
    LONG processedEventSequence = 0;

    while (WaitForSingleObject(g_stopEvent, kPollIntervalMs) == WAIT_TIMEOUT) {
        LONG eventSequence = InterlockedCompareExchange(
            &g_foregroundEventSequence,
            0,
            0
        );

        if (eventSequence != processedEventSequence) {
            processedEventSequence = eventSequence;
            DWORD eventThreadId = static_cast<DWORD>(
                InterlockedCompareExchange(&g_lastEventThreadId, 0, 0)
            );
            DWORD eventTime = static_cast<DWORD>(
                InterlockedCompareExchange(&g_lastEventTime, 0, 0)
            );
            PollForeground(L"POLL_AFTER_WIN_EVENT", eventThreadId, eventTime);
        } else {
            PollForeground(L"POLL_THREAD");
        }
    }

    return 0;
}

void CALLBACK ForegroundEventCallback(
    HWINEVENTHOOK hook,
    DWORD event,
    HWND hwnd,
    LONG idObject,
    LONG idChild,
    DWORD eventThreadId,
    DWORD eventTime)
{
    (void)hook;
    (void)idObject;
    (void)idChild;

    if (event != EVENT_SYSTEM_FOREGROUND || hwnd == nullptr)
        return;

    InterlockedExchangePointer(
        &g_lastEventWindow,
        reinterpret_cast<PVOID>(hwnd)
    );
    InterlockedExchange(
        &g_lastEventThreadId,
        static_cast<LONG>(eventThreadId)
    );
    InterlockedExchange(
        &g_lastEventTime,
        static_cast<LONG>(eventTime)
    );
    InterlockedIncrement(&g_foregroundEventSequence);
}

int main()
{
    InitializeCriticalSection(&g_outputLock);
    HINSTANCE module = GetModuleHandleW(nullptr);

    g_keyboardHook = SetWindowsHookExW(
        WH_KEYBOARD_LL,
        KeyboardHookCallback,
        module,
        0
    );

    g_mouseHook = SetWindowsHookExW(
        WH_MOUSE_LL,
        MouseHookCallback,
        module,
        0
    );

    if (g_keyboardHook == nullptr || g_mouseHook == nullptr) {
        std::wcerr
            << L"SetWindowsHookEx failed. Error: "
            << GetLastError()
            << L"\n";

        if (g_keyboardHook != nullptr)
            UnhookWindowsHookEx(g_keyboardHook);
        if (g_mouseHook != nullptr)
            UnhookWindowsHookEx(g_mouseHook);
        DeleteCriticalSection(&g_outputLock);
        return 1;
    }

    g_foregroundHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND,
        EVENT_SYSTEM_FOREGROUND,
        nullptr,
        ForegroundEventCallback,
        0,
        0,
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS
    );

    if (g_foregroundHook == nullptr) {
        std::wcerr
            << L"SetWinEventHook failed. Error: "
            << GetLastError()
            << L"\n";
        UnhookWindowsHookEx(g_keyboardHook);
        UnhookWindowsHookEx(g_mouseHook);
        DeleteCriticalSection(&g_outputLock);
        return 1;
    }

    g_stopEvent = CreateEventW(
        nullptr,
        TRUE,
        FALSE,
        nullptr
    );
    if (g_stopEvent == nullptr) {
        std::wcerr
            << L"CreateEvent failed. Error: "
            << GetLastError()
            << L"\n";
        UnhookWinEvent(g_foregroundHook);
        UnhookWindowsHookEx(g_keyboardHook);
        UnhookWindowsHookEx(g_mouseHook);
        DeleteCriticalSection(&g_outputLock);
        return 1;
    }

    g_pollThread = CreateThread(
        nullptr,
        0,
        PollThreadProc,
        nullptr,
        0,
        nullptr
    );
    if (g_pollThread == nullptr) {
        std::wcerr
            << L"CreateThread failed. Error: "
            << GetLastError()
            << L"\n";
        CloseHandle(g_stopEvent);
        UnhookWinEvent(g_foregroundHook);
        UnhookWindowsHookEx(g_keyboardHook);
        UnhookWindowsHookEx(g_mouseHook);
        DeleteCriticalSection(&g_outputLock);
        return 1;
    }

    std::wcout
        << L"Monitoring foreground window and user activity...\n"
        << L"Sources: WinEvent, 500 ms polling, keyboard/mouse activity hooks.\n"
        << L"ACTIVE < 60s idle, IDLE >= 60s, AWAY >= 5min.\n"
        << L"No keys, mouse positions, video or audio are recorded.\n"
        << L"Press Ctrl+C to stop.\n";

    PollForeground(L"START");

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    SetEvent(g_stopEvent);
    WaitForSingleObject(g_pollThread, 2000);
    CloseHandle(g_pollThread);
    CloseHandle(g_stopEvent);
    UnhookWinEvent(g_foregroundHook);
    UnhookWindowsHookEx(g_keyboardHook);
    UnhookWindowsHookEx(g_mouseHook);
    DeleteCriticalSection(&g_outputLock);
    return 0;
}
