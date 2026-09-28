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
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QPainter>
#include <QSet>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollBar>
#include <QScrollArea>
#include <QSizePolicy>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTextStream>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWheelEvent>

#include <algorithm>
#include <array>
#include <cmath>
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

// Timeline/Activity/Efficiency/violations all used to span just
// [first captured timestamp, last captured timestamp], so a device that only
// recorded 5 minutes today stretched those 5 minutes across the whole bar --
// the real viewer's bar always spans the full day (00:00-24:00), with actual
// activity occupying only its real slice of that fixed width.
QPair<qint64, qint64> dayRangeMs(const QString &day)
{
    const QDate date = QDate::fromString(day, QStringLiteral("yyyyMMdd"));
    if (!date.isValid()) {
        return {0, 0};
    }
    const qint64 start = QDateTime(date, QTime(0, 0)).toMSecsSinceEpoch();
    return {start, start + 24 * 60 * 60 * 1000};
}

// History.qml's page background. Background.qml itself is compiled C++ (not
// in qml_real), but the anti-aliased rounded caps of sliderBar/bg_none.png
// were rendered against exactly this color.
const QColor kHistoryBackground(0x47, 0x48, 0x50);
const QColor kTickColor(0x54, 0x54, 0x5a);       // chart/TimeLine.qml
const QColor kGridColor(0x53, 0x53, 0x5b);       // chart/Grid.qml
const QColor kSeparatorDark(0x43, 0x44, 0x4c);   // utils/Chart.qml separator1
const QColor kSeparatorLight(0x4c, 0x4d, 0x54);
const QColor kActivityColor(0x7a, 0xa1, 0xe2);   // content/HistoLine.qml
const QColor kMarkerColor(0xf0, 0xe6, 0x8c);     // HistoryPlayerMarkerControl.qml: "khaki"

// Geometry of History.qml's sliderAndMeta block (125px) and chartsItem
// (opened to 186px), in page coordinates.
constexpr int kControlBlockHeight = 125;
constexpr int kChartsItemHeight = 186;
constexpr int kSliderBarY = 35;         // MultiSessionsSlider: ListView topMargin 35
constexpr int kSliderBarHeight = 4;     // SliderBar.qml __sliderBar
constexpr int kTimeLineY = 62;          // MultiSessionsSlider column, single session
constexpr int kTimeLineHighHeight = 16; // TimeLine.qml highLineHeight
constexpr int kTimeLineLowHeight = 12;  // TimeLine.qml lowLineHeight
constexpr int kChartLeft = 10;          // Filters anchors.leftMargin
constexpr int kChartBottomMargin = 15;  // Filters anchors.bottomMargin
constexpr int kLabelsLeft = 20;         // ExtraHeaders mainHeaderText leftMargin
constexpr int kRowHeight = 30;          // ExtraHeaders.rowHeight / HistoLine outer Item
constexpr int kHistogramHeight = 20;    // HistoLine.histogramHeight
constexpr int kProductivityHeight = 22; // Line.qml singleLineHeight (not a filter line)

void ensureHistoryFonts()
{
    static const bool loaded = [] {
        for (const QString &file : {QStringLiteral(":/fonts/Roboto-Regular.ttf"),
                                    QStringLiteral(":/fonts/Roboto-Medium.ttf"),
                                    QStringLiteral(":/fonts/Roboto-Bold.ttf")}) {
            QFontDatabase::addApplicationFont(file);
        }
        return true;
    }();
    Q_UNUSED(loaded)
}

// Fonts.rb_small_b: Roboto Bold 10px -- a real screenshot's
// "Friday, September 25, 2026" date label is exactly 134px wide, which is
// what Roboto Bold measures at 10px.
QFont smallBoldFont()
{
    QFont font(QStringLiteral("Roboto"));
    font.setPixelSize(10);
    font.setBold(true);
    return font;
}

// Fonts.rr_medium: Roboto Regular 12px ("Activity" measures 40px in the
// same screenshot).
QFont mediumFont()
{
    QFont font(QStringLiteral("Roboto"));
    font.setPixelSize(12);
    return font;
}

// QML BorderImage with horizontalTileMode: Repeat (and the default vertical
// Stretch): fixed left/right caps, middle slice tiled across.
void drawBorderImage(QPainter &painter, const QRectF &target, const QPixmap &pixmap, int borderLeft,
                     int borderRight)
{
    if (pixmap.isNull() || target.width() <= 0 || target.height() <= 0) {
        return;
    }
    const int height = qMax(1, qRound(target.height()));
    const QPixmap scaled = pixmap.height() == height
        ? pixmap
        : pixmap.scaled(pixmap.width(), height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    const qreal left = qMin<qreal>(borderLeft, target.width() / 2);
    const qreal right = qMin<qreal>(borderRight, target.width() - left);
    if (left > 0) {
        painter.drawPixmap(QRectF(target.left(), target.top(), left, height), scaled,
                           QRectF(0, 0, left, height));
    }
    if (right > 0) {
        painter.drawPixmap(QRectF(target.right() - right, target.top(), right, height), scaled,
                           QRectF(scaled.width() - right, 0, right, height));
    }
    const int middleWidth = scaled.width() - borderLeft - borderRight;
    const qreal targetMiddle = target.width() - left - right;
    if (middleWidth > 0 && targetMiddle > 0) {
        painter.drawTiledPixmap(QRectF(target.left() + left, target.top(), targetMiddle, height),
                                scaled.copy(borderLeft, 0, middleWidth, height));
    }
}

// Two 1px lines, darker then lighter -- the separator style Chart.qml's
// separator1 spells out, used for its HSeparator/VSeparator too.
void drawHorizontalSeparator(QPainter &painter, qreal x, qreal y, qreal width)
{
    painter.fillRect(QRectF(x, y, width, 1), kSeparatorDark);
    painter.fillRect(QRectF(x, y + 1, width, 1), kSeparatorLight);
}

void drawVerticalSeparator(QPainter &painter, qreal x, qreal y, qreal height)
{
    painter.fillRect(QRectF(x, y, 1, height), kSeparatorDark);
    painter.fillRect(QRectF(x + 1, y, 1, height), kSeparatorLight);
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

HistoryVideoHeader::HistoryVideoHeader(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(24);
}

void HistoryVideoHeader::setName(const QString &name)
{
    m_name = name;
    update();
}

void HistoryVideoHeader::setMoment(const QString &moment)
{
    m_moment = moment;
    update();
}

void HistoryVideoHeader::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x3f, 0x40, 0x47));
    painter.fillRect(QRect(0, 0, width(), 1), QColor(0x33, 0x34, 0x3a));
    painter.fillRect(QRect(0, 1, width(), 1), QColor(0x3b, 0x3c, 0x43));
    painter.fillRect(QRect(0, height() - 2, width(), 1), QColor(0x3b, 0x3c, 0x42));
    painter.fillRect(QRect(0, height() - 1, width(), 1), QColor(0x41, 0x42, 0x49));

    // name: Fonts.rr_medium_b, horizontally centered on its own;
    // currentMarker: Fonts.rm_medium_b, anchored 10px right of the name.
    QFont nameFont = mediumFont();
    nameFont.setBold(true);
    QFont markerFont(QStringLiteral("Roboto Medium"));
    markerFont.setPixelSize(12);
    markerFont.setBold(true);
    const QFontMetrics nameMetrics(nameFont);
    const int nameWidth = nameMetrics.horizontalAdvance(m_name);
    const int nameX = (width() - nameWidth) / 2;
    painter.setPen(Qt::white);
    painter.setFont(nameFont);
    painter.drawText(QRect(nameX, 0, nameWidth + 1, height()), Qt::AlignLeft | Qt::AlignVCenter, m_name);
    if (!m_moment.isEmpty()) {
        painter.setFont(markerFont);
        painter.drawText(QRect(nameX + nameWidth + 10, 0, width(), height()),
                         Qt::AlignLeft | Qt::AlignVCenter, m_moment);
    }
}

