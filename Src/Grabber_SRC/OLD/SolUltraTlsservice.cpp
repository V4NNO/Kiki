// SolUltraTlsservice.cpp
//
// Visible, current-session activity monitor reconstructed from the window and
// idle-tracking behavior observed in the tlshost.exe Ghidra project.
//
// Scope intentionally excludes screen/video/audio capture, keyboard content,
// mouse positions, remote transport, persistence, services, and cross-session
// process creation. It records only foreground-window metadata and idle state.

#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <shellapi.h>

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>

namespace {

const DWORD kDefaultPollIntervalMs = 500;
const DWORD kMinimumPollIntervalMs = 100;
const DWORD kMaximumPollIntervalMs = 60000;
const DWORD kIdleThresholdMs = 60 * 1000;
const DWORD kAwayThresholdMs = 5 * 60 * 1000;
const DWORD kHeartbeatMs = 60 * 1000;

struct Options {
    std::wstring outputPath = L"SolUltraTlsservice_activity.csv";
    DWORD pollIntervalMs = kDefaultPollIntervalMs;
};

enum class ActivityState {
    Active,
    Idle,
    Away
};

struct IdleInfo {
    bool available = false;
    DWORD milliseconds = 0;
};

struct ActivitySnapshot {
    HWND hwnd = nullptr;
    DWORD processId = 0;
    DWORD windowThreadId = 0;
    DWORD sessionId = 0;
    std::wstring title;
    std::wstring className;
    std::wstring processPath;
    std::wstring desktopName;
    IdleInfo idle;
    ActivityState state = ActivityState::Active;
};

HWINEVENTHOOK gForegroundHook = nullptr;
HANDLE gStopEvent = nullptr;
HANDLE gPollThread = nullptr;
DWORD gMainThreadId = 0;
volatile LONG gForegroundEventSequence = 0;
volatile LONG gLastForegroundEventThreadId = 0;
volatile LONG gLastForegroundEventTime = 0;

std::string WideToUtf8(const std::wstring& value)
{
    if (value.empty()) {
        return std::string();
    }

    int bytes = WideCharToMultiByte(
        CP_UTF8,
        0,
        value.data(),
        static_cast<int>(value.size()),
        nullptr,
        0,
        nullptr,
        nullptr);
    if (bytes <= 0) {
        return std::string();
    }

    std::string result(static_cast<size_t>(bytes), '\0');
    WideCharToMultiByte(
        CP_UTF8,
        0,
        value.data(),
        static_cast<int>(value.size()),
        &result[0],
        bytes,
        nullptr,
        nullptr);
    return result;
}

std::wstring UnsignedToWide(DWORD value)
{
    std::wstringstream stream;
    stream << value;
    return stream.str();
}

std::string CsvField(const std::string& value)
{
    std::string escaped;
    escaped.reserve(value.size() + 2);
    escaped.push_back('"');
    for (size_t index = 0; index < value.size(); ++index) {
        if (value[index] == '"') {
            escaped.push_back('"');
        }
        escaped.push_back(value[index]);
    }
    escaped.push_back('"');
    return escaped;
}

std::string CsvField(const std::wstring& value)
{
    return CsvField(WideToUtf8(value));
}

std::string TimestampLocal()
{
    SYSTEMTIME local{};
    GetLocalTime(&local);

    std::ostringstream text;
    text << std::setfill('0')
         << std::setw(4) << local.wYear << '-'
         << std::setw(2) << local.wMonth << '-'
         << std::setw(2) << local.wDay << 'T'
         << std::setw(2) << local.wHour << ':'
         << std::setw(2) << local.wMinute << ':'
         << std::setw(2) << local.wSecond << '.'
         << std::setw(3) << local.wMilliseconds;
    return text.str();
}

const char* ActivityStateText(ActivityState state)
{
    switch (state) {
    case ActivityState::Active:
        return "ACTIVE";
    case ActivityState::Idle:
        return "IDLE";
    case ActivityState::Away:
        return "AWAY";
    }
    return "UNKNOWN";
}

ActivityState ClassifyIdle(DWORD idleMs)
{
    if (idleMs >= kAwayThresholdMs) {
        return ActivityState::Away;
    }
    if (idleMs >= kIdleThresholdMs) {
        return ActivityState::Idle;
    }
    return ActivityState::Active;
}

std::wstring ActivityStateTextWide(ActivityState state)
{
    switch (state) {
    case ActivityState::Active:
        return L"ACTIVE";
    case ActivityState::Idle:
        return L"IDLE";
    case ActivityState::Away:
        return L"AWAY";
    }
    return L"UNKNOWN";
}

std::wstring GetWindowText(HWND hwnd)
{
    if (hwnd == nullptr) {
        return std::wstring();
    }

    int length = GetWindowTextLengthW(hwnd);
    if (length <= 0) {
        return std::wstring();
    }

    std::vector<wchar_t> buffer(static_cast<size_t>(length) + 1, L'\0');
    int copied = GetWindowTextW(hwnd, buffer.data(), static_cast<int>(buffer.size()));
    return copied > 0 ? std::wstring(buffer.data(), static_cast<size_t>(copied))
                      : std::wstring();
}

std::wstring GetWindowClass(HWND hwnd)
{
    std::vector<wchar_t> buffer(512, L'\0');
    int copied = GetClassNameW(hwnd, buffer.data(), static_cast<int>(buffer.size()));
    return copied > 0 ? std::wstring(buffer.data(), static_cast<size_t>(copied))
                      : std::wstring();
}

std::wstring GetProcessPath(DWORD processId)
{
    if (processId == 0) {
        return std::wstring();
    }

    typedef BOOL (WINAPI *QueryFullProcessImageNameWFn)(HANDLE, DWORD, LPWSTR, PDWORD);

    HMODULE kernel32 = GetModuleHandleW(L"kernel32.dll");
    QueryFullProcessImageNameWFn queryFullProcessImageNameW = kernel32 == nullptr
        ? nullptr
        : reinterpret_cast<QueryFullProcessImageNameWFn>(
            GetProcAddress(kernel32, "QueryFullProcessImageNameW"));
    if (queryFullProcessImageNameW == nullptr) {
        return std::wstring();
    }

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
    if (process == nullptr) {
        return std::wstring();
    }

    std::vector<wchar_t> buffer(32768, L'\0');
    DWORD length = static_cast<DWORD>(buffer.size());
    BOOL ok = queryFullProcessImageNameW(process, 0, buffer.data(), &length);
    CloseHandle(process);

    return ok ? std::wstring(buffer.data(), static_cast<size_t>(length))
              : std::wstring();
}

std::wstring GetInputDesktopName()
{
    HDESK desktop = OpenInputDesktop(0, FALSE, DESKTOP_READOBJECTS);
    if (desktop == nullptr) {
        return L"<unavailable>";
    }

    DWORD bytesNeeded = 0;
    GetUserObjectInformationW(desktop, UOI_NAME, nullptr, 0, &bytesNeeded);
    if (bytesNeeded == 0) {
        CloseDesktop(desktop);
        return L"<unavailable>";
    }

    std::vector<wchar_t> buffer(
        static_cast<size_t>(bytesNeeded / sizeof(wchar_t)) + 1,
        L'\0');
    BOOL ok = GetUserObjectInformationW(
        desktop,
        UOI_NAME,
        buffer.data(),
        bytesNeeded,
        &bytesNeeded);
    CloseDesktop(desktop);

    return ok ? std::wstring(buffer.data()) : L"<unavailable>";
}

IdleInfo GetIdleInfo()
{
    LASTINPUTINFO input{};
    input.cbSize = sizeof(input);

    if (!GetLastInputInfo(&input)) {
        return IdleInfo{};
    }

    IdleInfo result;
    result.available = true;
    // Unsigned subtraction intentionally handles the 32-bit tick rollover.
    result.milliseconds = GetTickCount() - input.dwTime;
    return result;
}

ActivitySnapshot BuildSnapshot()
{
    ActivitySnapshot result;
    result.hwnd = GetForegroundWindow();
    result.idle = GetIdleInfo();
    result.state = ClassifyIdle(result.idle.milliseconds);
    result.desktopName = GetInputDesktopName();

    if (result.hwnd == nullptr) {
        ProcessIdToSessionId(GetCurrentProcessId(), &result.sessionId);
        return result;
    }

    result.windowThreadId = GetWindowThreadProcessId(result.hwnd, &result.processId);
    ProcessIdToSessionId(result.processId, &result.sessionId);
    result.title = GetWindowText(result.hwnd);
    result.className = GetWindowClass(result.hwnd);
    result.processPath = GetProcessPath(result.processId);
    return result;
}

bool SameForegroundIdentity(const ActivitySnapshot& left, const ActivitySnapshot& right)
{
    return left.hwnd == right.hwnd &&
           left.processId == right.processId &&
           left.windowThreadId == right.windowThreadId;
}

bool SameMetadata(const ActivitySnapshot& left, const ActivitySnapshot& right)
{
    return left.title == right.title &&
           left.className == right.className &&
           left.processPath == right.processPath &&
           left.desktopName == right.desktopName &&
           left.sessionId == right.sessionId;
}

class ActivityLog {
public:
    ActivityLog() : file_(nullptr) {}

