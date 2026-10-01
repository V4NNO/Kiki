#include "historyview.h"

#include <QComboBox>
#include <QTabBar>
#include <QToolTip>
#include <QMenu>
#include <QHelpEvent>
#include <QCalendarWidget>
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
#include <QPainterPath>
#include <QSet>
#include <QShortcut>
#include <QAbstractItemView>
#include <QApplication>
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
#include <QUrl>
#include <QToolButton>
#include <QVBoxLayout>
#include <QWheelEvent>

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>

namespace {

// EfficiencyColors.qml "normal" colors, used for the chart's category bands.
QColor categoryColor(const QString &category)
{
    if (category == QStringLiteral("productive")) {
        return QColor(0x1f, 0x80, 0x57);
    }
    if (category == QStringLiteral("unproductive")) {
        return QColor(0x9d, 0x45, 0x3e);
    }
    if (category == QStringLiteral("neutral")) {
        return QColor(0xc2, 0x9c, 0x0b);
    }
    return QColor(0xa4, 0xa7, 0xab); // noneColorsMap.normal
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
    // Screens that stay keep what they already show; only new ones load.
    QList<Screen> screens;
    for (quint32 streamId : streamIds) {
        const auto existing = std::find_if(m_screens.cbegin(), m_screens.cend(),
                                           [streamId](const Screen &s) { return s.streamId == streamId; });
        screens.append(existing != m_screens.cend() ? *existing : Screen{streamId, {}, Status::Loading});
    }
    m_screens = screens;
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
    m_pressed = event->button() == Qt::LeftButton;
    m_dragging = false;
    m_pressGlobal = event->globalPosition().toPoint();
    m_pressScroll = m_scrollBar ? m_scrollBar->value() : 0;
}

void HistoryVideoStrip::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_pressed || !m_scrollBar) {
        return;
    }
    const int dx = event->globalPosition().toPoint().x() - m_pressGlobal.x();
    if (!m_dragging && qAbs(dx) >= QApplication::startDragDistance()) {
        m_dragging = true;
        setCursor(Qt::ClosedHandCursor);
    }
    if (m_dragging) {
        m_scrollBar->setValue(m_pressScroll - dx);
    }
}

void HistoryVideoStrip::mouseReleaseEvent(QMouseEvent *event)
{
    const bool wasDrag = m_dragging;
    m_pressed = false;
    m_dragging = false;
    unsetCursor();
    if (wasDrag) {
        return;
    }
    // A plain click on a screen is Video.qml's MouseArea -> panelFull.
    for (const QRect &r : screenRects()) {
        if (r.contains(event->pos())) {
            emit clicked();
            return;
        }
    }
}

namespace {
// TriLine.qml: 42px rows (content 5px down, 10px in), 1px apart.
constexpr int kTriLineHeight = 42;
constexpr int kTriLineSpacing = 1;
// Info.qml / WebPagesAndPrograms.qml insets inside the 420px panel.
constexpr int kInfoLeft = 10;
constexpr int kInfoTop = 10;
constexpr int kInfoWidth = 400; // panel.width - 20

// ProgressBar { kind: "big" } (compiled C++, measured from a real
// screenshot): a 78x14 inset pill with the rounded percentage centered.
constexpr int kProgressWidth = 78;
constexpr int kProgressHeight = 14;

QFont bigFont() // Fonts.rr_big
{
    QFont font(QStringLiteral("Roboto"));
    font.setPixelSize(14);
    return font;
}

QString triLinePercent(double percent)
{
    // TriLine.qml's ProgressBar value rounding.
    if (percent < 1) {
        return QString::number(std::floor(percent * 100) / 100);
    }
    if (percent < 10) {
        return QString::number(std::floor(percent * 10) / 10);
    }
    return QString::number(std::floor(percent));
}
}

HistoryInfoPanel::HistoryInfoPanel(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    relayout();
}

void HistoryInfoPanel::setItems(const QList<Item> &webPages, const QList<Item> &programs)
{
    m_webPages = webPages;
    m_programs = programs;
    relayout();
}

void HistoryInfoPanel::relayout()
{
    // WebPagesAndPrograms: Column(spacing 9) { WebPages, Programs }, each a
    // Column(spacing 10) { centered title, TriLine rows spacing 1 } followed
    // by an HSeparator.
    m_rows.clear();
    m_titles.clear();
    m_separatorsY.clear();
    int y = kInfoTop;
    m_separatorsY.append(0); // Info's own top HSeparator
    const auto addSection = [&](const QString &title, int height, const QList<Item> &items) {
        m_titles.append({QRect(kInfoLeft, y, kInfoWidth, height), title});
        y += height + 10;
        for (const Item &item : items) {
            m_rows.append({QRect(kInfoLeft, y, kInfoWidth, kTriLineHeight), item});
            y += kTriLineHeight + kTriLineSpacing;
        }
        m_separatorsY.append(y);
        y += 9;
    };
    addSection(QStringLiteral("Web pages"), 20, m_webPages);
    addSection(QStringLiteral("Programs"), 20, m_programs);
    setMinimumHeight(y + kInfoTop);
    m_hoveredRow = -1;
    update();
}

QRect HistoryInfoPanel::categorizationRect(const Row &row) const
{
    // CategorizationButton (small, 13x13): 15px left of the ProgressBar,
    // which sits 10px in from the row's right edge.
    const int centerY = row.rect.top() + 5 + kTriLineHeight / 2 - 3;
    return QRect(row.rect.right() - 10 - kProgressWidth - 15 - 13, centerY - 6, 13, 13);
}

void HistoryInfoPanel::mouseMoveEvent(QMouseEvent *event)
{
    int hovered = -1;
    for (int i = 0; i < m_rows.size(); ++i) {
        if (m_rows.at(i).rect.adjusted(0, 0, 0, kTriLineSpacing).contains(event->pos())) {
            hovered = i;
            break;
        }
    }
    if (hovered != m_hoveredRow) {
        m_hoveredRow = hovered;
        update();
    }
}

void HistoryInfoPanel::leaveEvent(QEvent *)
{
    if (m_hoveredRow != -1) {
        m_hoveredRow = -1;
        update();
    }
}

void HistoryInfoPanel::mousePressEvent(QMouseEvent *event)
{
    // TriLine: clicking the texts or the categorization button opens the
    // categorization for that resource.
    if (m_hoveredRow < 0 || m_hoveredRow >= m_rows.size()) {
        return;
    }
    Q_UNUSED(event)
    emit categorizationRequested(m_rows.at(m_hoveredRow).item.resource);
}

