#include "historyview.h"

#include "efficiencycategorybutton.h"

#include <QComboBox>
#include <QDateEdit>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFile>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>

#include <algorithm>
#include <array>
#include <limits>

namespace {

// Category display name/color now live in EfficiencyCategoryButton (the one
// place category-editing UI exists) -- use those instead of duplicating the
// logic here.

QString formatDuration(qint64 ms)
{
    const qint64 totalMinutes = ms / 60000;
    return QStringLiteral("%1h %2m").arg(totalMinutes / 60).arg(totalMinutes % 60);
}

// Shared time-of-day -> pixel mapping used by ActivityBarWidget,
// EfficiencyBarWidget and TimelineWidget so they stay pixel-aligned when
// stacked -- the yellow current-position line has to land on the same x in
// all three.
int xForTimeOfDay(qint64 timestampMs, qint64 rangeStartMs, qint64 rangeEndMs, int widgetWidth)
{
    if (rangeEndMs <= rangeStartMs) {
        return 0;
    }
    const double fraction = qBound(
        0.0, static_cast<double>(timestampMs - rangeStartMs) / (rangeEndMs - rangeStartMs), 1.0);
    return static_cast<int>(fraction * widgetWidth);
}

// The fixed-width left column shared by the transport controls (Change
// settings/Video+speed/Text, see the HBox in HistoryView's constructor) and
// every chart row below it (Activity/Efficiency/ViolationsFilter, still
// full-page-width widgets that reserve this much blank space on their own
// left edge) so the yellow current-position line lands
// on the exact same x in all of them and the whole block reads as one
// aligned unit, matching the real viewer's History.qml layout (button
// column left, chart/timeline content right, sharing one inset). TimeAxis
// is the one exception: it now lives INSIDE the button-column HBox itself
// (see the constructor), so it draws in plain local coordinates with no
// offset of its own -- adding one there would double up with the HBox's
// own positioning.
// At the reference window width the chart starts at x=283.  History.qml
// gives the page 8 px of outer inset, leaving a 275 px controls/labels
// column.  Keeping this value shared is important: the scrubber, time
// labels and every chart must start on exactly the same vertical line.
constexpr int kLeftColumnWidth = 275;

constexpr int kChartGridStep = 36;

void drawChartBackground(QPainter &painter, int left, int top, int chartWidth, int chartHeight)
{
    painter.fillRect(QRect(left, top, chartWidth, chartHeight), QColor(0x3b, 0x3d, 0x45));
    painter.setPen(QPen(QColor(0x44, 0x46, 0x4e), 1));
    for (int x = left; x <= left + chartWidth; x += kChartGridStep) {
        painter.drawLine(x, top, x, top + chartHeight);
    }
}

void drawPositionLine(QPainter &painter, qint64 positionMs, qint64 rangeStartMs, qint64 rangeEndMs,
                     int widgetWidth, int widgetHeight, int leftOffset = 0)
{
    if (positionMs <= 0 || rangeEndMs <= rangeStartMs) {
        return;
    }
    const int x = leftOffset
        + xForTimeOfDay(positionMs, rangeStartMs, rangeEndMs, widgetWidth - leftOffset);
    painter.setPen(QPen(QColor(0, 0, 0, 160), 3));
    painter.drawLine(x, 0, x, widgetHeight);
    painter.setPen(QPen(QColor(0xff, 0xd7, 0x00), 2));
    painter.drawLine(x, 0, x, widgetHeight);
}

// Groups consecutive same-window keystroke entries into "pressing period"
// rows, matching the real Kickidler keylogger table's shape (Src/Viewer_SRC/
// qml_real/.../keylogger/Table.qml: Date/Pressing period/Application/Title/
// Keystrokes columns) as closely as our flatter HistoryKeystrokeEntry data
// (no separate application field) allows. A gap over kGroupGapMs starts a
// new row even for the same window.
struct KeylogRow {
    qint64 fromMs = 0;
    qint64 toMs = 0;
    QString windowTitle;
    QString text;
};

QList<KeylogRow> groupKeystrokeEntries(const QList<HistoryKeystrokeEntry> &entries)
{
    constexpr qint64 kGroupGapMs = 60000;
    QList<KeylogRow> rows;
    for (const HistoryKeystrokeEntry &entry : entries) {
        if (!rows.isEmpty() && rows.last().windowTitle == entry.windowTitle
            && entry.timestampMs - rows.last().toMs <= kGroupGapMs) {
            rows.last().toMs = entry.timestampMs;
            rows.last().text += entry.text;
        } else {
            rows.append({entry.timestampMs, entry.timestampMs, entry.windowTitle, entry.text});
        }
    }
    return rows;
}

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

bool writeMinimalXlsx(const QString &path, const QList<QStringList> &rows)
{
    QString sheetXml = QStringLiteral(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<worksheet xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\"><sheetData>");
    for (int r = 0; r < rows.size(); ++r) {
        sheetXml += QStringLiteral("<row r=\"%1\">").arg(r + 1);
        const QStringList &row = rows.at(r);
        for (int c = 0; c < row.size(); ++c) {
            sheetXml += QStringLiteral("<c r=\"%1%2\" t=\"inlineStr\"><is><t xml:space=\"preserve\">%3</t></is></c>")
                            .arg(xlsxColumnLetter(c))
                            .arg(r + 1)
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
        "</Types>";
    static const QByteArray kRootRels =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
        "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument\" Target=\"xl/workbook.xml\"/>"
        "</Relationships>";
    static const QByteArray kWorkbook =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" "
        "xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">"
        "<sheets><sheet name=\"Keylogger\" sheetId=\"1\" r:id=\"rId1\"/></sheets></workbook>";
    static const QByteArray kWorkbookRels =
        "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
        "<Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">"
        "<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet\" Target=\"worksheets/sheet1.xml\"/>"
        "</Relationships>";

    QByteArray zip;
    QList<QByteArray> centralRecords;
    zipAppendEntry(zip, centralRecords, "[Content_Types].xml", kContentTypes);
    zipAppendEntry(zip, centralRecords, "_rels/.rels", kRootRels);
    zipAppendEntry(zip, centralRecords, "xl/workbook.xml", kWorkbook);
    zipAppendEntry(zip, centralRecords, "xl/_rels/workbook.xml.rels", kWorkbookRels);
    zipAppendEntry(zip, centralRecords, "xl/worksheets/sheet1.xml", sheetXml.toUtf8());

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
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(zip);
    return true;
}

}

ActivityBarWidget::ActivityBarWidget(QWidget *parent)
    : QWidget(parent)
{
    // Real Kickidler viewer's activity chart is compact, not a dominant
    // tall block -- matches HistoLine.qml's histogramHeight (20).
    setFixedHeight(24);
}

void ActivityBarWidget::setSamples(const QList<HistoryActivitySample> &samples, qint64 rangeStartMs,
                                   qint64 rangeEndMs)
{
    m_samples = samples;
    m_rangeStart = rangeStartMs;
    m_rangeEnd = rangeEndMs;
    update();
}

void ActivityBarWidget::setCurrentPositionMs(qint64 positionMs)
{
    m_currentPositionMs = positionMs;
    update();
}

void ActivityBarWidget::setBucketMs(qint64 bucketMs)
{
    if (bucketMs <= 0 || m_bucketMs == bucketMs) {
        return;
    }
    m_bucketMs = bucketMs;
    update();
}

void ActivityBarWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x24, 0x27, 0x2e));
    drawChartBackground(painter, kLeftColumnWidth, 0, qMax(1, width() - kLeftColumnWidth), height());
    // chart/ExtraHeaders.qml's row label, in the same left column as the
    // transport buttons and every other chart row (see kLeftColumnWidth) --
    // confirmed from a real screenshot (the label reads "Activity", plain
    // left-aligned text, not a guess).
    painter.setPen(QColor(0xb6, 0xb6, 0xb8));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 8));
    painter.drawText(QRect(8, 0, kLeftColumnWidth - 12, height()), Qt::AlignLeft | Qt::AlignVCenter,
                     QStringLiteral("Activitate"));
    if (m_samples.isEmpty() || m_rangeEnd <= m_rangeStart) {
        return;
    }

    // Bucket into fixed-size columns (m_bucketMs, see setBucketMs) and sum
    // each bucket's actual inputEvents -- a real intensity measure, not
    // just "was there any activity" -- then normalize to the busiest
    // bucket in view, same idea as a real activity histogram (matches
    // HistoLine.qml's `value[idx]`, a continuous 0..1 fed from the
    // backend). An earlier version instead counted "active vs not" samples
    // against an expected-samples-per-bucket estimate, which collapsed to
    // a flat 0-or-1 result whenever a bucket held only one raw sample
    // (e.g. at a 1-second Time step) -- every bar came out identical
    // instead of varying with how active the period actually was.
    const qint64 kBucketMs = m_bucketMs;
    QHash<qint64, qint64> eventsByBucket;
    qint64 maxBucketEvents = 1;
    for (const HistoryActivitySample &sample : m_samples) {
        if (sample.inputEvents <= 0) {
            continue;
        }
        const qint64 bucket = (sample.timestampMs - m_rangeStart) / kBucketMs;
        const qint64 total = eventsByBucket.value(bucket, 0) + sample.inputEvents;
        eventsByBucket[bucket] = total;
        maxBucketEvents = qMax(maxBucketEvents, total);
    }

    const qint64 bucketCount = qMax<qint64>(1, (m_rangeEnd - m_rangeStart) / kBucketMs + 1);
    const int barAreaWidth = qMax(1, width() - kLeftColumnWidth);
    const double bucketWidth = static_cast<double>(barAreaWidth) / static_cast<double>(bucketCount);
    painter.setPen(Qt::NoPen);
    // #7aa1e2: exact color from the real viewer's HistoLine.qml canvas draw.
    painter.setBrush(QColor(0x7a, 0xa1, 0xe2));
    for (auto it = eventsByBucket.cbegin(); it != eventsByBucket.cend(); ++it) {
        const double fraction = qBound(0.0, static_cast<double>(it.value()) / maxBucketEvents, 1.0);
        const int barHeight = qMax(1, static_cast<int>(fraction * (height() - 3)));
        const int x = kLeftColumnWidth + static_cast<int>(it.key() * bucketWidth);
        const int barWidth = qMax(1, static_cast<int>(bucketWidth) - 1);
        painter.drawRect(x, height() - barHeight, barWidth, barHeight);
    }

    drawPositionLine(painter, m_currentPositionMs, m_rangeStart, m_rangeEnd, width(), height(),
                     kLeftColumnWidth);
}

EfficiencyBarWidget::EfficiencyBarWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(20);
}