HistoryVideoStrip::HistoryVideoStrip(QWidget *parent)
    : QWidget(parent)
{
    // utils/Spinner.qml: spinner.png rotated by 45 degrees every 300 ms.
    m_spinnerTimer = new QTimer(this);
    m_spinnerTimer->setInterval(300);
    connect(m_spinnerTimer, &QTimer::timeout, this, [this] {
        m_spinnerAngle = (m_spinnerAngle + 45) % 360;
        update();
    });
}

void HistoryVideoStrip::setStreams(const QList<quint32> &streamIds)
{
    m_screens.clear();
    for (quint32 streamId : streamIds) {
        m_screens.append({streamId, {}, Status::Loading});
    }
    relayout();
}

void HistoryVideoStrip::setAllLoading()
{
    for (Screen &screen : m_screens) {
        screen.status = Status::Loading;
        screen.image = {};
    }
    relayout();
}

void HistoryVideoStrip::setFrame(quint32 streamId, const QImage &image)
{
    for (Screen &screen : m_screens) {
        if (screen.streamId == streamId) {
            screen.image = image;
            screen.status = image.isNull() ? Status::Offline : Status::Online;
        }
    }
    relayout();
}

void HistoryVideoStrip::setOffline(quint32 streamId)
{
    for (Screen &screen : m_screens) {
        if (screen.streamId == streamId) {
            screen.image = {};
            screen.status = Status::Offline;
        }
    }
    relayout();
}

void HistoryVideoStrip::setViewSize(const QSize &size)
{
    if (size != m_viewSize) {
        m_viewSize = size;
        relayout();
    }
}

QList<QRect> HistoryVideoStrip::screenRects() const
{
    // Video.qml: Image height = video height - header - 5; width unset while
    // online (PreserveAspectFit -> aspect-correct width), width/count while
    // loading or offline; Row spacing 30, left-padded to center it.
    QList<QRect> rects;
    if (m_screens.isEmpty() || m_viewSize.isEmpty()) {
        return rects;
    }
    const int frameHeight = qMax(1, m_viewSize.height() - 5);
    const int evenWidth = m_viewSize.width() / m_screens.size();
    QList<int> widths;
    int total = 0;
    for (const Screen &screen : m_screens) {
        const int width = screen.status == Status::Online && screen.image.height() > 0
            ? qRound(static_cast<double>(frameHeight) * screen.image.width() / screen.image.height())
            : evenWidth;
        widths.append(width);
        total += width;
    }
    total += 30 * (m_screens.size() - 1);
    int x = total > m_viewSize.width() ? 0 : (m_viewSize.width() - total) / 2;
    for (int width : std::as_const(widths)) {
        rects.append(QRect(x, 0, width, frameHeight));
        x += width + 30;
    }
    return rects;
}

void HistoryVideoStrip::relayout()
{
    const QList<QRect> rects = screenRects();
    const int contentWidth = rects.isEmpty() ? 0 : rects.last().right() + 1;
    setFixedSize(qMax(contentWidth, m_viewSize.width()), qMax(1, m_viewSize.height()));
    bool loading = false;
    for (const Screen &screen : std::as_const(m_screens)) {
        loading = loading || screen.status == Status::Loading;
    }
    if (loading && !m_spinnerTimer->isActive()) {
        m_spinnerTimer->start();
    } else if (!loading) {
        m_spinnerTimer->stop();
    }
    update();
}

void HistoryVideoStrip::paintEvent(QPaintEvent *)
{
    static const QPixmap spinner(QStringLiteral(":/history/spinner/spinner.png"));
    static const QPixmap emptyStream(QStringLiteral(":/history/big_emptyStream.png"));
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    const QList<QRect> rects = screenRects();
    for (int i = 0; i < rects.size() && i < m_screens.size(); ++i) {
        const Screen &screen = m_screens.at(i);
        const QRect &r = rects.at(i);
        switch (screen.status) {
        case Status::Online:
            painter.drawImage(r, screen.image);
            break;
        case Status::Loading:
            painter.save();
            painter.translate(r.center());
            painter.rotate(m_spinnerAngle);
            painter.drawPixmap(-spinner.width() / 2, -spinner.height() / 2, spinner);
            painter.restore();
            break;
        case Status::Offline: {
            // big_emptyStream.png over "No video" (18pt bold #38373d),
            // spacing 10, centered in a #424349 box.
            painter.fillRect(r, QColor(0x42, 0x43, 0x49));
            QFont font(QStringLiteral("Roboto"));
            font.setPointSize(18);
            font.setBold(true);
            const QFontMetrics metrics(font);
            const QString text = QStringLiteral("No video");
            const int blockHeight = emptyStream.height() + 10 + metrics.height();
            const int top = r.center().y() - blockHeight / 2;
            painter.drawPixmap(r.center().x() - emptyStream.width() / 2, top, emptyStream);
            painter.setFont(font);
            painter.setPen(QColor(0x38, 0x37, 0x3d));
            painter.drawText(QRect(r.left(), top + emptyStream.height() + 10, r.width(), metrics.height()),
                             Qt::AlignHCenter | Qt::AlignTop, text);
            break;
        }
        }
    }
}

void HistoryVideoStrip::mousePressEvent(QMouseEvent *event)
{
    for (const QRect &r : screenRects()) {
        if (r.contains(event->pos())) {
            emit clicked();
            return;
        }
    }
}

KeystreamBar::KeystreamBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(30);
}

void KeystreamBar::setText(const QString &past, const QString &future)
{
    if (past == m_past && future == m_future) {
        return;
    }
    m_past = past;
    m_future = future;
    update();
}

void KeystreamBar::wheelEvent(QWheelEvent *event)
{
    // The real bar is a ScrollView, so an overlong line can be scrolled.
    const int delta = event->angleDelta().x() != 0 ? event->angleDelta().x() : event->angleDelta().y();
    m_scroll = qMax(0, m_scroll - delta / 2);
    update();
}

void KeystreamBar::paintEvent(QPaintEvent *)
{
    static const QPixmap background(QStringLiteral(":/history/keylogger_bg.png"));
    QPainter painter(this);
    // BorderImage, 2px top/bottom borders, tiled horizontally, stretched
    // vertically -- keylogger_bg.png is 1px wide.
    const int bgHeight = background.height();
    painter.drawTiledPixmap(QRect(0, 0, width(), 2), background.copy(0, 0, 1, 2));
    painter.drawTiledPixmap(QRect(0, height() - 2, width(), 2), background.copy(0, bgHeight - 2, 1, 2));
    painter.drawTiledPixmap(QRect(0, 2, width(), height() - 4),
                            background.copy(0, 2, 1, bgHeight - 4).scaled(1, height() - 4));

    // Row of two Keystream texts (Fonts.rr_medium): typed so far in white,
    // still to come in gray; x = max(0, (viewport - row) / 2), leftMargin 5.
    const QFont font = mediumFont();
    painter.setFont(font);
    const QFontMetrics metrics(font);
    const int viewport = width() - 10;
    const int pastWidth = metrics.horizontalAdvance(m_past);
    const int rowWidth = pastWidth + metrics.horizontalAdvance(m_future);
    const int maxScroll = qMax(0, rowWidth - viewport);
    const int scroll = qMin(m_scroll, maxScroll);
    const int x = 5 + qMax(0, (viewport - rowWidth) / 2) - scroll;
    painter.setClipRect(QRect(5, 0, viewport, height()));
    painter.setPen(Qt::white);
    painter.drawText(QRect(x, 0, pastWidth + 1, height()), Qt::AlignLeft | Qt::AlignVCenter, m_past);
    painter.setPen(QColor(Qt::gray));
    painter.drawText(QRect(x + pastWidth, 0, rowWidth - pastWidth + 1, height()),
                     Qt::AlignLeft | Qt::AlignVCenter, m_future);
}

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget(parent)
{
    // Spans sliderAndMeta from its top down to where TimeLine starts, so the
    // bar lands at the real y=35 of the 125px block.
    setFixedHeight(kTimeLineY);
    setMouseTracking(true);
}

