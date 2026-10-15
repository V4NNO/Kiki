#include "keyloggermodel.h"

#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QHash>
#include <QMap>
#include <QRegularExpression>

#include <array>
#include <cmath>

namespace Keylogger {

QString keyName(int code)
{
    // FUN_1404e5660, complete.
    static const QHash<int, const char *> kNames = [] {
        QHash<int, const char *> names;
        const std::pair<int, const char *> table[] = {
            {1, "ESC"}, {2, "1"}, {3, "2"}, {4, "3"}, {5, "4"}, {6, "5"}, {7, "6"}, {8, "7"}, {9, "8"},
            {10, "9"}, {0x0b, "0"}, {0x0c, "MINUS"}, {0x0d, "EQUAL"}, {0x0e, "BACKSPACE"}, {0x0f, "TAB"},
            {0x10, "Q"}, {0x11, "W"}, {0x12, "E"}, {0x13, "R"}, {0x14, "T"}, {0x15, "Y"}, {0x16, "U"},
            {0x17, "I"}, {0x18, "O"}, {0x19, "P"}, {0x1a, "LEFTBRACE"}, {0x1b, "RIGHTBRACE"},
            {0x1c, "ENTER"}, {0x1d, "LEFTCTRL"}, {0x1e, "A"}, {0x1f, "S"}, {0x20, "D"}, {0x21, "F"},
            {0x22, "G"}, {0x23, "H"}, {0x24, "J"}, {0x25, "K"}, {0x26, "L"}, {0x27, "SEMICOLON"},
            {0x28, "APOSTROPHE"}, {0x29, "GRAVE"}, {0x2a, "LEFTSHIFT"}, {0x2b, "BACKSLASH"}, {0x2c, "Z"},
            {0x2d, "X"}, {0x2e, "C"}, {0x2f, "V"}, {0x30, "B"}, {0x31, "N"}, {0x32, "M"}, {0x33, "COMMA"},
            {0x34, "DOT"}, {0x35, "SLASH"}, {0x36, "RIGHTSHIFT"}, {0x37, "KPASTERISK"}, {0x38, "LEFTALT"},
            {0x39, "SPACE"}, {0x3a, "CAPSLOCK"}, {0x3b, "F1"}, {0x3c, "F2"}, {0x3d, "F3"}, {0x3e, "F4"},
            {0x3f, "F5"}, {0x40, "F6"}, {0x41, "F7"}, {0x42, "F8"}, {0x43, "F9"}, {0x44, "F10"},
            {0x45, "NUMLOCK"}, {0x46, "SCROLLLOCK"}, {0x47, "KP7"}, {0x48, "KP8"}, {0x49, "KP9"},
            {0x4a, "KPMINUS"}, {0x4b, "KP4"}, {0x4c, "KP5"}, {0x4d, "KP6"}, {0x4e, "KPPLUS"}, {0x4f, "KP1"},
            {0x50, "KP2"}, {0x51, "KP3"}, {0x52, "KP0"}, {0x53, "KPDOT"}, {0x57, "F11"}, {0x58, "F12"},
            {0x5a, "KATAKANA"}, {0x5b, "HIRAGANA"}, {0x60, "KPENTER"}, {0x61, "RIGHTCTRL"},
            {0x62, "KPSLASH"}, {0x63, "SYSRQ"}, {0x64, "RIGHTALT"}, {0x66, "HOME"}, {0x67, "UP"},
            {0x68, "PAGEUP"}, {0x69, "LEFT"}, {0x6a, "RIGHT"}, {0x6b, "END"}, {0x6c, "DOWN"},
            {0x6d, "PAGEDOWN"}, {0x6e, "INSERT"}, {0x6f, "DELETE"}, {0x71, "MUTE"}, {0x72, "VOLUMEDOWN"},
            {0x73, "VOLUMEUP"}, {0x77, "PAUSE"}, {0x7d, "LEFTMETA"}, {0x7e, "RIGHTMETA"}, {0x7f, "COMPOSE"},
            {0x8a, "HELP"}, {0x8e, "SLEEP"}, {0xa3, "NEXTSONG"}, {0xa4, "PLAYPAUSE"}, {0xa5, "PREVIOUSSONG"},
            {0xa6, "STOPCD"}, {0xb7, "F13"}, {0xb8, "F14"}, {0xb9, "F15"}, {0xba, "F16"}, {0xbb, "F17"},
            {0xbc, "F18"}, {0xbd, "F19"}, {0xbe, "F20"}, {0xbf, "F21"}, {0xc0, "F22"}, {0xc1, "F23"},
            {0xc2, "F24"}, {0xd2, "PRINT"},
        };
        for (const auto &entry : table) {
            names.insert(entry.first, entry.second);
        }
        return names;
    }();
    return QString::fromLatin1(kNames.value(code, "UNKNOWN"));
}

namespace {
// FUN_1402e5b70's state: ALT, CTRL, META, SHIFT held; cutSystemKeys.
struct TranslatorState {
    bool alt = false;
    bool ctrl = false;
    bool meta = false;
    bool shift = false;
    bool cut = false;
    bool emulateBackspace = false;
    QString output;
};

// FUN_1402e5f00: the held modifiers, always in this order.
QString modifierPrefix(const TranslatorState &state)
{
    QString prefix;
    if (state.alt) {
        prefix += QStringLiteral("ALT + ");
    }
    if (state.ctrl) {
        prefix += QStringLiteral("CTRL + ");
    }
    if (state.meta) {
        prefix += QStringLiteral("META + ");
    }
    if (state.shift) {
        prefix += QStringLiteral("SHIFT + ");
    }
    return prefix;
}

// FUN_1402e5bd0: one key press as text.
QString token(const TranslatorState &state, const QString &translated, const QString &name, int code)
{
    const bool anyModifier = state.alt || state.ctrl || state.meta || state.shift;
    const bool onlyShift = state.shift && !state.alt && !state.ctrl && !state.meta;
    const bool hasText = !translated.isEmpty();
    bool bracket;
    if (state.cut) {
        // Hide system keys: no text, or a combination with Alt/Ctrl/Meta.
        if (!hasText || (anyModifier && !onlyShift)) {
            return QString();
        }
        bracket = anyModifier;
    } else {
        bracket = anyModifier || !hasText;
    }
    QString keyText;
    QString body;
    if (code == 0x0e) { // BACKSPACE
        keyText = QString(QChar(0x2190));
        bracket = anyModifier;
    } else if (code == 0x1c || code == 0x60) { // ENTER, KPENTER
        keyText = QString(QChar(0x23ce));
        bracket = anyModifier;
    } else if (code == 0x39) { // SPACE
        if (anyModifier) {
            bracket = true;
            keyText = name;
        } else {
            keyText = QStringLiteral(" ");
        }
    } else {
        keyText = anyModifier || !hasText ? name : translated;
    }
    if (onlyShift) {
        if (code == 0x60 || code == 0x1c) {
            // Shift + Enter: the glyph alone, in brackets.
            bracket = true;
            body = keyText;
        } else if (hasText) {
            // Shift alone with text: the text as typed, unbracketed.
            bracket = false;
            body = translated;
        }
    }
    if (body.isEmpty()) {
        body = modifierPrefix(state) + keyText;
    }
    return bracket ? QLatin1Char('<') + body + QLatin1Char('>') : body;
}

// FUN_1402e5fd0: one event.
void feed(TranslatorState &state, const QString &translated, const QString &name, int code, int type)
{
    const bool down = type == KeyEvent::Press || type == KeyEvent::Repeat;
    switch (code) {
    case 0x1d:
    case 0x61:
        state.ctrl = down;
        return;
    case 0x2a:
    case 0x36:
        state.shift = down;
        return;
    case 0x38:
    case 0x64:
        state.alt = down;
        return;
    case 0x7d:
    case 0x7e:
        state.meta = down;
        return;
    default:
        break;
    }
    if (!down) {
        return;
    }
    if (state.emulateBackspace && code == 0x0e) {
        state.output.chop(1);
        return;
    }
    const QString text = token(state, translated, name, code);
    if (!text.isEmpty()) {
        state.output += text;
    }
}

QDate localDate(qint64 ms)
{
    return QDateTime::fromMSecsSinceEpoch(ms).date();
}
} // namespace

QString translate(const QList<KeyEvent> &events, bool cutSystemKeys, bool emulateBackspace, bool nameFromTable)
{
    TranslatorState state;
    state.cut = cutSystemKeys;
    state.emulateBackspace = emulateBackspace;
    for (const KeyEvent &event : events) {
        feed(state, event.translated, nameFromTable || event.name.isEmpty() ? keyName(event.code) : event.name,
             event.code, event.type);
    }
    return state.output;
}

QPair<QString, QString> splitKeystream(const QList<KeyEvent> &events, const QList<LegacyKeyChunk> &legacy,
                                       qint64 moment)
{
    // History.qml: v.moment <= moment in white, the rest in gray. A chunk
    // of the earlier keylogger is only known to be typed by its save moment.
    QString pastLegacy;
    QString futureLegacy;
    for (const LegacyKeyChunk &chunk : legacy) {
        (moment >= 0 && chunk.endMs <= moment ? pastLegacy : futureLegacy) += chunk.text;
    }
    QList<KeyEvent> past;
    QList<KeyEvent> future;
    for (const KeyEvent &event : events) {
        (moment >= 0 && event.moment <= moment ? past : future).append(event);
    }
    return {pastLegacy + translate(past, false, false, false), futureLegacy + translate(future, false, false, false)};
}

namespace {
// The ApplicationType lambdas FUN_1402d9490 picks from its flags:
// FUN_1402d5910 (showBinForSites), FUN_1402d5ce0 (sites and programs),
// FUN_1402d5ae0 (sites), FUN_1402d5be0 (programs), FUN_1402d5a70 (none).
// Type 0: the event is not part of the result.
QPair<int, QString> classify(const KeyEvent &event, const ResultOptions &options)
{
    const bool page = !event.site.isEmpty();
    if (options.showBinForSites) {
        return page ? qMakePair(2, event.application + QLatin1Char('/') + event.site) : qMakePair(1, event.application);
    }
    if (options.showSites && options.showExecutables) {
        return page ? qMakePair(2, event.site) : qMakePair(1, event.application);
    }
    if (options.showSites) {
        return page ? qMakePair(2, event.site) : qMakePair(0, QString());
    }
    if (options.showExecutables) {
        return page ? qMakePair(0, QString()) : qMakePair(1, event.application);
    }
    return qMakePair(0, QString());
}
} // namespace

QList<KeyloggerRow> buildResult(const QList<KeyEvent> &events, const QList<LegacyKeyChunk> &legacy,
                                const ResultOptions &options)
{
    // FUN_1402da600: the filter split at whitespace.
    QStringList keywords = options.keywordFilter.split(QRegularExpression(QStringLiteral("\\s")));
    keywords.removeAll(QString());
    // FUN_1402d9030: any keyword, case-insensitive, as a substring.
    const auto matches = [&keywords](const QString &field) {
        if (keywords.isEmpty()) {
            return true;
        }
        if (field.isEmpty()) {
            return false;
        }
        for (const QString &keyword : std::as_const(keywords)) {
            if (field.contains(keyword, Qt::CaseInsensitive)) {
                return true;
            }
        }
        return false;
    };
    // The record's filter: appFilter against the application part (the
    // program under showBinForSites), then a keyword in it, the title or
    // the typed text.
    const auto accepted = [&](const QString &application, const QString &title, const QString &raw) {
        return (options.appFilter.isEmpty() || options.appFilter == application)
               && (matches(application) || matches(title) || matches(raw));
    };

    QList<KeyloggerRow> rows;

    // The earlier keylogger's chunks: records of their own, in front (they
    // all precede the first recorded event). Programs only -- no site was
    // kept with them.
    for (const LegacyKeyChunk &chunk : legacy) {
        if (!options.showBinForSites && !options.showExecutables) {
            break;
        }
        const QString title = chunk.title.isEmpty() ? QStringLiteral("No title") : chunk.title;
        if (!accepted(chunk.application, title, chunk.text) || chunk.text.trimmed().isEmpty()) {
            continue;
        }
        const qint64 from = chunk.beginKnown ? chunk.beginMs : chunk.endMs;
        KeyloggerRow row;
        row.date = localDate(from);
        row.fromMs = from;
        row.toMs = chunk.endMs;
        row.application = chunk.application;
        row.title = title;
        row.keystream = chunk.text;
        row.estimated = true;
        rows.append(row);
    }

    struct Group {
        int type = 0;         // ApplicationType of the record (0: none open)
        QString application;  // local_320: the program, the site, or under
                              // showBinForSites the program part
        QString site;         // local_2f0: the site part (showBinForSites)
        QString title;        // local_308
        QDate date;           // local_2e0
        qint64 from = 0;      // local_2f8
        QList<KeyEvent> events;
        QString raw;          // local_2e8: the text of presses / repeats
    };
    const auto emitGroup = [&](const Group &group, qint64 to) {
        if (group.type == 0 || group.events.isEmpty()) {
            return;
        }
        if (!accepted(group.application, group.title, group.raw)) {
            return;
        }
        // FUN_1402daab0: the native Translator over the record's events; a
        // blank result is no row, anything else is kept exactly as it is.
        const QString keystream = translate(group.events, options.cutSystemKeys, options.cutSystemKeys, true);
        if (keystream.trimmed().isEmpty()) {
            return;
        }
        KeyloggerRow row;
        row.date = group.date;
        row.fromMs = group.from;
        row.toMs = to;
        row.application = group.site.isEmpty() ? group.application : group.site;
        row.isSite = group.type == 2;
        row.title = group.title;
        row.keystream = keystream;
        rows.append(row);
    };

    Group group;
    for (const KeyEvent &event : events) {
        const QPair<int, QString> kind = classify(event, options);
        QString application = kind.second;
        QString site;
        if (options.showBinForSites) {
            const int slash = application.indexOf(QLatin1Char('/'));
            if (slash >= 0) {
                site = application.mid(slash + 1);
                application = application.left(slash);
            }
        }
        if (kind.first == 0) {
            continue;
        }
        const bool dayChanged = group.type != 0 && group.date != localDate(event.moment);
        const QString title = event.title.isEmpty() ? QStringLiteral("No title") : event.title;
        if (group.type == 0 || group.type != kind.first || group.application != application || group.title != title
            || group.site != site || dayChanged) {
            // The record so far ends where this one begins -- across
            // midnight at 23:59:59.999 (set on this event's day, as the
            // original does; only the time of day is ever shown).
            qint64 to = event.moment;
            if (dayChanged) {
                to = QDateTime(localDate(event.moment), QTime(23, 59, 59, 999)).toMSecsSinceEpoch();
            }
            emitGroup(group, to);
            Group next;
            next.type = kind.first;
            next.application = application;
            next.site = site;
            next.title = title;
            next.from = event.moment;
            if (dayChanged) {
                next.from = QDateTime(localDate(event.moment), QTime(0, 0, 0, 0)).toMSecsSinceEpoch();
            }
            next.date = localDate(next.from);
            group = next;
        }
        group.events.append(event);
        if (!event.translated.isEmpty() && (event.type == KeyEvent::Press || event.type == KeyEvent::Repeat)) {
            group.raw += event.translated;
        }
    }
    // The last record ends at the dataset's last event (FUN_1402e4570).
    if (!events.isEmpty()) {
        emitGroup(group, events.last().moment);
    }
    return rows;
}

ChartData buildChart(const QList<KeyEvent> &events, int lengthSec, int granulaSec)
{
    ChartData chart;
    chart.granulaMs = qint64(granulaSec) * 1000;
    if (events.isEmpty() || lengthSec < 1 || granulaSec < 1) {
        return chart;
    }
    const int step = granulaSec * 1000;
    const int length = lengthSec * 1000;
    const qint64 end = events.last().moment;
    qint64 start = end - length;
    const qint64 remainder = start % step;
    start -= remainder;
    const int count = int(std::ceil(float(length + int(remainder)) / float(step)));
    if (count <= 0) {
        return chart;
    }
    QList<double> held(count, 0.0);
    QMap<int, qint64> pressed; // key code -> its last press
    for (const KeyEvent &event : events) {
        if (event.moment < start) {
            continue;
        }
        if (event.type == KeyEvent::Press) {
            pressed[event.code] = event.moment;
            continue;
        }
        if (event.type != KeyEvent::Release) {
            continue;
        }
        const auto it = pressed.find(event.code);
        if (it == pressed.end()) {
            continue;
        }
        const qint64 down = it.value();
        pressed.erase(it);
        const qint64 up = event.moment;
        const int first = int(std::floor(float(down - start) / float(step)));
        const int last = int(std::floor(float(up - start) / float(step)));
        for (int k = first; k <= last; ++k) {
            const qint64 bucketStart = start + qint64(k) * step;
            const qint64 bucketEnd = bucketStart + step;
            const qint64 from = down <= bucketStart ? bucketStart : down;
            const qint64 to = up <= bucketEnd ? up : bucketEnd;
            // The original indexes its QVector unchecked here (an assert);
            // a span reaching past the last bucket is dropped instead.
            if (k < 0 || k >= count) {
                continue;
            }
            held[k] += double(to - from);
        }
    }
    const double scale = 1.0 / double(step);
    for (int k = 0; k < count; ++k) {
        const double value = held.at(k) * scale;
        chart.content.append({start + qint64(k) * step, value});
        chart.maxV = qMax(chart.maxV, value);
    }
    return chart;
}

namespace {
// --- Minimal .xlsx writer (KeyloggerExporter.qml's XLSX option) ---
// A .xlsx is a ZIP of OOXML parts; Qt has no public zip-writing API, so
// this hand-rolls just enough of the ZIP format (stored/uncompressed
// entries only -- no need for real compression for a small keylog export)
// plus the handful of XML parts Excel actually requires to open a
// single-sheet workbook (inline strings, no shared-strings table/styles).

quint32 crc32Of(const QByteArray &data)
{
    static const auto table = [] {
        std::array<quint32, 256> t{};
        for (quint32 i = 0; i < 256; ++i) {
            quint32 c = i;
            for (int k = 0; k < 8; ++k) {
                c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            }
            t[i] = c;
        }
        return t;
    }();
    quint32 crc = 0xFFFFFFFFu;
    for (unsigned char byte : data) {
        crc = table[(crc ^ byte) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFu;
}

void appendLE16(QByteArray &out, quint16 value) { out.append(char(value & 0xFF)); out.append(char((value >> 8) & 0xFF)); }
void appendLE32(QByteArray &out, quint32 value)
{
    out.append(char(value & 0xFF));
    out.append(char((value >> 8) & 0xFF));
    out.append(char((value >> 16) & 0xFF));
    out.append(char((value >> 24) & 0xFF));
}

// Appends one stored (uncompressed) entry's local header + data, and
// returns the central-directory record to append later (its "relative
// offset of local header" needs the offset the entry was written at).
QByteArray zipCentralRecordFor(const QByteArray &nameUtf8, quint32 crc, quint32 size, quint32 offset)
{
    QByteArray record;
    appendLE32(record, 0x02014b50);
    appendLE16(record, 20); // version made by
    appendLE16(record, 20); // version needed
    appendLE16(record, 0);  // flags
    appendLE16(record, 0);  // compression: stored
    appendLE16(record, 0);  // mod time
    appendLE16(record, 0x21); // mod date (a valid, arbitrary date)
    appendLE32(record, crc);
    appendLE32(record, size); // compressed size == uncompressed size (stored)
    appendLE32(record, size);
    appendLE16(record, quint16(nameUtf8.size()));
    appendLE16(record, 0); // extra length
    appendLE16(record, 0); // comment length
    appendLE16(record, 0); // disk number start
    appendLE16(record, 0); // internal attrs
    appendLE32(record, 0); // external attrs
    appendLE32(record, offset);
    record.append(nameUtf8);
    return record;
}

void zipAppendEntry(QByteArray &zip, QList<QByteArray> &centralRecords, const QByteArray &name,
                    const QByteArray &data)
{
    const quint32 offset = quint32(zip.size());
    const quint32 crc = crc32Of(data);
    appendLE32(zip, 0x04034b50);
    appendLE16(zip, 20); // version needed
    appendLE16(zip, 0);  // flags
    appendLE16(zip, 0);  // compression: stored
    appendLE16(zip, 0);  // mod time
    appendLE16(zip, 0x21); // mod date
    appendLE32(zip, crc);
    appendLE32(zip, quint32(data.size()));
    appendLE32(zip, quint32(data.size()));
    appendLE16(zip, quint16(name.size()));
    appendLE16(zip, 0); // extra length
    zip.append(name);
    zip.append(data);
    centralRecords.append(zipCentralRecordFor(name, crc, quint32(data.size()), offset));
}

QString xlsxColumnLetter(int zeroBasedColumn)
{
    QString letters;
    int n = zeroBasedColumn;
    do {
        letters.prepend(QChar('A' + (n % 26)));
        n = n / 26 - 1;
    } while (n >= 0);
    return letters;
}

bool writeXlsx(const QString &path, const QList<QStringList> &rows, QString *error)
{
    QString sheetXml = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\"><sheetData>");
    for (int r = 0; r < rows.size(); ++r) {
        sheetXml += QStringLiteral("<row r=\"%1\">").arg(r + 1);
        const QStringList &row = rows.at(r);
        for (int c = 0; c < row.size(); ++c) {
            // Every cell a string, in the data format (thin borders), the
            // header row in the header format.
            sheetXml += QStringLiteral("<c r=\"%1%2\" s=\"%3\" t=\"inlineStr\"><is><t xml:space=\"preserve\">%4</t></is></c>")
                            .arg(xlsxColumnLetter(c))
                            .arg(r + 1)
                            .arg(r == 0 ? 2 : 1)
                            .arg(row.at(c).toHtmlEscaped());
        }
        sheetXml += QStringLiteral("</row>");
    }
    sheetXml += QStringLiteral("</sheetData></worksheet>");

    static const QByteArray kContentTypes =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">"
        "<Default Extension=\"rels\" ContentType=\"application/vnd.openxmlformats-package.relationships+xml\"/>"
        "<Default Extension=\"xml\" ContentType=\"application/xml\"/>"
        "<Override PartName=\"/xl/workbook.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml\"/>"
        "<Override PartName=\"/xl/worksheets/sheet1.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml\"/>"
        "<Override PartName=\"/xl/styles.xml\" ContentType=\"application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml\"/>"
        "</Types>";
    // QXlsx::Format: data cells left / top aligned with thin borders on all
    // four sides; the header the same plus its one extra property (0x1f =
    // 4 -- read as the light gray fill pattern).
    static const QByteArray kStyles =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<styleSheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">"
        "<fonts count=\"1\"><font><sz val=\"11\"/><name val=\"Calibri\"/><family val=\"2\"/><scheme val=\"minor\"/></font></fonts>"
        "<fills count=\"3\"><fill><patternFill patternType=\"none\"/></fill><fill><patternFill patternType=\"gray125\"/></fill>"
        "<fill><patternFill patternType=\"lightGray\"/></fill></fills>"
        "<borders count=\"2\"><border><left/><right/><top/><bottom/><diagonal/></border>"
        "<border><left style=\"thin\"><color auto=\"1\"/></left><right style=\"thin\"><color auto=\"1\"/></right>"
        "<top style=\"thin\"><color auto=\"1\"/></top><bottom style=\"thin\"><color auto=\"1\"/></bottom><diagonal/></border></borders>"
        "<cellStyleXfs count=\"1\"><xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\"/></cellStyleXfs>"
        "<cellXfs count=\"3\"><xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\" xfId=\"0\"/>"
        "<xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"1\" xfId=\"0\" applyBorder=\"1\" applyAlignment=\"1\"><alignment horizontal=\"left\" vertical=\"top\"/></xf>"
        "<xf numFmtId=\"0\" fontId=\"0\" fillId=\"2\" borderId=\"1\" xfId=\"0\" applyFill=\"1\" applyBorder=\"1\" applyAlignment=\"1\"><alignment horizontal=\"left\" vertical=\"top\"/></xf></cellXfs>"
        "<cellStyles count=\"1\"><cellStyle name=\"Normal\" xfId=\"0\" builtinId=\"0\"/></cellStyles>"
        "</styleSheet>";
    static const QByteArray kRootRels =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
        "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"xl/workbook.xml\"/>"
        "</Relationships>";
    static const QByteArray kWorkbook =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" "
        "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">"
        "<sheets><sheet name=\"Sheet1\" sheetId=\"1\" r:id=\"rId1\"/></sheets></workbook>";
    static const QByteArray kWorkbookRels =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
        "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet\" Target=\"worksheets/sheet1.xml\"/>"
        "<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>"
        "</Relationships>";

    QByteArray zip;
    QList<QByteArray> centralRecords;
    zipAppendEntry(zip, centralRecords, "[Content_Types].xml", kContentTypes);
    zipAppendEntry(zip, centralRecords, "_rels/.rels", kRootRels);
    zipAppendEntry(zip, centralRecords, "xl/workbook.xml", kWorkbook);
    zipAppendEntry(zip, centralRecords, "xl/_rels/workbook.xml.rels", kWorkbookRels);
    zipAppendEntry(zip, centralRecords, "xl/worksheets/sheet1.xml", sheetXml.toUtf8());
    zipAppendEntry(zip, centralRecords, "xl/styles.xml", kStyles);

    const quint32 centralStart = quint32(zip.size());
    for (const QByteArray &record : std::as_const(centralRecords)) {
        zip.append(record);
    }
    const quint32 centralSize = quint32(zip.size()) - centralStart;

    appendLE32(zip, 0x06054b50);
    appendLE16(zip, 0); // disk number
    appendLE16(zip, 0); // disk with central dir
    appendLE16(zip, quint16(centralRecords.size()));
    appendLE16(zip, quint16(centralRecords.size()));
    appendLE32(zip, centralSize);
    appendLE32(zip, centralStart);
    appendLE16(zip, 0); // comment length

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        *error = file.errorString();
        return false;
    }
    if (file.write(zip) != zip.size()) {
        *error = file.errorString();
        return false;
    }
    return true;
}


// The five columns, the period's times in timeFormat; records whose
// keystream is blank are skipped.
QList<QStringList> exportRows(const QList<KeyloggerRow> &rows, const QString &timeFormat)
{
    QList<QStringList> out;
    out.append({QStringLiteral("Date"), QStringLiteral("Pressing period"), QStringLiteral("Application"),
                QStringLiteral("Title"), QStringLiteral("Keystrokes")});
    for (const KeyloggerRow &row : rows) {
        // FUN_1402d79d0: trimmed() only decides whether the record is
        // written; the keystream goes out as translated.
        if (row.keystream.trimmed().isEmpty()) {
            continue;
        }
        out.append({row.date.toString(QStringLiteral("dd.MM.yyyy")),
                    QDateTime::fromMSecsSinceEpoch(row.fromMs).toString(timeFormat) + QStringLiteral(" - ")
                        + QDateTime::fromMSecsSinceEpoch(row.toMs).toString(timeFormat),
                    row.application, row.title, row.keystream});
    }
    return out;
}
} // namespace

QString exportToCsv(const QList<KeyloggerRow> &rows, const QString &fileName)
{
    // FUN_1402d79d0: a text-mode file (CRLF), UTF-8 with a BOM, every field
    // quoted (quotes doubled), "HH:mm:ss - HH:mm:ss".
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return file.errorString();
    }
    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out.setGenerateByteOrderMark(true);
    const QList<QStringList> table = exportRows(rows, QStringLiteral("HH:mm:ss"));
    for (const QStringList &row : table) {
        QStringList quoted;
        for (QString field : row) {
            field.replace(QLatin1Char('"'), QStringLiteral("\"\""));
            quoted << QLatin1Char('"') + field + QLatin1Char('"');
        }
        out << quoted.join(QLatin1Char(',')) << QLatin1Char('\n');
    }
    out.flush();
    if (out.status() != QTextStream::Ok) {
        return file.errorString();
    }
    return QStringLiteral("success");
}

QString exportToXlsx(const QList<KeyloggerRow> &rows, const QString &fileName)
{
    // FUN_1402d84c0: QXlsx, every cell a string, "HH.mm.ss - HH.mm.ss".
    QString error;
    if (!writeXlsx(fileName, exportRows(rows, QStringLiteral("HH.mm.ss")), &error)) {
        return error;
    }
    return QStringLiteral("success");
}

} // namespace Keylogger
