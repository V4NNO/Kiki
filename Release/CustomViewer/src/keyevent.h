#pragma once

#include <QList>
#include <QString>

// One keyboard event as the original's Keylogger engine holds it (the
// 0x30-byte dataset record / the QML keyboardEvents map): when, what
// happened, which key, and the context it was typed in.
struct KeyEvent {
    // The original's dataset types: 1 press, 2 auto-repeat, 3 release.
    enum Type { Press = 1, Repeat = 2, Release = 3 };
    qint64 moment = 0;   // ms since the epoch
    int type = Press;
    int code = 0;        // Linux evdev KEY_* code (the PC scan code); 0 unknown
    QString name;        // the key's name; empty: the evdev table's (FUN_1404e5660)
    QString translated;  // layout / Shift resolved text; empty for keys without one
    QString application; // the program in front (executable)
    QString site;        // the site of the web page in front (empty: none)
    QString title;       // the window title
};