    ~ActivityLog()
    {
        Close();
    }

    bool Open(const std::wstring& path)
    {
        file_ = _wfopen(path.c_str(), L"wb");
        if (file_ == nullptr) {
            return false;
        }

        const unsigned char bom[] = {0xef, 0xbb, 0xbf};
        fwrite(bom, 1, sizeof(bom), file_);
        WriteLine(
            "timestamp,event,source,state,idle_ms,duration_ms,pid,window_tid,"
            "session_id,desktop,class,title,process_path");
        return true;
    }

    void Close()
    {
        if (file_ != nullptr) {
            fclose(file_);
            file_ = nullptr;
        }
    }

    void Record(
        const char* event,
        const char* source,
        const ActivitySnapshot& snapshot,
        DWORD durationMs)
    {
        if (file_ == nullptr) {
            return;
        }

        std::ostringstream line;
        line << TimestampLocal() << ','
             << CsvField(event) << ','
             << CsvField(source) << ','
             << CsvField(ActivityStateText(snapshot.state)) << ','
             << (snapshot.idle.available ? snapshot.idle.milliseconds : 0) << ','
             << durationMs << ','
             << snapshot.processId << ','
             << snapshot.windowThreadId << ','
             << snapshot.sessionId << ','
             << CsvField(snapshot.desktopName) << ','
             << CsvField(snapshot.className) << ','
             << CsvField(snapshot.title) << ','
             << CsvField(snapshot.processPath);
        WriteLine(line.str());
    }

private:
    void WriteLine(const std::string& line)
    {
        fwrite(line.data(), 1, line.size(), file_);
        fwrite("\r\n", 1, 2, file_);
        fflush(file_);
    }

