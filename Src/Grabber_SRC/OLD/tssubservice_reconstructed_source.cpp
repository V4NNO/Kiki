// Reconstructed source-style pseudocode for tssubservice.exe
// Basis: tssubservice_static_analysis_ghidra.txt / Ghidra MCP static analysis.
// This is NOT original source and is NOT expected to compile.
// It focuses on the observed subAgent UI/clipboard/HTTP/IPC behavior.

#include <windows.h>
#include <string>
#include <vector>
#include <map>

struct Arguments {
    std::map<std::string, std::string> values;
};

struct NamedPipeClient {
    std::wstring pipeName;
    HANDLE handle;
};

struct HttpRequest {
    std::string method;   // "get" or "post"
    std::string postfix;
    std::string payloadJson;
};

struct HttpManager {
    std::string webToken;       // observed around object +0x68
    std::string webInterface;   // observed around object +0x70
    std::string skpVersion;     // observed around object +0x90
    std::vector<HttpRequest> queue;
    bool requestInProcess;      // observed around object +0xc0
};

struct ClipboardManager {
    bool enabled;
};

struct SettingsProvider {
    std::string title;       // "Time tracking"
    std::string dbName;      // "settings.sqlite"
    std::string homePath;
};

struct UIManager {
    bool initialized;
    SettingsProvider settings;
};

// entry @ 0x140002f3b
void entry() {
    __security_init_cookie();
    FUN_1400b8390();
}

// FUN_140017280
int main_tssubservice(int argc, char** argv) {
    set_application_name("KickidlerGrabber");

    if (argc == 2 && std::string(argv[1]) == "-get-network-proxies") {
        output_network_proxies();
        return 0;
    }

    // Original uses QApplication and QGuiApplication::setQuitOnLastWindowClosed(false).
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    Arguments args = parse_arguments(argc, argv);

    DWORD currentPid = GetCurrentProcessId();
    DWORD sessionId = 0;
    ProcessIdToSessionId(currentPid, &sessionId);

    std::string loggingPostfix = args.values["-logging-postfix"];
    std::string logName = build_subagent_log_name(sessionId, loggingPostfix);
    init_logging(logName);

    auto ipcIt = args.values.find("-ipc-name");
    if (ipcIt == args.values.end()) {
        log_error("IPC name not specified");
        return 1;
    }

    std::string ipcName = ipcIt->second;
    log_info("start agent for " + std::to_string(sessionId));

    SubAgent agent;
    agent.init();
    agent.connect_to_ipc(ipcName);

    return app.exec();
}

// FUN_1400320a0
HANDLE open_named_pipe_with_retry(const wchar_t* pipeName, DWORD access, DWORD flags) {
    for (int attempt = 0; attempt < 2; ++attempt) {
        HANDLE h = CreateFileW(pipeName, access, 0, nullptr, OPEN_EXISTING, flags, nullptr);
        if (h != INVALID_HANDLE_VALUE) {
            return h;
        }

        if (GetLastError() != ERROR_PIPE_BUSY) {
            break;
        }

        if (!WaitNamedPipeW(pipeName, 2000)) {
            break;
        }
    }

    return nullptr;
}

// FUN_140032180
HANDLE open_message_mode_pipe(const std::wstring& pipeName) {
    HANDLE h = open_named_pipe_with_retry(pipeName.c_str(), 0x103, 0x110000);
    if (!h) {
        return nullptr;
    }

    DWORD mode = 2;
    if (!SetNamedPipeHandleState(h, &mode, nullptr, nullptr)) {
        CloseHandle(h);
        return nullptr;
    }

    return h;
}

