#include "browserurlreader.h"

#include <QSet>
#include <QThread>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <uiautomation.h>
#include <uiautomationclient.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;
#endif

bool BrowserUrlReader::isKnownBrowser(const QString &executable)
{
    static const QSet<QString> browsers = {
        QStringLiteral("chrome.exe"), QStringLiteral("msedge.exe"),
        QStringLiteral("firefox.exe"), QStringLiteral("brave.exe")
    };
    return browsers.contains(executable.toLower());
}

#ifdef Q_OS_WIN

struct BrowserUrlReader::Private {
    ComPtr<IUIAutomation> automation;
    // The window the cached element and url belong to; everything below is
    // dropped as soon as a different window is asked about.
    quintptr window = 0;
    ComPtr<IUIAutomationElement> addressBar;
    // The last url reported for this window and the title it was reported
    // with; pendingUrl is a newer address-bar value not yet confirmed (see
    // read()).
    QString lastUrl;
    QString lastTitle;
    QString pendingUrl;

    void forgetWindow(quintptr newWindow)
    {
        window = newWindow;
        addressBar.Reset();
        lastUrl.clear();
        lastTitle.clear();
        pendingUrl.clear();
    }

    ComPtr<IUIAutomationElement> findFirst(const ComPtr<IUIAutomationElement> &root,
                                           PROPERTYID property, const wchar_t *value)
    {
        VARIANT var;
        var.vt = VT_BSTR;
        var.bstrVal = SysAllocString(value);
        ComPtr<IUIAutomationCondition> condition;
        automation->CreatePropertyCondition(property, var, &condition);
        VariantClear(&var);
        if (!condition) {
            return nullptr;
        }
        ComPtr<IUIAutomationElement> found;
        root->FindFirst(TreeScope_Descendants, condition.Get(), &found);
        return found;
    }

    ComPtr<IUIAutomationElement> findAddressBar(quintptr hwnd)
    {
        ComPtr<IUIAutomationElement> root;
        if (FAILED(automation->ElementFromHandle(reinterpret_cast<HWND>(hwnd), &root)) || !root) {
            return nullptr;
        }
        ComPtr<IUIAutomationElement> found =
            findFirst(root, UIA_NamePropertyId, L"Address and search bar"); // Chrome, Edge, Brave
        if (!found) {
            found = findFirst(root, UIA_AutomationIdPropertyId, L"urlbar-input"); // Firefox
        }
        return found;
    }

    // The address bar's current text, and whether it has keyboard focus. A
    // kept element that stopped answering (the browser rebuilt its UI) is
    // looked up again once.
    bool readAddressBar(QString *value, bool *focused)
    {
        for (int pass = 0; pass < 2; ++pass) {
            if (!addressBar) {
                addressBar = findAddressBar(window);
                if (!addressBar) {
                    return false;
                }
            }
            ComPtr<IUIAutomationValuePattern> valuePattern;
            BSTR text = nullptr;
            if (SUCCEEDED(addressBar->GetCurrentPatternAs(UIA_ValuePatternId, IID_PPV_ARGS(&valuePattern)))
                && valuePattern && SUCCEEDED(valuePattern->get_CurrentValue(&text)) && text) {
                *value = QString::fromWCharArray(text);
                SysFreeString(text);
                BOOL hasFocus = FALSE;
                addressBar->get_CurrentHasKeyboardFocus(&hasFocus);
                *focused = hasFocus != FALSE;
                return true;
            }
            addressBar.Reset();
        }
        return false;
    }
};

BrowserUrlReader::BrowserUrlReader()
    : d(std::make_unique<Private>())
{
    CoCreateInstance(CLSID_CUIAutomation, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&d->automation));
    // Bound how long one hung browser can hold up the observation (and with
    // it the idle/lock sampling that shares ActivityProbe's thread).
    ComPtr<IUIAutomation2> automation2;
    if (d->automation && SUCCEEDED(d->automation.As(&automation2)) && automation2) {
        automation2->put_TransactionTimeout(1500);
        automation2->put_ConnectionTimeout(1500);
    }
}

BrowserUrlReader::~BrowserUrlReader() = default;

QString BrowserUrlReader::read(quintptr window, const QString &executable, const QString &title)
{
    if (!isKnownBrowser(executable) || !d->automation || window == 0) {
        d->forgetWindow(0);
        return QString();
    }
    if (window != d->window) {
        d->forgetWindow(window);
    }
    QString value;
    bool focused = false;
    if (!d->readAddressBar(&value, &focused)) {
        d->forgetWindow(window);
        return QString();
    }
    if (focused) {
        // The text may be a half-typed query, not the page's url. Typing does
        // not change the window title; a committed navigation eventually does.
        return title == d->lastTitle ? d->lastUrl : QString();
    }
    // Chromium updates the address bar on commit and the window title only
    // once the new document sets it: measured on Edge, 0-200 ms during which
    // the OLD title sits next to the NEW url. Two guards keep such a pair out:
    if (!d->lastUrl.isEmpty() && title == d->lastTitle) {
        // Same title as the pair reported last. A different url under it is
        // taken only when seen a second time -- by then a real navigation has
        // changed the title too, while a page that keeps its title is
        // accepted one poll late. Until then the previous pair stands.
        if (value != d->lastUrl && value != d->pendingUrl) {
            d->pendingUrl = value;
            return d->lastUrl;
        }
    } else if (value != d->lastUrl) {
        // A new pair: the window seen for the first time (e.g. back in front
        // after another app, having navigated meanwhile), or url and title
        // both moved. The address bar may already be a page ahead of the
        // title, so let it settle: read it again after kSettleMs. The caller
        // (observeForeground) re-reads the title after this returns, so the
        // pair is only kept if neither part changed over that span.
        constexpr unsigned long kSettleMs = 350;
        QThread::msleep(kSettleMs);
        QString settled;
        if (!d->readAddressBar(&settled, &focused) || settled != value || focused) {
            // Still moving; the next poll decides (the element is kept).
            d->lastUrl.clear();
            d->lastTitle.clear();
            d->pendingUrl.clear();
            return QString();
        }
    }
    d->pendingUrl.clear();
    d->lastUrl = value;
    d->lastTitle = title;
    return value;
}

#else

struct BrowserUrlReader::Private {
};

BrowserUrlReader::BrowserUrlReader()
    : d(std::make_unique<Private>())
{
}

BrowserUrlReader::~BrowserUrlReader() = default;

QString BrowserUrlReader::read(quintptr, const QString &, const QString &)
{
    return QString();
}

#endif