void TimelineWidget::setTimestamps(const QList<qint64> &timestamps)
{
    m_timestamps = timestamps;
    m_currentIndex = timestamps.isEmpty() ? -1 : qBound(0, m_currentIndex, timestamps.size() - 1);
    update();
}

void TimelineWidget::setRange(qint64 rangeStartMs, qint64 rangeEndMs)
{
    m_rangeStart = rangeStartMs;
    m_rangeEnd = rangeEndMs;
    update();
}

void TimelineWidget::setStepMs(qint64 stepMs)
{
    if (stepMs > 0 && stepMs != m_stepMs) {
        m_stepMs = stepMs;
        update();
    }
}

void TimelineWidget::setCurrentIndex(int index)
{
    if (index < 0 || index >= m_timestamps.size()) {
        return;
    }
    m_currentIndex = index;
    update();
}

int TimelineWidget::xForTime(qint64 timestampMs) const
{
    // SliderBar.qml positions everything in whole markers (markWidth *
    // marker), a marker being one Time step.
    if (m_rangeEnd <= m_rangeStart || m_stepMs <= 0) {
        return 0;
    }
    const double markWidth = static_cast<double>(width()) * m_stepMs / (m_rangeEnd - m_rangeStart);
    const qint64 marker = qMax<qint64>(0, (timestampMs - m_rangeStart) / m_stepMs);
    return static_cast<int>(markWidth * marker);
}

QRect TimelineWidget::pickRect() const
{
    if (m_currentIndex < 0 || m_currentIndex >= m_timestamps.size()) {
        return {};
    }
    // 19x19 pick image, vertically centered on the 4px bar, horizontally
    // centered on its marker (Math.max(markWidth, 19) wide).
    const int x = xForTime(m_timestamps.at(m_currentIndex));
    return QRect(x - 9, kSliderBarY + kSliderBarHeight / 2 - 9, 19, 19);
}

int TimelineWidget::indexForX(int x) const
{
    if (m_timestamps.isEmpty() || width() <= 0 || m_rangeEnd <= m_rangeStart) {
        return -1;
    }
    const double fraction = qBound(0.0, static_cast<double>(x) / width(), 1.0);
    const qint64 targetMs = m_rangeStart + static_cast<qint64>(fraction * (m_rangeEnd - m_rangeStart));
    // Nearest captured frame -- frames aren't evenly spaced.
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
    // Only the bar itself (plus the pick) is clickable, like SliderBar.qml's
    // MouseArea over __sliderBar.
    const QRect hitArea(0, kSliderBarY - 8, width(), kSliderBarHeight + 16);
    if (!hitArea.contains(event->pos()) && !pickRect().contains(event->pos())) {
        return;
    }
    m_pressed = true;
    seekToX(event->pos().x());
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pressed && (event->buttons() & Qt::LeftButton)) {
        seekToX(event->pos().x());
        return;
    }
    const bool hovered = pickRect().contains(event->pos());
    if (hovered != m_pickHovered) {
        m_pickHovered = hovered;
        update();
    }
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_pressed = false;
    update();
}

void TimelineWidget::leaveEvent(QEvent *)
{
    if (m_pickHovered) {
        m_pickHovered = false;
        update();
    }
}

void TimelineWidget::paintEvent(QPaintEvent *)
{
    static const QPixmap bgNone(QStringLiteral(":/history/sliderBar/bg_none.png"));
    static const QPixmap bgLoaded(QStringLiteral(":/history/sliderBar/bg_loaded_cropped.png"));
    static const QPixmap pickNormal(QStringLiteral(":/history/sliderBar/pick_normal.png"));
    static const QPixmap pickHovered(QStringLiteral(":/history/sliderBar/pick_hovered.png"));
    static const QPixmap pickPressed(QStringLiteral(":/history/sliderBar/pick_pressed.png"));

    QPainter painter(this);
    painter.fillRect(rect(), kHistoryBackground);
    drawBorderImage(painter, QRectF(0, kSliderBarY, width(), kSliderBarHeight), bgNone, 4, 4);
    if (m_rangeEnd <= m_rangeStart || m_timestamps.isEmpty()) {
        return;
    }

    // loadedRanges: runs of markers that have frames, each drawn
    // (stop - start + 1) markers wide. Frames are a few seconds apart, so at
    // fine Time steps (1s) neighbouring frames still count as one run when
    // they're within a minute of each other.
    const double markWidth = static_cast<double>(width()) * m_stepMs / (m_rangeEnd - m_rangeStart);
    const qint64 mergeMarkers = qMax<qint64>(1, 60 * 1000 / m_stepMs);
    qint64 runStart = -1;
    qint64 runStop = -1;
    const auto flushRun = [&] {
        if (runStart >= 0) {
            drawBorderImage(painter,
                            QRectF(runStart * markWidth, kSliderBarY,
                                   qMax(1.0, (runStop - runStart + 1) * markWidth), kSliderBarHeight),
                            bgLoaded, 0, 0);
        }
    };
    for (qint64 timestamp : std::as_const(m_timestamps)) {
        const qint64 marker = (timestamp - m_rangeStart) / m_stepMs;
        if (runStart >= 0 && marker <= runStop + mergeMarkers) {
            runStop = qMax(runStop, marker);
            continue;
        }
        flushRun();
        runStart = marker;
        runStop = marker;
    }
    flushRun();

    const QRect pick = pickRect();
    if (!pick.isNull()) {
        painter.drawPixmap(pick.topLeft(),
                           m_pressed ? pickPressed : (m_pickHovered ? pickHovered : pickNormal));
    }
}

namespace {
QString timeLineLowLabel(qint64 ms)
{
    // TimeLine.qml rebuilds "hh:mm" from toLocaleTimeString() -- with the
    // real viewer's English locale that's the 12-hour clock ("12:00",
    // "01:29", ...), without the AM/PM suffix.
    const QTime time = QDateTime::fromMSecsSinceEpoch(ms).time();
    const int hour = time.hour() % 12 == 0 ? 12 : time.hour() % 12;
    return QStringLiteral("%1:%2").arg(hour, 2, 10, QLatin1Char('0')).arg(time.minute(), 2, 10,
                                                                      QLatin1Char('0'));
}
}

TimeAxisWidget::TimeAxisWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(kTimeLineHighHeight + kTimeLineLowHeight);
}

void TimeAxisWidget::setRange(qint64 rangeStartMs, qint64 rangeEndMs)
{
    m_rangeStart = rangeStartMs;
    m_rangeEnd = rangeEndMs;
    update();
}

void TimeAxisWidget::setStepMs(qint64 stepMs)
{
    if (stepMs > 0 && stepMs != m_stepMs) {
        m_stepMs = stepMs;
        update();
    }
}

void TimeAxisWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), kHistoryBackground);
    const qint64 span = m_rangeEnd - m_rangeStart;
    if (span <= 0 || width() <= 0) {
        return;
    }

    // TimeLine.qml's updateTicker, step for step.
    qint64 step = m_stepMs > 0 ? m_stepMs : 60 * 1000;
    while (span / step > 1000) {
        step *= 2;
    }
    struct Mark {
        qint64 ms;
        int index;
    };
    QList<Mark> highLabels;
    QList<Mark> lowLabels;
    QDate previousDate = QDateTime::fromMSecsSinceEpoch(m_rangeStart - step).date();
    int index = 0;
    for (qint64 pos = m_rangeStart; pos < m_rangeEnd; pos += step, ++index) {
        const QDate date = QDateTime::fromMSecsSinceEpoch(pos).date();
        if (date != previousDate) {
            previousDate = date;
            highLabels.append({pos, index});
        }
        lowLabels.append({pos, index});
    }
    if (highLabels.isEmpty()) {
        highLabels.append({m_rangeStart, 0});
    }
    if (lowLabels.size() == highLabels.size()) {
        lowLabels.clear();
    }

    const double marks = static_cast<double>(span) / step;
    const double markWidth = width() / marks;
    const double highLabelWidth = static_cast<double>(width()) / highLabels.size();

    const QFont font = smallBoldFont();
    painter.setFont(font);
    const QFontMetrics metrics(font);
    const QLocale english(QLocale::English);

    for (const Mark &mark : std::as_const(highLabels)) {
        const double x = markWidth * mark.index;
        const QDate date = QDateTime::fromMSecsSinceEpoch(mark.ms).date();
        // formatHighLabel: Locale.LongFormat unless it doesn't fit its slot
        // or would run past the right edge, then Locale.ShortFormat.
        QString text = english.toString(date, QStringLiteral("dddd, MMMM d, yyyy"));
        const int longWidth = metrics.horizontalAdvance(text);
        if (longWidth + x > width() || longWidth > highLabelWidth) {
            text = english.toString(date, QStringLiteral("M/d/yy"));
        }
        if (x + metrics.horizontalAdvance(text) >= width()) {
            continue;
        }
        painter.fillRect(QRectF(x, 0, 1, kTimeLineHighHeight), kTickColor);
        painter.setPen(Qt::white);
        painter.drawText(QRectF(x + 3, 0, highLabelWidth, kTimeLineHighHeight),
                         Qt::AlignLeft | Qt::AlignTop,
                         metrics.elidedText(text, Qt::ElideRight, qMax(1, int(highLabelWidth))));
    }

    if (lowLabels.isEmpty()) {
        return;
    }
    const int lowY = kTimeLineHighHeight;
    // indexVisible: how many marks the first label's implicit width (+5)
    // covers, so shown labels never overlap.
    const int labelImplicitWidth = metrics.horizontalAdvance(timeLineLowLabel(lowLabels.first().ms)) + 5;
    const int indexVisible = lowLabels.size() > 1
        ? qMax(1, static_cast<int>(std::ceil(labelImplicitWidth / (static_cast<double>(width())
                                                                     / (lowLabels.size() - 1)))))
        : 1;
    for (int i = 0; i < lowLabels.size(); ++i) {
        const Mark &mark = lowLabels.at(i);
        const QString text = timeLineLowLabel(mark.ms);
        const double x = markWidth * mark.index;
        const bool visible = i == 0
            || (i % indexVisible == 0 && x + metrics.horizontalAdvance(text) <= width());
        if (!visible) {
            continue;
        }
        painter.fillRect(QRectF(x, lowY, 1, kTimeLineLowHeight), kTickColor);
        painter.setPen(Qt::white);
        painter.drawText(QRectF(x + 2, lowY, width() - x, kTimeLineLowHeight),
                         Qt::AlignLeft | Qt::AlignTop, text);
    }
}

HistoryChartWidget::HistoryChartWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(kChartsItemHeight);
}

qint64 HistoryChartWidget::chartStepMs(qint64 rangeMs, qint64 userStepMs)
{
    // HistoryTab.qml: chartTimeStep = alingStep(rangeSeconds, stepSeconds, 60).
    static constexpr qint64 kRoundedSteps[] = {
        1 * 60,           2 * 60,           5 * 60,           10 * 60,          15 * 60,
        20 * 60,          30 * 60,          1 * 60 * 60,      2 * 60 * 60,      4 * 60 * 60,
        6 * 60 * 60,      8 * 60 * 60,      12 * 60 * 60,     1 * 24 * 60 * 60, 2 * 24 * 60 * 60,
        4 * 24 * 60 * 60, 5 * 24 * 60 * 60, 10 * 24 * 60 * 60,
    };
    constexpr double kMaxAmount = 60;
    const double distance = rangeMs / 1000.0;
    const qint64 step = qMax<qint64>(1, userStepMs / 1000);
    if (distance / step <= kMaxAmount) {
        return step * 1000;
    }
    for (qint64 rounded : kRoundedSteps) {
        if (rounded >= distance / kMaxAmount) {
            return rounded * 1000;
        }
    }
    const qint64 day = 24 * 60 * 60;
    return static_cast<qint64>(std::ceil(distance / kMaxAmount / day)) * day * 1000;
}

void HistoryChartWidget::setRange(qint64 rangeStartMs, qint64 rangeEndMs)
{
    m_rangeStart = rangeStartMs;
    m_rangeEnd = rangeEndMs;
    update();
}

void HistoryChartWidget::setStepMs(qint64 stepMs)
{
    if (stepMs > 0 && stepMs != m_stepMs) {
        m_stepMs = stepMs;
        update();
    }
}

void HistoryChartWidget::setGridLeft(int x)
{
    if (x != m_gridLeft) {
        m_gridLeft = x;
        update();
    }
}

void HistoryChartWidget::setActivity(const QList<HistoryActivitySample> &samples)
{
    m_samples = samples;
    update();
}

void HistoryChartWidget::setEfficiency(const QList<HistoryAppSegment> &segments,
                                       const QHash<QString, QString> &categories)
{
    m_segments = segments;
    m_categories = categories;
    update();
}

void HistoryChartWidget::setCurrentPositionMs(qint64 positionMs)
{
    m_currentPositionMs = positionMs;
    update();
}

void HistoryChartWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), kHistoryBackground);

    // Filters.qml: anchors.fill chartsItem, leftMargin 10, bottomMargin 15;
    // Chart.qml with needPlayerLine splits it 40% (Activity/Efficiency) /
    // separator1 (10px) / 60% (filters).
    const double chartHeight = height() - kChartBottomMargin;
    const double extraHeight = chartHeight * 0.4;
    const double filtersTop = extraHeight + 10;
    const double filtersHeight = chartHeight * 0.6;
    const int gridLeft = m_gridLeft;
    const double gridWidth = qMax(1, width() - gridLeft);
    const qint64 span = m_rangeEnd - m_rangeStart;

    drawHorizontalSeparator(painter, kChartLeft, 0, width() - kChartLeft);

    // ExtraHeaders: labels 20px in, first text 10px down, rows 30 apart,
    // VSeparator at its right edge minus 13.
    painter.setFont(mediumFont());
    painter.setPen(Qt::white);
    painter.drawText(QPointF(kChartLeft + kLabelsLeft, 10 + QFontMetrics(mediumFont()).ascent()),
                     QStringLiteral("Activity"));
    painter.drawText(QPointF(kChartLeft + kLabelsLeft,
                             kRowHeight + 10 + QFontMetrics(mediumFont()).ascent()),
                     QStringLiteral("Efficiency"));
    drawVerticalSeparator(painter, gridLeft - 13 - 2, 0, extraHeight);

    if (span <= 0) {
        return;
    }
    const qint64 chartStep = chartStepMs(span, m_stepMs);

    // Grid.qml, once over the extra rows and once over the filters area:
    // marks+1 one-pixel lines, markWidth = width / marks.
    const double marks = static_cast<double>(span) / chartStep;
    const double gridMarkWidth = gridWidth / marks;
    const int lineCount = static_cast<int>(marks) + 1;
    for (int i = 0; i < lineCount; ++i) {
        const double x = gridLeft + gridMarkWidth * i;
        painter.fillRect(QRectF(x, 0, 1, extraHeight), kGridColor);
        painter.fillRect(QRectF(x, filtersTop, 1, filtersHeight), kGridColor);
    }

    painter.save();
    painter.setClipRect(QRectF(gridLeft, 0, gridWidth, extraHeight));

    // HistoLine.qml: fillRect(relPos * w, (1 - v) * h, relInterval * w + 1, h)
    // inside a 20px area centered in its 30px row. Activity granula is
    // max(chartStep / 5, 60s) (ChartsModel.qml).
    const qint64 activityGranula = qMax<qint64>(chartStep / 5, 60 * 1000);
    QHash<qint64, qint64> eventsByBucket;
    qint64 maxEvents = 0;
    for (const HistoryActivitySample &sample : std::as_const(m_samples)) {
        if (sample.inputEvents <= 0 || sample.timestampMs < m_rangeStart
            || sample.timestampMs >= m_rangeEnd) {
            continue;
        }
        const qint64 bucket = (sample.timestampMs - m_rangeStart) / activityGranula;
        const qint64 total = eventsByBucket.value(bucket) + sample.inputEvents;
        eventsByBucket[bucket] = total;
        maxEvents = qMax(maxEvents, total);
    }
    const double histogramTop = (kRowHeight - kHistogramHeight) / 2.0;
    const double activityWidth = gridWidth * activityGranula / span + 1;
    for (auto it = eventsByBucket.cbegin(); it != eventsByBucket.cend(); ++it) {
        const double value = static_cast<double>(it.value()) / maxEvents;
        const double x = gridLeft + gridWidth * (it.key() * activityGranula) / span;
        const double y = histogramTop + (1 - value) * kHistogramHeight;
        painter.fillRect(QRectF(x, y, activityWidth, histogramTop + kHistogramHeight - y),
                         kActivityColor);
    }

    // Line.qml (displayKind "colors"): one column per chart step, split into
    // equal-height bands, one per category that occurred in it, stacked in
    // the real viewer's order -- non-productive, productive, neutral,
    // uncategorized from top to bottom.
    static const QStringList kBandOrder = {QStringLiteral("unproductive"), QStringLiteral("productive"),
                                           QStringLiteral("neutral"), QStringLiteral("none")};
    QHash<qint64, QSet<QString>> categoriesByBucket;
    for (const HistoryAppSegment &segment : std::as_const(m_segments)) {
        const qint64 from = qMax(segment.startMs, m_rangeStart);
        const qint64 to = qMin(segment.endMs, m_rangeEnd);
        if (to <= from) {
            continue;
        }
        QString category = m_categories.value(segment.application, QStringLiteral("none"));
        if (!kBandOrder.contains(category)) {
            category = QStringLiteral("none");
        }
        for (qint64 bucket = (from - m_rangeStart) / chartStep;
             bucket <= (to - 1 - m_rangeStart) / chartStep; ++bucket) {
            categoriesByBucket[bucket].insert(category);
        }
    }
    const double productivityTop = kRowHeight;
    const double productivityWidth = gridWidth * chartStep / span + 1;
    for (auto it = categoriesByBucket.cbegin(); it != categoriesByBucket.cend(); ++it) {
        QStringList bands;
        for (const QString &category : kBandOrder) {
            if (it.value().contains(category)) {
                bands.append(category);
            }
        }
        const double x = gridLeft + gridWidth * (it.key() * chartStep) / span - 0.5;
        const double bandHeight = static_cast<double>(kProductivityHeight) / bands.size();
        double y = productivityTop;
        for (const QString &category : std::as_const(bands)) {
            painter.fillRect(QRectF(x, y, productivityWidth, bandHeight),
                             EfficiencyCategoryButton::color(category));
            y += bandHeight;
        }
    }
    painter.restore();

    // separator1: 10px tall, two lines centered, 25px short of the right.
    drawHorizontalSeparator(painter, kChartLeft, extraHeight + 4, width() - kChartLeft - 25);

    // HistoryPlayerMarkerControl: 1px khaki line at the current marker,
    // chart height + 10 tall.
    if (m_currentPositionMs >= m_rangeStart && m_currentPositionMs < m_rangeEnd) {
        const double markerWidth = gridWidth * m_stepMs / span;
        const double x = gridLeft + markerWidth * ((m_currentPositionMs - m_rangeStart) / m_stepMs);
        painter.fillRect(QRectF(x, 0, 1, chartHeight + 10), kMarkerColor);
    }
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
    ensureHistoryFonts();
    auto *root = new QVBoxLayout(this);
    // No page margins: videoCell/keylogger/sliderAndMeta/chartsItem span
    // the page edge to edge and carry their own insets (History.qml).
    root->setContentsMargins(0, 0, 0, 0);
    // History.qml's panelViolation toggle button is anchored directly to
    // the bottom edge of the same container the slider lives in (bottomMargin
    // -5, i.e. no gap at all, even slightly overlapping) -- root's spacing
    // is kept small everywhere for that reason, not just between these two.
    root->setSpacing(0);

    // History.qml's videoCell fills the page above the keylogger bar (8px
    // gap), with Video.qml inset 7px inside it: #45464d, 1px #414248 lines
    // left/right, the 24px header, then the row of screens.
    m_videoPanel = new QWidget(this);
    m_videoPanel->setObjectName(QStringLiteral("historyVideo"));
    m_videoPanel->setAttribute(Qt::WA_StyledBackground);
    m_videoPanel->setStyleSheet(QStringLiteral(
        "QWidget#historyVideo { background: #45464d; border-left: 1px solid #414248;"
        " border-right: 1px solid #414248; }"
        "QScrollArea { background: transparent; border: none; }"
        "QScrollBar:horizontal { background: transparent; height: 8px; margin: 0; }"
        "QScrollBar::handle:horizontal { background: #5a5b63; border-radius: 4px; min-width: 30px; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }"
        "QListWidget#historyApps { background: #3f4047; border-left: 2px solid #43444c;"
        " border-right: 2px solid #43444c; color: white; }"));
    auto *videoLayout = new QVBoxLayout(m_videoPanel);
    videoLayout->setContentsMargins(0, 0, 0, 0);
    videoLayout->setSpacing(0);
    m_videoHeader = new HistoryVideoHeader(m_videoPanel);
    videoLayout->addWidget(m_videoHeader);

    auto *videoBody = new QHBoxLayout;
    videoBody->setContentsMargins(0, 0, 0, 0);
    videoBody->setSpacing(0);
    m_monitorStripArea = new QScrollArea(m_videoPanel);
    m_monitorStripArea->setFrameShape(QFrame::NoFrame);
    m_monitorStripArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_monitorStripArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_monitorStripArea->viewport()->setAutoFillBackground(false);
    m_monitorStripArea->viewport()->installEventFilter(this);
    m_videoStrip = new HistoryVideoStrip;
    m_monitorStripArea->setWidget(m_videoStrip);
    connect(m_videoStrip, &HistoryVideoStrip::clicked, this, &HistoryView::togglePanelFull);
    videoBody->addWidget(m_monitorStripArea, 1);
    // Running applications panel: 420px wide on the video's right edge,
    // below the header (History.qml `panel`), closed by default.
    m_runningAppsList = new QListWidget(m_videoPanel);
    m_runningAppsList->setObjectName(QStringLiteral("historyApps"));
    m_runningAppsList->setFixedWidth(420);
    m_runningAppsList->hide();
    videoBody->addWidget(m_runningAppsList);
    videoLayout->addLayout(videoBody, 1);

    // Video.qml's "excuse", centered over the video area (Fonts.rr_xtra).
    m_statusLabel = new QLabel(m_monitorStripArea);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_statusLabel->setStyleSheet(QStringLiteral(
        "background: transparent; color: white; font-family: Roboto; font-size: 20px;"));
    m_statusLabel->hide();

    auto *videoCell = new QVBoxLayout;
    videoCell->setContentsMargins(7, 7, 7, 7 + 8);
    videoCell->addWidget(m_videoPanel);
    root->addLayout(videoCell, 1);

    m_keystream = new KeystreamBar(this);
    root->addWidget(m_keystream);

    // appButton: "Running applications" over the video's top-right corner
    // (topMargin 30 / rightMargin 5, less the Button's own -5 margins),
    // opacity 0.8 -- see positionOverlays().
    m_toggleAppsButton = new QPushButton(QStringLiteral("Running applications"), this);
    m_toggleAppsButton->setObjectName(QStringLiteral("flatButton"));
    m_toggleAppsButton->setIcon(
        QIcon(QStringLiteral(":/history/video/buttonRunningApp/applications_arrow_left.png")));
    m_toggleAppsButton->setIconSize(QSize(8, 14));
    auto *appButtonOpacity = new QGraphicsOpacityEffect(m_toggleAppsButton);
    appButtonOpacity->setOpacity(0.8);
    m_toggleAppsButton->setGraphicsEffect(appButtonOpacity);
    connect(m_toggleAppsButton, &QPushButton::clicked, this, &HistoryView::onToggleRunningApps);

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

    // History.qml's sliderAndMeta: a fixed 125px block, leftMargin 12, with
    // column1 (Change period / Video+speed / Text, spacing 10 after a 5px
    // spacer), column2 (the 60px round play button, 11px down), column21
    // (audio, only at Time step = 1s) and then the slider + TimeLine; the
    // panelViolation button sits at its bottom center. Buttons use the
    // original history/*.png assets.
    m_changeSettingsButton = new QPushButton(QStringLiteral("Change settings"), this);
    m_changeSettingsButton->setObjectName(QStringLiteral("historyChangeButton"));
    m_changeSettingsButton->setFixedSize(140, 25);
    connect(m_changeSettingsButton, &QPushButton::clicked, this, &HistoryView::onChangeSettingsClicked);
    // Not shown -- day/period selection lives in the "Change settings"
    // dialog; m_dayCombo stays alive for applyPeriodFilter/onDayChanged.
    m_dayCombo = new QComboBox(this);
    m_dayCombo->hide();

    const QIcon exportIcon = [] {
        QIcon icon;
        icon.addPixmap(QPixmap(QStringLiteral(":/history/avi_export_normal.png")), QIcon::Normal);
        icon.addPixmap(QPixmap(QStringLiteral(":/history/avi_export_hovered.png")), QIcon::Active);
        icon.addPixmap(QPixmap(QStringLiteral(":/history/avi_export_normal.png")), QIcon::Disabled);
        return icon;
    }();
    // "Video" in the real viewer saves an AVI; with no encoder here it
    // exports the day's frames as a PNG sequence instead.
    m_exportVideoButton = new QPushButton(exportIcon, QStringLiteral("Video"), this);
    m_exportVideoButton->setObjectName(QStringLiteral("historyAssetButton"));
    m_exportVideoButton->setIconSize(QSize(13, 9));
    m_exportVideoButton->setFixedSize(65, 19);
    m_exportVideoButton->setToolTip(QStringLiteral("Export imagini (secventa PNG -- fara encoder video)"));
    connect(m_exportVideoButton, &QPushButton::clicked, this, &HistoryView::onExportVideoClicked);
    // SpeedButton.qml options.
    m_speedCombo = new QComboBox(this);
    m_speedCombo->setObjectName(QStringLiteral("historySpeed"));
    m_speedCombo->addItems({QStringLiteral("0,5x"), QStringLiteral("0,75x"), QStringLiteral("1x"),
                            QStringLiteral("2x"), QStringLiteral("4x"), QStringLiteral("8x"),
                            QStringLiteral("16x")});
    m_speedCombo->setFixedSize(65, 19);
    m_speedCombo->setCurrentIndex(2); // 1x
    connect(m_speedCombo, &QComboBox::currentIndexChanged, this, &HistoryView::onSpeedChanged);
    m_keylogTableButton = new QPushButton(exportIcon, QStringLiteral("Text"), this);
    m_keylogTableButton->setObjectName(QStringLiteral("historyAssetButton"));
    m_keylogTableButton->setIconSize(QSize(13, 9));
    m_keylogTableButton->setFixedSize(65, 19);
    m_keylogTableButton->setToolTip(QStringLiteral("Keylogger"));
    connect(m_keylogTableButton, &QPushButton::clicked, this, &HistoryView::onKeylogTableClicked);

    m_playButton = new QPushButton(this);
    m_playButton->setObjectName(QStringLiteral("historyPlay"));
    m_playButton->setFixedSize(60, 60);
    m_playButton->setIconSize(QSize(23, 30));
    connect(m_playButton, &QPushButton::clicked, this, &HistoryView::onPlayClicked);
    setPlaying(false);
    // Audio.qml -- only at Time step = 1s (see applyTimeStep). Always
    // disabled: nothing in this project captures audio.
    m_muteButton = new QPushButton(this);
    m_muteButton->setObjectName(QStringLiteral("historyAudio"));
    m_muteButton->setFixedSize(39, 39);
    m_muteButton->setIconSize(QSize(32, 32));
    {
        QIcon icon;
        const QPixmap off(QStringLiteral(":/history/volume_off.png"));
        icon.addPixmap(off, QIcon::Normal);
        icon.addPixmap(off, QIcon::Disabled);
        m_muteButton->setIcon(icon);
    }
    m_muteButton->setEnabled(false);
    m_muteButton->setToolTip(QStringLiteral("Fara audio in acest sistem."));

    m_timeline = new TimelineWidget(this);
    connect(m_timeline, &TimelineWidget::indexSelected, this, &HistoryView::onTimelineMoved);
    m_timeAxis = new TimeAxisWidget(this);

    auto *controlBlock = new QWidget(this);
    controlBlock->setObjectName(QStringLiteral("historyControls"));
    controlBlock->setFixedHeight(kControlBlockHeight);
    controlBlock->setStyleSheet(QStringLiteral(
        "QWidget#historyControls { background: #474850; }"
        ".QWidget { background: transparent; }"
        "QPushButton#historyChangeButton { background: #404148; color: white; border: none;"
        "  border-radius: 3px; font-family: Roboto; font-size: 12px; padding: 0; }"
        "QPushButton#historyChangeButton:hover { background: #6b6d79; }"
        "QPushButton#historyChangeButton:pressed { background: #34373e; }"
        "QPushButton#historyAssetButton { border-image: url(:/history/speedButton/speed_bg.png);"
        "  border: none; color: white; font-family: Roboto; font-size: 11px; text-align: left;"
        "  padding: 0 0 0 10px; }"
        "QPushButton#historyAssetButton:hover {"
        "  border-image: url(:/history/speedButton/speed_bg_hovered.png); }"
        "QPushButton#historyAssetButton:pressed {"
        "  border-image: url(:/history/speedButton/speed_bg_pressed.png); }"
        "QComboBox#historySpeed { border-image: url(:/history/speedButton/speed_bg.png); border: none;"
        "  color: white; font-family: Roboto; font-size: 11px; padding: 0 0 0 10px; }"
        "QComboBox#historySpeed:hover {"
        "  border-image: url(:/history/speedButton/speed_bg_hovered.png); }"
        "QComboBox#historySpeed::drop-down { border: none; width: 16px; }"
        "QComboBox#historySpeed::down-arrow {"
        "  image: url(:/history/speedButton/triangle_normal.png); width: 8px; height: 5px; }"
        "QComboBox#historySpeed::down-arrow:hover {"
        "  image: url(:/history/speedButton/triangle_hovered.png); }"
        "QPushButton#historyPlay { border-image: url(:/history/bg_play.png); border: none;"
        "  padding: 0 0 0 8px; }"
        "QPushButton#historyAudio { border-image: url(:/history/bg_audio.png); border: none;"
        "  padding: 0; }"));
    auto *controlLayout = new QVBoxLayout(controlBlock);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->setSpacing(0);

    m_leftColumn = new QWidget(controlBlock);
    auto *buttonColumns = new QHBoxLayout(m_leftColumn);
    buttonColumns->setContentsMargins(12, 0, 0, 0);
    buttonColumns->setSpacing(5);
    auto *column1 = new QVBoxLayout;
    column1->setContentsMargins(0, 15, 0, 0);
    column1->setSpacing(10);
    column1->addWidget(m_changeSettingsButton);
    auto *twoButtons = new QHBoxLayout;
    twoButtons->setSpacing(10);
    twoButtons->addWidget(m_exportVideoButton);
    twoButtons->addWidget(m_speedCombo);
    column1->addLayout(twoButtons);
    column1->addWidget(m_keylogTableButton, 0, Qt::AlignLeft);
    column1->addStretch(1);
    buttonColumns->addLayout(column1);
    auto *column2 = new QVBoxLayout;
    column2->setContentsMargins(0, 11, 0, 0);
    column2->addWidget(m_playButton);
    column2->addStretch(1);
    buttonColumns->addLayout(column2);
    auto *column21 = new QVBoxLayout;
    column21->setContentsMargins(0, 21, 0, 0);
    column21->addWidget(m_muteButton);
    column21->addStretch(1);
    buttonColumns->addLayout(column21);
    buttonColumns->addStretch(1);

    auto *rightColumn = new QWidget(controlBlock);
    auto *rightColumnLayout = new QVBoxLayout(rightColumn);
    rightColumnLayout->setContentsMargins(0, 0, 0, 0);
    rightColumnLayout->setSpacing(0);
    rightColumnLayout->addWidget(m_timeline);
    rightColumnLayout->addWidget(m_timeAxis);
    rightColumnLayout->addStretch(1);

    auto *combinedRow = new QHBoxLayout;
    combinedRow->setContentsMargins(0, 0, 0, 0);
    combinedRow->setSpacing(0);
    combinedRow->addWidget(m_leftColumn);
    combinedRow->addWidget(rightColumn, 1);
    controlLayout->addLayout(combinedRow, 1);

    auto *violationToggleRow = new QHBoxLayout;
    violationToggleRow->addStretch();
    m_violationToggleButton = new QPushButton(controlBlock);
    m_violationToggleButton->setObjectName(QStringLiteral("flatButton"));
    // "flatButton" has 6px padding top+bottom -- under ~26px squeezes text.
    m_violationToggleButton->setFixedHeight(26);
    connect(m_violationToggleButton, &QPushButton::clicked, this,
            &HistoryView::onToggleViolationPanel);
    violationToggleRow->addWidget(m_violationToggleButton);
    violationToggleRow->addStretch();
    controlLayout->addLayout(violationToggleRow);
    root->addWidget(controlBlock);
    m_controlBlock = controlBlock;

    // chartsItem, opened to 186px by panelViolation (starts closed).
    m_chart = new HistoryChartWidget(this);
    m_chart->hide();
    root->addWidget(m_chart);
    updateViolationToggleText();
    applyTimeStep();

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
    connect(&m_connection, &ViewerConnection::historyFrameMissing, this,
            &HistoryView::onHistoryFrameMissing);
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
    m_videoStrip->setStreams(m_deviceMonitorStreams.value(m_currentDeviceKey));
    positionOverlays();
}

