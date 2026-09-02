// Reconstructed source-style pseudocode for OldKickExe/RuntimeBroker.exe
// Basis: RuntimeBroker_static_analysis_ghidra.txt / Ghidra MCP static analysis.
// This is NOT original source and is NOT expected to compile.
// Despite the filename, the observed strings identify it as Kickidler app-grabberSubAgent.

#include <string>
#include <vector>
#include <map>

using HANDLE = void*;
using DWORD = unsigned long;

struct Arguments {
    std::map<std::string, std::string> values;
};

struct NamedPipeClient {
    std::wstring pipeName;
    HANDLE handle;
};

struct HttpRequest {
    std::string method;       // "get" or "post"
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

struct SubAgent {
    NamedPipeClient pipe;
    ClipboardManager clipboard;
    HttpManager http;
    UIManager ui;

    void init() {
        init_active_directory_api();
        collect_user_info();
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

// entry @ 0x140002f68
void entry() {
    __security_init_cookie();
    FUN_1400b81b0();
}

// FUN_140017280
int main_RuntimeBroker_subagent(int argc, char** argv) {
    set_application_name("KickidlerGrabber");

    if (argc == 2 && std::string(argv[1]) == "-get-network-proxies") {
        output_network_proxies();
        return 0;
    }

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    Arguments args = parse_arguments(argc, argv);

    DWORD sessionId = process_id_to_session_id(get_current_process_id());
    std::string loggingPostfix = args.values["-logging-postfix"];
    init_logging(build_subagent_log_name(sessionId, loggingPostfix));

    auto ipc = args.values.find("-ipc-name");
    if (ipc == args.values.end()) {
        log_error("IPC name not specified");
        return 1;
    }

    log_info("start agent for " + to_string(sessionId));

    SubAgent agent;
    agent.init();
    agent.connect_to_ipc(ipc->second);

    return app.exec();
}

// FUN_14001cb90
void connect_local_rpc_service(SubAgent* agent, QLocalSocket* socket) {
    if (socket_already_connected(socket)) {
        log_error("Attempted to connect to already connected IPC (!)");
        return;
    }

    RpcObject* rpc = create_rpc_object(socket);
    if (!rpc) {
        log_error("Failed to create RPC object");
        abort_socket(socket);
        return;
    }

    rpc->bind("getUserInfo", agent, "onGetUserInfo");
    rpc->bind("getSystemProxies", agent, "onGetSystemProxies");
    rpc->bind("resetClipboard", agent, "onResetClipboard");
    rpc->bind("setClipboardText", agent, "onSetClipboardText");
    rpc->bind("setClipboardHtml", agent, "onSetClipboardHtml");
    rpc->bind("setClipboardImage", agent, "onSetClipboardImage");
    rpc->bind("setClipboardMimeData", agent, "onSetClipboardMimeData");
    rpc->bind("clearClipboard", agent, "onClearClipboard");
    rpc->bind("getClipboardUrls", agent, "onGetClipboardUrls");
    rpc->bind("setSelfControlState", agent, "onSelfControlState");
    rpc->bind("setSelfControlDayActivity", agent, "onSelfControlDayActivity");
    rpc->bind("setSelfControlDayRating", agent, "onSelfControlDayRating");
    rpc->bind("setSelfControlWebToken", agent, "onSelfControlWebToken");
    rpc->bind("notifySelfControlTasksChanged", agent, "onSelfControlTasksChanged");
    rpc->bind("setNotificationAlert", agent, "onNotificationAlert");
    rpc->bind("setWebinterfaceLink", agent, "onWebinterfaceLink");

    connect(socket, "disconnected()", agent, "quit()");
    connect(socket, "error()", agent, "onSocketError()");
    connect(socket, "stateChanged()", agent, "onSocketStateChanged()");

    rpc->call("testPing", "test ping");
}

// FUN_1400320a0
HANDLE open_named_pipe_with_retry(const wchar_t* pipeName, DWORD access, DWORD flags) {
    for (int attempt = 0; attempt < 2; ++attempt) {
        HANDLE h = CreateFileW_stub(pipeName, access, flags);
        if (h != invalid_handle()) {
            return h;
        }

        if (last_error() != 0xE7) {
            break;
        }

        if (!WaitNamedPipeW_stub(pipeName, 2000)) {
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

    if (!SetNamedPipeHandleState_stub(h, 2)) {
        CloseHandle_stub(h);
        return nullptr;
    }

    return h;
}

// FUN_140032210
HANDLE duplicate_pipe_to_process(const wchar_t* pipeName, HANDLE targetProcess) {
    HANDLE localPipe = open_named_pipe_with_retry(pipeName, 0x103, 0x110000);
    if (!localPipe) {
        return invalid_handle();
    }

    return DuplicateHandle_stub(localPipe, targetProcess);
}

// FUN_1400323a0
bool named_pipe_handshake(HANDLE pipe, void* state) {
    unsigned char request[0x50]{};
    unsigned char reply[0x50]{};

    write_i32(request + 0x00, 1);
    write_u32(request + 0x04, get_current_process_id());

    if (!TransactNamedPipe_stub(pipe, request, sizeof(request), reply, sizeof(reply))) {
        return false;
    }

    int replyType = read_i32(reply + 0x00);
    int status = read_i32(reply + 0x04);
    if (replyType != 2 || status == 0) {
        return false;
    }

    unsigned char ack[0x50]{};
    write_i32(ack + 0x00, 3);
    WriteFile_stub(pipe, ack, sizeof(ack));

    // Original writes returned handles/fields into state +0x40/+0x48/+0x50/+0x58.
    (void)state;
    return true;
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
        return;
    }

    if (req.method == "post") {
        JsonObject json;
        json["skp_version"] = http->skpVersion;
        json["web_token"] = http->webToken;
        json.merge(req.payloadJson);

        log_info("subAgent send http POST: " + http->webInterface + req.postfix);
        qnetwork_post_json(http->webInterface + req.postfix, json.to_string());
        return;
    }

    assertion_failed("never here");
}

// Clipboard behavior observed through RPC strings and Qt imports.
void init_clipboard_monitor(ClipboardManager* clipboard) {
    clipboard->enabled = true;
    connect_qclipboard_dataChanged(on_clipboard_changed);
}

void on_clipboard_changed() {
    QMimeData* mime = get_clipboard_mime_data();
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
        send_clipboard_image(encode_image_as_png(mime->imageData()));
    } else {
        send_clipboard_other();
    }
}

// FUN_140023ec0
std::map<std::string, std::string> collect_user_info() {
    std::map<std::string, std::string> info;

    if (!get_user_name_ex_fqdn(info["distinguishedName"])) {
        log_error("GetUserNameEx failed for NameFullyQualifiedDN feature");
    }

    if (!get_user_name_ex_unique_id(info["GUID"])) {
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

    if (info["fullName"].empty()) {
        info["fullName"] = get_user_name_w();
    }

    info["local"] = info["distinguishedName"].empty() ? "true" : "false";
    info["fail"] = (info["distinguishedName"].empty() || info["GUID"].empty()) ? "true" : "false";
    info["home"] = qstandardpaths_writable_home();
    return info;
}

void init_active_directory_api() {
    void* activeds = LoadLibraryA_stub("activeds.dll");
    if (!activeds) {
        log_error("Failed to load activeds.dll");
        return;
    }

    auto ADsGetObject = GetProcAddress_stub(activeds, "ADsGetObject");
    auto ADsOpenObject = GetProcAddress_stub(activeds, "ADsOpenObject");
    auto FreeADsMem = GetProcAddress_stub(activeds, "FreeADsMem");

    if (ADsGetObject && ADsOpenObject && FreeADsMem) {
        CoInitialize_stub();
    }
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

    std::string ini = settingsDir + "settings.ini";
    if (file_exists(ini)) {
        read_legacy_setting(ini, "common/language");
        read_legacy_setting(ini, "self_control/dayActivityEnabled");
        read_legacy_setting(ini, "self_control/dayActivityInPercentage");
        read_legacy_setting(ini, "self_control/dayRatingEnabled");
        read_legacy_setting(ini, "self_control/dataManagementEnabled");
        read_legacy_setting(ini, "self_control/dataFetchingEnabled");
        remove_file(ini);
    }
}

// FUN_140086b50 / UI Manager metaobject
void init_ui_manager(UIManager* ui) {
    ui->initialized = init_qt_ui_resources("grabberSubAgent::ui::Manager");
    if (!ui->initialized) {
        return;
    }

    init_settings_provider(&ui->settings);
    create_system_tray_icon();
    create_self_control_ui();
}

// FUN_140099230
void track_screen_geometry(QScreen* screen) {
    if (!screen) {
        log_info("ui: null screen track attempt");
        return;
    }

    connect_availableGeometryChanged(screen, on_screen_geometry_changed);
    Rect r = screen->availableGeometry();
    store_screen_geometry(r);
}

// Stub declarations used only for readability.
struct QApplication { QApplication(int, char**) {} void setQuitOnLastWindowClosed(bool) {} int exec() { return 0; } };
struct QLocalSocket {};
struct RpcObject { void bind(const char*, SubAgent*, const char*) {} void call(const char*, const char*) {} };
struct QMimeData { bool hasUrls(); bool hasHtml(); bool hasText(); bool hasImage(); std::vector<std::string> urls(); std::string html(); std::string text(); std::string imageData(); };
struct JsonObject { std::map<std::string,std::string> fields; std::string& operator[](const std::string& k) { return fields[k]; } void merge(const std::string&) {} std::string to_string() const { return "{}"; } };
struct Rect { int x; int y; int w; int h; };
struct QScreen { Rect availableGeometry(); };
void __security_init_cookie() {}
void FUN_1400b81b0() {}
void set_application_name(const char*) {}
void output_network_proxies() {}
Arguments parse_arguments(int, char**) { return {}; }
DWORD get_current_process_id() { return 0; }
DWORD process_id_to_session_id(DWORD) { return 0; }
std::string build_subagent_log_name(DWORD, const std::string&) { return "subAgent"; }
void init_logging(const std::string&) {}
void log_error(const std::string&) {}
void log_info(const std::string&) {}
std::string to_string(DWORD) { return {}; }
std::wstring widen(const std::string&) { return {}; }
HANDLE CreateFileW_stub(const wchar_t*, DWORD, DWORD) { return nullptr; }
HANDLE invalid_handle() { return (HANDLE)-1; }
DWORD last_error() { return 0; }
bool WaitNamedPipeW_stub(const wchar_t*, DWORD) { return true; }
bool SetNamedPipeHandleState_stub(HANDLE, DWORD) { return true; }
void CloseHandle_stub(HANDLE) {}
HANDLE DuplicateHandle_stub(HANDLE, HANDLE) { return nullptr; }
bool TransactNamedPipe_stub(HANDLE, void*, size_t, void*, size_t) { return true; }
void WriteFile_stub(HANDLE, void*, size_t) {}
void write_i32(unsigned char*, int) {}
void write_u32(unsigned char*, DWORD) {}
int read_i32(unsigned char*) { return 0; }
bool socket_already_connected(QLocalSocket*) { return false; }
RpcObject* create_rpc_object(QLocalSocket*) { return nullptr; }
void abort_socket(QLocalSocket*) {}
void connect(QLocalSocket*, const char*, SubAgent*, const char*) {}
void qnetwork_get(const std::string&) {}
void qnetwork_post_json(const std::string&, const std::string&) {}
void assertion_failed(const char*) {}
void connect_qclipboard_dataChanged(void (*)()) {}
QMimeData* get_clipboard_mime_data() { return nullptr; }
void send_clipboard_urls(const std::vector<std::string>&) {}
void send_clipboard_html(const std::string&) {}
void send_clipboard_text(const std::string&) {}
void send_clipboard_image(const std::string&) {}
void send_clipboard_other() {}
std::string encode_image_as_png(const std::string&) { return {}; }
bool get_user_name_ex_fqdn(std::string&) { return false; }
bool get_user_name_ex_unique_id(std::string&) { return false; }
void try_collect_user_info_from_ldap(std::map<std::string,std::string>&) {}
void try_collect_user_info_from_registry(std::map<std::string,std::string>&) {}
std::string get_user_name_w() { return {}; }
std::string qstandardpaths_writable_home() { return {}; }
void* LoadLibraryA_stub(const char*) { return nullptr; }
void* GetProcAddress_stub(void*, const char*) { return nullptr; }
void CoInitialize_stub() {}
std::string qdir_home_path() { return {}; }
bool ensure_directory_exists(const std::string&) { return true; }
void open_qsqlite_database(const std::string&) {}
bool file_exists(const std::string&) { return false; }
void read_legacy_setting(const std::string&, const std::string&) {}
void remove_file(const std::string&) {}
bool init_qt_ui_resources(const char*) { return true; }
void create_system_tray_icon() {}
void create_self_control_ui() {}
void connect_availableGeometryChanged(QScreen*, void (*)(Rect)) {}
void on_screen_geometry_changed(Rect) {}
void store_screen_geometry(Rect) {}