void EfficiencyBarWidget::setSegments(const QList<HistoryAppSegment> &segments,
                                      const QHash<QString, QString> &categories,
                                      qint64 rangeStartMs, qint64 rangeEndMs)
{
    m_segments = segments;
    m_categories = categories;
    m_rangeStart = rangeStartMs;
    m_rangeEnd = rangeEndMs;
    update();
}

void EfficiencyBarWidget::setCurrentPositionMs(qint64 positionMs)
{
    m_currentPositionMs = positionMs;
    update();
}

void EfficiencyBarWidget::paintEvent(QPaintEvent *)
{
    // Matches the real viewer's chart/content/Line.qml: when several
    // distinct categories fall in the same pixel column (because segment
    // boundaries are finer than one pixel of screen time), it does NOT
    // blend or let the last one win -- it stacks them as equal-height
    // horizontal bands within that column, top-to-bottom in the order they
    // actually happened. Drawing each segment as one full-height rect (the
    // previous approach) made only the LAST segment touching a column
    // visible, silently hiding the others whenever more than one category
    // occurred within the same on-screen pixel.
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x24, 0x27, 0x2e));
    drawChartBackground(painter, kLeftColumnWidth, 0, qMax(1, width() - kLeftColumnWidth), height());
    painter.setPen(QColor(0xb6, 0xb6, 0xb8));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 8));
    painter.drawText(QRect(8, 0, kLeftColumnWidth - 12, height()), Qt::AlignLeft | Qt::AlignVCenter,
                     QStringLiteral("Eficiență"));
    const int barAreaWidth = qMax(1, width() - kLeftColumnWidth);
    if (!m_segments.isEmpty() && m_rangeEnd > m_rangeStart) {
        const double span = static_cast<double>(m_rangeEnd - m_rangeStart);

        QList<HistoryAppSegment> sorted = m_segments;
        std::sort(sorted.begin(), sorted.end(),
                 [](const HistoryAppSegment &a, const HistoryAppSegment &b) {
                     return a.startMs < b.startMs;
                 });

        // column -> ordered, de-duplicated (consecutive) list of categories
        // that occurred there.
        QHash<int, QList<QString>> columnCategories;
        for (const HistoryAppSegment &segment : std::as_const(sorted)) {
            const QString category =
                m_categories.value(segment.application, QStringLiteral("none"));
            const double startFraction = (segment.startMs - m_rangeStart) / span;
            const double endFraction = (segment.endMs - m_rangeStart) / span;
            const int x0 = static_cast<int>(qBound(0.0, startFraction, 1.0) * barAreaWidth);
            const int x1 =
                qMax(x0 + 1, static_cast<int>(qBound(0.0, endFraction, 1.0) * barAreaWidth));
            for (int x = x0; x < x1; ++x) {
                QList<QString> &categories = columnCategories[x];
                if (categories.isEmpty() || categories.last() != category) {
                    categories.append(category);
                }
            }
        }

        painter.setPen(Qt::NoPen);
        for (auto it = columnCategories.cbegin(); it != columnCategories.cend(); ++it) {
            const QList<QString> &categories = it.value();
            const double bandHeight = static_cast<double>(height()) / categories.size();
            double y = 0.0;
            for (const QString &category : categories) {
                painter.setBrush(EfficiencyCategoryButton::color(category));
                const int top = static_cast<int>(y);
                const int bottom = static_cast<int>(y + bandHeight);
                painter.drawRect(kLeftColumnWidth + it.key(), top, 1, qMax(1, bottom - top));
                y += bandHeight;
            }
        }
    }
    drawPositionLine(painter, m_currentPositionMs, m_rangeStart, m_rangeEnd, width(), height(),
                     kLeftColumnWidth);
}

namespace {
constexpr int kViolationRowHeight = 20;
// Shares kLeftColumnWidth with the transport controls and every other
// chart row (see its own comment) so the violations rows' bars line up
// with Activity/Efficiency above them instead of using their own width.
constexpr int kViolationLabelWidth = kLeftColumnWidth;
}

ViolationsFilterWidget::ViolationsFilterWidget(QWidget *parent)
    : QWidget(parent)
{
}

void ViolationsFilterWidget::setRows(const QList<ViolationRow> &rows, qint64 rangeStartMs,
                                     qint64 rangeEndMs)
{
    m_rows = rows;
    m_rangeStart = rangeStartMs;
    m_rangeEnd = rangeEndMs;
    setFixedHeight(qMax(kViolationRowHeight, rows.size() * kViolationRowHeight));
    update();
}

void ViolationsFilterWidget::setCurrentPositionMs(qint64 positionMs)
{
    m_currentPositionMs = positionMs;
    update();
}

QSize ViolationsFilterWidget::sizeHint() const
{
    return QSize(400, qMax(kViolationRowHeight, m_rows.size() * kViolationRowHeight));
}

void ViolationsFilterWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x24, 0x27, 0x2e));
    const int stripX = kViolationLabelWidth;
    const int stripWidth = qMax(1, width() - stripX);
    const double span = static_cast<double>(m_rangeEnd - m_rangeStart);

    painter.setFont(QFont(QStringLiteral("Segoe UI"), 8));
    for (int row = 0; row < m_rows.size(); ++row) {
        const ViolationRow &violation = m_rows.at(row);
        const int y = row * kViolationRowHeight;

        painter.setPen(QColor(0xb6, 0xb6, 0xb8));
        painter.drawText(QRect(4, y, kViolationLabelWidth - 8, kViolationRowHeight),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         painter.fontMetrics().elidedText(violation.label, Qt::ElideRight,
                                                          kViolationLabelWidth - 12));

        drawChartBackground(painter, stripX, y, stripWidth, kViolationRowHeight - 2);

        if (span > 0) {
            painter.setBrush(QColor(0xe7, 0x4c, 0x3c));
            for (const auto &violationRange : violation.ranges) {
                const double startFraction = (violationRange.first - m_rangeStart) / span;
                const double endFraction = (violationRange.second - m_rangeStart) / span;
                const int x = stripX + static_cast<int>(qBound(0.0, startFraction, 1.0) * stripWidth);
                const int right =
                    stripX + static_cast<int>(qBound(0.0, endFraction, 1.0) * stripWidth);
                painter.drawRect(x, y, qMax(1, right - x), kViolationRowHeight - 2);
            }
        }
    }

    if (m_currentPositionMs > 0 && span > 0) {
        const int x = stripX + xForTimeOfDay(m_currentPositionMs, m_rangeStart, m_rangeEnd, stripWidth);
        painter.setPen(QPen(QColor(0, 0, 0, 160), 3));
        painter.drawLine(x, 0, x, height());
        painter.setPen(QPen(QColor(0xff, 0xd7, 0x00), 2));
        painter.drawLine(x, 0, x, height());
    }
}

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget(parent)
{
    // Paired only with the settings/Play/Mute row (see the constructor),
    // not the whole 3-row button block -- matches a real screenshot: only
    // ~10-20px of blank space between the video/keylogger above and the
    // scrub circle, which a taller widget (stretched to match 3 button
    // rows, or History.qml's literal 125px) can't give without leaving a
    // tall empty area above the track. The track itself still sits near
    // this widget's own bottom edge (see paintEvent), matching the real
    // slider's position right at the row's bottom.
    setFixedHeight(30);
    setMouseTracking(false);
}

void TimelineWidget::setTimestamps(const QList<qint64> &timestamps)
{
    m_timestamps = timestamps;
    m_currentIndex = timestamps.isEmpty() ? -1 : qBound(0, m_currentIndex, timestamps.size() - 1);
    update();
}

void TimelineWidget::setCurrentIndex(int index)
{
    if (index < 0 || index >= m_timestamps.size()) {
        return;
    }
    m_currentIndex = index;
    update();
}

QSize TimelineWidget::sizeHint() const
{
    return QSize(400, 30);
}

int TimelineWidget::indexForX(int x) const
{
    if (m_timestamps.isEmpty() || width() <= 0) {
        return -1;
    }
    const qint64 rangeStart = m_timestamps.first();
    const qint64 rangeEnd = m_timestamps.last();
    if (rangeEnd <= rangeStart) {
        return 0;
    }
    const double fraction = qBound(0.0, static_cast<double>(x) / width(), 1.0);
    const qint64 targetMs = rangeStart + static_cast<qint64>(fraction * (rangeEnd - rangeStart));
    // Nearest timestamp to targetMs -- frames aren't necessarily evenly
    // spaced, so a straight index-from-fraction would drift.
    int closest = 0;
    qint64 closestDelta = qAbs(m_timestamps.first() - targetMs);
    for (int i = 1; i < m_timestamps.size(); ++i) {
        const qint64 delta = qAbs(m_timestamps.at(i) - targetMs);
        if (delta < closestDelta) {
            closestDelta = delta;
            closest = i;
        }
    }
    return closest;
}

void TimelineWidget::seekToX(int x)
{
    const int index = indexForX(x);
    if (index < 0) {
        return;
    }
    m_currentIndex = index;
    update();
    emit indexSelected(index);
}

void TimelineWidget::mousePressEvent(QMouseEvent *event)
{
    seekToX(event->pos().x());
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        seekToX(event->pos().x());
    }
}

void TimelineWidget::paintEvent(QPaintEvent *)
{
    // Track sits near the BOTTOM of the whole control block (see the
    // constructor comment), not vertically centered across it -- that's
    // what puts it right next to the violation-panel toggle button below,
    // matching History.qml.
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x1b, 0x1e, 0x24));
    if (m_timestamps.isEmpty()) {
        return;
    }
    const qint64 rangeStart = m_timestamps.first();
    const qint64 rangeEnd = m_timestamps.last();
    // In History.qml the 4 px SliderBar sits at about y=102 inside the
    // 125 px sliderAndMeta block, leaving room for its 19 px pick image.
    const int trackY = qMax(7, height() - 22);

    painter.setPen(QPen(QColor(0x3a, 0x3e, 0x47), 3));
    painter.drawLine(0, trackY, width(), trackY);

    if (m_currentIndex >= 0 && m_currentIndex < m_timestamps.size()) {
        const int x = xForTimeOfDay(m_timestamps.at(m_currentIndex), rangeStart, rangeEnd, width());
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0xff, 0xd7, 0x00));
        painter.drawEllipse(QPoint(x, trackY), 5, 5);
    }
}

namespace {
constexpr int kTimeAxisDateHeight = 18;
constexpr int kTimeAxisTicksHeight = 26;
}

TimeAxisWidget::TimeAxisWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(kTimeAxisDateHeight + kTimeAxisTicksHeight);
}

void TimeAxisWidget::setRange(qint64 rangeStartMs, qint64 rangeEndMs)
{
    m_rangeStart = rangeStartMs;
    m_rangeEnd = rangeEndMs;
    update();
}

void TimeAxisWidget::setCurrentPositionMs(qint64 positionMs)
{
    m_currentPositionMs = positionMs;
    update();
}