void HistoryInfoPanel::paintEvent(QPaintEvent *)
{
    static const QPixmap activeProgram(QStringLiteral(":/sessionInfo/images/active_program.png"));
    static const QPixmap categorize(QStringLiteral(":/sessionInfo/categorizationButton/small_normal.png"));
    QPainter painter(this);
    // Background { kind: "hatching" }: a real screenshot shows it as a flat
    // #46474e (the "hatching" is below one level of pixel noise).
    painter.fillRect(rect(), QColor(0x46, 0x47, 0x4e));

    for (int y : std::as_const(m_separatorsY)) {
        drawHorizontalSeparator(painter, y == 0 ? 0 : kInfoLeft, y, y == 0 ? width() : kInfoWidth);
    }

    painter.setPen(Qt::white);
    // Section headers "Web pages"/"Programs" are Fonts.rb_big_b (Roboto Bold,
    // 11pt bold) -- the row titles below use the regular rr_big instead.
    QFont sectionFont = bigFont();
    sectionFont.setBold(true);
    painter.setFont(sectionFont);
    for (const Title &title : std::as_const(m_titles)) {
        painter.drawText(title.rect, Qt::AlignCenter, title.text);
    }

    const QFont titleFont = bigFont();
    const QFont resourceFont = mediumFont();
    const QFontMetrics titleMetrics(titleFont);
    const QFontMetrics resourceMetrics(resourceFont);
    for (int i = 0; i < m_rows.size(); ++i) {
        const Row &row = m_rows.at(i);
        const QRect &r = row.rect;
        if (i == m_hoveredRow) {
            painter.fillRect(r.adjusted(0, 1, 0, 2), QColor(255, 255, 255, 8));
        }
        drawHorizontalSeparator(painter, r.left(), r.top(), r.width());
        drawVerticalSeparator(painter, r.left() + 1, r.top(), r.height() + 2);
        drawVerticalSeparator(painter, r.right() - 1, r.top(), r.height() + 2);

        const int contentLeft = r.left() + 10;
        const int centerY = r.top() + 5 + kTriLineHeight / 2;
        int textLeft = contentLeft + 2;
        if (row.item.active) {
            painter.drawPixmap(contentLeft + 2, centerY - 4 - activeProgram.height() / 2, activeProgram);
            textLeft = contentLeft + 2 + activeProgram.width() + 6;
        }
        // Text column: title over resource, spacing 5, centered 3px up.
        const int textWidth = qMax(10, r.width() - 13 - kProgressWidth - 2 - 64 - (textLeft - contentLeft - 2));
        const int blockHeight = titleMetrics.height() + 5 + resourceMetrics.height();
        const int top = centerY - 3 - blockHeight / 2;
        painter.setFont(titleFont);
        painter.setPen(Qt::white);
        painter.drawText(QRect(textLeft, top, textWidth, titleMetrics.height()), Qt::AlignLeft | Qt::AlignVCenter,
                         titleMetrics.elidedText(row.item.title, Qt::ElideRight, textWidth));
        painter.setFont(resourceFont);
        const QString category = row.item.category.isEmpty() ? QStringLiteral("none") : row.item.category;
        // statusToColor gives the brighter "hovered" EfficiencyColors
        // variants here (a real screenshot's productive text is #29a16e,
        // i.e. #28a570, not the #1f8057 chart color); uncategorized is white.
        static const QHash<QString, QColor> kTextColors = {
            {QStringLiteral("productive"), QColor(0x28, 0xa5, 0x70)},
            {QStringLiteral("neutral"), QColor(0xec, 0xbd, 0x0b)},
            {QStringLiteral("unproductive"), QColor(0xcc, 0x55, 0x4a)},
        };
        painter.setPen(kTextColors.value(category, QColor(Qt::white)));
        painter.drawText(QRect(textLeft, top + titleMetrics.height() + 5, textWidth, resourceMetrics.height()),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         resourceMetrics.elidedText(row.item.resource, Qt::ElideRight, textWidth));

        if (i == m_hoveredRow) {
            painter.drawPixmap(categorizationRect(row).topLeft(), categorize);
        }

        const QRectF bar(r.right() - 10 - kProgressWidth, centerY - 3 - kProgressHeight / 2.0, kProgressWidth,
                         kProgressHeight);
        QPainterPath pill;
        pill.addRoundedRect(bar, kProgressHeight / 2.0, kProgressHeight / 2.0);
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setClipPath(pill);
        // Measured: #43444b body, a 4px inner shadow from the top
        // (#39393f -> #44454b) and a 2px lighter bottom lip (#4d4e55/#4a4b52).
        painter.fillRect(bar, QColor(0x43, 0x44, 0x4b));
        QLinearGradient shadow(bar.topLeft(), QPointF(bar.left(), bar.top() + 4));
        shadow.setColorAt(0, QColor(0x39, 0x39, 0x3f));
        shadow.setColorAt(1, QColor(0x44, 0x45, 0x4b));
        painter.fillRect(QRectF(bar.left(), bar.top(), bar.width(), 4), shadow);
        painter.fillRect(QRectF(bar.left(), bar.bottom() - 2, bar.width(), 1), QColor(0x4d, 0x4e, 0x55));
        painter.fillRect(QRectF(bar.left(), bar.bottom() - 1, bar.width(), 1), QColor(0x4a, 0x4b, 0x52));
        painter.fillRect(QRectF(bar.left(), bar.top(), bar.width() * qBound(0.0, row.item.percent, 100.0) / 100,
                                bar.height()),
                         QColor(0x6f, 0x71, 0x7f));
        painter.restore();
        painter.setPen(Qt::white);
        QFont percentFont(QStringLiteral("Roboto"));
        percentFont.setPixelSize(13);
        painter.setFont(percentFont);
        painter.drawText(bar, Qt::AlignCenter, triLinePercent(row.item.percent) + QStringLiteral("%"));
    }
}

namespace {
// Measured from a real screenshot of the dialog (695x286).
constexpr int kDialogWidth = 695;
constexpr int kDialogHeight = 286;
constexpr int kDialogRowCenter0 = 71;
constexpr double kDialogRowPitch = 23.5;
// RatingButtons option x positions (text starts) -- 10px after Row.qml's
// 220px name column.
constexpr int kRatingX[] = {274, 341, 389, 471};
const QString kRatingCategories[] = {QStringLiteral("productive"), QStringLiteral("neutral"),
                                     QStringLiteral("unproductive"), QStringLiteral("none")};
const QString kRatingLabels[] = {QStringLiteral("Productive"), QStringLiteral("Neutral"),
                                 QStringLiteral("Unproductive"), QStringLiteral("Uncategorized")};

QFont ratingFont()
{
    QFont font(QStringLiteral("Roboto"));
    font.setPixelSize(12);
    return font;
}
}

CategorizationDialog::CategorizationDialog(const QString &resource, const QString &globalCategory,
                                           const QString &employeeCategory, const QString &employeeName,
                                           QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::Dialog)
    , m_employeeName(employeeName)
    , m_category(globalCategory.isEmpty() ? QStringLiteral("none") : globalCategory)
    , m_employeeCategory(employeeCategory.isEmpty() ? QStringLiteral("none") : employeeCategory)
{
    // mask: a web page is categorized by its site, a program by itself.
    const QUrl url(resource);
    m_title = QStringLiteral("Efficiency %1").arg(url.isValid() && !url.host().isEmpty() ? url.host() : resource);
    setModal(true);
    setFixedSize(kDialogWidth, kDialogHeight);
    setMouseTracking(true);
}

QRect CategorizationDialog::ratingRect(int row, int option) const
{
    const QFontMetrics metrics(ratingFont());
    const int centerY = qRound(kDialogRowCenter0 + row * kDialogRowPitch);
    return QRect(kRatingX[option], centerY - 9, metrics.horizontalAdvance(kRatingLabels[option]), 18);
}

QRect CategorizationDialog::cancelRect() const
{
    return QRect(286, 238, 56, 26);
}

QRect CategorizationDialog::okRect() const
{
    return QRect(354, 238, 56, 26);
}

QRect CategorizationDialog::closeRect() const
{
    return QRect(width() - 26, 8, 20, 20);
}

void CategorizationDialog::mouseMoveEvent(QMouseEvent *event)
{
    m_hover = event->pos();
    update();
}

