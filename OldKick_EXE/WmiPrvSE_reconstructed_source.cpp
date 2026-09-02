// Reconstructed source-style pseudocode for OldKickExe/WmiPrvSE.exe.
// Generated from static Ghidra observations only.
// This is NOT original source code and is NOT intended to compile.

#include <windows.h>

// ---------------------------------------------------------------------------
// Observed executable role
// ---------------------------------------------------------------------------
//
// The old WmiPrvSE.exe is not behaving like Microsoft WMI Provider Host.
// It behaves like an older Kickidler service/grabber component:
// - starts a Qt event loop;
// - initializes session/user/desktop helpers;
// - installs keyboard/mouse low-level hooks;
// - imports WinEvent/window/focus APIs;
// - imports GDI capture APIs;
// - uses Qt local sockets/shared memory and named pipe APIs for IPC.

struct QStringLike {};
struct QApplicationLike {};
struct QSettingsLike {};
struct QTimerLike {};
struct QMutexLike {};
struct QMapLike {};
struct QHashLike {};

struct InputActivityCollector {
    void *vtable;
    QMutexLike mutex;
    bool initialized;
    void *unknown_20;
    void *unknown_28;
    void *qdatetime_30;
    void *qdatetime_40;
    QStringLike last_text_like;
    void *list_a;
    void *list_b;
    void *unknown_68;
    void *unknown_70;
    HHOOK keyboard_hook;      // observed object +0x78
    HHOOK mouse_hook;         // observed object +0x80
    void *unknown_88;
    void *unknown_90;
    bool compatibility_flag;  // observed object +0x98
};

struct WindowMetadataTracker {
    void *vtable;
    QMutexLike mutex;         // object +0x10
    QTimerLike timer;         // object +0x18, interval 500 ms
    QMapLike map_38;
    QHashLike hash_40;
    QHashLike hash_48;
    void *unknown_50;
    QHashLike hash_58;
    void *unknown_60;
    QMapLike map_68;
    void *unknown_70;
    unsigned short unknown_78;
    bool running_state;       // object +0x7a
};

struct SessionUserHelper {
    bool ready;
    HMODULE lib_a;
    HMODULE lib_b;
    FARPROC fn_18;
    FARPROC fn_20;
    FARPROC fn_28;
    FARPROC fn_30;
    FARPROC fn_38;
    FARPROC fn_40;
    HKEY user_registry_key;
};

struct TopLevelGrabberObject {
    WindowMetadataTracker tracker;
    char padding_80[8];
    SessionUserHelper session_helper; // initialized at object +0x88
    void *field_d8;
};

struct ProviderMapHelper {
    HMODULE lib_a;
    HMODULE lib_b;
    FARPROC create_or_resolve_fn;
    FARPROC secondary_fn;
    void *provider_object;
    QMapLike provider_map;    // observed object +0x28
};

// ---------------------------------------------------------------------------
// entry @ 0x14000d4fe
// ---------------------------------------------------------------------------

int entry()
{
    // MSVC CRT startup wrapper.
    // Observed behavior:
    // - initialize CRT
    // - get command line
    // - call FUN_1401e1610
    // - exit with return code
    return 0;
}

// ---------------------------------------------------------------------------
// FUN_1401e1610 @ 0x1401e1610
// Unicode command-line wrapper.
// ---------------------------------------------------------------------------

int FUN_1401e1610()
{
    int argc = 0;
    wchar_t **wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!wargv) {
        return -1;
    }

    // Static observation: each wchar argv is converted through WideCharToMultiByte.
    char **argv = nullptr;
    // argv = allocate argc + 1 pointers;
    // for each argument:
    //     len = WideCharToMultiByte(...);
    //     argv[i] = allocate(len);
    //     WideCharToMultiByte(..., argv[i], len, ...);

    LocalFree(wargv);

    // Real main.
    int rc = FUN_1400302a0(argc, argv);

    // free argv strings and argv array.
    return rc;
}

// ---------------------------------------------------------------------------
// FUN_1400302a0 @ 0x1400302a0
// Real main / Qt service-grabber application initializer.
// ---------------------------------------------------------------------------

int FUN_1400302a0(int argc, char **argv)
{
    // QCoreApplication::setApplicationName(...)
    // Decode argv[0], configure logging path, install qInstallMessageHandler.

    SetProcessShutdownParameters(0, 0);

    DWORD session_id = 0xffffffff;
    ProcessIdToSessionId(GetCurrentProcessId(), &session_id);

    // Builds a session-dependent name/string. This likely separates IPC/log
    // state per Windows session.
    // QString sessionName = format("...%u...", session_id);

    // QApplication app(argc, argv);
    // QSettings settings(...);
    // Resource registration.

    FUN_14004b740_register_resources();

    // Manager object creation:
    // FUN_1400872d0(manager, application_path, session_info, optional_arg);
    // if manager initialization succeeds:
    //     QApplication::exec();

    return 0;
}

