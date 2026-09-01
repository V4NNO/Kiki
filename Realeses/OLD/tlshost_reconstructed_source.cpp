// Reconstructed source-style pseudocode for tlshost.exe
// Basis: tlshost_static_analysis_ghidra.txt / Ghidra MCP static analysis.
// This is NOT original source and is NOT expected to compile.
// Unknown functions keep their Ghidra names where useful.

#include <windows.h>
#include <string>
#include <vector>
#include <map>

struct ImageFrame {
    int width;
    int height;
    int format;        // observed expectation around QImage::Format_RGB555
    std::vector<unsigned char> bytes;
};

struct VideoSequencer {
    int capacity;              // approx +0x18
    int readIndex;             // approx +0x28
    int availableCount;        // approx +0x2c
    bool paused;               // approx +0x30
    ImageFrame lastSentFrame;  // approx +0x40 / QImage
    ImageFrame compareFrame;   // approx +0x68 / QImage
};

struct AgentChannel {
    HANDLE mutex;
    void* sharedMemory;
    DWORD ownerPid;
};

struct ComponentNames {
    std::wstring hostExe;
    std::wstring serviceExe;
    std::wstring subserviceExe;
    std::wstring hostName;
    std::wstring serviceName;
    std::wstring subserviceName;
    std::wstring family;
    std::wstring displayName;
};

// FUN_140089540
ComponentNames map_component_profile(const std::wstring& currentExeName) {
    ComponentNames n{};

    // Observed strings:
    // tlshost.exe, tlsservice.exe, tlssubservice.exe
    // tlshost, tlsservice, tlssubservice, tls, Telemetry Service Host
    if (currentExeName == L"tlshost.exe" ||
        currentExeName == L"tlsservice.exe" ||
        currentExeName == L"tlssubservice.exe") {
        n.hostExe = L"tlshost.exe";
        n.serviceExe = L"tlsservice.exe";
        n.subserviceExe = L"tlssubservice.exe";
        n.hostName = L"tlshost";
        n.serviceName = L"tlsservice";
        n.subserviceName = L"tlssubservice";
        n.family = L"tls";
        n.displayName = L"Telemetry Service Host";
    }

    return n;
}

// FUN_14018ad90
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