QSize TimeAxisWidget::sizeHint() const
{
    return QSize(400, kTimeAxisDateHeight + kTimeAxisTicksHeight);
}

void TimeAxisWidget::paintEvent(QPaintEvent *)
{
    // Matches the real viewer's TimeLine.qml: a date label (highLabels) then
    // a row of evenly spaced HH:mm ticks (lowLabels), #54545a tick marks,
    // white bold labels. Purely visual -- TimeLine.qml has no MouseArea of
    // its own; scrubbing happens on the separate SliderBar (TimelineWidget).
    // NOT offset by kLeftColumnWidth -- unlike ActivityBarWidget/
    // EfficiencyBarWidget/ViolationsFilterWidget (still full page-width
    // widgets that reserve that much on their own left edge), this widget
    // now only occupies the right-hand cell of the button-column HBox (see
    // the constructor), so it's already positioned to the right of the
    // buttons by the layout itself -- adding the offset again here doubled
    // it, pushing the date/ticks far past where the buttons actually end.
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x1b, 0x1e, 0x24));
    if (m_rangeEnd <= m_rangeStart) {
        return;
    }

    painter.setPen(QColor(0xff, 0xff, 0xff));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 8, QFont::Bold));
    painter.drawText(QRect(4, 0, width() - 8, kTimeAxisDateHeight),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     QDateTime::fromMSecsSinceEpoch(m_rangeStart).toString(QStringLiteral("dddd, d MMMM yyyy")));

    const int tickY = kTimeAxisDateHeight;
    const int labelWidthEstimate = 60;
    const int tickCount = qMax(2, width() / labelWidthEstimate);
    for (int i = 0; i <= tickCount; ++i) {
        const qint64 tickMs = m_rangeStart + (m_rangeEnd - m_rangeStart) * i / tickCount;
        const int x = xForTimeOfDay(tickMs, m_rangeStart, m_rangeEnd, width());
        painter.setPen(QColor(0x54, 0x54, 0x5a));
        painter.drawLine(x, tickY, x, tickY + 4);
        painter.setPen(QColor(0xff, 0xff, 0xff));
        const QString label = QDateTime::fromMSecsSinceEpoch(tickMs).toString(QStringLiteral("HH:mm"));
        painter.drawText(QRect(x - 24, tickY + 4, 48, 12), Qt::AlignHCenter | Qt::AlignTop, label);
    }

    // Continues the same yellow line as Activity/Efficiency/ViolationsFilter
    // below it -- no offset needed here (unlike those, which are full
    // page-width widgets): this widget's local x=0 is already at global
    // x=kLeftColumnWidth via the HBox layout, so the same fraction of ITS
    // OWN width lands on the same global x as their offset formula does.
    drawPositionLine(painter, m_currentPositionMs, m_rangeStart, m_rangeEnd, width(), height());
}

