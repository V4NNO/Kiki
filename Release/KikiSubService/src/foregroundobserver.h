#pragma once

#include <QString>
#include <QtGlobal>

#include <memory>

class BrowserUrlReader;

// One reading of the foreground window: the executable, the window title and
// (for a recognized browser) the address-bar url, all taken from the SAME
// window in the same poll. The original keeps them together the same way:
// node.exe's online_session_program row is one (process, window_lid), and
// program_title / program_url are both children of that row with their own
// life spans (report.select_live_program pairs a url with a title only where
// both ranges overlap on that program). Mixing an executable read at one
// moment with a title read at another is how OUTLOOK.EXE ended up carrying
// Ghidra's "CodeBrowser: ..." title.
struct ForegroundObservation {
    quintptr window = 0;
    QString application; // executable name; "desktop" when nothing is in front
    QString title;
    QString url;
};

// The platform calls an observation is made of. Behind an interface so the
// consistency rule in observeForeground() can be exercised deterministically
// (window switches landing between two calls) without a real desktop.
class ForegroundSource
{
public:
    virtual ~ForegroundSource() = default;
    virtual quintptr foregroundWindow() = 0;
    virtual QString executableOf(quintptr window) = 0;
    virtual QString titleOf(quintptr window) = 0;
    // May block (UI Automation walks the browser's accessibility tree).
    // Empty when the window isn't a recognized browser or no url was read.
    virtual QString urlOf(quintptr window, const QString &executable, const QString &title) = 0;
};

// The real desktop: Win32 for the window, its process image and title, UI
// Automation (BrowserUrlReader) for a browser's url. Every call takes the
// window explicitly, so all of them describe one HWND. COM must already be
// initialized on the thread that constructs and uses it. Off Windows it
// reports no foreground window.
class DesktopForegroundSource final : public ForegroundSource
{
public:
    DesktopForegroundSource();
    ~DesktopForegroundSource() override;

    quintptr foregroundWindow() override;
    QString executableOf(quintptr window) override;
    QString titleOf(quintptr window) override;
    QString urlOf(quintptr window, const QString &executable, const QString &title) override;

private:
    std::unique_ptr<BrowserUrlReader> m_browser;
};

// Reads executable, title and url of the current foreground window and only
// returns them together if the window was still in front, with the same
// title, after the (possibly slow) url read -- otherwise the url could belong
// to a page or window that is no longer the one described. Retries once; if
// the foreground is still moving, reports the window without a url rather
// than a mixed pair.
ForegroundObservation observeForeground(ForegroundSource &source);
