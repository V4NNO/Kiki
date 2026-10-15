#pragma once

#include "keyevent.h"
#include "keyloggerchart.h"
#include "keyloggertable.h"

#include <QList>
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

// Keylogger.Result's settings.
struct ResultOptions {
    bool cutSystemKeys = true;
    QString keywordFilter;
    bool showSites = true;
    bool showExecutables = true;
    // History by application (appFilter): only that program's records.
    QString appFilter;
};

// FUN_1402d9490 + FUN_1402daab0: the events grouped into records -- a new
// record whenever the application (site or program), the title or the
// local day changes; a record runs from its first event to the first event
// of the next one (the last to intervalEndMs), split at local midnight
// (23:59:59.999 / 00:00:00.000). Records failing the keyword filter (any
// whitespace-separated word, case-insensitive, in the application, the
// title or the raw keystream) are dropped, the keystream is translated and
// trimmed, and records left empty are dropped.
QList<KeyloggerRow> buildResult(const QList<KeyEvent> &events, qint64 intervalEndMs,
                                const ResultOptions &options);

// FUN_1402d0ee0: the intensity chart over the last lengthSec seconds before
// intervalEndMs, in granulaSec buckets aligned to the epoch -- each bucket
// the time keys were held down in it (press to release, split across
// buckets), as a share of the bucket.
struct ChartData {
    QList<KeyloggerChartPoint> content;
    double maxV = 0;
    qint64 granulaMs = 0;
};
ChartData buildChart(const QList<KeyEvent> &events, qint64 intervalEndMs, int lengthSec, int granulaSec);

// Result.exportToCsv / exportToXlsx (FUN_1402d79d0 / FUN_1402d84c0):
// "success", or the error text.
QString exportToCsv(const QList<KeyloggerRow> &rows, const QString &fileName);
QString exportToXlsx(const QList<KeyloggerRow> &rows, const QString &fileName);

} // namespace Keylogger