void CategorizationDialog::mousePressEvent(QMouseEvent *event)
{
    if (closeRect().contains(event->pos()) || cancelRect().contains(event->pos())) {
        reject();
        return;
    }
    if (okRect().contains(event->pos())) {
        accept();
        return;
    }
    for (int row = 0; row < 2; ++row) {
        for (int option = 0; option < 4; ++option) {
            if (ratingRect(row, option).contains(event->pos())) {
                (row == 0 ? m_category : m_employeeCategory) = kRatingCategories[option];
                update();
                return;
            }
        }
    }
}

void CategorizationDialog::paintEvent(QPaintEvent *)
{
    static const QPixmap indent(QStringLiteral(":/sessionInfo/categorizationPanel/indent.png"));
    static const QPixmap multiuser(QStringLiteral(":/sessionInfo/categorizationPanel/multiuser_on.png"));
    static const QPixmap user(QStringLiteral(":/sessionInfo/categorizationPanel/user_on.png"));
    QPainter painter(this);

    // Background { kind: "hatching" }, measured pixel-exact from the real
    // dialog: a 3px diagonal pattern, color by (x + y) mod 3.
    static const QImage hatch = [] {
        QImage tile(3, 3, QImage::Format_RGB32);
        const QRgb colors[] = {qRgb(0x48, 0x49, 0x51), qRgb(0x47, 0x48, 0x4f), qRgb(0x48, 0x49, 0x50)};
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                tile.setPixel(x, y, colors[(x + y) % 3]);
            }
        }
        return tile;
    }();
    painter.fillRect(rect(), QBrush(hatch));
    painter.setPen(QColor(0x62, 0x62, 0x6a));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    // Title (Fonts.rb_big_b area): white, 23px in; close cross top-right.
    painter.setPen(Qt::white);
    painter.setFont(bigFont());
    painter.drawText(QRect(23, 12, width() - 60, 24), Qt::AlignLeft | Qt::AlignVCenter, m_title);
    painter.setPen(QPen(closeRect().contains(m_hover) ? QColor(Qt::white) : QColor(0xbf, 0xbf, 0xc0), 1.6));
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawLine(QPointF(674, 13), QPointF(682, 22));
    painter.drawLine(QPointF(682, 13), QPointF(674, 22));
    painter.setRenderHint(QPainter::Antialiasing, false);
    drawHorizontalSeparator(painter, 1, 39, width() - 2);

    // Rows: name column (icons + text, 220px), then the rating options.
    const QString names[] = {QStringLiteral("All employees"), m_employeeName};
    const QFont nameFont = bigFont();
    const QFontMetrics nameMetrics(nameFont);
    for (int row = 0; row < 2; ++row) {
        const int centerY = qRound(kDialogRowCenter0 + row * kDialogRowPitch);
        const bool readOnly = false;
        int x = 23;
        if (row > 0) {
            painter.drawPixmap(x, centerY - 6, indent);
            x += 11 + 10;
        }
        const QPixmap &icon = row == 0 ? multiuser : user;
        painter.drawPixmap(x + (11 - icon.width()) / 2, centerY - icon.height() / 2, icon);
        x += 11 + 10;
        painter.setFont(nameFont);
        painter.setPen(readOnly ? QColor(Qt::gray) : QColor(Qt::white));
        const int nameWidth = 274 - 10 - x;
        painter.drawText(QRect(x, centerY - 11, nameWidth, 20), Qt::AlignLeft | Qt::AlignVCenter,
                         nameMetrics.elidedText(names[row], Qt::ElideRight, nameWidth));

        // RatingButtons: selected option in its (hovered) category color,
        // Uncategorized selected in white, the rest #a3a6a9.
        const QString selected = row == 0 ? m_category : m_employeeCategory;
        painter.setFont(ratingFont());
        for (int option = 0; option < 4; ++option) {
            QColor color(0xa3, 0xa6, 0xa9);
            if (kRatingCategories[option] == selected) {
                static const QColor kSelected[] = {QColor(0x28, 0xa5, 0x70), QColor(0xec, 0xbd, 0x0b),
                                                   QColor(0xcc, 0x55, 0x4a), QColor(Qt::white)};
                color = kSelected[option];
            } else if (!readOnly && ratingRect(row, option).contains(m_hover)) {
                color = QColor(Qt::white);
            }
            if (readOnly) {
                color = color.darker(130);
            }
            painter.setPen(color);
            painter.drawText(ratingRect(row, option), Qt::AlignLeft | Qt::AlignVCenter, kRatingLabels[option]);
        }
    }

    // Cancel (Button) / OK (GreenButton), centered at the bottom.
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(cancelRect().contains(m_hover) ? QColor(0x4b, 0x4c, 0x53) : QColor(0x42, 0x43, 0x49));
    painter.drawRoundedRect(cancelRect(), 4, 4);
    painter.setBrush(okRect().contains(m_hover) ? QColor(0x23, 0xb4, 0x76) : QColor(0x1e, 0xa1, 0x68));
    painter.drawRoundedRect(okRect(), 4, 4);
    painter.setPen(Qt::white);
    painter.setFont(ratingFont());
    painter.drawText(cancelRect(), Qt::AlignCenter, QStringLiteral("Cancel"));
    painter.drawText(okRect(), Qt::AlignCenter, QStringLiteral("OK"));
}

namespace {
// Measured from real screenshots of the dialog (449x292): fields 23px tall
// (HistoryChoicePanel's perdiodKind/perdiodValue), 20px apart, 87px in.
constexpr int kChoiceWidth = 449;
constexpr int kChoiceHeight = 292;
constexpr int kChoiceFieldX = 87;
constexpr int kChoiceFieldTop = 63;
constexpr int kChoiceFieldPitch = 43;
constexpr int kChoiceFieldHeight = 23;
constexpr int kChoiceFieldWidth = 340;
constexpr int kChoiceStepWidth = 160;
const QColor kChoiceFieldText(0x7d, 0x7e, 0x83);

const QList<qint64> kTimeSteps = {1000,      5000,      10000,     20000,       30000,      60000,
                                  5 * 60000, 10 * 60000, 20 * 60000, 30 * 60000, 3600000, 2 * 3600000};

QString timeStepText(qint64 ms)
{
    // TimeStepComboBox's "N second(s)" / "N minute(s)" / "N hour(s)".
    if (ms >= 3600000 && ms % 3600000 == 0) {
        return QStringLiteral("%1 hour(s)").arg(ms / 3600000);
    }
    if (ms >= 60000 && ms % 60000 == 0) {
        return QStringLiteral("%1 minute(s)").arg(ms / 60000);
    }
    return QStringLiteral("%1 second(s)").arg(ms / 1000);
}

QFont choiceLabelFont() // Fonts.rb_medium_b -- renders as regular weight in real screenshots
{
    QFont font(QStringLiteral("Roboto"));
    font.setPixelSize(12);
    return font;
}

// GenericBox's Background { kind: "hatching" } as seen in this dialog.
QBrush choiceHatch()
{
    static const QImage tile = [] {
        QImage image(3, 3, QImage::Format_RGB32);
        const QRgb colors[] = {qRgb(0x49, 0x4a, 0x52), qRgb(0x47, 0x48, 0x50), qRgb(0x47, 0x48, 0x4f)};
        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                image.setPixel(x, y, colors[(x + y) % 3]);
            }
        }
        return image;
    }();
    return QBrush(tile);
}

