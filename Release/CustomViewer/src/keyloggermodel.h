#pragma once

#include "keyevent.h"
#include "keyloggerchart.h"
#include "keyloggertable.h"

#include <QList>
#include <QPair>
#include <QString>
#include <QStringList>

namespace Keylogger {

// FUN_1404e5660: evdev KEY_* code -> name ("UNKNOWN" for anything else).
QString keyName(int code);

// Keylogger.Translator (FUN_1402e5fd0 / FUN_1402e5bd0 / FUN_1402e5f00):
// the events rendered as one string. cutSystemKeys drops keys without text
// and every combination with Alt, Ctrl or Meta (Shift alone is kept).
// The native path (records, exports, FUN_1402da990) names the keys from the
// evdev table and passes its flag as both cutSystemKeys and
// emulateBackspace (Backspace then deletes the previous character); the
// QML path (Keystream, FUN_1402dcaf0) passes neither and uses each event's
// own name.
QString translate(const QList<KeyEvent> &events, bool cutSystemKeys, bool emulateBackspace = false,
                  bool nameFromTable = false);

// The keystream of a History period up to (past) and after (future) the
// moment: the earlier keylogger's chunks as their saved text at their save
// moment, then the events -- each half its own Translator (QML path).
QPair<QString, QString> splitKeystream(const QList<KeyEvent> &events, const QList<LegacyKeyChunk> &legacy,
                                       qint64 moment);

// Keylogger.Result's settings (cutSystemKeys at +0x20, appFilter +0x30,
// showSites / showExecutables / showBinForSites +0x40..+0x42).
struct ResultOptions {
    bool cutSystemKeys = true;
    QString keywordFilter;
    bool showSites = true;
    bool showExecutables = true;
    // History's KeyloggerExporter: showBinForSites = !(appFilter is a web
    // page); a page is then "program/site" -- grouped by both, labelled by
    // the site, filtered by the program.
    bool showBinForSites = false;
    // HistoryTab.appFilterName: only the records of that application
    // (compared with the program, or with the site when showBinForSites is
    // off and the filter is a web page).
    QString appFilter;
};

// FUN_1402d9490 + FUN_1402daab0: the events grouped into records.
//  - Each event is classified (the ApplicationType lambdas): showBinForSites
//    -> a page "program/site" (2) else the program (1); otherwise sites and
//    programs per showSites / showExecutables, an event of a kind not shown
//    being left out entirely.
//  - A new record starts when the type, the application, the title ("No
//    title" when empty), the site part or the local day changes; it runs
//    from its first event to the first event of the next record, the last
//    one to the last event of the whole dataset; across midnight the old
//    one ends at 23:59:59.999 and the new one starts at 00:00:00.000.
//  - A record is kept when it matches appFilter and any keyword (the
//    filter split at whitespace, case-insensitive) is in its application,
//    title or typed text, and its translated keystream (the native path) is
//    not blank -- the keystream itself is kept as translated, untrimmed.
// The earlier keylogger's chunks become records of their own (estimated),
// before the event records, under the same filters.
QList<KeyloggerRow> buildResult(const QList<KeyEvent> &events, const QList<LegacyKeyChunk> &legacy,
                                const ResultOptions &options);

// FUN_1402d0ee0: the intensity chart. The window ends at the dataset's last
// event (FUN_1402e4570), starts lengthSec before it, floored to a multiple
// of the step since the epoch, and holds ceilf((length + remainder) / step)
// buckets. Only events inside the window count: a press (type 1; auto-
// repeats are ignored) stores its moment per key code, overwriting an
// earlier one; a release takes it and adds the held time to every bucket
// it spans (bucket indexes in float, as the original computes them). A
// bucket's value is its held time / the step (DAT_140bf19c0 = 1.0). No
// events: no chart. The earlier keylogger's chunks have no presses or
// releases and contribute nothing.
struct ChartData {
    QList<KeyloggerChartPoint> content;
    double maxV = 0;
    qint64 granulaMs = 0;
};
ChartData buildChart(const QList<KeyEvent> &events, int lengthSec, int granulaSec);

// Result.exportToCsv / exportToXlsx (FUN_1402d79d0 / FUN_1402d84c0):
// "success", or the error text.
QString exportToCsv(const QList<KeyloggerRow> &rows, const QString &fileName);
QString exportToXlsx(const QList<KeyloggerRow> &rows, const QString &fileName);

} // namespace Keylogger
