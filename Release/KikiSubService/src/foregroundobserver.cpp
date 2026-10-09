#include "foregroundobserver.h"

#include "browserurlreader.h"

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

namespace {
ForegroundObservation describe(ForegroundSource &source, quintptr window)
{
    ForegroundObservation observation;
    observation.window = window;
    if (window != 0) {
        observation.application = source.executableOf(window);
        observation.title = source.titleOf(window);
    }
    if (observation.application.isEmpty()) {
        observation.application = QStringLiteral("desktop");
    }
    return observation;
}
} // namespace

ForegroundObservation observeForeground(ForegroundSource &source)
{
    constexpr int kAttempts = 2;
    for (int attempt = 0; attempt < kAttempts; ++attempt) {
        ForegroundObservation observation = describe(source, source.foregroundWindow());
        if (observation.window == 0) {
            return observation;
        }
        observation.url = source.urlOf(observation.window, observation.application, observation.title);
        if (source.foregroundWindow() == observation.window
            && source.titleOf(observation.window) == observation.title) {
            return observation;
        }
    }
    // Still switching or navigating after the retry: describe whatever is in
    // front now, without a url that may not belong to it.
    return describe(source, source.foregroundWindow());
}

DesktopForegroundSource::DesktopForegroundSource()
    : m_browser(std::make_unique<BrowserUrlReader>())
{
}

DesktopForegroundSource::~DesktopForegroundSource() = default;

QString DesktopForegroundSource::urlOf(quintptr window, const QString &executable,
                                       const QString &title)
{
    return m_browser->read(window, executable, title);
}

#ifdef Q_OS_WIN

quintptr DesktopForegroundSource::foregroundWindow()
{
    return reinterpret_cast<quintptr>(GetForegroundWindow());
}

QString DesktopForegroundSource::executableOf(quintptr window)
{
    DWORD processId = 0;
    GetWindowThreadProcessId(reinterpret_cast<HWND>(window), &processId);
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

QString DesktopForegroundSource::titleOf(quintptr window)
{
    // Full length: GetWindowTextW into a fixed 256-char buffer cut long
    // titles (the original keeps up to 440 characters of program_title).
    const HWND hwnd = reinterpret_cast<HWND>(window);
    const int length = GetWindowTextLengthW(hwnd);
    if (length <= 0) {
        return QString();
    }
    std::unique_ptr<wchar_t[]> buffer(new wchar_t[length + 1]());
    const int copied = GetWindowTextW(hwnd, buffer.get(), length + 1);
    return QString::fromWCharArray(buffer.get(), qMax(0, copied));
}

#else

quintptr DesktopForegroundSource::foregroundWindow()
{
    return 0;
}

QString DesktopForegroundSource::executableOf(quintptr)
{
    return QString();
}

QString DesktopForegroundSource::titleOf(quintptr)
{
    return QString();
}

#endif