    FILE* file_;
};

class ActivityMonitor {
public:
    ActivityMonitor(ActivityLog& log, DWORD pollIntervalMs)
        : log_(log),
          pollIntervalMs_(pollIntervalMs),
          havePrevious_(false),
          foregroundSinceTick_(0),
          lastHeartbeatTick_(0) {}

    void Poll(const char* source)
    {
        ActivitySnapshot current = BuildSnapshot();
        DWORD now = GetTickCount();

        if (!havePrevious_) {
            previous_ = current;
            havePrevious_ = true;
            foregroundSinceTick_ = now;
            lastHeartbeatTick_ = now;
            RecordAndDisplay("MONITOR_START", source, current, 0);
            return;
        }

        if (!SameForegroundIdentity(previous_, current)) {
            RecordAndDisplay(
                "FOREGROUND_END",
                source,
                previous_,
                now - foregroundSinceTick_);
            previous_ = current;
            foregroundSinceTick_ = now;
            lastHeartbeatTick_ = now;
            RecordAndDisplay("FOREGROUND_START", source, current, 0);
            return;
        }

        if (!SameMetadata(previous_, current)) {
            previous_ = current;
            RecordAndDisplay(
                "WINDOW_UPDATED",
                source,
                current,
                now - foregroundSinceTick_);
            return;
        }

        if (previous_.state != current.state) {
            previous_ = current;
            RecordAndDisplay(
                "STATE_CHANGE",
                source,
                current,
                now - foregroundSinceTick_);
            return;
        }

        previous_ = current;
        if (now - lastHeartbeatTick_ >= kHeartbeatMs) {
            lastHeartbeatTick_ = now;
            RecordAndDisplay(
                "HEARTBEAT",
                source,
                current,
                now - foregroundSinceTick_);
        }
    }