// FUN_140032210
HANDLE open_pipe_and_duplicate_to_process(const wchar_t* pipeName, HANDLE targetProcess) {
    HANDLE local = open_named_pipe_with_retry(pipeName, 0x103, 0x110000);
    if (!local) {
        return INVALID_HANDLE_VALUE;
    }

    HANDLE remote = INVALID_HANDLE_VALUE;
    if (!DuplicateHandle(GetCurrentProcess(), local, targetProcess,
                         &remote, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
        return INVALID_HANDLE_VALUE;
    }

    return remote;
}

// FUN_1400323a0
bool named_pipe_handshake(HANDLE pipe, void* state) {
    unsigned char request[0x50]{};
    unsigned char reply[0x50]{};
    DWORD bytes = 0;

    *reinterpret_cast<int*>(request + 0x00) = 1;
    *reinterpret_cast<DWORD*>(request + 0x04) = GetCurrentProcessId();

    BOOL ok = TransactNamedPipe(pipe, request, sizeof(request),
                                reply, sizeof(reply), &bytes, nullptr);
    if (!ok) {
        return false;
    }

    int replyType = *reinterpret_cast<int*>(reply + 0x00);
    int replyStatus = *reinterpret_cast<int*>(reply + 0x04);
    if (replyType != 2 || replyStatus == 0) {
        return false;
    }

    unsigned char ack[0x50]{};
    *reinterpret_cast<int*>(ack + 0x00) = 3;
    WriteFile(pipe, ack, sizeof(ack), &bytes, nullptr);

    // Original stores returned fields into state offsets +0x40/+0x48/+0x50/+0x58.
    (void)state;
    return true;
}

// FUN_1400209f0
void* qt_metacast_io_service(void* object, const char* className) {
    if (className && std::string(className) == "grabberSubAgent::io::Service") {
        return object;
    }
    return qt_base_metacast(object, className);
}

// FUN_14006f8f0
void init_clipboard_monitor(ClipboardManager* clipboard) {
    if (!clipboard->enabled) {
        return;
    }

    // Original obtains QGuiApplication::clipboard(), disconnects old callback,
    // attaches the QClipboard object and connects dataChanged to a handler.
    connect_qclipboard_dataChanged(on_clipboard_changed);
}

// FUN_14006f410
void on_clipboard_changed() {
    QMimeData* mime = get_qt_clipboard_mime_data();
    if (!mime) {
        return;
    }

    if (mime->hasUrls()) {
        send_clipboard_urls(mime->urls());
    } else if (mime->hasHtml()) {
        send_clipboard_html(mime->html());
    } else if (mime->hasText()) {
        send_clipboard_text(mime->text());
    } else if (mime->hasImage()) {
        QImage image = mime->imageData();
        if (!image.isNull()) {
            std::vector<unsigned char> png = encode_qimage_as_png(image);
            send_clipboard_image(png);
        }
    } else {
        send_clipboard_other();
    }
}

// FUN_140083580
void process_http_queue(HttpManager* http) {
    if (http->webToken.empty()) {
        log_info("web token is empty await some");
        return;
    }

    if (http->webInterface.empty()) {
        log_info("web interface link is empty await some");
        return;
    }

    if (http->queue.empty()) {
        log_info("nothing to send");
        return;
    }

    HttpRequest req = http->queue.front();
    http->queue.erase(http->queue.begin());

    if (req.method == "get") {
        std::string url = http->webInterface + req.postfix + "?web_token=" + http->webToken;
        log_info("subAgent send http GET: " + url);
        qnetwork_get(url);
    } else if (req.method == "post") {
        std::string json = "{";
        json += "\"skp_version\":\"" + http->skpVersion + "\",";
        json += "\"web_token\":\"" + http->webToken + "\",";
        json += req.payloadJson;
        json += "}";

        log_info("subAgent send http POST: " + http->webInterface + req.postfix);
        qnetwork_post_json(http->webInterface + req.postfix, json);
    } else {
        assertion_failed("never here");
    }
}

// FUN_1400875d0
void init_ui_manager(UIManager* ui) {
    ui->initialized = init_qt_ui_resources("gsa");
    if (!ui->initialized) {
        log_error("agent ui cannot be initialized");
        return;
    }

    log_info("agent ui initialized");

    init_settings_provider(&ui->settings);
    set_application_icon(":/images/selfcontrol_icon.ico");
    create_system_tray_icon();
    create_tray_context_menu();
    connect_ui_signals();
}

// FUN_140089c10
void init_settings_provider(SettingsProvider* settings) {
    settings->title = "Time tracking";
    settings->dbName = "settings.sqlite";
    settings->homePath = qdir_home_path();

    if (settings->homePath.empty()) {
        log_error("unable to locate home path");
        return;
    }

    std::string settingsDir = settings->homePath + "/Kickidler/";
    if (!ensure_directory_exists(settingsDir)) {
        log_error("unable to create settings directory: " + settingsDir);
        return;
    }

    open_qsqlite_database(settingsDir + settings->dbName);

    std::string oldIni = settingsDir + "settings.ini";
    if (file_exists(oldIni)) {
        read_legacy_setting(oldIni, "common/language");
        read_legacy_setting(oldIni, "self_control/dayActivityEnabled");
        read_legacy_setting(oldIni, "self_control/dayActivityInPercentage");
        read_legacy_setting(oldIni, "self_control/dayRatingEnabled");
        read_legacy_setting(oldIni, "self_control/dataManagementEnabled");
        read_legacy_setting(oldIni, "self_control/dataFetchingEnabled");
        remove_file(oldIni);
    }
}

// FUN_1400210d0
void init_active_directory_api() {
    HMODULE activeDs = LoadLibraryA("activeds.dll");
    if (!activeDs) {
        log_error("Failed to load activeds.dll");
        return;
    }

    auto ADsGetObject = GetProcAddress(activeDs, "ADsGetObject");
    auto ADsOpenObject = GetProcAddress(activeDs, "ADsOpenObject");
    auto FreeADsMem = GetProcAddress(activeDs, "FreeADsMem");

    if (ADsGetObject && ADsOpenObject && FreeADsMem) {
        CoInitialize(nullptr);
    }
}

// FUN_140023ec0
std::map<std::string, std::string> collect_user_info() {
    std::map<std::string, std::string> info;

    wchar_t buffer[1024]{};
    ULONG len = 1024;

    if (GetUserNameExW(NameFullyQualifiedDN, buffer, &len)) {
        info["distinguishedName"] = narrow(buffer);
        log_info("AD info successfully fetched by GetUserNameEx");
    } else {
        log_error("GetUserNameEx failed for NameFullyQualifiedDN feature");
    }

    len = 1024;
    if (GetUserNameExW(NameUniqueId, buffer, &len)) {
        info["GUID"] = lower(narrow(buffer));
    } else {
        log_error("GetUserNameEx failed for NameUniqueId feature");
    }

    if (info["distinguishedName"].empty() || info["GUID"].empty()) {
        log_info("failed to query AD info from AD, trying to get from LDAP");
        try_collect_user_info_from_ldap(info);
    }

    if (info["distinguishedName"].empty() || info["GUID"].empty()) {
        log_info("failed to query AD info from LDAP, trying to get from registry");
        try_collect_user_info_from_registry(info);
    }

    DWORD userLen = 1024;
    if (GetUserNameW(buffer, &userLen)) {
        info["fullName"] = narrow(buffer);
    } else {
        log_error("GetUserNameW failed");
    }

    info["home"] = qstandardpaths_writable_home();
    info["local"] = info["distinguishedName"].empty() ? "true" : "false";
    info["fail"] = (info["distinguishedName"].empty() || info["GUID"].empty()) ? "true" : "false";
    return info;
}

// FUN_140099410
void track_qscreen_geometry(void* qscreen) {
    if (!qscreen) {
        log_info("ui: null screen track attempt");
        return;
    }

    connect_availableGeometryChanged(qscreen, on_screen_geometry_changed);

    Rect r = qscreen_available_geometry(qscreen);
    if (screen_geometry_changed(r)) {
        store_screen_geometry(r);
        emit_screen_geometry_changed(r);
    }
}

struct SubAgent {
    NamedPipeClient pipe{};
    ClipboardManager clipboard{};
    HttpManager http{};
    UIManager ui{};

    void init() {
        init_active_directory_api();
        collect_user_info();

        clipboard.enabled = true;
        init_clipboard_monitor(&clipboard);

        init_ui_manager(&ui);
    }

    void connect_to_ipc(const std::string& ipcName) {
        pipe.pipeName = widen(ipcName);
        pipe.handle = open_message_mode_pipe(pipe.pipeName);
        if (pipe.handle) {
            named_pipe_handshake(pipe.handle, this);
        }
    }
};

// Stub helpers/classes used only to make the reconstruction readable.
struct QApplication { QApplication(int&, char**) {} void setQuitOnLastWindowClosed(bool) {} int exec() { return 0; } };
struct QMimeData { bool hasUrls(); bool hasHtml(); bool hasText(); bool hasImage(); std::vector<std::string> urls(); std::string html(); std::string text(); QImage imageData(); };
struct QImage { bool isNull(); };
struct Rect { int x; int y; int w; int h; };
void __security_init_cookie() {}
void FUN_1400b8390() {}
void set_application_name(const char*) {}
void output_network_proxies() {}
Arguments parse_arguments(int, char**) { return {}; }
std::string build_subagent_log_name(DWORD, const std::string&) { return "subAgent"; }
void init_logging(const std::string&) {}
void log_error(const std::string&) {}
void log_info(const std::string&) {}
void* qt_base_metacast(void*, const char*) { return nullptr; }
void connect_qclipboard_dataChanged(void (*)()) {}
QMimeData* get_qt_clipboard_mime_data() { return nullptr; }
void send_clipboard_urls(const std::vector<std::string>&) {}
void send_clipboard_html(const std::string&) {}
void send_clipboard_text(const std::string&) {}
void send_clipboard_image(const std::vector<unsigned char>&) {}
void send_clipboard_other() {}
std::vector<unsigned char> encode_qimage_as_png(const QImage&) { return {}; }
void qnetwork_get(const std::string&) {}
void qnetwork_post_json(const std::string&, const std::string&) {}
void assertion_failed(const char*) {}
bool init_qt_ui_resources(const char*) { return true; }
void set_application_icon(const char*) {}
void create_system_tray_icon() {}
void create_tray_context_menu() {}
void connect_ui_signals() {}
std::string qdir_home_path() { return {}; }
bool ensure_directory_exists(const std::string&) { return true; }
void open_qsqlite_database(const std::string&) {}
bool file_exists(const std::string&) { return false; }
void read_legacy_setting(const std::string&, const std::string&) {}
void remove_file(const std::string&) {}
std::string narrow(const wchar_t*) { return {}; }
std::string lower(const std::string& s) { return s; }
void try_collect_user_info_from_ldap(std::map<std::string, std::string>&) {}
void try_collect_user_info_from_registry(std::map<std::string, std::string>&) {}
std::string qstandardpaths_writable_home() { return {}; }
void connect_availableGeometryChanged(void*, void (*)(Rect)) {}
void on_screen_geometry_changed(Rect) {}
Rect qscreen_available_geometry(void*) { return {}; }
bool screen_geometry_changed(Rect) { return true; }
void store_screen_geometry(Rect) {}
void emit_screen_geometry_changed(Rect) {}
std::wstring widen(const std::string&) { return {}; }