// ---------------------------------------------------------------------------
// FUN_14003e3e0 @ 0x14003e3e0
// OS/version compatibility setup.
// ---------------------------------------------------------------------------

void FUN_14003e3e0(InputActivityCollector *self)
{
    OSVERSIONINFOA vi = {};
    vi.dwOSVersionInfoSize = sizeof(vi);
    self->compatibility_flag = true;

    if (GetVersionExA(&vi)) {
        // Observed: flag depends on major/minor/build values.
        self->compatibility_flag = true;
    } else {
        // log GetLastError()
    }
}

// ---------------------------------------------------------------------------
// FUN_14003aee0 @ 0x14003aee0
// Base constructor-like initializer for input/activity state.
// ---------------------------------------------------------------------------

InputActivityCollector *FUN_14003aee0(InputActivityCollector *self)
{
    // QObject constructor, QMutex constructor, QDateTime and QString init.
    self->initialized = false;
    self->keyboard_hook = nullptr;
    self->mouse_hook = nullptr;
    self->compatibility_flag = false;
    return self;
}

// ---------------------------------------------------------------------------
// FUN_14003cb20 @ 0x14003cb20
// Input/activity collector constructor.
// ---------------------------------------------------------------------------

InputActivityCollector *FUN_14003cb20(InputActivityCollector *self)
{
    FUN_14003aee0(self);

    self->keyboard_hook = nullptr;
    self->mouse_hook = nullptr;
    self->unknown_88 = nullptr;
    self->unknown_90 = nullptr;

    FUN_14003e3e0(self);

    // Logs whether compatibility mode / newer OS path is active.
    return self;
}

// ---------------------------------------------------------------------------
// FUN_14003d990 @ 0x14003d990
// Installs keyboard and mouse low-level hooks.
// ---------------------------------------------------------------------------

void FUN_14003d990(InputActivityCollector *self)
{
    HMODULE module = GetModuleHandleA(nullptr);

    self->keyboard_hook = SetWindowsHookExA(
        WH_KEYBOARD_LL,       // observed numeric 0x0d
        LowLevelHookProc,     // callback address is stored in data, symbol unresolved
        module,
        0
    );

    if (!self->keyboard_hook) {
        DWORD err = GetLastError();
        (void)err; // log error
    }

    module = GetModuleHandleA(nullptr);

    self->mouse_hook = SetWindowsHookExA(
        WH_MOUSE_LL,          // observed numeric 0x0e
        LowLevelHookProc,
        module,
        0
    );

    if (!self->mouse_hook) {
        DWORD err = GetLastError();
        (void)err; // log error
    }
}

LRESULT CALLBACK LowLevelHookProc(int code, WPARAM wparam, LPARAM lparam)
{
    // Static analysis confirmed hook installation, but MCP did not recover the
    // actual callback body at the referenced data address.
    // Expected behavior based on imports:
    // - CallNextHookEx
    // - GetForegroundWindow
    // - GetWindowThreadProcessId
    // - ToUnicodeEx/GetKeyboardLayout/GetKeyState for keyboard text
    return CallNextHookEx(nullptr, code, wparam, lparam);
}

// ---------------------------------------------------------------------------
// FUN_14003ebd0 @ 0x14003ebd0
// Switches current thread to the active input desktop.
// ---------------------------------------------------------------------------

void FUN_14003ebd0()
{
    HDESK desk = OpenInputDesktop(0, FALSE, 8);
    if (!desk) {
        DWORD err = GetLastError();
        (void)err; // log error
        return;
    }

    char desktop_name[256] = {};
    if (!GetUserObjectInformationA(desk, UOI_NAME, desktop_name, sizeof(desktop_name), nullptr)) {
        DWORD err = GetLastError();
        (void)err; // log error
        CloseDesktop(desk);
        return;
    }

    // Observed: QStringList contains desktop names and decides whether to call
    // SetThreadDesktop.
    bool allowed = true;
    if (allowed) {
        if (!SetThreadDesktop(desk)) {
            DWORD err = GetLastError();
            (void)err; // log error
        }
    }

    CloseDesktop(desk);
}

// ---------------------------------------------------------------------------
// FUN_1400409b0 @ 0x1400409b0
// Window/focus metadata tracker constructor.
// ---------------------------------------------------------------------------