QString styledMenuSheet()
{
    return QStringLiteral(
        "QMenu { background: #45464d; color: white; border: 1px solid #414248; padding: 2px; font-family: Roboto; font-size: 12px; }"
        "QMenu::item { padding: 4px 14px; }"
        "QMenu::item:selected { background: #5a5b63; }"
        "QMenu::item:disabled { color: #7d7e83; }");
}
}

HistoryChoiceDialog::HistoryChoiceDialog(Mode mode, const QList<QPair<quint32, QString>> &employees,
                                         quint32 employee, const QDate &day, qint64 timeStepMs,
                                         QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::Dialog)
    , m_title(mode == Mode::Add ? QStringLiteral("Add history watching")
                                : QStringLiteral("Change range and employee"))
    , m_employees(employees)
    , m_employee(employee)
    , m_day(day.isValid() ? day : QDate::currentDate())
    , m_timeStepMs(timeStepMs)
{
    setModal(true);
    setFixedSize(kChoiceWidth, kChoiceHeight);
    setMouseTracking(true);
}

QRect HistoryChoiceDialog::fieldRect(int row) const
{
    return QRect(kChoiceFieldX, kChoiceFieldTop + row * kChoiceFieldPitch,
                 row == 3 ? kChoiceStepWidth : kChoiceFieldWidth, kChoiceFieldHeight);
}

QRect HistoryChoiceDialog::cancelRect() const
{
    return QRect(161, 246, 57, 25);
}

QRect HistoryChoiceDialog::okRect() const
{
    return QRect(229, 246, 57, 25);
}

QRect HistoryChoiceDialog::closeRect() const
{
    return QRect(width() - 26, 8, 20, 20);
}

QRect HistoryChoiceDialog::infoRect() const
{
    // Row { spacing: 10 } after the 160px TimeStepComboBox.
    const QRect step = fieldRect(3);
    return QRect(step.right() + 1 + 10, step.center().y() - 12, 24, 24);
}

QRect HistoryChoiceDialog::previousRect() const
{
    const QRect field = fieldRect(2);
    return QRect(field.right() - 24, field.top(), 11, field.height());
}

QRect HistoryChoiceDialog::nextRect() const
{
    const QRect field = fieldRect(2);
    return QRect(field.right() - 13, field.top(), 11, field.height());
}

QString HistoryChoiceDialog::employeeName() const
{
    for (const auto &entry : m_employees) {
        if (entry.first == m_employee) {
            return entry.second;
        }
    }
    return {};
}

bool HistoryChoiceDialog::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip) {
        const auto *help = static_cast<QHelpEvent *>(event);
        if (m_timeStepMs != 1000 && infoRect().contains(help->pos())) {
            QToolTip::showText(help->globalPos(), QStringLiteral("No audio will be downloaded for this step"), this);
        } else {
            QToolTip::hideText();
        }
        return true;
    }
    return QDialog::event(event);
}

void HistoryChoiceDialog::mouseMoveEvent(QMouseEvent *event)
{
    m_hover = event->pos();
    update();
}

void HistoryChoiceDialog::showMenu(int row)
{
    QMenu menu(this);
    menu.setStyleSheet(styledMenuSheet());
    menu.setMinimumWidth(fieldRect(row).width());
    if (row == 0) {
        for (const auto &entry : m_employees) {
            menu.addAction(entry.second)->setData(entry.first);
        }
    } else if (row == 1) {
        // TimeRangeReport's kinds; only Day is supported by this History.
        const QString kinds[] = {QStringLiteral("Day"), QStringLiteral("Week"), QStringLiteral("Month"),
                                 QStringLiteral("Quarter"), QStringLiteral("Arbitrary period")};
        for (const QString &kind : kinds) {
            QAction *action = menu.addAction(kind);
            action->setEnabled(kind == kinds[0]);
        }
    } else if (row == 3) {
        for (qint64 step : kTimeSteps) {
            menu.addAction(timeStepText(step))->setData(step);
        }
    }
    const QAction *chosen = menu.exec(mapToGlobal(fieldRect(row).bottomLeft() + QPoint(0, 1)));
    if (!chosen || !chosen->data().isValid()) {
        return;
    }
    if (row == 0) {
        m_employee = chosen->data().toUInt();
    } else if (row == 3) {
        m_timeStepMs = chosen->data().toLongLong();
    }
    update();
}

void HistoryChoiceDialog::mousePressEvent(QMouseEvent *event)
{
    const QPoint pos = event->pos();
    if (closeRect().contains(pos) || cancelRect().contains(pos)) {
        reject();
        return;
    }
    if (okRect().contains(pos)) {
        if (m_employee == 0) {
            QMessageBox::warning(this, QStringLiteral("Error"), QStringLiteral("No selected employee"));
            return;
        }
        accept();
        return;
    }
    // SpinTextField: ◀ goes one period back, ▶ forward (not past today).
    if (previousRect().contains(pos)) {
        m_day = m_day.addDays(-1);
        update();
        return;
    }
    if (nextRect().contains(pos)) {
        if (m_day < QDate::currentDate()) {
            m_day = m_day.addDays(1);
            update();
        }
        return;
    }
    for (int row : {0, 1, 3}) {
        if (fieldRect(row).contains(pos)) {
            showMenu(row);
            return;
        }
    }
    if (fieldRect(2).contains(pos)) {
        auto *popup = new QCalendarWidget;
        popup->setWindowFlags(Qt::Popup);
        popup->setAttribute(Qt::WA_DeleteOnClose);
        popup->setMaximumDate(QDate::currentDate());
        popup->setSelectedDate(m_day);
        connect(popup, &QCalendarWidget::clicked, this, [this, popup](const QDate &date) {
            m_day = date;
            popup->close();
            update();
        });
        popup->move(mapToGlobal(fieldRect(2).bottomLeft() + QPoint(0, 1)));
        popup->show();
    }
}

