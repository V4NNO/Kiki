#include "browserurlprobe.h"

#include <QElapsedTimer>
#include <QSet>
#include <QTimer>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <psapi.h>
#include <uiautomation.h>
#include <uiautomationclient.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
#endif

namespace {
bool isKnownBrowserExe(const QString &exeName)
{
    static const QSet<QString> browsers = {
        QStringLiteral("chrome.exe"), QStringLiteral("msedge.exe"),
        QStringLiteral("firefox.exe"), QStringLiteral("brave.exe")
    };
    return browsers.contains(exeName.toLower());
}

#ifdef Q_OS_WIN
QString foregroundExeName()
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
    wchar_t path[MAX_PATH] = {0};
    DWORD size = MAX_PATH;
    QString name;
    if (QueryFullProcessImageNameW(process, 0, path, &size)) {
        const QString fullPath = QString::fromWCharArray(path, static_cast<int>(size));
        const int slash = fullPath.lastIndexOf(QLatin1Char('\\'));
        name = slash >= 0 ? fullPath.mid(slash + 1) : fullPath;
    }
    CloseHandle(process);
    return name;
}
#endif
} // namespace

#ifdef Q_OS_WIN

// Lives entirely in this .cpp (Qt's "worker object" pattern -- AUTOMOC picks
// up the Q_OBJECT below via the #include "browserurlprobe.moc" at the
// bottom) so BrowserUrlProbe's header doesn't need to expose any UI
// Automation/COM types.
class BrowserUrlProbe::Worker : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

    ~Worker() override
    {
        m_automation.Reset();
        if (m_comInitialized) {
            CoUninitialize();
        }
    }

public slots:
    void init()
    {
        // MTA, not STA: UIA client calls can block on a provider (e.g. a
        // browser tab building its accessibility tree for the first time),
        // and MTA means that block only ever affects this thread's own
        // queued work, never anyone else's.
        m_comInitialized = SUCCEEDED(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
        CoCreateInstance(CLSID_CUIAutomation, nullptr, CLSCTX_INPROC_SERVER,
                         IID_PPV_ARGS(&m_automation));

        auto *timer = new QTimer(this);
        timer->setInterval(2000);
        connect(timer, &QTimer::timeout, this, &Worker::poll);
        timer->start();
    }

signals:
    void urlChanged(const QString &url);
    void logMessage(const QString &message);

private slots:
    void poll()
    {
        QElapsedTimer stopwatch;
        stopwatch.start();
        const HWND fg = GetForegroundWindow();
        const QString exeName = foregroundExeName();
        const QString url = extractUrl(fg, exeName);
        const qint64 elapsedMs = stopwatch.elapsed();
        if (elapsedMs > 150) {
            emit logMessage(QStringLiteral("[browserurl] poll pentru %1 a durat %2 ms")
                                .arg(exeName.isEmpty() ? QStringLiteral("?") : exeName)
                                .arg(elapsedMs));
        }
        if (url != m_lastEmittedUrl) {
            m_lastEmittedUrl = url;
            emit urlChanged(url);
        }
    }

private:
    QString extractUrl(HWND fg, const QString &exeName)
    {
        if (!isKnownBrowserExe(exeName) || !m_automation || !fg) {
            m_lastHwnd = nullptr;
            return QString();
        }
        if (fg == m_lastHwnd) {
            return m_lastUrl; // unchanged since last poll, don't re-walk the a11y tree
        }
        m_lastHwnd = fg;
        m_lastUrl.clear();

        ComPtr<IUIAutomationElement> root;
        if (FAILED(m_automation->ElementFromHandle(fg, &root)) || !root) {
            return QString();
        }

        ComPtr<IUIAutomationElement> found = findByName(root, L"Address and search bar");
        if (!found) {
            found = findByAutomationId(root, L"urlbar-input"); // Firefox
        }
        if (!found) {
            return QString();
        }

        ComPtr<IUIAutomationValuePattern> valuePattern;
        if (FAILED(found->GetCurrentPatternAs(UIA_ValuePatternId, IID_PPV_ARGS(&valuePattern)))
            || !valuePattern) {
            return QString();
        }
        BSTR value = nullptr;
        if (FAILED(valuePattern->get_CurrentValue(&value)) || !value) {
            return QString();
        }
        m_lastUrl = QString::fromWCharArray(value);
        SysFreeString(value);
        return m_lastUrl;
    }

    ComPtr<IUIAutomationElement> findByName(const ComPtr<IUIAutomationElement> &root,
                                            const wchar_t *name)
    {
        VARIANT var;
        var.vt = VT_BSTR;
        var.bstrVal = SysAllocString(name);
        ComPtr<IUIAutomationCondition> condition;
        m_automation->CreatePropertyCondition(UIA_NamePropertyId, var, &condition);
        VariantClear(&var);
        if (!condition) {
            return nullptr;
        }
        ComPtr<IUIAutomationElement> found;
        root->FindFirst(TreeScope_Descendants, condition.Get(), &found);
        return found;
    }

    ComPtr<IUIAutomationElement> findByAutomationId(const ComPtr<IUIAutomationElement> &root,
                                                    const wchar_t *automationId)
    {
        VARIANT var;
        var.vt = VT_BSTR;
        var.bstrVal = SysAllocString(automationId);
        ComPtr<IUIAutomationCondition> condition;
        m_automation->CreatePropertyCondition(UIA_AutomationIdPropertyId, var, &condition);
        VariantClear(&var);
        if (!condition) {
            return nullptr;
        }
        ComPtr<IUIAutomationElement> found;
        root->FindFirst(TreeScope_Descendants, condition.Get(), &found);
        return found;
    }

    bool m_comInitialized = false;
    ComPtr<IUIAutomation> m_automation;
    HWND m_lastHwnd = nullptr;
    QString m_lastUrl;
    QString m_lastEmittedUrl;
};

#else

class BrowserUrlProbe::Worker : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
public slots:
    void init() {}
signals:
    void urlChanged(const QString &url);
    void logMessage(const QString &message);
};

#endif

BrowserUrlProbe::BrowserUrlProbe(QObject *parent)
    : QObject(parent)
{
    m_worker = new Worker();
    m_worker->moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, m_worker, &Worker::init);
    connect(&m_thread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &Worker::urlChanged, this, &BrowserUrlProbe::urlChanged);
    connect(m_worker, &Worker::logMessage, this, &BrowserUrlProbe::logMessage);
}

BrowserUrlProbe::~BrowserUrlProbe()
{
    stop();
}

void BrowserUrlProbe::start()
{
    if (!m_thread.isRunning()) {
        m_thread.start();
    }
}

void BrowserUrlProbe::stop()
{
    if (m_thread.isRunning()) {
        m_thread.quit();
        m_thread.wait();
    }
}

#include "browserurlprobe.moc"