WindowMetadataTracker *FUN_1400409b0(WindowMetadataTracker *self)
{
    // QObject constructor.
    // QMutex recursive constructor.
    // QTimer constructor.
    // QMap/QHash shared-null initialization.

    self->running_state = false;

    // QObject::connect(timer.timeout, self, thunk_FUN_140043c20)
    // QTimer::setInterval(timer, 500)

    return self;
}

// ---------------------------------------------------------------------------
// FUN_140043c20 @ 0x140043c20
// Timer callback wrapper.
// ---------------------------------------------------------------------------

void FUN_140043c20(WindowMetadataTracker *self)
{
    // QMutexLocker lock(&self->mutex);
    if (!self->running_state) {
        // qt_assert
    }

    // Observed: calls virtual method at vtable +0x68.
    // Likely periodic update/flush for window/focus metadata.
    // self->vtable->tick_or_flush(self);
}

// ---------------------------------------------------------------------------
// FUN_140049860 @ 0x140049860
// WTS/session/user-token helper initializer.
// ---------------------------------------------------------------------------

void FUN_140049860(SessionUserHelper *self)
{
    self->ready = false;
    self->lib_a = LoadLibraryA("...");
    if (!self->lib_a) {
        return;
    }

    self->lib_b = LoadLibraryA("...");
    if (!self->lib_b) {
        FreeLibrary(self->lib_a);
        self->lib_a = nullptr;
        return;
    }

    self->fn_18 = GetProcAddress(self->lib_a, "...");
    self->fn_20 = GetProcAddress(self->lib_b, "...");
    self->fn_28 = GetProcAddress(self->lib_b, "...");
    self->fn_30 = GetProcAddress(self->lib_b, "...");
    self->fn_38 = GetProcAddress(self->lib_b, "...");
    self->fn_40 = GetProcAddress(self->lib_b, "...");

    if (!self->fn_18 || !self->fn_20 || !self->fn_28 ||
        !self->fn_30 || !self->fn_38 || !self->fn_40) {
        FreeLibrary(self->lib_a);
        FreeLibrary(self->lib_b);
        self->lib_a = nullptr;
        self->lib_b = nullptr;
        return;
    }

    self->ready = true;

    DWORD *session_ptr = nullptr;
    if (!WTSQuerySessionInformationA(WTS_CURRENT_SERVER_HANDLE,
                                     0xffffffff,
                                     WTSActiveConsoleSessionId,
                                     reinterpret_cast<LPSTR *>(&session_ptr),
                                     nullptr)) {
        DWORD err = GetLastError();
        (void)err; // log error
        return;
    }

    DWORD session_id = *session_ptr;
    WTSFreeMemory(session_ptr);

    HANDLE token = nullptr;
    if (!WTSQueryUserToken(session_id, &token)) {
        DWORD err = GetLastError();
        (void)err; // log error
        return;
    }

    // Observed:
    // - allocate buffer;
    // - GetTokenInformation(TokenUser);
    // - ConvertSidToStringSidW;
    // - RegOpenKeyExW(HKEY_USERS, sid_string, ...).

    CloseHandle(token);
}

// ---------------------------------------------------------------------------
// FUN_140046270 @ 0x140046270
// Top-level collector/grabber object constructor candidate.
// ---------------------------------------------------------------------------

TopLevelGrabberObject *FUN_140046270()
{
    auto *self = new TopLevelGrabberObject();
    FUN_1400409b0(&self->tracker);
    FUN_140049860(&self->session_helper);
    self->field_d8 = nullptr;
    return self;
}

// ---------------------------------------------------------------------------
// FUN_140050370 @ 0x140050370
// Provider/accessibility helper constructor candidate.
// ---------------------------------------------------------------------------

ProviderMapHelper *FUN_140050370(ProviderMapHelper *self)
{
    self->lib_a = nullptr;
    self->lib_b = nullptr;
    self->create_or_resolve_fn = nullptr;
    self->secondary_fn = nullptr;
    self->provider_object = nullptr;

    FUN_140051810(self);
    FUN_1400536e0(self);
    return self;
}

// ---------------------------------------------------------------------------
// FUN_140051810 @ 0x140051810
// Dynamic provider loader.
// ---------------------------------------------------------------------------

void FUN_140051810(ProviderMapHelper *self)
{
    FUN_140050bf0_cleanup_provider(self);

    self->lib_a = LoadLibraryW(L"..."); // observed LoadLibraryW, string not decoded
    if (!self->lib_a) {
        FUN_140050bf0_cleanup_provider(self);
        return;
    }

    self->lib_b = LoadLibraryW(L"..."); // observed LoadLibraryW, string not decoded
    if (!self->lib_b) {
        FUN_140050bf0_cleanup_provider(self);
        return;
    }

    self->create_or_resolve_fn = GetProcAddress(self->lib_a, "...");
    self->secondary_fn = GetProcAddress(self->lib_b, "...");

    if (!self->create_or_resolve_fn || !self->secondary_fn) {
        FUN_140050bf0_cleanup_provider(self);
        return;
    }

    // Observed: calls resolved function and checks HRESULT < 0.
    // HRESULT hr = ((fn_t)self->create_or_resolve_fn)(..., callback_object);
    // if (hr < 0) cleanup;
}