HistoryView::HistoryView(ViewerConnection &connection, QWidget *parent)
    : QWidget(parent)
    , m_connection(connection)
{
    // Layout matches the real Kickidler viewer's History.qml as closely as
    // Qt Widgets allows: the video dominates the page (no top bar with big
    // employee/day pickers -- those live in the "Change period" dialog, see
    // HistoryChoicePanel.qml), a thin keylogger ticker sits directly under
    // it, then the transport controls, then a violation panel that starts
    // COLLAPSED and only opens via its own toggle button (panelViolation in
    // History.qml: `height: 0`, opens via "Open violation panel"/"Hide
    // violation panel", not shown by default like our first attempt had it).
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    // History.qml's panelViolation toggle button is anchored directly to
    // the bottom edge of the same container the slider lives in (bottomMargin
    // -5, i.e. no gap at all, even slightly overlapping) -- root's spacing
    // is kept small everywhere for that reason, not just between these two.
    root->setSpacing(0);

    // Every screen of the current device, shown live at once, side by side
    // -- not a dropdown you pick one monitor from.
    m_monitorStripArea = new QScrollArea(this);
    m_monitorStripArea->setObjectName(QStringLiteral("historyPreview"));
    m_monitorStripArea->setWidgetResizable(true);
    m_monitorStripArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_monitorStripArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_monitorStripArea->setMinimumSize(400, 250);
    m_monitorStripContainer = new QWidget(m_monitorStripArea);
    m_monitorStripLayout = new QHBoxLayout(m_monitorStripContainer);
    m_monitorStripLayout->setContentsMargins(4, 4, 4, 4);
    m_monitorStripLayout->setSpacing(8);
    m_monitorStripArea->setWidget(m_monitorStripContainer);

    // No maximumHeight here (tried, reverted) -- History.qml's own
    // sliderAndMeta is anchored `anchors.bottom: chartsItem.top`, i.e. the
    // controls block sits directly above the violation panel and rides up
    // and down as chartsItem's height animates between 0 (collapsed) and
    // 186 (open); video fills whatever's left above that. A hard cap on
    // the video column breaks exactly that: it stops video from expanding
    // into the space chartsItem frees up when collapsed, so the controls
    // stayed pinned high instead of dropping down to follow the (now
    // closed) panel. The real fix for the overflow this was working
    // around is resizeMonitorStripToFit()'s qBound floor/ceiling on the
    // PREVIEW's minimum size (see below) -- that's what stops the runaway
    // lock-in, without blocking legitimate growth into freed space.
    m_videoColumn = new QWidget(this);
    auto *videoColumnLayout = new QVBoxLayout(m_videoColumn);
    videoColumnLayout->setContentsMargins(0, 0, 0, 0);
    videoColumnLayout->setSpacing(2);

    // history/video/Header.qml: a dedicated 24px bar, background #3f4047
    // with 2px border lines top/bottom, employee name + "( date time )"
    // CENTERED as one unit -- not a docked left-aligned label with a
    // stretch, which was this project's own earlier (wrong) guess before a
    // real screenshot confirmed the centered layout.
    auto *videoHeaderBar = new QWidget(this);
    videoHeaderBar->setFixedHeight(24);
    videoHeaderBar->setStyleSheet(
        QStringLiteral("background: #3f4047; border-top: 1px solid #33343a; "
                       "border-bottom: 1px solid #414249;"));
    auto *videoHeaderRow = new QHBoxLayout(videoHeaderBar);
    videoHeaderRow->setContentsMargins(0, 0, 0, 0);
    videoHeaderRow->addStretch();
    m_employeeLabel = new QLabel(videoHeaderBar);
    m_employeeLabel->setStyleSheet(QStringLiteral("color: white; font-weight: 700;"));
    videoHeaderRow->addWidget(m_employeeLabel);
    m_timeLabel = new QLabel(videoHeaderBar);
    m_timeLabel->setStyleSheet(QStringLiteral("color: white; font-weight: 700; padding-left: 10px;"));
    videoHeaderRow->addWidget(m_timeLabel);
    videoHeaderRow->addStretch();
    videoColumnLayout->addWidget(videoHeaderBar);

    auto *videoBody = new QHBoxLayout;
    videoBody->addWidget(m_monitorStripArea, 1);
    m_runningAppsList = new QListWidget(this);
    m_runningAppsList->setFixedWidth(420); // matches History.qml's `panel { width: 420 }`
    m_runningAppsList->hide(); // matches appButton's panel: closed by default
    videoBody->addWidget(m_runningAppsList);
    videoColumnLayout->addLayout(videoBody, 1);

    // appButton (Running Applications toggle): a semi-transparent overlay
    // floating on TOP of the video's top-right corner in the real app, not
    // a docked row -- see resizeEvent() for how its geometry is kept
    // pinned there.
    m_toggleAppsButton = new QPushButton(QStringLiteral("Running applications ◀"), m_monitorStripArea);
    m_toggleAppsButton->setObjectName(QStringLiteral("flatButton"));
    m_toggleAppsButton->setStyleSheet(
        QStringLiteral("QPushButton#flatButton { background: rgba(56,58,65,0.8); }"));
    connect(m_toggleAppsButton, &QPushButton::clicked, this, &HistoryView::onToggleRunningApps);
    m_toggleAppsButton->raise();

    // Keylogger: a single-line ticker directly under the video, not a
    // separate "Text mode" of the whole page -- History.qml's `keylogger`
    // BorderImage is always visible right under videoCell, showing text
    // flowing around the current playback moment. The full grouped table
    // (real columns: Date/Pressing period/Application/Title/Keystrokes,
    // Table.qml) opens on demand via m_keylogTableButton instead, since
    // that table belongs to the Tracker tile's own Keylogger tab in the
    // real app, not to History's main view.
    m_keylogTicker = new QLabel(this);
    m_keylogTicker->setObjectName(QStringLiteral("privacy"));
    m_keylogTicker->setFixedHeight(30); // matches History.qml's keylogger BorderImage height
    m_keylogTicker->setAlignment(Qt::AlignCenter);
    m_keylogTicker->setStyleSheet(QStringLiteral("color: #9fa5ae;"));
    videoColumnLayout->addWidget(m_keylogTicker);
    // Stretch weights 1:8 -- video still grows a little with the window,
    // A small FIXED gap, not a competing stretch (tried and reverted --
    // shrank video to a tiny box) and not a large fixed value either (also
    // tried and reverted -- 90px pushed the controls block far enough down
    // that it landed BELOW the visible window on a normal-sized window,
    // since this page has no scroll area: anything that doesn't fit is
    // simply clipped, not scrolled to). Keeping this small guarantees the
    // controls stay on-screen; see kLeftColumnWidth's callers for the
    // actual alignment work, which doesn't depend on this value.
    root->addWidget(m_videoColumn, 1);
    root->addSpacing(10);

    // Real Kickidler keylogger table columns (keylogger/Table.qml):
    // Date/Pressing period/Application/Title/Keystrokes. The Application
    // column is a placeholder (always "--") until HistoryKeystrokeEntry
    // actually carries an application field -- see the KeylogRow/
    // groupKeystrokeEntries comment above; it needs a PersonalHost change,
    // not just a client-side one, so it's deferred. Lives in a popup dialog
    // (see onKeylogTableClicked), not as a mode of the main page.
    m_textLog = new QTableWidget(0, 5, this);
    m_textLog->setHorizontalHeaderLabels(
        {QStringLiteral("Date"), QStringLiteral("Pressing period"), QStringLiteral("Application"),
         QStringLiteral("Title"), QStringLiteral("Keystrokes")});
    m_textLog->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_textLog->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_textLog->setSelectionMode(QAbstractItemView::SingleSelection);
    m_textLog->verticalHeader()->setVisible(false);
    m_textLog->horizontalHeader()->setStretchLastSection(true);
    m_textLog->setColumnWidth(0, 90);
    m_textLog->setColumnWidth(1, 150);
    m_textLog->setColumnWidth(2, 110);
    m_textLog->setColumnWidth(3, 180);
    // Not in any layout until onKeylogTableClicked reparents it into a
    // dialog -- without this, Qt shows it anyway, unpositioned, at (0,0)
    // over everything else (this was the "garbled Date/boxes in the
    // top-left corner" bug).
    m_textLog->hide();

    // History.qml's transport controls are a COLUMN to the left of the
    // timeline (Change settings / Play+Audio / Video+speed / Text), not a
    // horizontal row above it -- see the fixed-width leftColumn widget
    // below, built with the same kLeftColumnWidth every chart row
    // (Activity/Efficiency/ViolationsFilter/TimeAxis) reserves on their own
    // left edge, so the whole block (buttons, timeline, charts) shares one
    // aligned left edge and the yellow position line lands at the same x
    // everywhere.
    m_changeSettingsButton = new QPushButton(QStringLiteral("Change settings"), this);
    m_changeSettingsButton->setObjectName(QStringLiteral("historyPill"));
    m_changeSettingsButton->setFixedSize(217, 31);
    connect(m_changeSettingsButton, &QPushButton::clicked, this, &HistoryView::onChangeSettingsClicked);
    m_dayCombo = new QComboBox(this);
    m_keylogTableButton = new QPushButton(QStringLiteral("→ Text"), this);
    m_keylogTableButton->setObjectName(QStringLiteral("historyPill"));
    m_keylogTableButton->setFixedSize(69, 30);
    m_keylogTableButton->setToolTip(QStringLiteral("Keylogger"));
    connect(m_keylogTableButton, &QPushButton::clicked, this, &HistoryView::onKeylogTableClicked);
    // "Video" export in the real viewer saves an actual AVI (VideoSaverSelector.qml);
    // we have no video encoder in this project's dependencies, so this
    // exports the day's captured frames as a numbered PNG sequence instead
    // -- same underlying data (requestHistoryFrame per timestamp), honestly
    // labeled as image export rather than pretending to produce a video file.
    m_exportVideoButton = new QPushButton(QStringLiteral("→ Video"), this);
    m_exportVideoButton->setObjectName(QStringLiteral("historyPill"));
    m_exportVideoButton->setFixedSize(82, 31);
    m_exportVideoButton->setToolTip(QStringLiteral("Export imagini (secventa PNG -- fara encoder video)"));
    connect(m_exportVideoButton, &QPushButton::clicked, this, &HistoryView::onExportVideoClicked);
    m_playButton = new QPushButton(QStringLiteral("▶"), this);
    m_playButton->setObjectName(QStringLiteral("historyIconPill"));
    m_playButton->setFixedSize(22, 22);
    connect(m_playButton, &QPushButton::clicked, this, &HistoryView::onPlayClicked);
    // Real speed options (SpeedButton.qml): 0.5x/0.75x/1x/2x/4x/8x/16x.
    m_speedCombo = new QComboBox(this);
    m_speedCombo->setObjectName(QStringLiteral("historyPill"));
    m_speedCombo->addItems({QStringLiteral("0.5x"), QStringLiteral("0.75x"), QStringLiteral("1x"),
                            QStringLiteral("2x"), QStringLiteral("4x"), QStringLiteral("8x"),
                            QStringLiteral("16x")});
    m_speedCombo->setFixedSize(60, 22);
    m_speedCombo->setCurrentIndex(2); // 1x
    connect(m_speedCombo, &QComboBox::currentIndexChanged, this, &HistoryView::onSpeedChanged);
    m_muteButton = new QPushButton(QStringLiteral("🔇"), this);
    m_muteButton->setObjectName(QStringLiteral("historyIconPill"));
    m_muteButton->setFixedSize(22, 22);
    m_muteButton->setEnabled(false);
    m_muteButton->setToolTip(QStringLiteral("Fara audio in acest sistem."));
    // Audio.qml is only visible at Time step = 1s -- default Time step here
    // is 5 minutes (m_timeStepMs), so it starts hidden.
    m_muteButton->setVisible(m_timeStepMs <= 1000);
    m_timeline = new TimelineWidget(this);
    connect(m_timeline, &TimelineWidget::indexSelected, this, &HistoryView::onTimelineMoved);

    // Re-read History.qml directly: `sliderAndMeta` (this whole block) is
    // ONE fixed-height (125px there) container where the button columns
    // (Change settings / Play+Audio / Video+speed / Text) and the slider
    // are SIBLINGS sharing that same height -- not two separate rows with
    // the slider only paired with row 1. panelViolation (the toggle button
    // below) is anchored to THIS container's own bottom edge, no gap. So
    // here: one fixed-width column with all 3 button rows stacked, paired
    // with m_timeline (which now spans the whole column's height and draws
    // its track near ITS OWN bottom -- see TimelineWidget::paintEvent) in a
    // single HBox, immediately followed by the toggle row.
    // Two SEPARATE fixed-width rows, not one tall column: only row 1
    // (Change settings + Play + Mute) sits next to the timeline -- pairing
    // the timeline with the WHOLE 3-row button block (as a previous pass
    // tried) made the gap between the video/keylogger above and the slider
    // circle far too tall (the timeline widget stretched to match that
    // whole block's height, with just blank fill above its own track).
    // Video+speed and Text sit in their own compact row below, confined to
    // the same column width, nothing beside them.
    auto *settingsRowContainer = new QWidget(this);
    settingsRowContainer->setFixedWidth(kLeftColumnWidth);
    auto *settingsRow = new QHBoxLayout(settingsRowContainer);
    settingsRow->setContentsMargins(10, 3, 10, 3);
    settingsRow->setSpacing(6);
    settingsRow->addWidget(m_changeSettingsButton);
    settingsRow->addWidget(m_playButton);
    settingsRow->addWidget(m_muteButton);
    settingsRow->addStretch();
    // Not shown -- the reference screenshot's button column has no visible
    // day picker at all (day/period selection lives entirely in the
    // "Change settings" dialog, see onChangeSettingsClicked); m_dayCombo
    // stays alive and fully functional (applyPeriodFilter/onDayChanged
    // still use it), just never added to a visible layout.
    m_dayCombo->hide();

    auto *belowButtonsContainer = new QWidget(this);
    belowButtonsContainer->setFixedWidth(kLeftColumnWidth);
    auto *belowButtonsLayout = new QVBoxLayout(belowButtonsContainer);
    belowButtonsLayout->setContentsMargins(10, 0, 10, 4);
    belowButtonsLayout->setSpacing(4);
    auto *videoRow = new QHBoxLayout;
    videoRow->setSpacing(6);
    videoRow->addWidget(m_exportVideoButton);
    videoRow->addWidget(m_speedCombo);
    videoRow->addStretch();
    belowButtonsLayout->addLayout(videoRow);
    belowButtonsLayout->addWidget(m_keylogTableButton, 0, Qt::AlignLeft);

    // history/MultiSessionsSlider.qml equivalent -- the real one is one
    // slider PER login session within the viewed range (Up/Down switches
    // which session's slider is active), since a real History "period" can
    // span several logins. Ours works on whole days from one flat
    // per-day store (historyrecorder.cpp), with no concept of session
    // boundaries at all -- there's nothing to actually switch between yet,
    // so this is a single, disabled placeholder row rather than a fake
    // multi-session UI. A real one needs PersonalHost to record session
    // start/end boundaries, not just per-day buckets.
    // Hidden by default and left that way: the real MultiSessionsSlider
    // only appears at all once a viewed period actually spans more than
    // one login session -- with our flat per-day model that's never true,
    // so permanently showing this placeholder just added a row that isn't
    // in the reference screenshot at all. Kept around, unparented from any
    // visible layout below, for whenever PersonalHost gains real session
    // boundaries.
    auto *sessionSliderContainer = new QWidget(this);
    auto *sessionSliderRow = new QHBoxLayout(sessionSliderContainer);
    sessionSliderRow->setContentsMargins(0, 0, 0, 0);
    auto *sessionUpButton = new QToolButton(this);
    sessionUpButton->setText(QStringLiteral("▲"));
    sessionUpButton->setEnabled(false);
    auto *sessionDownButton = new QToolButton(this);
    sessionDownButton->setText(QStringLiteral("▼"));
    sessionDownButton->setEnabled(false);
    auto *sessionLabel = new QLabel(QStringLiteral("Sesiunea 1/1"), this);
    sessionLabel->setStyleSheet(QStringLiteral("color: #6f747d; font-size: 8pt;"));
    sessionLabel->setToolTip(
        QStringLiteral("Grabber-ul nu inregistreaza inca limitele sesiunilor de login -- "
                       "toata ziua e tratata ca o singura sesiune."));
    sessionSliderRow->addWidget(sessionUpButton);
    sessionSliderRow->addWidget(sessionDownButton);
    sessionSliderRow->addWidget(sessionLabel);
    sessionSliderRow->addStretch();
    sessionSliderContainer->hide();
    root->addWidget(sessionSliderContainer);

    // Left column (all 3 button rows stacked) | right column (timeline +
    // date/ticks stacked) as the TWO cells of ONE HBox, so Qt gives them
    // the exact same height automatically (whichever is taller wins, the
    // shorter one's own trailing stretch just absorbs the difference
    // inside itself) -- a real screenshot showed the button column
    // stopping short while the timeline+date/ticks column kept going,
    // leaving a bare/mismatched gap under the buttons instead of both
    // sides ending flush together right above Activity/Efficiency.
    // Always visible, unlike m_violationPanel -- MultiSessionsSlider.qml's
    // own TimeLine (date+ticks) lives inside the always-shown slider block,
    // not inside the collapsible chartsItem (Activity/Efficiency/Filters).
    m_timeAxis = new TimeAxisWidget(this);
    auto *leftColumn = new QWidget(this);
    leftColumn->setFixedWidth(kLeftColumnWidth);
    auto *leftColumnLayout = new QVBoxLayout(leftColumn);
    leftColumnLayout->setContentsMargins(0, 0, 0, 0);
    leftColumnLayout->setSpacing(4);
    leftColumnLayout->addWidget(settingsRowContainer);
    leftColumnLayout->addWidget(belowButtonsContainer);
    leftColumnLayout->addStretch(1);

    auto *rightColumn = new QWidget(this);
    auto *rightColumnLayout = new QVBoxLayout(rightColumn);
    rightColumnLayout->setContentsMargins(0, 0, 0, 0);
    rightColumnLayout->setSpacing(0);
    rightColumnLayout->addWidget(m_timeline);
    rightColumnLayout->addWidget(m_timeAxis);
    rightColumnLayout->addStretch(1);

    auto *combinedRow = new QHBoxLayout;
    combinedRow->setContentsMargins(0, 0, 0, 0);
    combinedRow->setSpacing(0);
    combinedRow->addWidget(leftColumn);
    combinedRow->addWidget(rightColumn, 1);
    root->addLayout(combinedRow);

    // Violation panel toggle: centered below the transport row, matches
    // History.qml's panelViolation button (bottom-center, arrow icon,
    // "Open violation panel" / "Hide violation panel" -- panel starts
    // collapsed).
    auto *violationToggleRow = new QHBoxLayout;
    violationToggleRow->addStretch();
    m_violationToggleButton = new QPushButton(QStringLiteral("▲ Open violation panel"), this);
    m_violationToggleButton->setObjectName(QStringLiteral("flatButton"));
    // Shrunk from 36 -- this row is literally the only thing standing
    // between "-> Text" (bottom of the button column) and the Activity/
    // Efficiency frame below it, so its own height IS most of that gap.
    // 20 (tried) clipped the text top/bottom -- "flatButton" has 6px
    // padding top+bottom baked into its stylesheet, so anything under
    // about 26px squeezes the label instead of shrinking real whitespace.
    m_violationToggleButton->setFixedHeight(26);
    connect(m_violationToggleButton, &QPushButton::clicked, this,
            &HistoryView::onToggleViolationPanel);
    violationToggleRow->addWidget(m_violationToggleButton);
    violationToggleRow->addStretch();
    root->addLayout(violationToggleRow);

    // "Violation panel": Activity (5-minute activity pillars) + Efficiency
    // (productive/neutral/unproductive coloring) -- matching Chart.qml's top
    // "extraLine" charts -- plus a filters strip chart below them (matching
    // Chart.qml's "filtersLine": one row per violation rule, red blocks
    // where triggered; see ViolationsFilterWidget/refreshViolationsFilter).
    // Grouped so onToggleViolationPanel can hide/show all of it as one
    // unit. Starts hidden; TimeAxis is toggled alongside this widget.
    m_violationPanel = new QWidget(this);
    auto *violationLayout = new QVBoxLayout(m_violationPanel);
    violationLayout->setContentsMargins(0, 0, 0, 0);
    // Zero spacing: Activity/Efficiency/ViolationsFilter are pixel-adjacent
    // so each one's own position-line segment (they all share
    // kLeftColumnWidth and the same rangeStart/rangeEnd, and continue the
    // same line m_timeAxis above them already draws) reads as one
    // continuous line down the whole block instead of a dashed one with
    // visible gaps at every row boundary.
    violationLayout->setSpacing(0);
    m_activityBar = new ActivityBarWidget(m_violationPanel);
    violationLayout->addWidget(m_activityBar);
    m_efficiencyBar = new EfficiencyBarWidget(m_violationPanel);
    violationLayout->addWidget(m_efficiencyBar);
    m_violationsFilter = new ViolationsFilterWidget(m_violationPanel);
    violationLayout->addWidget(m_violationsFilter);
    violationLayout->addStretch();
    m_violationPanel->setFixedHeight(104);
    m_violationPanel->hide();
    root->addWidget(m_violationPanel);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName(QStringLiteral("privacy"));
    m_statusLabel->setWordWrap(true);
    root->addWidget(m_statusLabel);

    // utils/LoadingStatusDialog.qml equivalent.
    m_loadingDialog = new QDialog(this, Qt::FramelessWindowHint | Qt::Tool);
    m_loadingDialog->setModal(false);
    m_loadingDialog->setFixedSize(220, 60);
    m_loadingDialog->setStyleSheet(
        QStringLiteral("QDialog { background: #24272e; border: 1px solid #414248; }"));
    auto *loadingLayout = new QVBoxLayout(m_loadingDialog);
    m_loadingLabel = new QLabel(m_loadingDialog);
    m_loadingLabel->setAlignment(Qt::AlignCenter);
    m_loadingLabel->setStyleSheet(QStringLiteral("color: #9fa5ae;"));
    loadingLayout->addWidget(m_loadingLabel);

    m_playbackTimer = new QTimer(this);
    m_playbackTimer->setInterval(800);
    connect(m_playbackTimer, &QTimer::timeout, this, &HistoryView::onPlaybackTick);

    connect(m_dayCombo, &QComboBox::currentIndexChanged, this, &HistoryView::onDayChanged);

    connect(&m_connection, &ViewerConnection::historyDaysReceived, this, &HistoryView::onDaysReceived);
    connect(&m_connection, &ViewerConnection::historyFramesReceived, this,
            &HistoryView::onFramesReceived);
    connect(&m_connection, &ViewerConnection::historyFrameReceived, this,
            &HistoryView::onFrameReceived);
    connect(&m_connection, &ViewerConnection::historyActivityReceived, this,
            &HistoryView::onActivityReceived);
    connect(&m_connection, &ViewerConnection::historyAppSegmentsReceived, this,
            &HistoryView::onAppSegmentsReceived);
    connect(&m_connection, &ViewerConnection::historyRunningApplicationsReceived, this,
            &HistoryView::onRunningAppsReceived);
    connect(&m_connection, &ViewerConnection::historyCategoriesReceived, this,
            &HistoryView::onCategoriesReceived);
    connect(&m_connection, &ViewerConnection::historyKeystrokesReceived, this,
            &HistoryView::onKeystrokesReceived);
    connect(&m_connection, &ViewerConnection::historyError, this, &HistoryView::onHistoryError);
}

