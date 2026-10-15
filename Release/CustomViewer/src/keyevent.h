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
    // The key's name as captured (the QML keyboardEvents map's "name", what
    // FUN_1402dcaf0 hands the Translator); empty in data recorded before
    // names were kept -- the evdev table's (FUN_1404e5660) stands in.
    QString name;
    QString translated;  // layout / Shift resolved text; empty for keys without one
    QString application; // the program in front (executable)
    QString site;        // the site of the web page in front (empty: none)
    QString title;       // the window title
};

// A text chunk recorded by the earlier keylogger, before key events were
// kept one by one: only the text as it was rendered then, the program and
// title, the moment the chunk was saved and -- for later chunks -- the
// moment its first key was typed. There are no key codes, no releases and
// no per-key times: nothing here is presented as individual events.
struct LegacyKeyChunk {
    qint64 beginMs = 0;
    bool beginKnown = false;
    qint64 endMs = 0;
    QString text;
    QString application;
    QString title;
};

// A listKeyEvents answer (the Selector's dataset): the events in time
// order, the older chunks before them, and whether the host could deliver
// the whole interval (S_loadedComplete) or only part of it
// (S_loadedPartial, with the reason).
struct KeyEventsReply {
    QList<KeyEvent> events;
    QList<LegacyKeyChunk> legacy;
    bool complete = true;
    QString partialReason;
};