    void Run()
    {
        LONG processedSequence = 0;
        while (WaitForSingleObject(gStopEvent, pollIntervalMs_) == WAIT_TIMEOUT) {
            LONG sequence = InterlockedCompareExchange(&gForegroundEventSequence, 0, 0);
            Poll(sequence != processedSequence ? "WIN_EVENT" : "POLL");
            processedSequence = sequence;
        }
    }

    void Finish()
    {
        if (!havePrevious_) {
            return;
        }
        RecordAndDisplay(
            "MONITOR_STOP",
            "SHUTDOWN",
            previous_,
            GetTickCount() - foregroundSinceTick_);
    }

private:
    void RecordAndDisplay(
        const char* event,
        const char* source,
        const ActivitySnapshot& snapshot,
        DWORD durationMs)
    {
        log_.Record(event, source, snapshot, durationMs);

        std::wstring displayTitle = snapshot.title.empty()
            ? snapshot.className
            : snapshot.title;
        if (displayTitle.size() > 90) {
            displayTitle = displayTitle.substr(0, 87) + L"...";
        }

        std::wcout
            << L'[' << ActivityStateTextWide(snapshot.state) << L"] "
            << event << L" | PID " << snapshot.processId
            << L" | idle "
            << (snapshot.idle.available
                    ? UnsignedToWide(snapshot.idle.milliseconds / 1000) + L"s"
                    : L"unavailable")
            << L" | " << displayTitle << L'\n';
    }

    ActivityLog& log_;
    DWORD pollIntervalMs_;
    bool havePrevious_;
    ActivitySnapshot previous_;
    DWORD foregroundSinceTick_;
    DWORD lastHeartbeatTick_;
};

ActivityMonitor* gMonitor = nullptr;

DWORD WINAPI PollThreadProc(LPVOID)
{
    if (gMonitor != nullptr) {
        gMonitor->Run();
    }
    return 0;
}

void CALLBACK ForegroundEventCallback(
    HWINEVENTHOOK,
    DWORD event,
    HWND hwnd,
    LONG,
    LONG,
    DWORD eventThreadId,
    DWORD eventTime)
{
    if (event != EVENT_SYSTEM_FOREGROUND || hwnd == nullptr) {
        return;
    }

    InterlockedExchange(&gLastForegroundEventThreadId, static_cast<LONG>(eventThreadId));
    InterlockedExchange(&gLastForegroundEventTime, static_cast<LONG>(eventTime));
    InterlockedIncrement(&gForegroundEventSequence);
}

BOOL WINAPI ConsoleControlHandler(DWORD controlType)
{
    switch (controlType) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        if (gStopEvent != nullptr) {
            SetEvent(gStopEvent);
        }
        if (gMainThreadId != 0) {
            PostThreadMessageW(gMainThreadId, WM_QUIT, 0, 0);
        }
        return TRUE;
    default:
        return FALSE;
    }
}

void PrintUsage()
{
    std::wcout
        << L"Usage: SolUltraTlsservice.exe [--output <csv-file>] [--interval <ms>]\n"
        << L"\n"
        << L"Records activity metadata for the current interactive session.\n"
        << L"No screen/video/audio capture, key content, mouse positions, network,\n"
        << L"service installation, persistence, or cross-session access is used.\n";
}