void HistoryView::setDevices(const QHash<quint32, QString> &deviceNames,
                             const QHash<quint32, QList<quint32>> &deviceMonitorStreams,
                             const QHash<quint32, QString> &monitorNames)
{
    m_deviceNames = deviceNames;
    m_deviceMonitorStreams = deviceMonitorStreams;
    m_monitorNames = monitorNames;

    if (m_currentDeviceKey != 0 && deviceNames.contains(m_currentDeviceKey)) {
        // Still a known device -- just refresh the strip in case its
        // monitor list changed (a screen was added/removed).
        rebuildMonitorStrip();
        return;
    }
    // Either nothing selected yet, or the previously selected device is
    // gone -- fall back to whichever device comes first.
    m_currentDeviceKey = deviceNames.isEmpty() ? 0 : deviceNames.cbegin().key();
    if (m_currentDeviceKey != 0) {
        switchDevice(m_currentDeviceKey);
    }
}

void HistoryView::activate()
{
    if (m_activated || m_currentDeviceKey == 0) {
        return;
    }
    m_activated = true;
    switchDevice(m_currentDeviceKey);
}

void HistoryView::showLoadingDialog(const QString &message)
{
    m_loadingLabel->setText(message);
    const QPoint center = mapToGlobal(rect().center()) - QPoint(m_loadingDialog->width() / 2,
                                                                m_loadingDialog->height() / 2);
    m_loadingDialog->move(center);
    m_loadingDialog->show();
}

void HistoryView::hideLoadingDialog()
{
    m_loadingDialog->hide();
}

void HistoryView::openForDevice(quint32 deviceKey, const QString &day)
{
    const QString targetDay =
        day.isEmpty() ? QDate::currentDate().toString(QStringLiteral("yyyyMMdd")) : day;
    if (deviceKey != m_currentDeviceKey || m_dayCombo->count() == 0) {
        m_pendingJumpDay = targetDay;
        m_activated = true;
        switchDevice(deviceKey);
        return;
    }
    // Already on this device with days loaded -- jump immediately instead
    // of waiting for a days response that isn't coming.
    const int index = m_dayCombo->findText(targetDay);
    if (index >= 0) {
        m_dayCombo->setCurrentIndex(index);
    }
}

quint32 HistoryView::currentStreamId() const
{
    const QList<quint32> streams = m_deviceMonitorStreams.value(m_currentDeviceKey);
    return streams.isEmpty() ? 0 : streams.first();
}