// FUN_14018af00
HANDLE open_pipe_and_duplicate_to_process(const wchar_t* pipeName, HANDLE targetProcess) {
    HANDLE localPipe = open_named_pipe_with_retry(pipeName, 0x103, 0x110000);
    if (!localPipe) {
        return INVALID_HANDLE_VALUE;
    }

    HANDLE remoteHandle = INVALID_HANDLE_VALUE;
    if (!DuplicateHandle(GetCurrentProcess(), localPipe, targetProcess,
                         &remoteHandle, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
        return INVALID_HANDLE_VALUE;
    }

    return remoteHandle;
}

// FUN_14018b090
bool perform_named_pipe_handshake(HANDLE pipe, void* state) {
    unsigned char request[0x50]{};
    unsigned char response[0x50]{};
    DWORD transferred = 0;

    // Observed protocol shape:
    // request type 1, contains current PID and fields from state.
    *reinterpret_cast<int*>(request + 0x00) = 1;
    *reinterpret_cast<DWORD*>(request + 0x04) = GetCurrentProcessId();

    BOOL ok = TransactNamedPipe(pipe, request, sizeof(request),
                                response, sizeof(response),
                                &transferred, nullptr);
    if (!ok) {
        return false;
    }

    int replyType = *reinterpret_cast<int*>(response + 0x00);
    int replyStatus = *reinterpret_cast<int*>(response + 0x04);
    if (replyType != 2 || replyStatus == 0) {
        return false;
    }

    unsigned char ack[0x50]{};
    *reinterpret_cast<int*>(ack + 0x00) = 3;
    return !!WriteFile(pipe, ack, sizeof(ack), &transferred, nullptr);
}

// FUN_1402a3ab0
AgentChannel create_agent_channel(const std::string& key, size_t size) {
    AgentChannel ch{};

    ch.mutex = CreateMutexA(nullptr, FALSE, key.c_str());
    if (!ch.mutex) {
        return ch;
    }

    WaitForSingleObject(ch.mutex, INFINITE);

    // In original code this is Qt QSharedMemory::setNativeKey/create/data.
    ch.sharedMemory = nullptr; // placeholder for QSharedMemory::data()
    ch.ownerPid = GetCurrentProcessId();

    // Observed layout in shared memory:
    // offset 0: global counter
    // offset 8: QCoreApplication::applicationPid()

    ReleaseMutex(ch.mutex);
    return ch;
}

// FUN_140127170
void collect_opengl_telemetry() {
    HMODULE gl = LoadLibraryA("opengl32.dll");
    if (!gl) {
        return;
    }

    auto p_wglCreateContext = GetProcAddress(gl, "wglCreateContext");
    auto p_wglMakeCurrent = GetProcAddress(gl, "wglMakeCurrent");
    auto p_wglDeleteContext = GetProcAddress(gl, "wglDeleteContext");
    auto p_glGetString = GetProcAddress(gl, "glGetString");

    // Original creates a hidden 1x1 "Kickidler OpenGL Tester" window,
    // obtains HDC, sets pixel format, creates an OpenGL context, then calls
    // glGetString(GL_VENDOR/GL_RENDERER/GL_VERSION/GL_EXTENSIONS).

    (void)p_wglCreateContext;
    (void)p_wglMakeCurrent;
    (void)p_wglDeleteContext;
    (void)p_glGetString;

    FreeLibrary(gl);
}

// FUN_14028e700
void init_video_sequencer(VideoSequencer& seq, int capacity) {
    seq.capacity = capacity;
    seq.readIndex = 0;
    seq.availableCount = 0;
    seq.paused = false;
    seq.lastSentFrame = {};
    seq.compareFrame = {};
}

// FUN_14028fcd0
std::vector<unsigned char> serialize_frame_rect(const ImageFrame& frame,
                                                int x, int y, int w, int h) {
    std::vector<unsigned char> packet;

    // Original writes rectangle coordinates, width, height, image format,
    // then QImage::constBits() bytes into QByteArray.
    append_int(packet, x);
    append_int(packet, y);
    append_int(packet, w);
    append_int(packet, h);
    append_int(packet, frame.format);
    packet.insert(packet.end(), frame.bytes.begin(), frame.bytes.end());

    return packet;
}

// FUN_14028ee20
std::vector<std::vector<unsigned char>> build_video_delta_packets(VideoSequencer& seq) {
    std::vector<std::vector<unsigned char>> packets;

    if (seq.paused || seq.availableCount == 0) {
        return packets;
    }

    ImageFrame current = pop_next_frame_from_ring_buffer(seq);
    if (current.bytes.empty()) {
        return packets;
    }

    // Observed assert: image.format() == QImage::Format_RGB555.
    if (current.format != /*QImage::Format_RGB555*/ 12) {
        return packets;
    }

    if (seq.lastSentFrame.bytes.empty() ||
        seq.lastSentFrame.width != current.width ||
        seq.lastSentFrame.height != current.height) {
        packets.push_back(serialize_frame_rect(current, 0, 0, current.width, current.height));
    } else {
        // Original compares current QImage to previous QImage and decides
        // whether to send changed rectangles or a full frame.
        auto changedRects = calculate_changed_rects(seq.lastSentFrame, current);
        if (changedRects.empty()) {
            return packets;
        }

        if (changedRects_are_efficient(changedRects, current)) {
            for (const auto& r : changedRects) {
                packets.push_back(serialize_frame_rect(current, r.x, r.y, r.w, r.h));
            }
        } else {
            packets.push_back(serialize_frame_rect(current, 0, 0, current.width, current.height));
        }
    }

    seq.lastSentFrame = current;
    return packets;
}

// FUN_14028c570
void send_video_frame_if_ready(VideoSequencer& seq) {
    if (!is_stream_active()) {
        return;
    }

    auto packets = build_video_delta_packets(seq);
    if (packets.empty()) {
        return;
    }

    if (!is_time_negotiated_with_host()) {
        reset_or_drop_pending_frame();
        return;
    }

    for (const auto& packet : packets) {
        send_packet_to_session_transport(packet);
    }
}

// FUN_140086b60
void enumerate_sessions_and_launch_user_agent() {
    // Observed APIs:
    // WTSEnumerateSessionsW, WTSQuerySessionInformationW, WTSQueryUserToken,
    // CreatePipe, CreateProcessAsUserW, WaitForSingleObject, ReadFile.

    // High-level behavior:
    // 1. enumerate WTS sessions;
    // 2. find active user session;
    // 3. obtain user token;
    // 4. start helper/agent in that user session;
    // 5. read JSON response through redirected pipe;
    // 6. parse fields such as "uid" and "proxies".
}

int main_tlshost() {
    set_application_name("KickidlerGrabber");
    ComponentNames names = map_component_profile(L"tlshost.exe");

    collect_opengl_telemetry();
    enumerate_sessions_and_launch_user_agent();

    AgentChannel channel = create_agent_channel("grabber-agent-channel", 0x10000);
    (void)channel;

    VideoSequencer video{};
    init_video_sequencer(video, 8);

    while (service_is_running()) {
        process_ipc_messages();
        send_video_frame_if_ready(video);
        process_transport_events();
    }

    return 0;
}

// Stub helpers used only to make the reconstruction readable.
struct Rect { int x; int y; int w; int h; };
void append_int(std::vector<unsigned char>&, int) {}
ImageFrame pop_next_frame_from_ring_buffer(VideoSequencer&) { return {}; }
std::vector<Rect> calculate_changed_rects(const ImageFrame&, const ImageFrame&) { return {}; }
bool changedRects_are_efficient(const std::vector<Rect>&, const ImageFrame&) { return true; }
bool is_stream_active() { return true; }
bool is_time_negotiated_with_host() { return true; }
void reset_or_drop_pending_frame() {}
void send_packet_to_session_transport(const std::vector<unsigned char>&) {}
void set_application_name(const char*) {}
bool service_is_running() { return false; }
void process_ipc_messages() {}
void process_transport_events() {}