void HistoryView::positionOverlays()
{
    // The strip reserves the scrollbar's height up front so a horizontal
    // scrollbar appearing (screens wider than the view) can't shrink the
    // frames and make it disappear again.
    const int scrollbarHeight = m_monitorStripArea->horizontalScrollBar()->sizeHint().height();
    m_videoStrip->setViewSize(QSize(m_monitorStripArea->viewport()->width(),
                                    m_monitorStripArea->height() - scrollbarHeight));
    m_statusLabel->setGeometry(m_monitorStripArea->rect().adjusted(20, 0, -20, 0));
    const QSize hint = m_toggleAppsButton->sizeHint();
    m_toggleAppsButton->setGeometry(width() - hint.width(), 25, hint.width(), hint.height());
    m_toggleAppsButton->raise();
}

void HistoryView::updateStatusVisibility()
{
    m_statusLabel->setVisible(!m_statusLabel->text().isEmpty());
    m_statusLabel->raise();
}

void HistoryView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    positionOverlays();
}

void HistoryView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), kHistoryBackground);
}

bool HistoryView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_monitorStripArea->viewport() && event->type() == QEvent::Resize) {
        positionOverlays();
    }
    return QWidget::eventFilter(watched, event);
}

void HistoryView::switchDevice(quint32 deviceKey)
{
    m_playbackTimer->stop();
    setPlaying(false);
    m_currentDeviceKey = deviceKey;
    m_videoHeader->setName(m_deviceNames.value(deviceKey, QStringLiteral("Employee %1").arg(deviceKey)));
    rebuildMonitorStrip();

    m_dayCombo->clear();
    m_allDays.clear();
    m_periodStart = QDate();
    m_periodEnd = QDate();
    m_timestamps.clear();
    m_timeline->setTimestamps({});
    m_timeline->setRange(0, 0);
    m_timeAxis->setRange(0, 0);
    m_chart->setRange(0, 0);
    m_chart->setActivity({});
    m_chart->setEfficiency({}, m_categories);
    m_videoHeader->setMoment(QString());
    m_keystream->setText(QString(), QString());
    m_runningApps.clear();
    m_appSegments.clear();
    m_activitySamples.clear();
    m_keystrokeEntries.clear();
    m_runningAppsList->clear();
    m_textLog->setRowCount(0);

    const quint32 streamId = currentStreamId();
    if (streamId == 0) {
        return;
    }
    m_statusLabel->clear();
    updateStatusVisibility();
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
    setPlaying(false);
    m_timestamps.clear();
    m_timeline->setTimestamps({});
    m_videoStrip->setAllLoading();
    m_videoHeader->setMoment(QString());
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
    m_statusLabel->clear();
    updateStatusVisibility();
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
    // Header.qml's currentMarker: "( <short date> hh:mm:ss )", English
    // locale like the real viewer.
    const QDateTime moment = QDateTime::fromMSecsSinceEpoch(timestampMs);
    m_videoHeader->setMoment(QStringLiteral("( %1 %2 )")
                                 .arg(QLocale(QLocale::English).toString(moment.date(), QStringLiteral("M/d/yy")),
                                      moment.toString(QStringLiteral("hh:mm:ss"))));
    // Every screen of the device is shown at once (see rebuildMonitorStrip),
    // so a frame is fetched for each of its monitor streams, not just the
    // one used for the non-video queries (currentStreamId()).
    for (quint32 streamId : m_deviceMonitorStreams.value(m_currentDeviceKey)) {
        m_connection.requestHistoryFrame(streamId, timestampMs);
    }
    updateTextLogHighlight(timestampMs);
    updateKeystream(timestampMs);
    m_timeline->setCurrentIndex(index);
    m_chart->setCurrentPositionMs(timestampMs);
}