void HistoryView::rebuildMonitorStrip()
{
    QLayoutItem *item = nullptr;
    while ((item = m_monitorStripLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    m_monitorPreviewLabels.clear();

    for (quint32 streamId : m_deviceMonitorStreams.value(m_currentDeviceKey)) {
        auto *cell = new QWidget(m_monitorStripContainer);
        auto *cellLayout = new QVBoxLayout(cell);
        cellLayout->setContentsMargins(0, 0, 0, 0);
        cellLayout->setSpacing(2);

        auto *preview = new QLabel(cell);
        preview->setAlignment(Qt::AlignCenter);
        preview->setStyleSheet(QStringLiteral("background: #05080f; border: 1px solid #4b4e57;"));
        preview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        cellLayout->addWidget(preview, 1);

        auto *caption = new QLabel(m_monitorNames.value(streamId, QStringLiteral("Monitor %1")
                                                                       .arg(streamId)),
                                   cell);
        caption->setAlignment(Qt::AlignCenter);
        caption->setStyleSheet(QStringLiteral("color: #9fa5ae;"));
        cellLayout->addWidget(caption);

        m_monitorPreviewLabels.insert(streamId, preview);
        m_monitorStripLayout->addWidget(cell);
    }
    m_monitorStripLayout->addStretch();
    // The strip's viewport height isn't reliable yet the first time this
    // runs (same deferred-layout issue as DeviceDetailView's monitor
    // stacking) -- defer one event-loop turn.
    QTimer::singleShot(0, this, &HistoryView::resizeMonitorStripToFit);
}

void HistoryView::resizeMonitorStripToFit()
{
    if (m_monitorPreviewLabels.isEmpty() || !m_monitorStripArea) {
        return;
    }
    // Real Kickidler behavior (confirmed from the extracted QML,
    // __historyVideoSelf__: `width: __historyVideoSelf__.width /
    // videoFrames.length`): screens divide the available width evenly, no
    // scrolling, as long as there are few enough of them. We keep that for
    // the common case, but floor each screen's width so it never gets too
    // small to read -- past that floor the total width exceeds the
    // viewport and the strip scrolls horizontally (the scrollbar the user
    // explicitly asked for, for when a device has many screens).
    constexpr int kCaptionAndSpacing = 22;
    constexpr int kMargins = 8;
    constexpr int kMinPreviewWidth = 220;
    const int count = m_monitorPreviewLabels.size();
    // Capped at both ends -- floor so a tiny window doesn't crush the
    // preview to nothing, ceiling so this reactive sizing (it feeds off
    // the viewport's OWN current height, computed after a previous pass
    // already gave it generous space) can't lock in a minimumSize so tall
    // that the fixed-height rows below it (buttons, timeline, toggle) no
    // longer fit in the window at all -- exactly what was happening
    // before this cap: the controls were being pushed entirely past the
    // bottom edge, with no scroll area to reach them.
    const int availableHeight = qBound(
        160, m_monitorStripArea->viewport()->height() - kCaptionAndSpacing - kMargins, 420);
    const int evenWidth = (m_monitorStripArea->viewport()->width() - kMargins) / qMax(1, count);
    // No aspect-ratio cap on the width -- the real behavior (confirmed
    // above) is a plain width/count split, full stop. Capping width at
    // availableHeight*16/9 (removed) left a single monitor stuck at a
    // fixed ~16:9 box with a huge dead area to its right instead of
    // actually filling the strip, since evenWidth (the whole viewport, for
    // count=1) was almost always wider than that cap. The video label
    // itself already letterboxes via Qt::KeepAspectRatio when scaling the
    // frame in, so a wide box with a shorter 16:9 image centered inside it
    // is the correct (and real) look, not a bug to "fix" by shrinking the box.
    const int previewWidth = qMax(kMinPreviewWidth, evenWidth);
    for (QLabel *preview : std::as_const(m_monitorPreviewLabels)) {
        preview->setMinimumSize(previewWidth, availableHeight);
    }
}

void HistoryView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeMonitorStripToFit();
    if (m_toggleAppsButton && m_monitorStripArea) {
        const QSize hint = m_toggleAppsButton->sizeHint();
        m_toggleAppsButton->setGeometry(m_monitorStripArea->width() - hint.width() - 8, 8,
                                        hint.width(), hint.height());
        m_toggleAppsButton->raise();
    }
}

void HistoryView::switchDevice(quint32 deviceKey)
{
    m_playbackTimer->stop();
    m_playButton->setText(QStringLiteral("▶"));
    m_currentDeviceKey = deviceKey;
    m_employeeLabel->setText(m_deviceNames.value(deviceKey, QStringLiteral("Employee %1").arg(deviceKey)));
    rebuildMonitorStrip();

    m_dayCombo->clear();
    m_allDays.clear();
    m_periodStart = QDate();
    m_periodEnd = QDate();
    m_timestamps.clear();
    m_timeline->setTimestamps({});
    m_timeAxis->setRange(0, 0);
    m_timeLabel->clear();
    m_runningApps.clear();
    m_appSegments.clear();
    m_activitySamples.clear();
    m_keystrokeEntries.clear();
    m_runningAppsList->clear();
    m_textLog->setRowCount(0);
    m_violationsFilter->setRows({}, 0, 0);

    const quint32 streamId = currentStreamId();
    if (streamId == 0) {
        return;
    }
    m_statusLabel->setText(QStringLiteral("Se incarca zilele disponibile..."));
    showLoadingDialog(QStringLiteral("Downloading..."));
    m_connection.requestHistoryDays(streamId);
    m_connection.requestCategories(streamId);
}

void HistoryView::applyPeriodFilter()
{
    m_dayCombo->blockSignals(true);
    const QString previousDay = m_dayCombo->currentText();
    m_dayCombo->clear();
    for (const QString &day : std::as_const(m_allDays)) {
        const QDate date = QDate::fromString(day, QStringLiteral("yyyyMMdd"));
        if (m_periodStart.isValid() && date.isValid() && date < m_periodStart) {
            continue;
        }
        if (m_periodEnd.isValid() && date.isValid() && date > m_periodEnd) {
            continue;
        }
        m_dayCombo->addItem(day);
    }
    const int found = m_dayCombo->findText(previousDay);
    m_dayCombo->setCurrentIndex(found >= 0 ? found : (m_dayCombo->count() > 0 ? 0 : -1));
    m_dayCombo->blockSignals(false);
    if (m_dayCombo->currentIndex() >= 0) {
        refreshDayDependentData();
    }
}

void HistoryView::onDayChanged(int index)
{
    m_playbackTimer->stop();
    m_playButton->setText(QStringLiteral("▶"));
    m_timestamps.clear();
    m_timeline->setTimestamps({});
    for (QLabel *preview : std::as_const(m_monitorPreviewLabels)) {
        preview->clear();
    }
    m_timeLabel->clear();
    if (index < 0) {
        return;
    }
    refreshDayDependentData();
}

void HistoryView::refreshDayDependentData()
{
    const quint32 streamId = currentStreamId();
    const QString day = m_dayCombo->currentText();
    if (day.isEmpty()) {
        return;
    }
    m_statusLabel->setText(QStringLiteral("Se incarca momentele capturate..."));
    showLoadingDialog(QStringLiteral("Downloading..."));
    m_connection.requestHistoryFrames(streamId, day);
    m_connection.requestHistoryActivity(streamId, day);
    m_connection.requestHistoryAppSegments(streamId, day);
    m_connection.requestRunningApplications(streamId, day);
    m_connection.requestKeystrokes(streamId, day);
}

void HistoryView::onTimelineMoved(int index)
{
    requestFrameAt(index);
}

void HistoryView::requestFrameAt(int index)
{
    if (index < 0 || index >= m_timestamps.size()) {
        return;
    }
    const qint64 timestampMs = m_timestamps.at(index);
    m_timeLabel->setText(QStringLiteral("( %1 )").arg(
        QDateTime::fromMSecsSinceEpoch(timestampMs).toString(QStringLiteral("dd.MM.yyyy HH:mm:ss"))));
    // Every screen of the device is shown at once (see rebuildMonitorStrip),
    // so a frame is fetched for each of its monitor streams, not just the
    // one used for the non-video queries (currentStreamId()).
    for (quint32 streamId : m_deviceMonitorStreams.value(m_currentDeviceKey)) {
        m_connection.requestHistoryFrame(streamId, timestampMs);
    }
    updateTextLogHighlight(timestampMs);
    m_timeline->setCurrentIndex(index);
    m_timeAxis->setCurrentPositionMs(timestampMs);
    m_activityBar->setCurrentPositionMs(timestampMs);
    m_efficiencyBar->setCurrentPositionMs(timestampMs);
    m_violationsFilter->setCurrentPositionMs(timestampMs);
}

void HistoryView::onPlayClicked()
{
    if (m_playbackTimer->isActive()) {
        m_playbackTimer->stop();
        m_playButton->setText(QStringLiteral("▶"));
        return;
    }
    if (m_timestamps.isEmpty()) {
        return;
    }
    if (m_timeline->currentIndex() >= m_timeline->count() - 1) {
        requestFrameAt(0);
    }
    m_playButton->setText(QStringLiteral("⏸"));
    m_playbackTimer->start();
}

void HistoryView::onPlaybackTick()
{
    const int next = m_timeline->currentIndex() + 1;
    if (next > m_timeline->count() - 1) {
        m_playbackTimer->stop();
        m_playButton->setText(QStringLiteral("▶"));
        return;
    }
    requestFrameAt(next);
}

void HistoryView::onToggleViolationPanel()
{
    // Exact wording from the real viewer (History.qml's panelViolation
    // button): "Open violation panel" / "Hide violation panel", starts
    // closed.
    const bool visible = !m_violationPanel->isVisible();
    m_violationPanel->setVisible(visible);
    m_violationToggleButton->setText(visible ? QStringLiteral("▼ Hide violation panel")
                                             : QStringLiteral("▲ Open violation panel"));
}

void HistoryView::onSpeedChanged(int index)
{
    // Matches m_speedCombo's real 0.5x/0.75x/1x/2x/4x/8x/16x options --
    // interval scales inversely with the multiplier, based off 1x = 800ms.
    static const double multipliers[] = {0.5, 0.75, 1.0, 2.0, 4.0, 8.0, 16.0};
    const double multiplier = multipliers[qBound(0, index, 6)];
    m_playbackTimer->setInterval(qMax(20, static_cast<int>(800.0 / multiplier)));
}

void HistoryView::onKeylogTableClicked()
{
    // m_textLog is a persistent widget (so its scroll position/selection
    // survive being reopened) -- reparent it into a throwaway dialog and
    // back out again on close, rather than owning a QTableWidget per open.
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle(QStringLiteral("Keylogger"));
    dialog->resize(760, 420);
    auto *layout = new QVBoxLayout(dialog);
    layout->addWidget(m_textLog);

    // Matches the real viewer's KeyloggerExporter.qml (CSV/XLSX export) --
    // both formats are real now (see writeMinimalXlsx below).
    auto *exportRow = new QHBoxLayout;
    auto *exportCsvButton = new QPushButton(QStringLiteral("Export CSV"), dialog);
    auto *exportXlsxButton = new QPushButton(QStringLiteral("Export XLSX"), dialog);
    exportRow->addWidget(exportCsvButton);
    exportRow->addWidget(exportXlsxButton);
    exportRow->addStretch();
    layout->addLayout(exportRow);

    static const QStringList kHeaders = {QStringLiteral("Date"), QStringLiteral("Pressing period"),
                                         QStringLiteral("Application"), QStringLiteral("Title"),
                                         QStringLiteral("Keystrokes")};
    auto collectRows = [this]() {
        QList<QStringList> rows;
        rows.append(kHeaders);
        for (int row = m_textLog->rowCount() - 1; row >= 0; --row) {
            QStringList fields;
            for (int col = 0; col < 5; ++col) {
                fields << (m_textLog->item(row, col) ? m_textLog->item(row, col)->text() : QString());
            }
            rows.append(fields);
        }
        return rows;
    };

    connect(exportCsvButton, &QPushButton::clicked, this, [this, dialog, collectRows]() {
        const QString path = QFileDialog::getSaveFileName(
            dialog, QStringLiteral("Exporta keylogger"),
            QStringLiteral("keylogger_%1.csv").arg(m_dayCombo->currentText()),
            QStringLiteral("CSV (*.csv)"));
        if (path.isEmpty()) {
            return;
        }
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(dialog, QStringLiteral("Export"),
                                 QStringLiteral("Nu s-a putut scrie fisierul."));
            return;
        }
        QTextStream out(&file);
        const QList<QStringList> rows = collectRows();
        for (const QStringList &row : rows) {
            QStringList escaped;
            for (QString field : row) {
                field.replace(QLatin1Char('"'), QStringLiteral("\"\""));
                escaped << QStringLiteral("\"%1\"").arg(field);
            }
            out << escaped.join(QLatin1Char(',')) << "\n";
        }
    });
    connect(exportXlsxButton, &QPushButton::clicked, this, [this, dialog, collectRows]() {
        const QString path = QFileDialog::getSaveFileName(
            dialog, QStringLiteral("Exporta keylogger"),
            QStringLiteral("keylogger_%1.xlsx").arg(m_dayCombo->currentText()),
            QStringLiteral("Excel (*.xlsx)"));
        if (path.isEmpty()) {
            return;
        }
        if (!writeMinimalXlsx(path, collectRows())) {
            QMessageBox::warning(dialog, QStringLiteral("Export"),
                                 QStringLiteral("Nu s-a putut scrie fisierul."));
        }
    });

    m_textLog->show();
    connect(dialog, &QDialog::finished, this, [this, dialog]() {
        m_textLog->setParent(this);
        m_textLog->hide();
        dialog->deleteLater();
    });
    dialog->show();
}

