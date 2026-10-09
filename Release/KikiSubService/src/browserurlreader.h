#pragma once

#include <QString>
#include <QtGlobal>

#include <memory>

// Reads a browser window's address bar via UI Automation. Synchronous: it is
// called from ActivityProbe's own thread as part of one foreground
// observation (see foregroundobserver.h), so the url always comes from the
// same window, and the same moment, as the executable and title it is sent
// with. FindFirst() over a browser's UI Automation tree can block for a long
// time -- Chrome only builds its full accessibility tree lazily, the first
// time something queries it -- which is why this must never run on the
// process's main thread, where Keylogger's low-level input hooks live (see
// subservicehost.h's class comment).
//
// The address-bar element is found once per window and kept, but its value is
// read again on every call: navigating or switching tabs keeps the same HWND
// while the url changes, so caching the url per HWND (as the old
// BrowserUrlProbe did) froze it at whatever page was open when the window
// came to front.
class BrowserUrlReader
{
public:
    // COM must already be initialized on the calling thread.
    BrowserUrlReader();
    ~BrowserUrlReader();

    static bool isKnownBrowser(const QString &executable);

    // Empty when the window isn't a recognized browser, or no url could be
    // read. title is the window title of the same observation, and the url
    // returned is one that belongs with it: while the address bar has
    // keyboard focus its text may be what the user is typing, so the url
    // read earlier from this window is kept only as long as the title shows
    // the same page. The browser updates the address bar before the title,
    // so a new url under an unchanged title is confirmed on the next read
    // before it replaces the previous one, and a new (url, title) pair is
    // only returned once the address bar has stayed put for a short settle
    // delay (the caller re-checks the title over the same span).
    QString read(quintptr window, const QString &executable, const QString &title);

private:
    struct Private;
    std::unique_ptr<Private> d;
};