bool ParseOptions(int argc, wchar_t* argv[], Options& options)
{
    for (int index = 1; index < argc; ++index) {
        std::wstring argument = argv[index];
        if (argument == L"--help" || argument == L"-h") {
            PrintUsage();
            return false;
        }
        if (argument == L"--output" && index + 1 < argc) {
            options.outputPath = argv[++index];
            continue;
        }
        if (argument == L"--interval" && index + 1 < argc) {
            wchar_t* end = nullptr;
            unsigned long value = std::wcstoul(argv[++index], &end, 10);
            if (end == nullptr || *end != L'\0' ||
                value < kMinimumPollIntervalMs || value > kMaximumPollIntervalMs) {
                std::wcerr << L"--interval must be between "
                           << kMinimumPollIntervalMs << L" and "
                           << kMaximumPollIntervalMs << L" milliseconds.\n";
                return false;
            }
            options.pollIntervalMs = static_cast<DWORD>(value);
            continue;
        }

        std::wcerr << L"Unknown or incomplete option: " << argument << L"\n";
        return false;
    }
    return true;
}

void Cleanup()
{
    if (gStopEvent != nullptr) {
        SetEvent(gStopEvent);
    }
    if (gPollThread != nullptr) {
        WaitForSingleObject(gPollThread, 3000);
        CloseHandle(gPollThread);
        gPollThread = nullptr;
    }
    if (gForegroundHook != nullptr) {
        UnhookWinEvent(gForegroundHook);
        gForegroundHook = nullptr;
    }
    if (gStopEvent != nullptr) {
        CloseHandle(gStopEvent);
        gStopEvent = nullptr;
    }
    SetConsoleCtrlHandler(ConsoleControlHandler, FALSE);
}

} // namespace

int RunActivityMonitor(int argc, wchar_t* argv[])
{
    Options options;
    if (!ParseOptions(argc, argv, options)) {
        return argc > 1 &&
               (std::wstring(argv[1]) == L"--help" || std::wstring(argv[1]) == L"-h")
            ? 0
            : 2;
    }

    ActivityLog log;
    if (!log.Open(options.outputPath)) {
        std::wcerr << L"Cannot create activity log '" << options.outputPath
                   << L"'. Win32 error: " << GetLastError() << L"\n";
        return 1;
    }

    gMainThreadId = GetCurrentThreadId();
    // Create this thread's message queue before the console-control handler
    // can request WM_QUIT with PostThreadMessageW.
    MSG queueProbe{};
    PeekMessageW(&queueProbe, nullptr, 0, 0, PM_NOREMOVE);

    gStopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    if (gStopEvent == nullptr) {
        std::wcerr << L"CreateEventW failed. Error: " << GetLastError() << L"\n";
        return 1;
    }
    SetConsoleCtrlHandler(ConsoleControlHandler, TRUE);

    gForegroundHook = SetWinEventHook(
        EVENT_SYSTEM_FOREGROUND,
        EVENT_SYSTEM_FOREGROUND,
        nullptr,
        ForegroundEventCallback,
        0,
        0,
        WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
    if (gForegroundHook == nullptr) {
        std::wcerr << L"SetWinEventHook failed. Error: " << GetLastError() << L"\n";
        Cleanup();
        return 1;
    }

    ActivityMonitor monitor(log, options.pollIntervalMs);
    gMonitor = &monitor;
    monitor.Poll("START");

    gPollThread = CreateThread(nullptr, 0, PollThreadProc, nullptr, 0, nullptr);
    if (gPollThread == nullptr) {
        std::wcerr << L"CreateThread failed. Error: " << GetLastError() << L"\n";
        monitor.Finish();
        Cleanup();
        return 1;
    }

    std::wcout
        << L"SolUltra activity monitor is running.\n"
        << L"Log: " << options.outputPath << L"\n"
        << L"Foreground window + idle state only; no screen, video, audio, or key capture.\n"
        << L"Press Ctrl+C to stop.\n";

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    SetEvent(gStopEvent);
    WaitForSingleObject(gPollThread, 3000);
    CloseHandle(gPollThread);
    gPollThread = nullptr;
    monitor.Finish();
    gMonitor = nullptr;
    Cleanup();
    return 0;
}

int main()
{
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argv == nullptr) {
        std::cerr << "CommandLineToArgvW failed. Win32 error: " << GetLastError() << "\n";
        return 1;
    }

    int result = RunActivityMonitor(argc, argv);
    LocalFree(argv);
    return result;
}