void HistoryView::onExportVideoClicked()
{
    // See m_exportVideoButton's comment: PNG sequence, not an actual video
    // file (no encoder in this project's dependencies).
    if (m_timestamps.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("Nimic de exportat pentru aceasta zi."));
        return;
    }
    // VideoSaverSelector.qml's quality picker (Low/Medium/High).
    QDialog qualityDialog(this);
    qualityDialog.setWindowTitle(QStringLiteral("Export imagini"));
    auto *qualityLayout = new QVBoxLayout(&qualityDialog);
    qualityLayout->addWidget(new QLabel(QStringLiteral("Calitate:"), &qualityDialog));
    auto *qualityCombo = new QComboBox(&qualityDialog);
    qualityCombo->addItem(QStringLiteral("Low"), 50);
    qualityCombo->addItem(QStringLiteral("Medium"), 75);
    qualityCombo->addItem(QStringLiteral("High"), 100);
    qualityCombo->setCurrentIndex(2);
    qualityLayout->addWidget(qualityCombo);
    auto *qualityButtons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &qualityDialog);
    connect(qualityButtons, &QDialogButtonBox::accepted, &qualityDialog, &QDialog::accept);
    connect(qualityButtons, &QDialogButtonBox::rejected, &qualityDialog, &QDialog::reject);
    qualityLayout->addWidget(qualityButtons);
    if (qualityDialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString dir = QFileDialog::getExistingDirectory(
        this, QStringLiteral("Alege folderul pentru exportul imaginilor"));
    if (dir.isEmpty()) {
        return;
    }
    m_videoExport.active = true;
    m_videoExport.directory = dir;
    m_videoExport.streamId = currentStreamId();
    m_videoExport.pending = m_timestamps;
    m_videoExport.total = m_timestamps.size();
    m_videoExport.qualityPercent = qualityCombo->currentData().toInt();
    m_exportVideoButton->setEnabled(false);
    exportNextVideoFrame();
}

void HistoryView::exportNextVideoFrame()
{
    if (m_videoExport.pending.isEmpty()) {
        m_videoExport.active = false;
        m_exportVideoButton->setEnabled(true);
        m_statusLabel->setText(
            QStringLiteral("Export finalizat: %1 imagini in %2").arg(m_videoExport.total).arg(m_videoExport.directory));
        return;
    }
    const qint64 timestampMs = m_videoExport.pending.first();
    m_statusLabel->setText(QStringLiteral("Se exporta %1/%2...")
                               .arg(m_videoExport.total - m_videoExport.pending.size() + 1)
                               .arg(m_videoExport.total));
    m_connection.requestHistoryFrame(m_videoExport.streamId, timestampMs);
}

void HistoryView::onToggleRunningApps()
{
    // Matches History.qml's appButton: closed by default (see the
    // constructor's m_runningAppsList->hide()), arrow flips direction.
    const bool visible = !m_runningAppsList->isVisible();
    m_runningAppsList->setVisible(visible);
    m_toggleAppsButton->setText(visible ? QStringLiteral("Running applications ▶")
                                        : QStringLiteral("Running applications ◀"));
}

void HistoryView::onChangeSettingsClicked()
{
    // Employee/Period/Time step -- NOT a category editor (that moved to
    // EfficiencyCategoryButton, used directly in the Running Applications
    // list and in DeviceDetailView's Programs/Web pages panel, so there is
    // exactly one place category gets edited).
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle(QStringLiteral("Change range and employee"));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMinimumWidth(380);
    auto *layout = new QVBoxLayout(dialog);

    auto *description = new QLabel(QStringLiteral("Change range and employee"), dialog);
    description->setStyleSheet(QStringLiteral("font-weight: 700;"));
    layout->addWidget(description);

    auto *form = new QFormLayout;

    auto *employeeCombo = new QComboBox(dialog);
    for (auto it = m_deviceNames.cbegin(); it != m_deviceNames.cend(); ++it) {
        employeeCombo->addItem(it.value(), it.key());
    }
    const int currentEmployeeIndex = employeeCombo->findData(m_currentDeviceKey);
    if (currentEmployeeIndex >= 0) {
        employeeCombo->setCurrentIndex(currentEmployeeIndex);
    }
    form->addRow(QStringLiteral("Employee:"), employeeCombo);

    // Period has two modes in the real Kickidler viewer (HistoryChoicePanel.qml,
    // panelState.type: "recent" or "custom") -- a quick relative range (last
    // day/week/month/...) or an explicit date range. Mirrored here as a kind
    // selector that swaps which value editor is visible.
    auto *periodKindCombo = new QComboBox(dialog);
    periodKindCombo->addItem(QStringLiteral("Recent"), QStringLiteral("recent"));
    periodKindCombo->addItem(QStringLiteral("Custom"), QStringLiteral("custom"));
    form->addRow(QStringLiteral("Period:"), periodKindCombo);

    auto *recentRangeCombo = new QComboBox(dialog);
    recentRangeCombo->addItem(QStringLiteral("Last day"), QStringLiteral("d"));
    recentRangeCombo->addItem(QStringLiteral("Last week"), QStringLiteral("w"));
    recentRangeCombo->addItem(QStringLiteral("Last month"), QStringLiteral("m"));
    recentRangeCombo->addItem(QStringLiteral("Last quarter"), QStringLiteral("q"));
    recentRangeCombo->addItem(QStringLiteral("Last year"), QStringLiteral("y"));

    auto *periodRow = new QWidget(dialog);
    auto *periodLayout = new QHBoxLayout(periodRow);
    periodLayout->setContentsMargins(0, 0, 0, 0);
    auto *periodStartEdit = new QDateEdit(m_periodStart.isValid() ? m_periodStart : QDate::currentDate(),
                                          periodRow);
    periodStartEdit->setCalendarPopup(true);
    auto *periodEndEdit = new QDateEdit(m_periodEnd.isValid() ? m_periodEnd : QDate::currentDate(),
                                        periodRow);
    periodEndEdit->setCalendarPopup(true);
    auto *periodDash = new QLabel(QStringLiteral("—"), periodRow);
    periodLayout->addWidget(recentRangeCombo);
    periodLayout->addWidget(periodStartEdit);
    periodLayout->addWidget(periodDash);
    periodLayout->addWidget(periodEndEdit);
    form->addRow(QString(), periodRow);

    auto updatePeriodRowVisibility = [periodKindCombo, recentRangeCombo, periodStartEdit,
                                      periodEndEdit, periodDash]() {
        const bool recent = periodKindCombo->currentData().toString() == QStringLiteral("recent");
        recentRangeCombo->setVisible(recent);
        periodStartEdit->setVisible(!recent);
        periodEndEdit->setVisible(!recent);
        periodDash->setVisible(!recent);
    };
    connect(periodKindCombo, &QComboBox::currentIndexChanged, dialog, updatePeriodRowVisibility);
    updatePeriodRowVisibility();

    auto *timeStepCombo = new QComboBox(dialog);
    const QList<QPair<QString, qint64>> timeSteps = {
        {QStringLiteral("1 second"), 1000}, {QStringLiteral("5 seconds"), 5000},
        {QStringLiteral("10 seconds"), 10000}, {QStringLiteral("20 seconds"), 20000},
        {QStringLiteral("30 seconds"), 30000}, {QStringLiteral("1 minute"), 60000},
        {QStringLiteral("5 minutes"), 5 * 60000}, {QStringLiteral("10 minutes"), 10 * 60000},
        {QStringLiteral("20 minutes"), 20 * 60000}, {QStringLiteral("30 minutes"), 30 * 60000},
        {QStringLiteral("1 hour"), 3600000}, {QStringLiteral("2 hours"), 2 * 3600000},
    };
    for (const auto &step : timeSteps) {
        timeStepCombo->addItem(step.first, step.second);
    }
    const int currentStepIndex = timeStepCombo->findData(m_timeStepMs);
    timeStepCombo->setCurrentIndex(currentStepIndex >= 0 ? currentStepIndex : 6 /* 5 minutes */);
    form->addRow(QStringLiteral("Time step:"), timeStepCombo);

    layout->addLayout(form);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::close);
    connect(buttons, &QDialogButtonBox::accepted, this,
            [this, dialog, employeeCombo, periodKindCombo, recentRangeCombo, periodStartEdit,
             periodEndEdit, timeStepCombo]() {
                const quint32 newDeviceKey = employeeCombo->currentData().toUInt();
                if (periodKindCombo->currentData().toString() == QStringLiteral("recent")) {
                    m_periodEnd = QDate::currentDate();
                    const QString recentType = recentRangeCombo->currentData().toString();
                    if (recentType == QStringLiteral("d")) {
                        m_periodStart = m_periodEnd.addDays(-1);
                    } else if (recentType == QStringLiteral("w")) {
                        m_periodStart = m_periodEnd.addDays(-7);
                    } else if (recentType == QStringLiteral("m")) {
                        m_periodStart = m_periodEnd.addMonths(-1);
                    } else if (recentType == QStringLiteral("q")) {
                        m_periodStart = m_periodEnd.addMonths(-3);
                    } else {
                        m_periodStart = m_periodEnd.addYears(-1);
                    }
                } else {
                    m_periodStart = periodStartEdit->date();
                    m_periodEnd = periodEndEdit->date();
                }
                m_timeStepMs = timeStepCombo->currentData().toLongLong();
                // Confirmed in the real viewer's ChartsModel.qml: the
                // activity chart's own bucket resolution ("granula") is
                // NOT the raw Time step -- it's `max(timeStep/5, 60s)`,
                // i.e. never finer than 60 seconds even at Time step=1s.
                // Using the raw value made every bucket hold at most one
                // sample at fine time steps, so bars came out binary
                // (all-or-nothing) instead of a smooth gradient.
                m_activityBar->setBucketMs(qMax<qint64>(m_timeStepMs / 5, 60000));
                m_muteButton->setVisible(m_timeStepMs <= 1000);
                if (newDeviceKey != 0 && newDeviceKey != m_currentDeviceKey) {
                    switchDevice(newDeviceKey);
                } else {
                    applyPeriodFilter();
                }
                dialog->close();
            });
    layout->addWidget(buttons);
    dialog->show();
}

void HistoryView::onDaysReceived(quint32 streamId, const QStringList &days)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_allDays = days;
    if (days.isEmpty()) {
        m_dayCombo->clear();
        m_statusLabel->setText(QStringLiteral("Niciun istoric inregistrat inca pentru acest angajat."));
        hideLoadingDialog();
        return;
    }
    m_statusLabel->clear();
    hideLoadingDialog();
    applyPeriodFilter();

    if (!m_pendingJumpDay.isEmpty()) {
        const int index = m_dayCombo->findText(m_pendingJumpDay);
        if (index >= 0) {
            m_dayCombo->setCurrentIndex(index);
        }
        m_pendingJumpDay.clear();
    }
}