void HistoryChoiceDialog::paintEvent(QPaintEvent *)
{
    static const QPixmap triangle(QStringLiteral(":/history/speedButton/triangle_normal.png"));
    static const QPixmap info(QStringLiteral(":/history/info.png"));
    QPainter painter(this);
    painter.fillRect(rect(), choiceHatch());
    painter.setPen(QColor(0x62, 0x62, 0x6a));
    painter.drawRect(rect().adjusted(0, 0, -1, -1));

    painter.setPen(Qt::white);
    painter.setFont(bigFont());
    painter.drawText(QRect(22, 12, width() - 60, 24), Qt::AlignLeft | Qt::AlignVCenter, m_title);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(closeRect().contains(m_hover) ? QColor(Qt::white) : QColor(0xbf, 0xbf, 0xc0), 1.6));
    painter.drawLine(QPointF(width() - 20, 13), QPointF(width() - 12, 21));
    painter.drawLine(QPointF(width() - 12, 13), QPointF(width() - 20, 21));
    painter.setRenderHint(QPainter::Antialiasing, false);
    drawHorizontalSeparator(painter, 1, 39, width() - 2);

    // Grid labels (rowSpacing 20, columnSpacing 5).
    painter.setFont(choiceLabelFont());
    painter.setPen(Qt::white);
    const QPair<int, QString> labels[] = {{0, QStringLiteral("Employee:")},
                                          {1, QStringLiteral("Period:")},
                                          {3, QStringLiteral("Time step:")}};
    for (const auto &label : labels) {
        const QRect field = fieldRect(label.first);
        painter.drawText(QRect(21, field.top(), kChoiceFieldX - 21, field.height()),
                         Qt::AlignLeft | Qt::AlignVCenter, label.second);
    }

    // Fields: #45464d, a 2-line top edge and 1px sides, no bottom edge.
    for (int row = 0; row < 4; ++row) {
        const QRect r = fieldRect(row);
        painter.fillRect(r, QColor(0x45, 0x46, 0x4d));
        painter.fillRect(QRect(r.left(), r.top(), r.width(), 1), QColor(0x38, 0x39, 0x3f));
        painter.fillRect(QRect(r.left(), r.top() + 1, r.width(), 1), QColor(0x41, 0x42, 0x48));
        painter.fillRect(QRect(r.left(), r.top(), 1, r.height()), QColor(0x41, 0x42, 0x48));
        painter.fillRect(QRect(r.right(), r.top(), 1, r.height()), QColor(0x41, 0x42, 0x48));
    }
    painter.setFont(mediumFont());
    const QFontMetrics metrics(mediumFont());
    const auto drawFieldText = [&](int row, int inset, const QColor &color, const QString &text, int reserve) {
        const QRect r = fieldRect(row).adjusted(inset, 1, -reserve, 0);
        painter.setPen(color);
        painter.drawText(r, Qt::AlignLeft | Qt::AlignVCenter, metrics.elidedText(text, Qt::ElideRight, r.width()));
    };
    drawFieldText(0, 6, kChoiceFieldText, employeeName(), 24);
    drawFieldText(1, 12, kChoiceFieldText, QStringLiteral("Day"), 24);
    drawFieldText(2, 5, kChoiceFieldText, QLocale(QLocale::English).toString(m_day, QStringLiteral("M/d/yyyy")), 30);
    drawFieldText(3, 12, Qt::white, timeStepText(m_timeStepMs), 24);

    // Search magnifier (SearchTextField), combo triangles, ◀▶ spin arrows.
    painter.setRenderHint(QPainter::Antialiasing);
    const QRect search = fieldRect(0);
    painter.setPen(QPen(kChoiceFieldText, 1.6));
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(QPointF(search.right() - 9.5, search.top() + 10), 3.6, 3.6);
    painter.drawLine(QPointF(search.right() - 12.2, search.top() + 12.8), QPointF(search.right() - 15.5, search.top() + 16));
    painter.setRenderHint(QPainter::Antialiasing, false);
    for (int row : {1, 3}) {
        const QRect r = fieldRect(row);
        painter.drawPixmap(r.right() - 19, r.center().y() - 2, triangle);
    }
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    const QRect previous = previousRect();
    const QRect next = nextRect();
    painter.setBrush(QColor(0xc8, 0xc8, 0xc8));
    painter.drawPolygon(QPolygonF({QPointF(previous.left() + 1, previous.center().y() + 0.5),
                                   QPointF(previous.left() + 6, previous.center().y() - 3.5),
                                   QPointF(previous.left() + 6, previous.center().y() + 4.5)}));
    painter.setBrush(m_day < QDate::currentDate() ? QColor(0xc8, 0xc8, 0xc8) : QColor(0x6f, 0x70, 0x76));
    painter.drawPolygon(QPolygonF({QPointF(next.left() + 7, next.center().y() + 0.5),
                                   QPointF(next.left() + 2, next.center().y() - 3.5),
                                   QPointF(next.left() + 2, next.center().y() + 4.5)}));
    painter.setRenderHint(QPainter::Antialiasing, false);

    if (m_timeStepMs != 1000) {
        painter.drawPixmap(infoRect().topLeft(), info);
    }

    // Cancel (Button) / OK (GreenButton).
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(cancelRect().contains(m_hover) ? QColor(0x4b, 0x4c, 0x53) : QColor(0x42, 0x43, 0x49));
    painter.drawRoundedRect(cancelRect(), 4, 4);
    painter.setBrush(okRect().contains(m_hover) ? QColor(0x23, 0xb4, 0x76) : QColor(0x1e, 0xa1, 0x68));
    painter.drawRoundedRect(okRect(), 4, 4);
    painter.setPen(Qt::white);
    painter.setFont(mediumFont());
    painter.drawText(cancelRect(), Qt::AlignCenter, QStringLiteral("Cancel"));
    painter.drawText(okRect(), Qt::AlignCenter, QStringLiteral("OK"));
}

KeystreamBar::KeystreamBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(30);
    m_scrollBar = new QScrollBar(Qt::Horizontal, this);
    m_scrollBar->setStyleSheet(QStringLiteral(
        "QScrollBar:horizontal { background: transparent; height: 6px; margin: 0; }"
        "QScrollBar::handle:horizontal { background: #5a5b63; border-radius: 3px; min-width: 30px; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }"));
    connect(m_scrollBar, &QScrollBar::valueChanged, this, qOverload<>(&QWidget::update));
    updateScrollRange();
}

int KeystreamBar::rowWidth() const
{
    const QFontMetrics metrics(mediumFont());
    return metrics.horizontalAdvance(m_past) + metrics.horizontalAdvance(m_future);
}

void KeystreamBar::updateScrollRange()
{
    const int viewport = width() - 10;
    const int overflow = qMax(0, rowWidth() - viewport);
    m_scrollBar->setRange(0, overflow);
    m_scrollBar->setPageStep(qMax(1, viewport));
    m_scrollBar->setSingleStep(20);
    m_scrollBar->setVisible(overflow > 0);
    m_scrollBar->setGeometry(5, height() - 7, qMax(1, viewport), 6);
}

void KeystreamBar::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateScrollRange();
}

void KeystreamBar::setText(const QString &past, const QString &future)
{
    if (past == m_past && future == m_future) {
        return;
    }
    m_past = past;
    m_future = future;
    updateScrollRange();
    update();
}