void HistoryView::onPlayClicked()
{
    if (m_playbackTimer->isActive()) {
        m_playbackTimer->stop();
        setPlaying(false);
        return;
    }
    if (m_timestamps.isEmpty()) {
        return;
    }
    if (m_timeline->currentIndex() >= m_timeline->count() - 1) {
        requestFrameAt(0);
    }
    setPlaying(true);
    m_playbackTimer->start();
}

void HistoryView::onPlaybackTick()
{
    const int next = m_timeline->currentIndex() + 1;
    if (next > m_timeline->count() - 1) {
        m_playbackTimer->stop();
        setPlaying(false);
        return;
    }
    requestFrameAt(next);
}

void HistoryView::onToggleViolationPanel()
{
    m_chartOpen = !m_chartOpen;
    m_chart->setVisible(m_chartOpen && !m_panelFull);
    updateViolationToggleText();
}

void HistoryView::togglePanelFull()
{
    // chartsItem/sliderAndMeta heights are `panelFull ? 0 : ...`; the
    // keylogger bar drops to the page bottom.
    m_panelFull = !m_panelFull;
    m_controlBlock->setVisible(!m_panelFull);
    m_chart->setVisible(m_chartOpen && !m_panelFull);
}

void HistoryView::updateViolationToggleText()
{
    // History.qml's panelViolation: arrow_down + "Hide violation panel"
    // while open, arrow_up + "Open violation panel" while closed.
    const bool open = m_chartOpen;
    m_violationToggleButton->setIcon(QIcon(open
        ? QStringLiteral(":/history/iconButtonViolation/arrow_down.png")
        : QStringLiteral(":/history/iconButtonViolation/arrow_up.png")));
    m_violationToggleButton->setText(open ? QStringLiteral("Hide violation panel")
                                          : QStringLiteral("Open violation panel"));
}