void HistoryView::onFramesReceived(quint32 streamId, const QString &day,
                                   const QList<qint64> &timestamps)
{
    Q_UNUSED(day)
    if (streamId != currentStreamId()) {
        return;
    }
    m_timestamps = timestamps;
    m_timeline->setTimestamps(timestamps);
    m_timeAxis->setRange(timestamps.isEmpty() ? 0 : timestamps.first(),
                         timestamps.isEmpty() ? 0 : timestamps.last());
    if (timestamps.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("Nicio captura pentru aceasta zi."));
        hideLoadingDialog();
        return;
    }
    m_statusLabel->clear();
    hideLoadingDialog();
    requestFrameAt(timestamps.size() - 1);
}

void HistoryView::onFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image)
{
    if (m_videoExport.active && streamId == m_videoExport.streamId
        && !m_videoExport.pending.isEmpty() && m_videoExport.pending.first() == timestampMs) {
        m_videoExport.pending.removeFirst();
        const QString fileName = QDateTime::fromMSecsSinceEpoch(timestampMs)
                                     .toString(QStringLiteral("HH-mm-ss"));
        const QImage toSave = m_videoExport.qualityPercent >= 100
            ? image
            : image.scaled(image.size() * m_videoExport.qualityPercent / 100,
                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
        toSave.save(QStringLiteral("%1/%2.png").arg(m_videoExport.directory, fileName));
        exportNextVideoFrame();
    }

    QLabel *preview = m_monitorPreviewLabels.value(streamId, nullptr);
    if (!preview) {
        return; // not one of the current device's monitors
    }
    QPixmap pixmap = QPixmap::fromImage(image);
    if (preview->width() > 0 && preview->height() > 0) {
        pixmap = pixmap.scaled(preview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    preview->setPixmap(pixmap);
}

void HistoryView::onActivityReceived(quint32 streamId, const QString &day,
                                     const QList<HistoryActivitySample> &samples)
{
    Q_UNUSED(day)
    if (streamId != currentStreamId() || m_timestamps.isEmpty()) {
        return;
    }
    m_activitySamples = samples;
    m_activityBar->setSamples(samples, m_timestamps.first(), m_timestamps.last());
    refreshViolationsFilter();
}

void HistoryView::onAppSegmentsReceived(quint32 streamId, const QString &day,
                                        const QList<HistoryAppSegment> &segments)
{
    Q_UNUSED(day)
    if (streamId != currentStreamId()) {
        return;
    }
    m_appSegments = segments;
    refreshEfficiencyBar();
    refreshViolationsFilter();
}

void HistoryView::refreshEfficiencyBar()
{
    if (m_timestamps.isEmpty()) {
        return;
    }
    m_efficiencyBar->setSegments(m_appSegments, m_categories, m_timestamps.first(),
                                 m_timestamps.last());
}

void HistoryView::refreshViolationsFilter()
{
    if (m_timestamps.isEmpty()) {
        return;
    }
    const qint64 rangeStart = m_timestamps.first();
    const qint64 rangeEnd = m_timestamps.last();

    // Row 1: non-productive app usage -- straight from the same category
    // data EfficiencyBarWidget already uses, just filtered to one category
    // and reshaped into (start,end) spans.
    ViolationRow nonProductive;
    nonProductive.label = QStringLiteral("Aplicații neproductive");
    for (const HistoryAppSegment &segment : std::as_const(m_appSegments)) {
        const QString category = m_categories.value(segment.application, QStringLiteral("none"));
        if (category == QStringLiteral("unproductive")) {
            nonProductive.ranges.append({segment.startMs, segment.endMs});
        }
    }

    // Row 2: sustained inactivity -- a run of consecutive zero-activity
    // samples lasting at least kInactivityThresholdMs. HistoryActivitySample
    // doesn't carry an explicit "no more samples after this" marker, so a
    // run is closed either by a nonzero sample or by reaching the end of
    // the list.
    constexpr qint64 kInactivityThresholdMs = 5 * 60 * 1000;
    ViolationRow inactivity;
    inactivity.label = QStringLiteral("Inactivitate");
    qint64 runStart = -1;
    qint64 lastZeroTimestamp = -1;
    for (const HistoryActivitySample &sample : std::as_const(m_activitySamples)) {
        if (sample.inputEvents <= 0) {
            if (runStart < 0) {
                runStart = sample.timestampMs;
            }
            lastZeroTimestamp = sample.timestampMs;
        } else if (runStart >= 0) {
            if (lastZeroTimestamp - runStart >= kInactivityThresholdMs) {
                inactivity.ranges.append({runStart, lastZeroTimestamp});
            }
            runStart = -1;
        }
    }
    if (runStart >= 0 && lastZeroTimestamp - runStart >= kInactivityThresholdMs) {
        inactivity.ranges.append({runStart, lastZeroTimestamp});
    }

    m_violationsFilter->setRows({nonProductive, inactivity}, rangeStart, rangeEnd);
}

void HistoryView::onRunningAppsReceived(quint32 streamId, const QString &day,
                                        const QList<HistoryAppUsage> &applications)
{
    Q_UNUSED(day)
    if (streamId != currentStreamId()) {
        return;
    }
    m_runningApps = applications;
    rebuildRunningAppsList();
}

void HistoryView::rebuildRunningAppsList()
{
    m_runningAppsList->clear();
    const quint32 streamId = currentStreamId();
    for (const HistoryAppUsage &usage : std::as_const(m_runningApps)) {
        const QString category = m_categories.value(
            usage.application, usage.category.isEmpty() ? QStringLiteral("none") : usage.category);
        auto *item = new QListWidgetItem(m_runningAppsList);
        auto *row = new QWidget(m_runningAppsList);
        auto *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(4, 2, 4, 2);
        auto *label = new QLabel(QStringLiteral("%1  —  %2")
                                     .arg(usage.application, formatDuration(usage.totalMs)),
                                 row);
        label->setStyleSheet(QStringLiteral("color: %1;")
                                 .arg(EfficiencyCategoryButton::color(category).name()));
        rowLayout->addWidget(label, 1);
        auto *categoryButton = new EfficiencyCategoryButton(category, row);
        const QString application = usage.application;
        connect(categoryButton, &EfficiencyCategoryButton::categoryChanged, this,
                [this, streamId, application](const QString &newCategory) {
                    m_categories[application] = newCategory;
                    m_connection.setAppCategory(streamId, application, newCategory);
                    refreshEfficiencyBar();
                    refreshViolationsFilter();
                    rebuildRunningAppsList();
                });
        rowLayout->addWidget(categoryButton);
        item->setSizeHint(row->sizeHint());
        m_runningAppsList->setItemWidget(item, row);
    }
}

void HistoryView::onCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_categories = categories;
    refreshEfficiencyBar();
    refreshViolationsFilter();
    rebuildRunningAppsList();
}

void HistoryView::onKeystrokesReceived(quint32 streamId, const QString &day,
                                       const QList<HistoryKeystrokeEntry> &entries)
{
    Q_UNUSED(day)
    if (streamId != currentStreamId()) {
        return;
    }
    m_keystrokeEntries = entries;
    const QList<KeylogRow> rows = groupKeystrokeEntries(entries);

    m_textLog->setRowCount(rows.size());
    // Newest first, same as the real keylogger table (ListView.BottomToTop).
    for (int i = 0; i < rows.size(); ++i) {
        const KeylogRow &row = rows.at(rows.size() - 1 - i);
        const QString date =
            QDateTime::fromMSecsSinceEpoch(row.fromMs).toString(QStringLiteral("dd.MM.yyyy"));
        const QString period = QStringLiteral("%1 - %2")
                                   .arg(QDateTime::fromMSecsSinceEpoch(row.fromMs)
                                            .toString(QStringLiteral("HH:mm:ss")),
                                        QDateTime::fromMSecsSinceEpoch(row.toMs)
                                            .toString(QStringLiteral("HH:mm:ss")));
        m_textLog->setItem(i, 0, new QTableWidgetItem(date));
        m_textLog->setItem(i, 1, new QTableWidgetItem(period));
        m_textLog->setItem(i, 2, new QTableWidgetItem(QStringLiteral("--")));
        m_textLog->setItem(i, 3, new QTableWidgetItem(row.windowTitle));
        m_textLog->setItem(i, 4, new QTableWidgetItem(row.text));
    }
}

int HistoryView::nearestKeystrokeIndex(qint64 timestampMs) const
{
    if (m_keystrokeEntries.isEmpty()) {
        return -1;
    }
    int closestIndex = 0;
    qint64 closestDelta = qAbs(m_keystrokeEntries.first().timestampMs - timestampMs);
    for (int i = 1; i < m_keystrokeEntries.size(); ++i) {
        const qint64 delta = qAbs(m_keystrokeEntries.at(i).timestampMs - timestampMs);
        if (delta < closestDelta) {
            closestDelta = delta;
            closestIndex = i;
        }
    }
    return closestIndex;
}

void HistoryView::updateTextLogHighlight(qint64 timestampMs)
{
    const int closestIndex = nearestKeystrokeIndex(timestampMs);
    if (closestIndex < 0) {
        m_keylogTicker->clear();
        return;
    }

    // Compact ticker under the monitor strip, always kept in sync with the
    // timeline position regardless of Video/Text mode (item 7: typed text
    // should line up with what's on screen at that moment).
    const HistoryKeystrokeEntry &nearest = m_keystrokeEntries.at(closestIndex);
    m_keylogTicker->setText(QStringLiteral("%1  [%2]  %3")
                                .arg(QDateTime::fromMSecsSinceEpoch(nearest.timestampMs)
                                         .toString(QStringLiteral("HH:mm:ss")),
                                     nearest.windowTitle, nearest.text));

    if (!m_textLog->isVisible()) {
        return;
    }
    // Select+scroll to the table row (newest-first, see onKeystrokesReceived)
    // whose period contains -- or is nearest to -- the current timeline
    // position.
    const QList<KeylogRow> rows = groupKeystrokeEntries(m_keystrokeEntries);
    if (rows.isEmpty()) {
        return;
    }
    int nearestRow = 0;
    qint64 closestDelta = std::numeric_limits<qint64>::max();
    for (int i = 0; i < rows.size(); ++i) {
        const KeylogRow &row = rows.at(i);
        const qint64 delta = timestampMs < row.fromMs ? row.fromMs - timestampMs
            : timestampMs > row.toMs                  ? timestampMs - row.toMs
                                                        : 0;
        if (delta < closestDelta) {
            closestDelta = delta;
            nearestRow = i;
        }
    }
    const int tableRow = rows.size() - 1 - nearestRow; // rows are newest-first in the table
    m_textLog->selectRow(tableRow);
    m_textLog->scrollToItem(m_textLog->item(tableRow, 0));
}

void HistoryView::onHistoryError(quint32 streamId, const QString &message)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_statusLabel->setText(message);
    hideLoadingDialog();
}