void KeystreamBar::wheelEvent(QWheelEvent *event)
{
    const int delta = event->angleDelta().x() != 0 ? event->angleDelta().x() : event->angleDelta().y();
    m_scrollBar->setValue(m_scrollBar->value() - delta / 2);
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
    const int x = 5 + qMax(0, (viewport - rowWidth) / 2) - m_scrollBar->value();
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

void HistoryChartWidget::setFillMode(bool on)
{
    if (m_fillMode == on) {
        return;
    }
    m_fillMode = on;
    if (on) {
        // Let the layout stretch it to fill the tab instead of the fixed 186px.
        setMinimumHeight(0);
        setMaximumHeight(QWIDGETSIZE_MAX);
    } else {
        setFixedHeight(kChartsItemHeight);
    }
    update();
}

void HistoryChartWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), kHistoryBackground);

    // Filters.qml: anchors.fill chartsItem, leftMargin 10, bottomMargin 15;
    // Chart.qml with needPlayerLine splits it 40% (Activity/Efficiency) /
    // separator1 (10px) / 60% (filters).
    // Fill mode (Violations tab): the grid fills the whole widget height and
    // there's no filters area; History's fixed strip splits 40% extra / 60%
    // filters instead.
    const double chartHeight = m_fillMode ? height() : height() - kChartBottomMargin;
    const double extraHeight = m_fillMode ? chartHeight : chartHeight * 0.4;
    const double filtersTop = m_fillMode ? 0.0 : extraHeight + 10;
    const double filtersHeight = m_fillMode ? 0.0 : chartHeight * 0.6;
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
                             categoryColor(category));
            y += bandHeight;
        }
    }
    painter.restore();

    // separator1 (between the extra rows and the filters area): only in the
    // fixed History layout -- fill mode has no filters area below.
    if (!m_fillMode) {
        drawHorizontalSeparator(painter, kChartLeft, extraHeight + 4, width() - kChartLeft - 25);
    }

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
    auto *root = new QVBoxLayout(this);
    // No page margins: videoCell/keylogger/sliderAndMeta/chartsItem span
    // the page edge to edge and carry their own insets (History.qml).
    root->setContentsMargins(0, 0, 0, 0);

    // HistoryPanel.qml: a 37px strip with ViewerControls/Tabs.qml
    // right-aligned in it -- the "+" button, the tabs (at most 200px each,
    // closable, draggable), then the left/right buttons.
    auto *tabStrip = new QFrame(this);
    tabStrip->setObjectName(QStringLiteral("subbar"));
    tabStrip->setFixedHeight(37);
    auto *tabRow = new QHBoxLayout(tabStrip);
    tabRow->setContentsMargins(0, 0, 3, 0);
    tabRow->setSpacing(3);
    tabRow->addStretch(1);
    auto *addTabButton = new QToolButton(tabStrip);
    addTabButton->setObjectName(QStringLiteral("plus"));
    addTabButton->setText(QStringLiteral("+"));
    addTabButton->setToolTip(QStringLiteral("Add history watching"));
    connect(addTabButton, &QToolButton::clicked, this, &HistoryView::openAddTabDialog);
    tabRow->addWidget(addTabButton);
    tabRow->addSpacing(5);
    m_historyTabBar = new QTabBar(tabStrip);
    m_historyTabBar->setObjectName(QStringLiteral("tabs"));
    m_historyTabBar->setExpanding(false);
    m_historyTabBar->setTabsClosable(true);
    m_historyTabBar->setMovable(true);
    m_historyTabBar->setElideMode(Qt::ElideRight);
    m_historyTabBar->setStyleSheet(QStringLiteral("QTabBar#tabs::tab { max-width: 200px; }"));
    connect(m_historyTabBar, &QTabBar::currentChanged, this, &HistoryView::showHistoryTab);
    connect(m_historyTabBar, &QTabBar::tabCloseRequested, this, &HistoryView::closeHistoryTab);
    connect(m_historyTabBar, &QTabBar::tabMoved, this, [this](int from, int to) {
        m_historyTabs.move(from, to);
        if (m_currentTab == from) {
            m_currentTab = to;
        } else if (from < m_currentTab && to >= m_currentTab) {
            --m_currentTab;
        } else if (from > m_currentTab && to <= m_currentTab) {
            ++m_currentTab;
        }
    });
    tabRow->addWidget(m_historyTabBar);
    const auto addArrow = [&](Qt::ArrowType direction, int step) {
        auto *arrow = new QToolButton(tabStrip);
        arrow->setObjectName(QStringLiteral("plus"));
        arrow->setArrowType(direction);
        connect(arrow, &QToolButton::clicked, this, [this, step] {
            const int target = m_historyTabBar->currentIndex() + step;
            if (target >= 0 && target < m_historyTabBar->count()) {
                m_historyTabBar->setCurrentIndex(target);
            }
        });
        tabRow->addWidget(arrow);
    };
    addArrow(Qt::LeftArrow, -1);
    addArrow(Qt::RightArrow, 1);
    root->addWidget(tabStrip);
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
        "QScrollArea#historyApps { border-left: 2px solid #43444c; border-right: 2px solid #43444c; }"));
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
    m_videoStrip->setScrollBar(m_monitorStripArea->horizontalScrollBar());
    m_monitorStripArea->setWidget(m_videoStrip);
    connect(m_videoStrip, &HistoryVideoStrip::clicked, this, &HistoryView::togglePanelFull);
    videoBody->addWidget(m_monitorStripArea, 1);
    // Running applications panel: 420px wide on the video's right edge,
    // below the header (History.qml `panel`), closed by default.
    // 2px #43444c lines left/right, WebPagesAndPrograms scrolling inside.
    m_infoArea = new QScrollArea(m_videoPanel);
    m_infoArea->setObjectName(QStringLiteral("historyApps"));
    m_infoArea->setFixedWidth(420);
    m_infoArea->setWidgetResizable(true);
    m_infoArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_infoPanel = new HistoryInfoPanel;
    m_infoArea->setWidget(m_infoPanel);
    connect(m_infoPanel, &HistoryInfoPanel::categorizationRequested, this,
            &HistoryView::onCategorizationRequested);
    m_infoArea->hide();
    videoBody->addWidget(m_infoArea);
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
    m_toggleAppsButton->setObjectName(QStringLiteral("historyAppsButton"));
    // Controls Button, measured from a real screenshot: #6b6d79 (seen as
    // #63656f through the 0.8 opacity), 25px tall, flush under the video
    // header against its right edge, rounded bottom-left corner.
    m_toggleAppsButton->setFixedHeight(25);
    m_toggleAppsButton->setStyleSheet(QStringLiteral(
        "QPushButton#historyAppsButton { background: #6b6d79; color: white; border: none;"
        " border-bottom-left-radius: 5px; font-family: Roboto; font-size: 12px; padding: 0 10px; }"
        "QPushButton#historyAppsButton:hover { background: #757783; }"));
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
    // groupKeystrokeEntries comment above; it needs a KikiHost change,
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
    // dialog; m_dayCombo stays alive for selectDay/onDayChanged.
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

    // SliderBar.qml's rewind actions (Left/Right = one marker, Ctrl = ten)
    // and History.qml's stopPlay (Space).
    const auto addShortcut = [this](const QKeySequence &keys, auto slot) {
        auto *shortcut = new QShortcut(keys, this);
        shortcut->setContext(Qt::WidgetWithChildrenShortcut);
        connect(shortcut, &QShortcut::activated, this, slot);
    };
    addShortcut(QKeySequence(Qt::Key_Right), [this] { stepMarkers(1); });
    addShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right), [this] { stepMarkers(10); });
    addShortcut(QKeySequence(Qt::Key_Left), [this] { stepMarkers(-1); });
    addShortcut(QKeySequence(Qt::CTRL | Qt::Key_Left), [this] { stepMarkers(-10); });
    addShortcut(QKeySequence(Qt::Key_Space), [this] { onPlayClicked(); });

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
    connect(&m_connection, &ViewerConnection::historyWebVisitsReceived, this,
            &HistoryView::onWebVisitsReceived);
    connect(&m_connection, &ViewerConnection::historyCategoriesReceived, this,
            &HistoryView::onCategoriesReceived);
    connect(&m_connection, &ViewerConnection::historyEmployeeCategoriesReceived, this,
            &HistoryView::onEmployeeCategoriesReceived);
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
    const QList<quint32> previousStreams = m_deviceMonitorStreams.value(m_currentDeviceKey);
    m_deviceNames = deviceNames;
    m_deviceMonitorStreams = deviceMonitorStreams;
    m_monitorNames = monitorNames;

    if (m_historyTabs.isEmpty()) {
        // First device list: start with one tab for the first employee, today.
        if (!deviceNames.isEmpty()) {
            addHistoryTab({deviceNames.cbegin().key(), QDate::currentDate().toString(QStringLiteral("yyyyMMdd")),
                           m_timeStepMs});
        }
        return;
    }
    for (int i = 0; i < m_historyTabs.size(); ++i) {
        updateHistoryTabText(i);
    }
    if (m_currentDeviceKey != 0 && deviceNames.contains(m_currentDeviceKey)) {
        // The list is re-sent on every live update, so only act when this
        // employee's screens actually changed -- and then fetch the current
        // moment for them, instead of leaving new ones loading.
        if (deviceMonitorStreams.value(m_currentDeviceKey) != previousStreams) {
            rebuildMonitorStrip();
            requestFrameAt(m_timeline->currentIndex());
        }
    }
}