// ---------------------------------------------------------------------------
// FUN_1400536e0 @ 0x1400536e0
// Enumerates provider/interface objects.
// ---------------------------------------------------------------------------

bool FUN_1400536e0(ProviderMapHelper *self)
{
    if (!self->create_or_resolve_fn || !self->secondary_fn || !self->provider_object) {
        return false;
    }

    for (int index = 0;; ++index) {
        void *item = nullptr;

        // Observed: provider virtual call at +0x60 returns item by index.
        // if (provider->get_item(index, &item) < 0) break;

        FUN_140052240(self, &item);
    }

    return true;
}

// ---------------------------------------------------------------------------
// FUN_140052240 @ 0x140052240
// Processes provider objects and updates QMap by QString key.
// ---------------------------------------------------------------------------

void FUN_140052240(ProviderMapHelper *self, void **provider_item)
{
    // Observed pattern:
    // - retry setup with decreasing counter;
    // - enumerate child/provider interfaces;
    // - read a wide string name;
    // - convert to QString via QString::fromWCharArray;
    // - insert/update self->provider_map[name] with a wrapped object.

    (void)self;
    (void)provider_item;
}

// ---------------------------------------------------------------------------
// Imported capture/focus APIs not fully reconstructed by MCP xrefs.
// ---------------------------------------------------------------------------

void probable_focus_metadata_update()
{
    HWND hwnd = GetForegroundWindow();
    DWORD pid = 0;
    DWORD tid = GetWindowThreadProcessId(hwnd, &pid);

    GUITHREADINFO gui = {};
    gui.cbSize = sizeof(gui);
    GetGUIThreadInfo(tid, &gui);

    wchar_t title[512] = {};
    wchar_t klass[256] = {};
    GetWindowTextW(hwnd, title, 512);
    GetClassNameW(hwnd, klass, 256);

    RECT rect = {};
    GetWindowRect(hwnd, &rect);

    // Store into QMap/QHash-like metadata containers.
}

void probable_gdi_capture(HWND hwnd)
{
    HDC src = GetWindowDC(hwnd);
    if (!src) {
        src = GetDC(nullptr);
    }

    HDC mem = CreateCompatibleDC(src);
    RECT r = {};
    GetWindowRect(hwnd, &r);

    int w = r.right - r.left;
    int h = r.bottom - r.top;
    HBITMAP bmp = CreateCompatibleBitmap(src, w, h);
    HGDIOBJ old = SelectObject(mem, bmp);

    BOOL ok = PrintWindow(hwnd, mem, 0);
    if (!ok) {
        BitBlt(mem, 0, 0, w, h, src, 0, 0, SRCCOPY);
    }

    // GetDIBits(...) into frame buffer.
    // Send frame/metadata over shared memory/local socket/named pipe.

    SelectObject(mem, old);
    DeleteObject(bmp);
    DeleteDC(mem);
    ReleaseDC(hwnd, src);
}

// ---------------------------------------------------------------------------
// IPC model observed from imports.
// ---------------------------------------------------------------------------

void probable_ipc_model()
{
    // Qt path:
    // - QLocalSocket read/write
    // - QTcpServer/QSslSocket for network-capable communication
    // - QSharedMemory create/attach/data/detach for frame or metadata buffers

    // Win32 path:
    // - CreateFileW("\\\\.\\pipe\\...")
    // - WaitNamedPipeW
    // - SetNamedPipeHandleState
    // - TransactNamedPipe
    // - WriteFile
    // - DuplicateHandle
}

// ---------------------------------------------------------------------------
// Debugging targets for stale Running Applications metadata.
// ---------------------------------------------------------------------------

void recommended_debug_breakpoints()
{
    // Functions:
    // - FUN_14003d990
    // - FUN_14003ebd0
    // - FUN_1400409b0
    // - FUN_140043c20
    // - FUN_140049860
    // - FUN_140046270
    // - FUN_140050370
    // - FUN_140051810
    // - FUN_140052240

    // APIs:
    // - SetWinEventHook
    // - GetForegroundWindow
    // - GetGUIThreadInfo
    // - GetWindowThreadProcessId
    // - GetWindowTextW
    // - GetClassNameW
    // - OpenInputDesktop
    // - SetThreadDesktop
    // - WTSQuerySessionInformationA
    // - QLocalSocket / QSharedMemory calls
}