void HistoryView::setPlaying(bool playing)
{
    QIcon icon;
    icon.addPixmap(QPixmap(playing ? QStringLiteral(":/history/pause_normal.png")
                                   : QStringLiteral(":/history/play_normal.png")),
                   QIcon::Normal);
    icon.addPixmap(QPixmap(playing ? QStringLiteral(":/history/pause_active.png")
                                   : QStringLiteral(":/history/play_active.png")),
                   QIcon::Active);
    m_playButton->setIcon(icon);
    // play_normal is nudged 4px right of center, pause_normal 2px
    // (History.qml's horizontalCenterOffset).
    m_playButton->setStyleSheet(
        QStringLiteral("QPushButton { border-image: url(:/history/bg_play.png); border: none;"
                       " padding: 0 0 0 %1px; }")
            .arg(playing ? 4 : 8));
}

void HistoryView::applyTimeStep()
{
    // Audio.qml (column21) only exists at Time step = 1s, and shifts both
    // the slider/TimeLine (12 leftMargin + column1 140 + 5 + column2 60 + 5
    // [+ column21 39 + 5] + 1 + MultiSessionsSlider spacing 10) and the chart
    // grid (Filters leftMargin 10 + widthActionButtons = column1 + column2
    // + 20 + 12 [+ column21]).
    const bool audioVisible = m_timeStepMs <= 1000;
    m_muteButton->setVisible(audioVisible);
    const int audioWidth = audioVisible ? 39 : 0;
    m_leftColumn->setFixedWidth(12 + 140 + 5 + 60 + 5 + (audioVisible ? audioWidth + 5 : 0) + 1 + 10);
    m_chart->setGridLeft(10 + 140 + 60 + 20 + 12 + audioWidth);
    m_timeline->setStepMs(m_timeStepMs);
    m_timeAxis->setStepMs(m_timeStepMs);
    m_chart->setStepMs(m_timeStepMs);
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
        QMessageBox::information(this, QStringLiteral("Export imagini"),
                                 QStringLiteral("Nimic de exportat pentru aceasta zi."));
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
        hideLoadingDialog();
        QMessageBox::information(
            this, QStringLiteral("Export imagini"),
            QStringLiteral("Export finalizat: %1 imagini in %2").arg(m_videoExport.total).arg(m_videoExport.directory));
        return;
    }
    const qint64 timestampMs = m_videoExport.pending.first();
    showLoadingDialog(QStringLiteral("Se exporta %1/%2...")
                          .arg(m_videoExport.total - m_videoExport.pending.size() + 1)
                          .arg(m_videoExport.total));
    m_connection.requestHistoryFrame(m_videoExport.streamId, timestampMs);
}

void HistoryView::onToggleRunningApps()
{
    // Matches History.qml's appButton: closed by default (see the
    // constructor's m_runningAppsList->hide()), arrow flips direction.
    const bool visible = m_runningAppsList->isHidden();
    m_runningAppsList->setVisible(visible);
    m_toggleAppsButton->setIcon(QIcon(visible
        ? QStringLiteral(":/history/video/buttonRunningApp/applications_arrow_right.png")
        : QStringLiteral(":/history/video/buttonRunningApp/applications_arrow_left.png")));
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
                applyTimeStep();
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
        m_statusLabel->setText(QStringLiteral("No information for selected period"));
        updateStatusVisibility();
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
    if (streamId != currentStreamId()) {
        return;
    }
    m_timestamps = timestamps;
    m_timeline->setTimestamps(timestamps);
    const auto [dayStart, dayEnd] = dayRangeMs(day);
    m_timeline->setRange(dayStart, dayEnd);
    m_timeAxis->setRange(dayStart, dayEnd);
    m_chart->setRange(dayStart, dayEnd);
    if (timestamps.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("No information for selected period"));
        updateStatusVisibility();
        m_videoStrip->setStreams({});
        hideLoadingDialog();
        return;
    }
    m_statusLabel->clear();
    updateStatusVisibility();
    rebuildMonitorStrip();
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

    if (m_deviceMonitorStreams.value(m_currentDeviceKey).contains(streamId)) {
        m_videoStrip->setFrame(streamId, image);
    }
}

void HistoryView::onHistoryFrameMissing(quint32 streamId)
{
    if (m_videoExport.active && streamId == m_videoExport.streamId && !m_videoExport.pending.isEmpty()) {
        m_videoExport.pending.removeFirst();
        exportNextVideoFrame();
    }
    // PlayerVideoFrame.Offline: that screen has nothing near this moment.
    if (m_deviceMonitorStreams.value(m_currentDeviceKey).contains(streamId)) {
        m_videoStrip->setOffline(streamId);
    }
}

void HistoryView::onActivityReceived(quint32 streamId, const QString &day,
                                     const QList<HistoryActivitySample> &samples)
{
    Q_UNUSED(day)
    if (streamId != currentStreamId() || m_timestamps.isEmpty()) {
        return;
    }
    m_activitySamples = samples;
    m_chart->setActivity(samples);
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
}

void HistoryView::refreshEfficiencyBar()
{
    m_chart->setEfficiency(m_appSegments, m_categories);
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
    const int index = m_timeline->currentIndex();
    if (index >= 0 && index < m_timestamps.size()) {
        updateKeystream(m_timestamps.at(index));
    }
}

void HistoryView::updateKeystream(qint64 timestampMs)
{
    // History.qml's two Keystreams: events up to historyModel.moment in
    // white, the ones after it in gray.
    QString past;
    QString future;
    for (const HistoryKeystrokeEntry &entry : std::as_const(m_keystrokeEntries)) {
        (entry.timestampMs <= timestampMs ? past : future) += entry.text;
    }
    m_keystream->setText(past, future);
}

void HistoryView::updateTextLogHighlight(qint64 timestampMs)
{
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
    updateStatusVisibility();
    hideLoadingDialog();
}