void HistoryView::activate()
{
    if (m_activated || m_currentTab < 0) {
        return;
    }
    m_activated = true;
    showHistoryTab(m_currentTab);
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
    m_activated = true;
    // Like GoToHistoryDialog: the current tab if it's already this
    // employee, otherwise a new tab for them.
    if (m_currentTab >= 0 && m_historyTabs.at(m_currentTab).deviceKey == deviceKey) {
        m_historyTabs[m_currentTab].day = targetDay;
        jumpTo(deviceKey, targetDay);
        return;
    }
    addHistoryTab({deviceKey, targetDay, m_timeStepMs});
}

void HistoryView::jumpTo(quint32 deviceKey, const QString &day)
{
    m_currentDay = day;
    if (deviceKey != m_currentDeviceKey || m_allDays.isEmpty()) {
        switchDevice(deviceKey);
        return;
    }
    selectDay(day);
}

void HistoryView::selectDay(const QString &day)
{
    m_currentDay = day;
    const int index = m_dayCombo->findText(day);
    if (index < 0) {
        showNoData();
        return;
    }
    if (index == m_dayCombo->currentIndex()) {
        onDayChanged(index);
    } else {
        m_dayCombo->setCurrentIndex(index);
    }
}

void HistoryView::showNoData()
{
    m_playbackTimer->stop();
    setPlaying(false);
    m_dayCombo->blockSignals(true);
    m_dayCombo->setCurrentIndex(-1);
    m_dayCombo->blockSignals(false);
    m_timestamps.clear();
    m_timeline->setTimestamps({});
    const auto [dayStart, dayEnd] = dayRangeMs(m_currentDay);
    m_timeline->setRange(dayStart, dayEnd);
    m_timeAxis->setRange(dayStart, dayEnd);
    m_chart->setRange(dayStart, dayEnd);
    m_chart->setActivity({});
    m_chart->setEfficiency({}, m_categories);
    m_videoStrip->setStreams({});
    m_videoHeader->setMoment(QString());
    m_keystream->setText(QString(), QString());
    m_infoPanel->setItems({}, {});
    m_statusLabel->setText(QStringLiteral("No information for selected period"));
    updateStatusVisibility();
}

QList<QPair<quint32, QString>> HistoryView::employeeList() const
{
    QList<QPair<quint32, QString>> employees;
    for (auto it = m_deviceNames.cbegin(); it != m_deviceNames.cend(); ++it) {
        employees.append({it.key(), it.value()});
    }
    std::sort(employees.begin(), employees.end(),
              [](const auto &a, const auto &b) { return a.second.localeAwareCompare(b.second) < 0; });
    return employees;
}

void HistoryView::updateHistoryTabText(int index)
{
    if (index < 0 || index >= m_historyTabs.size()) {
        return;
    }
    const quint32 deviceKey = m_historyTabs.at(index).deviceKey;
    const QString name = m_deviceNames.value(deviceKey, QStringLiteral("Employee %1").arg(deviceKey));
    m_historyTabBar->setTabText(index, name);
    m_historyTabBar->setTabToolTip(index, name);
}

void HistoryView::storeCurrentTab()
{
    if (m_currentTab < 0 || m_currentTab >= m_historyTabs.size()) {
        return;
    }
    m_historyTabs[m_currentTab] = {m_currentDeviceKey, m_currentDay, m_timeStepMs};
}

void HistoryView::addHistoryTab(const HistoryTab &tab)
{
    storeCurrentTab();
    m_historyTabs.append(tab);
    const QSignalBlocker blocker(m_historyTabBar);
    m_historyTabBar->addTab(QString());
    updateHistoryTabText(m_historyTabs.size() - 1);
    m_historyTabBar->setCurrentIndex(m_historyTabs.size() - 1);
    m_currentTab = m_historyTabs.size() - 1;
    if (m_activated) {
        showHistoryTab(m_currentTab);
    }
}

void HistoryView::showHistoryTab(int index)
{
    if (index < 0 || index >= m_historyTabs.size()) {
        return;
    }
    if (index != m_currentTab) {
        storeCurrentTab();
        m_currentTab = index;
    }
    if (!m_activated) {
        return;
    }
    const HistoryTab tab = m_historyTabs.at(index);
    m_timeStepMs = tab.timeStepMs;
    applyTimeStep();
    // A different tab can be the same employee: force the reload so the
    // day/step of this tab are the ones shown.
    m_allDays.clear();
    jumpTo(tab.deviceKey, tab.day);
}

void HistoryView::openAddTabDialog()
{
    HistoryChoiceDialog dialog(HistoryChoiceDialog::Mode::Add, employeeList(), 0, QDate::currentDate(),
                               m_timeStepMs, this);
    dialog.move(mapToGlobal(rect().center()) - QPoint(dialog.width() / 2, dialog.height() / 2));
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    m_activated = true;
    addHistoryTab({dialog.employee(), dialog.day().toString(QStringLiteral("yyyyMMdd")), dialog.timeStepMs()});
}

void HistoryView::closeHistoryTab(int index)
{
    if (index < 0 || index >= m_historyTabs.size()) {
        return;
    }
    // Tabs.qml's ConfirmBox.
    const QString name = m_historyTabBar->tabText(index);
    if (QMessageBox::question(this, QStringLiteral("Close tab"),
                              QStringLiteral("Close tab confirm \u00AB%1\u00BB").arg(name))
        != QMessageBox::Yes) {
        return;
    }
    storeCurrentTab();
    m_historyTabs.removeAt(index);
    {
        const QSignalBlocker blocker(m_historyTabBar);
        m_historyTabBar->removeTab(index);
    }
    if (m_historyTabs.isEmpty()) {
        m_currentTab = -1;
        m_currentDeviceKey = 0;
        m_currentDay.clear();
        showNoData();
        // HistoryPanel.openPanelName(): with no tabs left, ask for one.
        openAddTabDialog();
        return;
    }
    m_currentTab = -1;
    const int next = qMin(index, int(m_historyTabs.size()) - 1);
    {
        const QSignalBlocker blocker(m_historyTabBar);
        m_historyTabBar->setCurrentIndex(next);
    }
    showHistoryTab(next);
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
    // Right edge 1px inside the video's right border, top on the header's
    // last line (videoCell 7px inset + 24px header - 1).
    const QSize hint = m_toggleAppsButton->sizeHint();
    const QRect video(m_videoPanel->mapTo(this, QPoint(0, 0)), m_videoPanel->size());
    m_toggleAppsButton->setGeometry(video.right() - hint.width(), video.top() + 24 - 1, hint.width(), 25);
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
    m_timestamps.clear();
    m_timeline->setTimestamps({});
    m_timeline->setRange(0, 0);
    m_timeAxis->setRange(0, 0);
    m_chart->setRange(0, 0);
    m_chart->setActivity({});
    m_employeeCategories.clear();
    m_chart->setEfficiency({}, m_categories);
    m_videoHeader->setMoment(QString());
    m_keystream->setText(QString(), QString());
    m_webVisits.clear();
    m_appSegments.clear();
    m_activitySamples.clear();
    m_keystrokeEntries.clear();
    m_infoPanel->setItems({}, {});
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
    m_connection.requestWebVisits(streamId, day);
    m_connection.requestKeystrokes(streamId, day);
}

void HistoryView::onTimelineMoved(int index)
{
    requestFrameAt(index);
}

void HistoryView::stepMarkers(int markers)
{
    // One marker = one Time step. Like setOnlineMarkerForRewind, a target
    // with nothing recorded jumps to the next recorded frame after it
    // (Right) or the last one before it (Left), clamped at the ends.
    const int current = m_timeline->currentIndex();
    if (current < 0 || current >= m_timestamps.size()) {
        return;
    }
    const qint64 target = m_timestamps.at(current) + markers * m_timeStepMs;
    int next = current;
    if (markers > 0) {
        const auto it = std::lower_bound(m_timestamps.cbegin(), m_timestamps.cend(), target);
        next = it == m_timestamps.cend() ? m_timestamps.size() - 1 : int(it - m_timestamps.cbegin());
    } else {
        const auto it = std::upper_bound(m_timestamps.cbegin(), m_timestamps.cend(), target);
        next = it == m_timestamps.cbegin() ? 0 : int(it - m_timestamps.cbegin()) - 1;
    }
    if (next != current) {
        requestFrameAt(next);
    }
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
    updateInfoPanel();
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
    const bool visible = m_infoArea->isHidden();
    m_infoArea->setVisible(visible);
    m_toggleAppsButton->setIcon(QIcon(visible
        ? QStringLiteral(":/history/video/buttonRunningApp/applications_arrow_right.png")
        : QStringLiteral(":/history/video/buttonRunningApp/applications_arrow_left.png")));
}

void HistoryView::onChangeSettingsClicked()
{
    HistoryChoiceDialog dialog(HistoryChoiceDialog::Mode::Change, employeeList(), m_currentDeviceKey,
                               QDate::fromString(m_currentDay, QStringLiteral("yyyyMMdd")), m_timeStepMs,
                               this);
    dialog.move(mapToGlobal(rect().center()) - QPoint(dialog.width() / 2, dialog.height() / 2));
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    m_timeStepMs = dialog.timeStepMs();
    applyTimeStep();
    const QString day = dialog.day().toString(QStringLiteral("yyyyMMdd"));
    if (m_currentTab >= 0) {
        m_historyTabs[m_currentTab] = {dialog.employee(), day, m_timeStepMs};
        updateHistoryTabText(m_currentTab);
    }
    jumpTo(dialog.employee(), day);
}

void HistoryView::onDaysReceived(quint32 streamId, const QStringList &days)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_allDays = days;
    hideLoadingDialog();
    {
        const QSignalBlocker blocker(m_dayCombo);
        m_dayCombo->clear();
        m_dayCombo->addItems(days);
        m_dayCombo->setCurrentIndex(-1);
    }
    m_statusLabel->clear();
    updateStatusVisibility();
    selectDay(m_currentDay.isEmpty() ? QDate::currentDate().toString(QStringLiteral("yyyyMMdd"))
                                     : m_currentDay);
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
    updateInfoPanel();
}

void HistoryView::refreshEfficiencyBar()
{
    m_chart->setEfficiency(m_appSegments, effectiveCategories());
}

void HistoryView::onWebVisitsReceived(quint32 streamId, const QString &day,
                                      const QList<HistoryAppSegment> &visits)
{
    Q_UNUSED(day)
    if (streamId != currentStreamId()) {
        return;
    }
    m_webVisits = visits;
    updateInfoPanel();
}

void HistoryView::updateInfoPanel()
{
    // infoFrame for the current marker: what was used during this Time step,
    // each resource's share of it, and which one was in use at the moment.
    const int index = m_timeline->currentIndex();
    if (index < 0 || index >= m_timestamps.size()) {
        m_infoPanel->setItems({}, {});
        return;
    }
    const qint64 moment = m_timestamps.at(index);
    const auto [dayStart, dayEnd] = dayRangeMs(m_dayCombo->currentText());
    Q_UNUSED(dayEnd)
    const qint64 step = qMax<qint64>(1000, m_timeStepMs);
    const qint64 windowStart = dayStart + (moment - dayStart) / step * step;
    const qint64 windowEnd = windowStart + step;
    const QHash<QString, QString> categories = effectiveCategories();
    const auto build = [&](const QList<HistoryAppSegment> &segments) {
        QHash<QString, qint64> usedMs;
        QHash<QString, QString> latestTitle;
        QString active;
        qint64 totalMs = 0;
        for (const HistoryAppSegment &segment : segments) {
            const qint64 overlap = qMin(segment.endMs, windowEnd) - qMax(segment.startMs, windowStart);
            if (overlap > 0) {
                usedMs[segment.application] += overlap;
                totalMs += overlap;
            }
            if (!segment.title.isEmpty()) {
                latestTitle[segment.application] = segment.title;
            }
            if (segment.startMs <= moment && moment < segment.endMs) {
                active = segment.application;
            }
        }
        QList<HistoryInfoPanel::Item> items;
        for (auto it = usedMs.cbegin(); it != usedMs.cend(); ++it) {
            const QString title = latestTitle.value(it.key());
            items.append({it.key(), title.isEmpty() ? QStringLiteral("No title") : title,
                          100.0 * it.value() / qMax<qint64>(1, totalMs),
                          categories.value(it.key()), it.key() == active});
        }
        std::sort(items.begin(), items.end(), [](const HistoryInfoPanel::Item &a, const HistoryInfoPanel::Item &b) {
            return a.percent > b.percent;
        });
        return items;
    };
    m_infoPanel->setItems(build(m_webVisits), build(m_appSegments));
}

void HistoryView::onCategorizationRequested(const QString &resource)
{
    CategorizationDialog dialog(resource, m_categories.value(resource), m_employeeCategories.value(resource),
                                m_deviceNames.value(m_currentDeviceKey), this);
    dialog.move(mapToGlobal(rect().center()) - QPoint(dialog.width() / 2, dialog.height() / 2));
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    if (dialog.category() != m_categories.value(resource, QStringLiteral("none"))) {
        m_categories[resource] = dialog.category();
        m_connection.setAppCategory(currentStreamId(), resource, dialog.category());
    }
    if (dialog.employeeCategory() != m_employeeCategories.value(resource, QStringLiteral("none"))) {
        if (dialog.employeeCategory() == QStringLiteral("none")) {
            m_employeeCategories.remove(resource);
        } else {
            m_employeeCategories[resource] = dialog.employeeCategory();
        }
        m_connection.setAppCategory(currentStreamId(), resource, dialog.employeeCategory(), true);
    }
    refreshEfficiencyBar();
    updateInfoPanel();
}

QHash<QString, QString> HistoryView::effectiveCategories() const
{
    QHash<QString, QString> categories = m_categories;
    for (auto it = m_employeeCategories.cbegin(); it != m_employeeCategories.cend(); ++it) {
        categories.insert(it.key(), it.value());
    }
    return categories;
}

void HistoryView::onEmployeeCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_employeeCategories = categories;
    refreshEfficiencyBar();
    updateInfoPanel();
}

void HistoryView::onCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_categories = categories;
    refreshEfficiencyBar();
    updateInfoPanel();
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
