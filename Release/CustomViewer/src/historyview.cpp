#include "historyview.h"
#include "devicetilewidget.h"
#include "multiswitch.h"
#include "savedialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QCursor>
#include <QDateTimeEdit>
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QVariantAnimation>
#include <QLocale>
#include <QProgressDialog>
#include <QRegularExpression>
#include <QSettings>
#include <QStandardPaths>
#include <QDir>
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
#include <QFileInfo>
#include <QFormLayout>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QMouseEvent>
#include <QFontDatabase>
#include <QPainter>
#include <QPainterPath>
#include <QSet>
#include <QShortcut>
#include <QAbstractSpinBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTextEdit>
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
// Every History page's cache files (one page per window), for the cleanup.
QHash<const HistoryView *, QSet<QString>> &referencedCacheFiles()
{
    static QHash<const HistoryView *, QSet<QString>> files;
    return files;
}
}

// Instrumentation for the seek path: enable with
// QT_LOGGING_RULES="kiki.history.timing.debug=true" to see, per seek, how
// long the host took to answer and how long until the picture was on screen.
Q_LOGGING_CATEGORY(lcHistoryTiming, "kiki.history.timing")

namespace {

// Line.qml (displayKind "colors") paints the Efficiency bands via
// mkStyle_colorFromUnit -> ActiveApplicationRanker.statusToColor, which
// returns the "hovered" EfficiencyColors variants for the three rated
// categories; only the uncategorized default is noneColorsMap.normal.
QColor categoryColor(const QString &category)
{
    if (category == QStringLiteral("productive")) {
        return QColor(0x28, 0xa5, 0x70); // productiveColorsMap.hovered
    }
    if (category == QStringLiteral("unproductive")) {
        return QColor(0xcc, 0x55, 0x4a); // nonProductiveColorsMap.hovered
    }
    if (category == QStringLiteral("neutral")) {
        return QColor(0xec, 0xbd, 0x0b); // neutralColorsMap.hovered
    }
    return QColor(0xa4, 0xa7, 0xab); // noneColorsMap.normal (statusToColor default)
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
// SliderBar.qml:257's Item { width: 10 } does not shorten the bar on screen:
// measured on the running original (1920px, Time step 1s) the bar's last
// pixel is x 1907, i.e. it ends at its column's right edge (W - 12).
constexpr int kSliderBarRightGap = 0;
constexpr int kTimeLineY = 62;          // MultiSessionsSlider column, single session
constexpr int kTimeLineHighHeight = 16; // TimeLine.qml highLineHeight
constexpr int kTimeLineLowHeight = 12;  // TimeLine.qml lowLineHeight
constexpr int kChartLeft = 10;          // Filters anchors.leftMargin
constexpr int kChartBottomMargin = 15;  // Filters anchors.bottomMargin
constexpr int kLabelsLeft = 20;         // ExtraHeaders mainHeaderText leftMargin
constexpr int kRowHeight = 30;          // ExtraHeaders.rowHeight / HistoLine outer Item
constexpr int kHistogramHeight = 20;    // HistoLine.histogramHeight
constexpr int kProductivityHeight = 22; // Line.qml singleLineHeight (not a filter line)
// Violations tab: Chart.qml labelsAreaRightMargin -- the grid ends where the
// TimeLine above it does.
constexpr int kViolationsRightMargin = 25;

// Fonts.rb_small_b: 8pt. The running original draws it at regular weight
// (its "Roboto Bold" family name is a face name -- see roboto() in
// devicedetailview.cpp): "Wednesday, October 7, 2026" measures 138px of ink,
// Roboto's design width at 8pt.
QFont smallBoldFont()
{
    QFont font(QStringLiteral("Roboto"));
    font.setPointSize(8);
    return font;
}

// History.qml:407 exportAviRow.width = avi_export_normal.png (13) + spacing
// 5 + "Video" in Fonts.rb_small (8pt, the same face as smallBoldFont; QML
// Text rounds its implicitWidth up) + anchors.leftMargin 10 * 2. The speed
// button (History.qml:495) and exportTextButton (History.qml:508) take the
// same width.
int exportAviButtonWidth()
{
    const double textWidth = QFontMetricsF(smallBoldFont()).horizontalAdvance(QStringLiteral("Video"));
    return 13 + 5 + static_cast<int>(std::ceil(textWidth)) + 2 * 10;
}

// history/video/Header.qml:37-39 fmt(): toLocaleDateString(Qt.locale(),
// Locale.ShortFormat) + " " + toLocaleTimeString(..., "hh:mm:ss").
QString headerMomentText(const QDateTime &moment)
{
    return QStringLiteral("%1 %2").arg(QLocale::system().toString(moment.date(), QLocale::ShortFormat),
                                       moment.toString(QStringLiteral("hh:mm:ss")));
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

void HistoryVideoStrip::setLoading(quint32 streamId)
{
    for (Screen &screen : m_screens) {
        if (screen.streamId == streamId) {
            screen.status = Status::Loading;
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
            if (!screen.image.isNull()) {
                QSize fitted = screen.image.size();
                fitted.scale(r.size(), Qt::KeepAspectRatio);
                painter.drawImage(QRect(r.center().x() - fitted.width() / 2, r.center().y() - fitted.height() / 2,
                                        fitted.width(), fitted.height()),
                                  screen.image);
            }
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

void HistoryInfoPanel::setInset(int inset)
{
    m_inset = inset;
    relayout();
}

void HistoryInfoPanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
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
    // WebPagesAndPrograms rows are infoWidth wide (the panel minus its
    // inset on both sides).
    const int rowWidth = qMax(100, width() - 2 * m_inset);
    int y = m_inset;
    m_separatorsY.append(0); // Info's own top HSeparator
    const auto addSection = [&](const QString &title, int height, const QList<Item> &items) {
        m_titles.append({QRect(m_inset, y, rowWidth, height), title});
        y += height + 10;
        for (const Item &item : items) {
            m_rows.append({QRect(m_inset, y, rowWidth, kTriLineHeight), item});
            y += kTriLineHeight + kTriLineSpacing;
        }
        m_separatorsY.append(y);
        y += 9;
    };
    addSection(QStringLiteral("Web pages"), 20, m_webPages);
    addSection(QStringLiteral("Programs"), 20, m_programs);
    setMinimumHeight(y + m_inset);
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

HistoryInfoPanel::TextRects HistoryInfoPanel::textRects(const Row &row) const
{
    static const QPixmap activeProgram(QStringLiteral(":/sessionInfo/images/active_program.png"));
    const QRect &r = row.rect;
    const QFontMetrics titleMetrics(bigFont());
    const QFontMetrics resourceMetrics(mediumFont());
    const int contentLeft = r.left() + 10;
    const int centerY = r.top() + 5 + kTriLineHeight / 2;
    const int textLeft = contentLeft + 2 + (row.item.active ? activeProgram.width() + 6 : 0);
    // Text column: title over resource, spacing 5, centered 3px up.
    const int textWidth = qMax(10, r.width() - 13 - kProgressWidth - 2 - 64 - (textLeft - contentLeft - 2));
    const int blockHeight = titleMetrics.height() + 5 + resourceMetrics.height();
    const int top = centerY - 3 - blockHeight / 2;
    TextRects rects;
    rects.titleLine = QRect(textLeft, top, textWidth, titleMetrics.height());
    rects.resourceLine = QRect(textLeft, top + titleMetrics.height() + 5, textWidth, resourceMetrics.height());
    rects.title = QRect(rects.titleLine.topLeft(),
                        QSize(qMin(textWidth, titleMetrics.horizontalAdvance(row.item.title)), rects.titleLine.height()));
    rects.resource = QRect(rects.resourceLine.topLeft(),
                           QSize(qMin(textWidth, resourceMetrics.horizontalAdvance(row.item.resource)),
                                 rects.resourceLine.height()));
    return rects;
}

HistoryInfoPanel::Part HistoryInfoPanel::partAt(const QPoint &pos, int *rowIndex) const
{
    *rowIndex = -1;
    for (int i = 0; i < m_rows.size(); ++i) {
        const Row &row = m_rows.at(i);
        if (!row.rect.adjusted(0, 0, 0, kTriLineSpacing).contains(pos)) {
            continue;
        }
        *rowIndex = i;
        // The CategorizationButton only exists while the row is hovered,
        // and it is.
        if (categorizationRect(row).contains(pos)) {
            return Part::Categorize;
        }
        const TextRects rects = textRects(row);
        if (rects.title.contains(pos)) {
            return Part::Title;
        }
        if (rects.resource.contains(pos)) {
            return Part::Resource;
        }
        return Part::None;
    }
    return Part::None;
}

void HistoryInfoPanel::mouseMoveEvent(QMouseEvent *event)
{
    int hovered = -1;
    const Part part = partAt(event->pos(), &hovered);
    if (hovered != m_hoveredRow || part != m_hoveredPart) {
        m_hoveredRow = hovered;
        m_hoveredPart = part;
        // TextWithToolTip cursorHoveredEnable / the buttons' cursorShape.
        setCursor(part == Part::None ? Qt::ArrowCursor : Qt::PointingHandCursor);
        update();
    }
}

void HistoryInfoPanel::leaveEvent(QEvent *)
{
    if (m_hoveredRow != -1 || m_hoveredPart != Part::None) {
        m_hoveredRow = -1;
        m_hoveredPart = Part::None;
        unsetCursor();
        update();
    }
}

void HistoryInfoPanel::mousePressEvent(QMouseEvent *event)
{
    // TriLine's row-wide MouseArea has its click commented out: only the
    // texts and the button act, on click (press and release on the same).
    m_pressedPart = event->button() == Qt::LeftButton ? partAt(event->pos(), &m_pressedRow) : Part::None;
}

void HistoryInfoPanel::mouseReleaseEvent(QMouseEvent *event)
{
    int row = -1;
    const Part part = partAt(event->pos(), &row);
    const Part pressed = m_pressedPart;
    const int pressedRow = m_pressedRow;
    m_pressedPart = Part::None;
    m_pressedRow = -1;
    if (event->button() != Qt::LeftButton || part == Part::None || part != pressed || row != pressedRow) {
        return;
    }
    const Item item = m_rows.at(row).item;
    if (part == Part::Resource && item.webPage) {
        // text1 onClickedText: Qt.openUrlExternally(text) for a web page.
        QDesktopServices::openUrl(QUrl::fromUserInput(item.resource));
        return;
    }
    // The title, a program's executable and the CategorizationButton all
    // emitCategorizationRequest().
    emit categorizationRequested(item.resource, item.webPage);
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
        drawHorizontalSeparator(painter, y == 0 ? 0 : m_inset, y, y == 0 ? width() : width() - 2 * m_inset);
    }

    painter.setPen(Qt::white);
    // Section headers "Web pages"/"Programs" are Fonts.rb_big_b, which the
    // running original draws at regular weight like rr_big (see roboto() in
    // devicedetailview.cpp).
    painter.setFont(bigFont());
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
        if (row.item.active) {
            painter.drawPixmap(contentLeft + 2, centerY - 4 - activeProgram.height() / 2, activeProgram);
        }
        const TextRects rects = textRects(row);
        const int textWidth = rects.titleLine.width();
        painter.setFont(titleFont);
        painter.setPen(Qt::white);
        painter.drawText(rects.titleLine, Qt::AlignLeft | Qt::AlignVCenter,
                         titleMetrics.elidedText(row.item.title, Qt::ElideRight, textWidth));
        // Fonts.rr_medium_u while a web page's url is hovered.
        QFont lineFont = resourceFont;
        lineFont.setUnderline(row.item.webPage && i == m_hoveredRow && m_hoveredPart == Part::Resource);
        painter.setFont(lineFont);
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
        painter.drawText(rects.resourceLine, Qt::AlignLeft | Qt::AlignVCenter,
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

CategorizationDialog::CategorizationDialog(const QString &mask, const QList<Row> &rows, QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::Dialog)
    , m_title(QStringLiteral("Efficiency %1").arg(mask))
    , m_rows(rows)
{
    for (Row &row : m_rows) {
        if (row.category.isEmpty()) {
            row.category = QStringLiteral("none");
        }
    }
    setModal(true);
    setFixedSize(kDialogWidth, kDialogHeight + extraHeight());
    setMouseTracking(true);
}

int CategorizationDialog::extraHeight() const
{
    return qRound(qMax(0, int(m_rows.size()) - 2) * kDialogRowPitch);
}

QRect CategorizationDialog::ratingRect(int row, int option) const
{
    const QFontMetrics metrics(ratingFont());
    const int centerY = qRound(kDialogRowCenter0 + row * kDialogRowPitch);
    return QRect(kRatingX[option], centerY - 9, metrics.horizontalAdvance(kRatingLabels[option]), 18);
}

QRect CategorizationDialog::cancelRect() const
{
    return QRect(286, 238 + extraHeight(), 56, 26);
}

QRect CategorizationDialog::okRect() const
{
    return QRect(354, 238 + extraHeight(), 56, 26);
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
    for (int row = 0; row < m_rows.size(); ++row) {
        if (m_rows.at(row).readOnly) {
            continue;
        }
        for (int option = 0; option < 4; ++option) {
            if (ratingRect(row, option).contains(event->pos())) {
                m_rows[row].category = kRatingCategories[option];
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
    const QFont nameFont = bigFont();
    const QFontMetrics nameMetrics(nameFont);
    for (int row = 0; row < m_rows.size(); ++row) {
        const int centerY = qRound(kDialogRowCenter0 + row * kDialogRowPitch);
        const bool readOnly = m_rows.at(row).readOnly;
        int x = 23;
        // Row.qml: indent.png on every row but the root's.
        if (row > 0) {
            painter.drawPixmap(x, centerY - 6, indent);
            x += 11 + 10;
        }
        const QPixmap &icon = m_rows.at(row).employee ? user : multiuser;
        painter.drawPixmap(x + (11 - icon.width()) / 2, centerY - icon.height() / 2, icon);
        x += 11 + 10;
        painter.setFont(nameFont);
        painter.setPen(readOnly ? QColor(Qt::gray) : QColor(Qt::white));
        const int nameWidth = 274 - 10 - x;
        painter.drawText(QRect(x, centerY - 11, nameWidth, 20), Qt::AlignLeft | Qt::AlignVCenter,
                         nameMetrics.elidedText(m_rows.at(row).name, Qt::ElideRight, nameWidth));

        // RatingButtons: selected option in its (hovered) category color,
        // Uncategorized selected in white, the rest #a3a6a9.
        const QString selected = m_rows.at(row).category;
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

// TimeStepComboBox.qml valuesDomain (ms).
const QList<qint64> kTimeSteps = {
    1000,          5000,           10000,          20000,          30000,
    60000,         5 * 60000,      10 * 60000,     20 * 60000,     30 * 60000,
    3600000,       2 * 3600000,    4 * 3600000,    8 * 3600000,
    86400000LL,    2 * 86400000LL, 5 * 86400000LL, 10 * 86400000LL, 15 * 86400000LL,
    30 * 86400000LL, 60 * 86400000LL, 120 * 86400000LL, 240 * 86400000LL, 365 * 86400000LL,
};

QString timeStepText(qint64 ms)
{
    // TimeStepComboBox.valueLabel: "%n second(s)" / "%n minute(s)" /
    // "%n hour(s)" / "%n day(s)".
    if (ms < 60000) {
        return QStringLiteral("%1 second(s)").arg(qint64(ms / 1000.0 + 0.1));
    }
    if (ms < 3600000) {
        return QStringLiteral("%1 minute(s)").arg(qint64(ms / 60000.0 + 0.1));
    }
    if (ms < 86400000) {
        return QStringLiteral("%1 hour(s)").arg(qint64(ms / 3600000.0 + 0.1));
    }
    return QStringLiteral("%1 day(s)").arg(qint64(ms / 86400000.0 + 0.1));
}

// TimeRangeReport's kinds, in combo order, and their recentType.
const char *const kPeriodKinds[] = {"Day", "Week", "Month", "Quarter", "Arbitrary period"};
const char *const kRecentTypes[] = {"d", "w", "m", "q", ""};

// JS Date#setFullYear(year, month0, day): month and day overflow into the
// next/previous month/year.
QDate jsDate(int year, int month0, int day)
{
    return QDate(year, 1, 1).addMonths(month0).addDays(day - 1);
}

// DateTimeUtils.js getMonday().
QDate mondayOf(const QDate &date)
{
    return date.addDays(1 - date.dayOfWeek());
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

QPair<QDateTime, QDateTime> HistoryPeriod::getPeriod(const QString &kind, int multiplier)
{
    const QDateTime current = QDateTime::currentDateTime();
    const QDate today = current.date();
    const int factor = multiplier - 1;
    QDate startDate = today;
    QDate endDate = today;
    if (kind == QStringLiteral("d")) {
        endDate = today.addDays(-factor);
        startDate = endDate;
    } else if (kind == QStringLiteral("w")) {
        endDate = today.addDays(-7 * factor);
        startDate = mondayOf(endDate);
        if (factor) {
            endDate = startDate.addDays(6);
        }
    } else if (kind == QStringLiteral("m")) {
        endDate = jsDate(today.year(), today.month() - 1 - factor, today.day());
        startDate = QDate(endDate.year(), endDate.month(), 1);
        if (factor) {
            endDate = startDate.addMonths(1).addDays(-1);
        }
    } else if (kind == QStringLiteral("q")) {
        endDate = jsDate(today.year(), today.month() - 1 - 3 * factor, today.day());
        startDate = QDate(endDate.year(), (endDate.month() - 1) / 3 * 3 + 1, 1);
        if (factor) {
            endDate = startDate.addMonths(3).addDays(-1);
        }
    }
    const QDateTime start(startDate, QTime(0, 0));
    // end keeps the current time of day, or 23:59:59 (and the current ms --
    // setHours(23,59,59) leaves them) for a past period; never past now.
    QDateTime end(endDate, factor ? QTime(23, 59, 59, current.time().msec()) : current.time());
    if (end > current) {
        end = current;
    }
    return {start, end};
}

int HistoryPeriod::getMultiplier(const QString &kind, const QDateTime &start)
{
    const QDate today = QDate::currentDate();
    const QDate startDate = start.date();
    if (kind == QStringLiteral("d")) {
        return (startDate < today ? static_cast<int>(startDate.daysTo(today)) : 0) + 1;
    }
    if (kind == QStringLiteral("w")) {
        if (startDate.dayOfWeek() != 1) {
            return 1;
        }
        const QDate monday = mondayOf(today);
        return (startDate < monday ? static_cast<int>(startDate.daysTo(monday)) : 0) / 7 + 1;
    }
    if (kind == QStringLiteral("m") || kind == QStringLiteral("q")) {
        const int months = kind == QStringLiteral("m") ? 1 : 3;
        const QDate first = kind == QStringLiteral("m")
                                ? QDate(today.year(), today.month(), 1)
                                : QDate(today.year(), (today.month() - 1) / 3 * 3 + 1, 1);
        int i = 0;
        QDate d = today;
        do {
            ++i;
            d = first.addMonths(-months * (i - 1));
        } while (d > startDate);
        return i;
    }
    return 1;
}

HistoryPeriod HistoryPeriod::recent(const QString &kind, int multiplier)
{
    HistoryPeriod period;
    period.recentType = kind;
    period.recentMultiplier = multiplier;
    const auto [start, end] = getPeriod(kind, multiplier);
    period.recentBegin = start;
    period.recentEnd = end;
    period.customBegin = QDateTime::currentDateTime();
    period.customEnd = period.customBegin;
    return period;
}

QString HistoryPeriod::formatPeriod(const QDateTime &start, const QDateTime &end)
{
    // localeDateMediumFormat: the short date format with a 4-digit year.
    const QLocale english(QLocale::English);
    const QString from = english.toString(start.date(), QStringLiteral("M/d/yyyy"));
    if (start.msecsTo(end) < 86400000) {
        return from;
    }
    return QStringLiteral("%1 - %2").arg(from, english.toString(end.date(), QStringLiteral("M/d/yyyy")));
}

HistoryChoiceDialog::HistoryChoiceDialog(Mode mode, const QList<QPair<quint32, QString>> &employees,
                                         quint32 employee, const HistoryPeriod &period,
                                         qint64 timeStepMs, QWidget *parent)
    : QDialog(parent, Qt::FramelessWindowHint | Qt::Dialog)
    , m_title(mode == Mode::Add ? QStringLiteral("Add history watching")
                                : QStringLiteral("Change range and employee"))
    , m_employees(employees)
    , m_employee(employee)
    , m_period(period)
    , m_timeStepMs(timeStepMs)
{
    setModal(true);
    setFixedSize(kChoiceWidth, kChoiceHeight);
    setMouseTracking(true);

    // HistoryChoicePanel's Component.onCompleted: a recent period's
    // multiplier is recomputed against today, then loadFromStatePanel().
    if (m_period.type == QStringLiteral("recent") && m_period.recentBegin.isValid()) {
        m_period.recentMultiplier = HistoryPeriod::getMultiplier(m_period.recentType, m_period.recentBegin);
    }
    if (m_period.type == QStringLiteral("custom")) {
        m_kind = 4;
    } else {
        for (int kind = 0; kind < 4; ++kind) {
            if (m_period.recentType == QLatin1String(kRecentTypes[kind])) {
                m_kind = kind;
            }
        }
        m_multipliers[m_kind] = qMax(1, m_period.recentMultiplier);
    }

    // The "Arbitrary period" Begin — End DateTimeInputs (useTime), timeBound
    // 00:00:00 / 23:59:59 applied when the date is picked.
    const QRect field = fieldRect(2);
    const int dashWidth = 7 + QFontMetrics(mediumFont()).horizontalAdvance(QStringLiteral("—")) + 7;
    const int editWidth = (field.width() - dashWidth) / 2;
    const QString editStyle = QStringLiteral(
        "QDateTimeEdit { background: #45464d; color: #7d7e83; border: none; border-top: 2px solid #38393f;"
        " font-family: Roboto; font-size: 12px; padding-left: 4px; }"
        "QDateTimeEdit::drop-down { border: none; width: 14px; }");
    const auto makeEdit = [&](int x, const QDateTime &value, const QTime &bound) {
        auto *edit = new QDateTimeEdit(this);
        edit->setCalendarPopup(true);
        edit->setDisplayFormat(QStringLiteral("M/d/yyyy hh:mm"));
        edit->setStyleSheet(editStyle);
        edit->setGeometry(x, field.top(), editWidth, field.height());
        edit->setDateTime(value.isValid() ? value : QDateTime::currentDateTime());
        connect(edit, &QDateTimeEdit::dateChanged, this, [this, edit, bound](const QDate &date) {
            const QSignalBlocker blocker(edit);
            edit->setDateTime(QDateTime(date, bound));
            updateTimeSteps();
        });
        connect(edit, &QDateTimeEdit::timeChanged, this, [this] { updateTimeSteps(); });
        return edit;
    };
    m_customBeginEdit = makeEdit(field.left(), m_period.customBegin, QTime(0, 0, 0));
    m_customEndEdit = makeEdit(field.right() + 1 - editWidth, m_period.customEnd, QTime(23, 59, 59));
    updateCustomFields();
    updateTimeSteps();
}

QString HistoryChoiceDialog::recentType() const
{
    return QLatin1String(kRecentTypes[m_kind]);
}

void HistoryChoiceDialog::updateCustomFields()
{
    m_customBeginEdit->setVisible(isCustom());
    m_customEndEdit->setVisible(isCustom());
    update();
}

qint64 HistoryChoiceDialog::targetRangeMs() const
{
    if (isCustom()) {
        return qMax<qint64>(m_customBeginEdit->dateTime().msecsTo(m_customEndEdit->dateTime()), 1000);
    }
    const auto [start, end] = HistoryPeriod::getPeriod(recentType(), m_multipliers[m_kind]);
    return qMax<qint64>(start.msecsTo(end), 1000);
}

QList<qint64> HistoryChoiceDialog::stepsForRange() const
{
    // minSteps 10; maxSteps 86400 up to a day, 2304 up to 8 days, else 300.
    const qint64 target = targetRangeMs();
    const double maxSteps = target <= 86400000LL ? 86400 : (target <= 8 * 86400000LL ? 2304 : 300);
    QList<qint64> steps;
    for (qint64 value : kTimeSteps) {
        const double count = static_cast<double>(target) / value;
        if (count >= 10 && count <= maxSteps) {
            steps.append(value);
        }
    }
    return steps;
}

void HistoryChoiceDialog::updateTimeSteps()
{
    // currentIndex = the step closest to desirableValue (the current one).
    const QList<qint64> steps = stepsForRange();
    qint64 best = -1;
    qint64 bestDistance = std::numeric_limits<qint64>::max();
    for (qint64 step : steps) {
        const qint64 distance = qAbs(step - m_timeStepMs);
        if (distance < bestDistance) {
            bestDistance = distance;
            best = step;
        }
    }
    if (best > 0) {
        m_timeStepMs = best;
    }
    update();
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
        for (int kind = 0; kind < 5; ++kind) {
            menu.addAction(QString::fromLatin1(kPeriodKinds[kind]))->setData(kind);
        }
    } else if (row == 3) {
        for (qint64 step : stepsForRange()) {
            menu.addAction(timeStepText(step))->setData(step);
        }
    }
    const QAction *chosen = menu.exec(mapToGlobal(fieldRect(row).bottomLeft() + QPoint(0, 1)));
    if (!chosen || !chosen->data().isValid()) {
        return;
    }
    if (row == 0) {
        m_employee = chosen->data().toUInt();
    } else if (row == 1) {
        m_kind = chosen->data().toInt();
        updateCustomFields();
        updateTimeSteps();
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
        const QDateTime customBegin = m_customBeginEdit->dateTime();
        const QDateTime customEnd = m_customEndEdit->dateTime();
        if (isCustom() && (!customBegin.isValid() || !customEnd.isValid())) {
            QMessageBox::warning(this, QStringLiteral("Error"), QStringLiteral("Period has not been chosen"));
            return;
        }
        if (isCustom() && customBegin >= customEnd) {
            QMessageBox::warning(this, QStringLiteral("Error"), QStringLiteral("Wrong period"));
            return;
        }
        // HistoryChoicePanel onOk: the recent range is computed now and
        // kept fixed in the state.
        m_period.type = isCustom() ? QStringLiteral("custom") : QStringLiteral("recent");
        m_period.recentType = recentType();
        m_period.recentMultiplier = m_multipliers[m_kind];
        const auto [recentBegin, recentEnd] = HistoryPeriod::getPeriod(m_period.recentType,
                                                                       m_period.recentMultiplier);
        m_period.recentBegin = recentBegin;
        m_period.recentEnd = recentEnd;
        m_period.customBegin = customBegin;
        m_period.customEnd = customEnd;
        accept();
        return;
    }
    // SpinTextField: ◀ goes one period back, ▶ forward (while the
    // multiplier is above 1, i.e. not past the current period).
    if (!isCustom() && previousRect().contains(pos)) {
        ++m_multipliers[m_kind];
        updateTimeSteps();
        return;
    }
    if (!isCustom() && nextRect().contains(pos)) {
        if (m_multipliers[m_kind] > 1) {
            --m_multipliers[m_kind];
            updateTimeSteps();
        }
        return;
    }
    for (int row : {0, 1, 3}) {
        if (fieldRect(row).contains(pos)) {
            showMenu(row);
            return;
        }
    }
    // Day only: pick the day from a calendar (sets the multiplier).
    if (m_kind == 0 && fieldRect(2).contains(pos)) {
        auto *popup = new QCalendarWidget;
        popup->setWindowFlags(Qt::Popup);
        popup->setAttribute(Qt::WA_DeleteOnClose);
        popup->setMaximumDate(QDate::currentDate());
        popup->setSelectedDate(HistoryPeriod::getPeriod(QStringLiteral("d"), m_multipliers[0]).first.date());
        connect(popup, &QCalendarWidget::clicked, this, [this, popup](const QDate &date) {
            m_multipliers[0] = HistoryPeriod::getMultiplier(QStringLiteral("d"), QDateTime(date, QTime(0, 0)));
            popup->close();
            updateTimeSteps();
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

    // Fields: #45464d, a 2-line top edge and 1px sides, no bottom edge. The
    // custom range's two inputs draw their own field.
    for (int row = 0; row < 4; ++row) {
        if (row == 2 && isCustom()) {
            continue;
        }
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
    drawFieldText(1, 12, kChoiceFieldText, QString::fromLatin1(kPeriodKinds[m_kind]), 24);
    if (isCustom()) {
        painter.setPen(Qt::white);
        painter.drawText(fieldRect(2), Qt::AlignCenter, QStringLiteral("—"));
    } else {
        const auto [start, end] = HistoryPeriod::getPeriod(recentType(), m_multipliers[m_kind]);
        drawFieldText(2, 5, kChoiceFieldText, HistoryPeriod::formatPeriod(start, end), 30);
    }
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
    if (!isCustom()) {
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        const QRect previous = previousRect();
        const QRect next = nextRect();
        painter.setBrush(QColor(0xc8, 0xc8, 0xc8));
        painter.drawPolygon(QPolygonF({QPointF(previous.left() + 1, previous.center().y() + 0.5),
                                       QPointF(previous.left() + 6, previous.center().y() - 3.5),
                                       QPointF(previous.left() + 6, previous.center().y() + 4.5)}));
        painter.setBrush(m_multipliers[m_kind] > 1 ? QColor(0xc8, 0xc8, 0xc8) : QColor(0x6f, 0x70, 0x76));
        painter.drawPolygon(QPolygonF({QPointF(next.left() + 7, next.center().y() + 0.5),
                                       QPointF(next.left() + 2, next.center().y() - 3.5),
                                       QPointF(next.left() + 2, next.center().y() + 4.5)}));
        painter.setRenderHint(QPainter::Antialiasing, false);
    }

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
        "QScrollBar::handle:horizontal { background: none; min-width: 12px; border-width: 0 5px 0 7px; border-image: url(:/controls/scrollView/h_normal.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::handle:horizontal:hover { border-image: url(:/controls/scrollView/h_hovered.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::handle:horizontal:pressed { border-image: url(:/controls/scrollView/h_pressed.png) 0 5 0 7 repeat repeat; }"
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
    const int overflow = m_viewDisabled ? 0 : qMax(0, rowWidth() - viewport);
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

void KeystreamBar::setViewDisabled(bool disabled)
{
    if (disabled == m_viewDisabled) {
        return;
    }
    m_viewDisabled = disabled;
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

    if (m_viewDisabled) {
        // Row { spacing 10; centerIn } of history/warning.png and the white
        // text (default font).
        static const QPixmap warning(QStringLiteral(":/history/warning.png"));
        const QString text = QStringLiteral("viewer keylogger disabled");
        const QFontMetrics metrics(font());
        const int rowWidth = warning.width() + 10 + metrics.horizontalAdvance(text);
        int x = (width() - rowWidth) / 2;
        painter.drawPixmap(x, (height() - warning.height()) / 2, warning);
        x += warning.width() + 10;
        painter.setFont(font());
        painter.setPen(Qt::white);
        painter.drawText(QRect(x, 0, width() - x, height()), Qt::AlignLeft | Qt::AlignVCenter, text);
        return;
    }

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

namespace {
// The TooltipController's tooltips, on the window's top layer: the textual
// one on tooltipController/bg_tooltip.png (borders 30/17/30/22, text 17px
// in, 3px up, implicit width + 38), the knob's own flat #424349 box with
// the session's bullet and name.
class HoverTip final : public QWidget
{
public:
    explicit HoverTip(QWidget *parent)
        : QWidget(parent)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents);
    }
    void showText(const QString &text)
    {
        m_session = false;
        m_text = text;
        const QFontMetrics metrics(mediumFont());
        resize(metrics.horizontalAdvance(text) + 38, 54);
        update();
    }
    void showSession(const QString &text, bool alive)
    {
        m_session = true;
        m_text = text;
        m_alive = alive;
        QFont font = mediumFont();
        font.setPointSize(8);
        resize(4 + 7 + 5 + QFontMetrics(font).horizontalAdvance(text) + 6, 24);
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        if (m_session) {
            painter.fillRect(rect(), QColor(0x42, 0x43, 0x49));
            painter.setPen(QColor(0x57, 0x58, 0x60));
            painter.drawRect(rect().adjusted(0, 0, -1, -1));
            // Session.qml: a 7px ring bullet, then the name (rb_small_b).
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(QPen(m_alive ? QColor(0x28, 0xa3, 0x6f) : QColor(0xed, 0x5b, 0x4c), 1.5));
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(QRectF(4.5, 7.5, 6, 6));
            QFont font = mediumFont();
            font.setPointSize(8);
            painter.setFont(font);
            painter.setPen(Qt::white);
            painter.drawText(QRect(4 + 7 + 5, 0, width() - 16, 21), Qt::AlignLeft | Qt::AlignVCenter, m_text);
            return;
        }
        static const QPixmap background(QStringLiteral(":/controls/tooltip/bg_tooltip.png"));
        // A nine-slice BorderImage.
        const int l = 30, t = 17, r = 30, b = 22;
        const int w = background.width(), h = background.height();
        const QRect target = rect();
        const auto slice = [&](const QRect &to, const QRect &from) {
            if (to.width() > 0 && to.height() > 0) {
                painter.drawPixmap(to, background, from);
            }
        };
        const int cw = target.width() - l - r, ch = target.height() - t - b;
        slice(QRect(0, 0, l, t), QRect(0, 0, l, t));
        slice(QRect(l, 0, cw, t), QRect(l, 0, w - l - r, t));
        slice(QRect(l + cw, 0, r, t), QRect(w - r, 0, r, t));
        slice(QRect(0, t, l, ch), QRect(0, t, l, h - t - b));
        slice(QRect(l, t, cw, ch), QRect(l, t, w - l - r, h - t - b));
        slice(QRect(l + cw, t, r, ch), QRect(w - r, t, r, h - t - b));
        slice(QRect(0, t + ch, l, b), QRect(0, h - b, l, b));
        slice(QRect(l, t + ch, cw, b), QRect(l, h - b, w - l - r, b));
        slice(QRect(l + cw, t + ch, r, b), QRect(w - r, h - b, r, b));
        painter.setFont(mediumFont());
        painter.setPen(Qt::white);
        painter.drawText(QRect(17, -3, width() - 17, height()), Qt::AlignLeft | Qt::AlignVCenter, m_text);
    }

private:
    QString m_text;
    bool m_session = false;
    bool m_alive = false;
};
} // namespace

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget(parent)
{
    // Spans sliderAndMeta from its top down to where TimeLine starts, so the
    // bar lands at the real y=35 of the 125px block.
    setFixedHeight(kTimeLineY);
    setMouseTracking(true);
    m_tipDelay = new QTimer(this);
    m_tipDelay->setSingleShot(true);
    m_tipDelay->setInterval(500);
    connect(m_tipDelay, &QTimer::timeout, this, [this] {
        if (m_tip == Tip::None) {
            return;
        }
        QWidget *layer = window();
        if (!m_tipWidget) {
            m_tipWidget = new HoverTip(layer);
        }
        auto *tip = static_cast<HoverTip *>(m_tipWidget.data());
        if (m_tip == Tip::Session) {
            tip->showSession(m_sessionLabel, m_sessionAlive);
        } else {
            tip->showText(QStringLiteral("No data for a given period"));
        }
        updateTip(mapFromGlobal(QCursor::pos()));
        tip->show();
        tip->raise();
        m_tipHide->start();
    });
    m_tipHide = new QTimer(this);
    m_tipHide->setSingleShot(true);
    m_tipHide->setInterval(8000);
    connect(m_tipHide, &QTimer::timeout, this, [this] {
        if (m_tipWidget) {
            m_tipWidget->hide();
        }
    });
}

void TimelineWidget::setSessionLabel(const QString &label, bool alive)
{
    m_sessionLabel = label;
    m_sessionAlive = alive;
}

TimelineWidget::Tip TimelineWidget::tipAt(const QPoint &pos) const
{
    if (m_markersAmount <= 0) {
        return Tip::None;
    }
    if (pickRect().contains(pos) && !m_sessionLabel.isEmpty()) {
        return Tip::Session;
    }
    // The online / loaded stretches carry MouseBlockers: only bg_none, i.e.
    // a marker with nothing, shows the textual tooltip.
    if (barRect().adjusted(0, -2, 0, 2).contains(pos)) {
        const int marker = markerForX(pos.x());
        if (marker >= 0 && !isAllowedMarker(marker)) {
            return Tip::NoData;
        }
    }
    return Tip::None;
}

void TimelineWidget::updateTip(const QPoint &pos)
{
    const Tip tip = m_dragging ? Tip::None : tipAt(pos);
    if (tip != m_tip) {
        m_tip = tip;
        if (m_tipWidget) {
            m_tipWidget->hide();
        }
        m_tipHide->stop();
        if (tip == Tip::None) {
            m_tipDelay->stop();
        } else {
            m_tipDelay->start();
        }
        return;
    }
    if (m_tipWidget && m_tipWidget->isVisible()) {
        // Follows the mouse: x + 10, clamped inside the window.
        QWidget *layer = m_tipWidget->parentWidget();
        const QPoint at = mapTo(layer, pos);
        const int x = at.x() + 10 + m_tipWidget->width() > layer->width() ? layer->width() - m_tipWidget->width()
                                                                          : at.x() + 10;
        const int y = at.y() + m_tipWidget->height() > layer->height() ? layer->height() - m_tipWidget->height()
                                                                       : at.y();
        m_tipWidget->move(x, y);
    }
}

void TimelineWidget::hideTip()
{
    m_tip = Tip::None;
    m_tipDelay->stop();
    m_tipHide->stop();
    if (m_tipWidget) {
        m_tipWidget->hide();
    }
}

void TimelineWidget::hideEvent(QHideEvent *event)
{
    hideTip();
    QWidget::hideEvent(event);
}

void TimelineWidget::setSessionRanges(const QList<QPair<qint64, qint64>> &ranges)
{
    m_sessionRanges = ranges;
    rebuildRanges();
}

void TimelineWidget::applySessionRanges()
{
    for (const auto &range : std::as_const(m_sessionRanges)) {
        const qint64 begin = qMax(range.first, m_rangeStart);
        const qint64 end = qMin(range.second, m_rangeEnd);
        if (end <= begin) {
            continue;
        }
        const int first = int((begin - m_rangeStart) / m_stepMs);
        const int last = qMin(m_markersAmount - 1, int((end - 1 - m_rangeStart) / m_stepMs));
        for (int marker = first; marker <= last; ++marker) {
            addMarker(m_effectiveRanges, marker);
        }
    }
}

int TimelineWidget::barWidth() const
{
    // SliderBar.qml:79-262: a RowLayout of the bar (fillWidth), a 10px Item
    // and sliderRightMargin (History.qml:41, 0) -- the bar is the slider's
    // width (MultiSessionsSlider.qml:117, = widthSlider) minus 10.
    return qMax(0, width() - kSliderBarRightGap);
}

double TimelineWidget::markWidth() const
{
    // SliderBar.qml: __sliderBar.width / markersAmount.
    return m_markersAmount > 0 ? static_cast<double>(barWidth()) / m_markersAmount : 0.0;
}

void TimelineWidget::addMarker(QList<MarkerRange> &ranges, int marker)
{
    // Keeps `ranges` sorted and non-overlapping; a marker that touches the
    // range before or after it merges them, so the bar never grows a seam.
    const auto after = std::upper_bound(
        ranges.cbegin(), ranges.cend(), marker,
        [](int value, const MarkerRange &range) { return value < range.start; });
    const int index = int(after - ranges.cbegin());
    if (index > 0) {
        MarkerRange &previous = ranges[index - 1];
        if (marker <= previous.stop) {
            return; // already covered
        }
        if (marker == previous.stop + 1) {
            previous.stop = marker;
            // It may now touch the next range too.
            if (index < ranges.size() && ranges.at(index).start <= previous.stop + 1) {
                previous.stop = qMax(previous.stop, ranges.at(index).stop);
                ranges.removeAt(index);
            }
            return;
        }
    }
    if (index < ranges.size() && ranges.at(index).start == marker + 1) {
        ranges[index].start = marker;
        return;
    }
    ranges.insert(index, MarkerRange{marker, marker});
}

bool TimelineWidget::contains(const QList<MarkerRange> &ranges, int marker)
{
    const auto after = std::upper_bound(
        ranges.cbegin(), ranges.cend(), marker,
        [](int value, const MarkerRange &range) { return value < range.start; });
    return after != ranges.cbegin() && marker <= (after - 1)->stop;
}

void TimelineWidget::rebuildRanges()
{
    // The marker grid itself changed (period or Time step), so every marker
    // index is renumbered: rebuild the gray layer and drop the blue one --
    // HistoryView re-applies it from its frame cache.
    m_markersAmount = (m_rangeEnd > m_rangeStart && m_stepMs > 0)
        ? int(qMax<qint64>(1, (m_rangeEnd - m_rangeStart + m_stepMs - 1) / m_stepMs))
        : 0;
    m_effectiveRanges.clear();
    m_loadedRanges.clear();
    if (m_markersAmount <= 0) {
        m_currentMarker = -1;
        update();
        return;
    }
    // m_timestamps is already sorted (setTimestamps), so this is one pass:
    // no sorting happens on repaint any more.
    int lastStop = -2;
    for (qint64 timestamp : std::as_const(m_timestamps)) {
        if (timestamp < m_rangeStart || timestamp >= m_rangeEnd) {
            continue;
        }
        const int marker = int((timestamp - m_rangeStart) / m_stepMs);
        if (marker <= lastStop) {
            continue;
        }
        // SliderBar.qml widens every recorded range by one marker, so a
        // recording whose frames land on alternate markers stays one range.
        if (!m_effectiveRanges.isEmpty() && marker <= lastStop + 1) {
            m_effectiveRanges.last().stop = marker;
        } else {
            m_effectiveRanges.append(MarkerRange{marker, marker});
        }
        lastStop = marker;
    }
    applySessionRanges();
    m_currentMarker = qBound(-1, m_currentMarker, m_markersAmount - 1);
    update();
}

void TimelineWidget::setTimestamps(const QList<qint64> &timestamps)
{
    m_timestamps = timestamps;
    std::sort(m_timestamps.begin(), m_timestamps.end());
    rebuildRanges();
}

void TimelineWidget::setRange(qint64 rangeStartMs, qint64 rangeEndMs)
{
    if (m_rangeStart == rangeStartMs && m_rangeEnd == rangeEndMs) {
        return;
    }
    m_rangeStart = rangeStartMs;
    m_rangeEnd = rangeEndMs;
    rebuildRanges();
}

void TimelineWidget::setStepMs(qint64 stepMs)
{
    if (stepMs <= 0 || stepMs == m_stepMs) {
        return;
    }
    m_stepMs = stepMs;
    rebuildRanges();
}

void TimelineWidget::markLoaded(int marker)
{
    if (marker < 0 || marker >= m_markersAmount || contains(m_loadedRanges, marker)) {
        return;
    }
    addMarker(m_loadedRanges, marker);
    update();
}

void TimelineWidget::markLoadedRange(qint64 fromMs, qint64 toMs)
{
    if (m_markersAmount <= 0 || toMs < fromMs) {
        return;
    }
    const int first = markerForTimestamp(fromMs);
    const int last = markerForTimestamp(toMs);
    if (first < 0 || last < first) {
        return;
    }
    bool changed = false;
    for (int marker = first; marker <= last; ++marker) {
        if (!contains(m_loadedRanges, marker)) {
            addMarker(m_loadedRanges, marker);
            changed = true;
        }
    }
    if (changed) {
        update();
    }
}

void TimelineWidget::clearLoaded()
{
    if (m_loadedRanges.isEmpty()) {
        return;
    }
    m_loadedRanges.clear();
    update();
}

void TimelineWidget::setCurrentMarker(int marker)
{
    if (marker < 0 || marker >= m_markersAmount || marker == m_currentMarker) {
        return;
    }
    m_currentMarker = marker;
    update();
}

bool TimelineWidget::isAllowedMarker(int marker) const
{
    return contains(m_effectiveRanges, marker);
}

int TimelineWidget::nearestAllowedMarker(int marker, bool preferRight) const
{
    if (m_effectiveRanges.isEmpty()) {
        return -1;
    }
    if (contains(m_effectiveRanges, marker)) {
        return marker;
    }
    const auto after = std::upper_bound(
        m_effectiveRanges.cbegin(), m_effectiveRanges.cend(), marker,
        [](int value, const MarkerRange &range) { return value < range.start; });
    const bool hasRight = after != m_effectiveRanges.cend();
    if (preferRight) {
        return hasRight ? after->start : (after - 1)->stop;
    }
    return after != m_effectiveRanges.cbegin() ? (after - 1)->stop : after->start;
}

int TimelineWidget::lastAllowedMarker() const
{
    return m_effectiveRanges.isEmpty() ? -1 : m_effectiveRanges.last().stop;
}

int TimelineWidget::markerForTimestamp(qint64 timestampMs) const
{
    if (m_markersAmount <= 0 || m_stepMs <= 0) {
        return -1;
    }
    return int(qBound<qint64>(0, (timestampMs - m_rangeStart) / m_stepMs,
                              qint64(m_markersAmount) - 1));
}

qint64 TimelineWidget::timestampForMarker(int marker) const
{
    if ((m_timestamps.isEmpty() && m_sessionRanges.isEmpty()) || marker < 0) {
        return -1;
    }
    const qint64 slotStart = m_rangeStart + qint64(marker) * m_stepMs;
    const qint64 slotEnd = slotStart + m_stepMs;
    // Binary search, not the linear "closest frame" scan the old slider did
    // on every mouse move.
    const auto at = std::lower_bound(m_timestamps.cbegin(), m_timestamps.cend(), slotStart);
    if (at != m_timestamps.cend() && *at < slotEnd) {
        return *at; // a frame inside this marker's own slice
    }
    // A reachable marker with no frame of its own (online, no video): its
    // own moment -- the screens answer for it (held picture or No video).
    if (isAllowedMarker(marker) || m_timestamps.isEmpty()) {
        return slotStart;
    }
    if (at == m_timestamps.cbegin()) {
        return m_timestamps.first();
    }
    const qint64 previousMs = *(at - 1);
    if (at == m_timestamps.cend()) {
        return previousMs;
    }
    return (slotStart - previousMs) <= (*at - slotStart) ? previousMs : *at;
}

QRect TimelineWidget::pickRect() const
{
    if (m_currentMarker < 0 || m_markersAmount <= 0) {
        return {};
    }
    // SliderBar.qml's __slider: Math.max(markWidth, 19) wide, pulled half an
    // image left only while it is at its natural 19px width; while dragged
    // it sits at draggedX instead of on the marker.
    const double width = pickWidth();
    const bool natural = width == 19.0;
    const double anchor = m_dragging ? m_dragX : markWidth() * m_currentMarker;
    const double x = anchor - (natural ? 9.5 : 0.0);
    return QRect(qRound(x), kSliderBarY + kSliderBarHeight / 2 - 9, qRound(width), 19);
}

double TimelineWidget::pickWidth() const
{
    return qMax(19.0, markWidth());
}

QRect TimelineWidget::barRect() const
{
    return QRect(0, kSliderBarY, barWidth(), kSliderBarHeight);
}

int TimelineWidget::rewindTarget(int current, int target, bool right) const
{
    if (isAllowedMarker(target)) {
        return target;
    }
    for (int i = 0; i < m_effectiveRanges.size(); ++i) {
        const MarkerRange &range = m_effectiveRanges.at(i);
        if (current >= range.start && current <= range.stop) {
            if (right) {
                return i < m_effectiveRanges.size() - 1 ? m_effectiveRanges.at(i + 1).start : range.stop;
            }
            return i > 0 ? m_effectiveRanges.at(i - 1).stop : range.start;
        }
    }
    return current;
}

int TimelineWidget::markerForX(int x) const
{
    const double step = markWidth();
    if (step <= 0.0) {
        return -1;
    }
    // SliderBar.qml: Math.floor(draggedX / markWidth).
    return int(qBound(0.0, std::floor(x / step), double(m_markersAmount - 1)));
}

void TimelineWidget::seekToX(int x)
{
    const int marker = markerForX(x);
    // SliderBar.qml only signals when the marker actually changes, which is
    // what keeps a drag from firing a request per pixel. And, like
    // HistoryTab.setOnlineMarker (what the original wires both the click and
    // the drag to), a marker with nothing recorded is simply refused: the
    // handle does not move into a gap.
    if (marker < 0 || marker == m_currentMarker || !isAllowedMarker(marker)) {
        return;
    }
    m_currentMarker = marker;
    update();
    emit markerSelected(marker);
}

void TimelineWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || m_markersAmount <= 0) {
        return;
    }
    // SliderBar.qml: only the pick starts a drag (mouseArea.onPressed); the
    // 4px bar's own MouseArea acts on onClicked, i.e. on release.
    if (pickRect().contains(event->pos())) {
        m_pressed = true;
        m_dragging = true;
        m_dragBeginX = event->pos().x();
        m_dragInitX = markWidth() * m_currentMarker;
        m_dragX = m_dragInitX;
        update();
        return;
    }
    m_barPressed = barRect().contains(event->pos());
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *event)
{
    updateTip(event->pos());
    if (m_dragging) {
        // onPositionChanged: draggedX clamped to [0, lastX], lastX =
        // bar width - pick width; the marker under it is requested whenever
        // it changes (a gap is refused, the pick still follows the mouse).
        const double lastX = barWidth() - pickWidth();
        m_dragX = qBound(0.0, m_dragInitX + (event->pos().x() - m_dragBeginX), qMax(0.0, lastX));
        update();
        seekToX(int(m_dragX));
        return;
    }
    const bool hovered = pickRect().contains(event->pos());
    if (hovered != m_pickHovered) {
        m_pickHovered = hovered;
        update();
    }
}

void TimelineWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging) {
        // onReleased: dragging = false -- the pick snaps back onto the marker.
        m_dragging = false;
    } else if (m_barPressed && barRect().contains(event->pos())) {
        seekToX(event->pos().x());
    }
    m_barPressed = false;
    m_pressed = false;
    update();
}

void TimelineWidget::leaveEvent(QEvent *)
{
    hideTip();
    if (m_pickHovered) {
        m_pickHovered = false;
        update();
    }
}

void TimelineWidget::drawRanges(QPainter &painter, const QList<MarkerRange> &ranges,
                                const QPixmap &image)
{
    // The ranges are precomputed and already in marker space, so a repaint is
    // one BorderImage per range -- SliderBar.qml's Repeater, exactly. Nothing
    // is sorted or re-derived here.
    const double step = markWidth();
    if (step <= 0.0) {
        return;
    }
    for (const MarkerRange &range : ranges) {
        drawBorderImage(painter,
                        QRectF(range.start * step, kSliderBarY,
                               qMax(1.0, (range.stop - range.start + 1) * step), kSliderBarHeight),
                        image, 0, 0);
    }
}

void TimelineWidget::paintEvent(QPaintEvent *)
{
    static const QPixmap bgNone(QStringLiteral(":/history/sliderBar/bg_none.png"));
    static const QPixmap bgOnline(QStringLiteral(":/history/sliderBar/bg_online_cropped.png"));
    static const QPixmap bgLoaded(QStringLiteral(":/history/sliderBar/bg_loaded_cropped.png"));
    static const QPixmap pickNormal(QStringLiteral(":/history/sliderBar/pick_normal.png"));
    static const QPixmap pickHovered(QStringLiteral(":/history/sliderBar/pick_hovered.png"));
    static const QPixmap pickPressed(QStringLiteral(":/history/sliderBar/pick_pressed.png"));

    QPainter painter(this);
    painter.fillRect(rect(), kHistoryBackground);
    // SliderBar.qml's three layers: bg_none everywhere, bg_online over every
    // recorded range (gray "exists but not downloaded"), bg_loaded on top
    // over what has actually been fetched (it grows as you watch).
    drawBorderImage(painter, QRectF(0, kSliderBarY, barWidth(), kSliderBarHeight), bgNone, 4, 4);
    if (m_markersAmount <= 0) {
        return;
    }
    drawRanges(painter, m_effectiveRanges, bgOnline);
    drawRanges(painter, m_loadedRanges, bgLoaded);

    const QRect pick = pickRect();
    if (!pick.isNull()) {
        drawBorderImage(painter, pick,
                        m_pressed ? pickPressed : (m_pickHovered ? pickHovered : pickNormal), 9, 9);
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

void HistoryChartWidget::setChartStepMs(qint64 stepMs)
{
    if (stepMs > 0 && stepMs != m_chartStepMs) {
        m_chartStepMs = stepMs;
        update();
    }
}

void HistoryChartWidget::setMarkerStepMs(qint64 stepMs)
{
    if (stepMs > 0 && stepMs != m_markerStepMs) {
        m_markerStepMs = stepMs;
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

void HistoryChartWidget::setActivity(const HistoryChartResult &result)
{
    m_activity = result;
    update();
}

void HistoryChartWidget::setProductivity(const HistoryChartResult &result)
{
    m_productivity = result;
    update();
}

void HistoryChartWidget::setFilterRows(const QList<HistoryChartFilterRow> &rows)
{
    // ChartsModel.qml allComplited: sorted by sortValue (the sum of all
    // values), largest first.
    const auto sortValue = [](const HistoryChartFilterRow &row) {
        double sum = 0;
        for (const HistoryChartSerie &serie : row.values.series) {
            for (double value : serie.values) {
                sum += value;
            }
        }
        return sum;
    };
    m_filterRows = rows;
    std::stable_sort(m_filterRows.begin(), m_filterRows.end(),
                     [&](const HistoryChartFilterRow &a, const HistoryChartFilterRow &b) {
                         return sortValue(a) > sortValue(b);
                     });
    update();
}

void HistoryChartWidget::clearSeries()
{
    m_activity = {};
    m_productivity = {};
    m_filterRows.clear();
    update();
}

void HistoryChartWidget::setCurrentPositionMs(qint64 positionMs)
{
    m_currentPositionMs = positionMs;
    update();
}

void HistoryChartWidget::setContentVisible(bool visible)
{
    if (m_contentVisible != visible) {
        m_contentVisible = visible;
        update();
    }
}

double HistoryChartWidget::gridWidthPx() const
{
    return qMax(1, width() - m_gridLeft - (m_fillMode ? kViolationsRightMargin : 0));
}

double HistoryChartWidget::markerWidthPx() const
{
    const qint64 span = m_rangeEnd - m_rangeStart;
    return span > 0 ? gridWidthPx() * m_markerStepMs / span : 0.0;
}

double HistoryChartWidget::markerX() const
{
    // x = sliderMarkWidt * selectedIndex + extraGrid.x.
    const int index = int((m_currentPositionMs - m_rangeStart) / qMax<qint64>(1, m_markerStepMs));
    return m_gridLeft + markerWidthPx() * index;
}

void HistoryChartWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_fillMode || event->button() != Qt::LeftButton || m_rangeEnd <= m_rangeStart || !m_contentVisible) {
        return;
    }
    const double x = event->position().x();
    // HistoryPlayerMarkerControl's sensor: 6px wide, centered on the line,
    // above the selector (z 1).
    if (m_currentPositionMs >= m_rangeStart && m_currentPositionMs < m_rangeEnd && qAbs(x - markerX()) <= 3) {
        m_markerDragging = true;
        m_dragBeginX = x;
        m_dragInitX = markerX();
        return;
    }
    // TimeRangeSelector over the grid.
    if (x >= m_gridLeft && x <= m_gridLeft + gridWidthPx()) {
        m_selecting = true;
        m_selectBeginX = x - m_gridLeft;
        m_zoneX = m_selectBeginX;
        m_zoneWidth = 0;
        update();
    }
}

void HistoryChartWidget::mouseMoveEvent(QMouseEvent *event)
{
    const double x = event->position().x();
    if (m_markerDragging) {
        const double lastX = m_gridLeft + gridWidthPx();
        const double newX = qBound(0.0, m_dragInitX + (x - m_dragBeginX), lastX);
        const double markWidth = markerWidthPx();
        if (markWidth > 0) {
            const int newMarker = qRound((newX - m_gridLeft) / markWidth);
            const int current = int((m_currentPositionMs - m_rangeStart) / qMax<qint64>(1, m_markerStepMs));
            if (newMarker != current && newMarker >= 0) {
                emit markerDragged(newMarker);
            }
        }
        return;
    }
    if (m_selecting) {
        const double current = qMax(0.0, x - m_gridLeft);
        m_zoneX = qMin(m_selectBeginX, current);
        m_zoneWidth = qMin(qAbs(current - m_selectBeginX), gridWidthPx() - m_zoneX);
        update();
    }
}

void HistoryChartWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (m_markerDragging) {
        m_markerDragging = false;
        return;
    }
    if (!m_selecting) {
        return;
    }
    m_selecting = false;
    const double span = double(m_rangeEnd - m_rangeStart);
    const qint64 left = m_rangeStart + qint64(span * (m_zoneX / gridWidthPx()));
    const qint64 right = m_rangeStart + qint64(span * ((m_zoneX + m_zoneWidth) / gridWidthPx()));
    m_zoneWidth = 0;
    update();
    // validRange: at least 5 minutes; anything shorter is dropped silently.
    if (right - left >= 5 * 60 * 1000) {
        emit intervalSelected(left, right);
    }
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

namespace {
// TimeToChartConverter.qml, built from a Selector's start/stop/granula.
struct TimeToChart {
    double t0 = 0;
    double dist = 1;
    double relativeInterval = 0;
    explicit TimeToChart(const HistoryChartResult &result)
        : t0(result.startMs)
        , dist(qMax<qint64>(1, result.stopMs - result.startMs))
        , relativeInterval(result.granulaMs / dist)
    {
    }
    double relativePosition(qint64 t) const { return (t - t0) / dist; }
};

// ActiveApplicationRanker.getArrayDistributions over [none, productive,
// neutral, nonProductive]: the ratings with volume, None also when nothing
// else has any -- drawn last-to-first, i.e. top to bottom nonProductive,
// productive, neutral, none.
QStringList arrayDistributions(const std::array<double, 4> &volumes)
{
    QStringList elements;
    if (volumes[3] > 0) {
        elements.prepend(QStringLiteral("unproductive"));
    }
    if (volumes[1] > 0) {
        elements.prepend(QStringLiteral("productive"));
    }
    if (volumes[2] > 0) {
        elements.prepend(QStringLiteral("neutral"));
    }
    if (volumes[0] > 0 || elements.isEmpty()) {
        elements.prepend(QStringLiteral("none"));
    }
    return elements;
}
} // namespace

void HistoryChartWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), kHistoryBackground);
    if (!m_contentVisible) {
        return;
    }

    // MultiSessionActivity: one HistoLine row (30px) per session serie;
    // ExtraContent.requestedHeigth is 192 with more than one, else 60.
    const int sessionCount = m_activity.series.size();
    // History (Filters.qml, needPlayerLine): anchors.fill chartsItem with
    // bottomMargin 15; extraLine 40% / separator1 10px / filtersLine 60%.
    // Violations tab (needPlayerLine false): extraLine requestedHeigth, the
    // filters area the rest.
    const double chartHeight = m_fillMode ? height() : height() - kChartBottomMargin;
    const double extraHeight = m_fillMode ? (sessionCount > 1 ? 192.0 : 60.0) : chartHeight * 0.4;
    const double filtersTop = extraHeight + 10;
    const double filtersHeight = m_fillMode ? qMax(0.0, height() - filtersTop) : chartHeight * 0.6;
    const int gridLeft = m_gridLeft;
    const double gridWidth = qMax(1, width() - gridLeft - (m_fillMode ? kViolationsRightMargin : 0));
    const qint64 span = m_rangeEnd - m_rangeStart;

    drawHorizontalSeparator(painter, kChartLeft, 0, width() - kChartLeft);

    // ExtraHeaders: "Activity" over the activity rows, "Efficiency" over the
    // productivity lanes, each 20px in and -- with a single session --
    // 10px down; header items are at least 10px tall and 20px apart while
    // there isn't exactly one session (column.spacing).
    const bool singleSession = sessionCount == 1;
    const double activityRowsHeight = sessionCount * kRowHeight + (sessionCount > 1 ? 15 : 0);
    const double productivityPadding = singleSession ? 0 : 20 + (sessionCount > 1 ? 18 : 0);
    const double productivityTop = activityRowsHeight + productivityPadding;
    painter.setFont(mediumFont());
    painter.setPen(Qt::white);
    const int ascent = QFontMetrics(mediumFont()).ascent();
    const double headerPad = singleSession ? 10 : 0;
    const double efficiencyHeaderTop =
        qMax(activityRowsHeight, 10.0) + (singleSession ? 0 : 20);
    painter.drawText(QPointF(kChartLeft + kLabelsLeft, headerPad + ascent), QStringLiteral("Activity"));
    painter.drawText(QPointF(kChartLeft + kLabelsLeft, efficiencyHeaderTop + headerPad + ascent),
                     QStringLiteral("Efficiency"));
    drawVerticalSeparator(painter, gridLeft - 13 - 2, 0, extraHeight);

    // FiltersLine: one label per filter row, top-aligned, 20px in; Content
    // rows (DendroidLine, filterLine) 10px apart. With a single session the
    // filter lanes are "doubled" (20px, rows 22px per session).
    const bool doubledFilters = singleSession;
    const double laneHeight = doubledFilters ? 20 : 10;
    const double lanePitch = laneHeight + 2;
    QList<double> filterRowTops;
    {
        double y = filtersTop;
        for (const HistoryChartFilterRow &row : std::as_const(m_filterRows)) {
            filterRowTops.append(y);
            const double rowHeight =
                qMax(row.values.series.size() * (doubledFilters ? 22.0 : 12.0), 22.0);
            painter.save();
            painter.setClipRect(QRectF(kChartLeft, filtersTop, gridLeft - kChartLeft, filtersHeight));
            painter.setPen(Qt::white);
            painter.drawText(QRectF(kChartLeft + kLabelsLeft, y, gridLeft - kChartLeft - kLabelsLeft - 15,
                                    rowHeight),
                             Qt::AlignLeft | Qt::AlignTop,
                             QFontMetrics(mediumFont()).elidedText(row.name, Qt::ElideRight,
                                                                   gridLeft - kChartLeft - kLabelsLeft - 15));
            painter.restore();
            y += rowHeight + 10;
        }
    }

    if (span <= 0) {
        return;
    }

    // Grid.qml, once over the extra rows and once over the filters area:
    // marks+1 one-pixel lines, markWidth = width / marks.
    const double marks = static_cast<double>(span) / qMax<qint64>(1, m_chartStepMs);
    const double gridMarkWidth = gridWidth / marks;
    const int lineCount = static_cast<int>(marks) + 1;
    for (int i = 0; i < lineCount; ++i) {
        const double x = gridLeft + gridMarkWidth * i;
        painter.fillRect(QRectF(x, 0, 1, extraHeight), kGridColor);
        painter.fillRect(QRectF(x, filtersTop, 1, filtersHeight), kGridColor);
    }

    painter.save();
    painter.setClipRect(QRectF(gridLeft, 0, gridWidth, extraHeight));

    // HistoLine.qml: fillRect(relativePosition(moment) * w, (1 - v) * h,
    // relativeInterval * w + 1, h) on a 20px canvas centered in its 30px
    // row, only for truthy values; v is the node's sum(volume_activity)/S.
    const TimeToChart activityConverter(m_activity);
    const double histogramTop0 = (sessionCount > 1 ? 15 : 0) + (kRowHeight - kHistogramHeight) / 2.0;
    QHash<QString, int> sessionRow;
    for (int row = 0; row < sessionCount; ++row) {
        const HistoryChartSerie &serie = m_activity.series.at(row);
        sessionRow.insert(serie.userName, row);
        const double top = histogramTop0 + row * kRowHeight;
        painter.save();
        painter.setClipRect(QRectF(gridLeft, top, gridWidth, kHistogramHeight), Qt::IntersectClip);
        const double barWidth = activityConverter.relativeInterval * gridWidth + 1;
        for (int i = 0; i < serie.moments.size() && i < serie.values.size(); ++i) {
            const double value = serie.values.at(i);
            if (value == 0) {
                continue;
            }
            const double x = gridLeft + activityConverter.relativePosition(serie.moments.at(i)) * gridWidth;
            painter.fillRect(QRectF(x, top + (1 - value) * kHistogramHeight, barWidth, kHistogramHeight),
                             kActivityColor);
        }
        painter.restore();
    }

    // ExtraContent's applicationsChart: one Line.qml lane ("colors") per
    // activity session, in that order (a session without a productivity
    // serie leaves its lane empty). Per moment: x = relPos * w - 0.5,
    // w = relInterval * w + 1, the lane (22px) split into equal bands, one
    // per rating with volume (getArrayDistributions), each in its
    // statusToColor.
    const TimeToChart productivityConverter(m_productivity);
    // DendroidLine lanes: spacing applicationsChartVerticalPadding - 14.
    const double productivityLanePitch = kProductivityHeight + (singleSession ? 0 : 20 - 14);
    for (const HistoryChartSerie &serie : std::as_const(m_productivity.series)) {
        if (!sessionRow.contains(serie.userName)) {
            continue;
        }
        const double laneTop = productivityTop + sessionRow.value(serie.userName) * productivityLanePitch;
        const double bandWidth = productivityConverter.relativeInterval * gridWidth + 1;
        for (int i = 0; i < serie.moments.size() && i < serie.volumes.size(); ++i) {
            const QStringList bands = arrayDistributions(serie.volumes.at(i));
            const double x =
                gridLeft + productivityConverter.relativePosition(serie.moments.at(i)) * gridWidth - 0.5;
            const double bandHeight = static_cast<double>(kProductivityHeight) / bands.size();
            double y = laneTop;
            for (int band = bands.size() - 1; band >= 0; --band) {
                painter.fillRect(QRectF(x, y, bandWidth, bandHeight), categoryColor(bands.at(band)));
                y += bandHeight;
            }
        }
    }
    painter.restore();

    // separator1 between the extra rows and the filters area.
    drawHorizontalSeparator(painter, kChartLeft, extraHeight + 4, width() - kChartLeft - 25);

    // Content rows, Line.qml "scaledColors" with isFilterLine: per session
    // lane, per moment with a value v != 0, a bar of the filter's color,
    // lane height scaled by v (at least 10%) and centered.
    painter.save();
    painter.setClipRect(QRectF(gridLeft, filtersTop, gridWidth, filtersHeight));
    for (int r = 0; r < m_filterRows.size(); ++r) {
        const HistoryChartFilterRow &row = m_filterRows.at(r);
        const TimeToChart converter(row.values);
        const double barWidth = converter.relativeInterval * gridWidth + 1;
        QColor color = row.color;
        color.setAlpha(255);
        for (int lane = 0; lane < row.values.series.size(); ++lane) {
            const HistoryChartSerie &serie = row.values.series.at(lane);
            const double laneTop = filterRowTops.at(r) + lane * lanePitch;
            for (int i = 0; i < serie.moments.size() && i < serie.values.size(); ++i) {
                const double v = serie.values.at(i);
                if (v == 0) {
                    continue;
                }
                const double h = laneHeight * (v <= 1 && v > 0 ? (v < 0.1 ? 0.1 : v) : 1);
                const double x = gridLeft + converter.relativePosition(serie.moments.at(i)) * gridWidth - 0.5;
                painter.fillRect(QRectF(x, laneTop + (laneHeight - h) / 2, barWidth, h), color);
            }
        }
    }
    painter.restore();

    // HistoryPlayerMarkerControl (History only): 1px khaki line at the
    // current marker, chart height + 10 tall.
    if (!m_fillMode && m_currentPositionMs >= m_rangeStart && m_currentPositionMs < m_rangeEnd) {
        const double markerWidth = gridWidth * m_markerStepMs / span;
        const double x =
            gridLeft + markerWidth * ((m_currentPositionMs - m_rangeStart) / m_markerStepMs);
        painter.fillRect(QRectF(x, 0, 1, chartHeight + 10), kMarkerColor);
    }

    // TimeRangeSelector while pressed: the zone and its two edges, #B7C245
    // once it spans at least 5 minutes, gray before, at 30% opacity.
    if (m_selecting && span > 0) {
        const bool valid = span * (m_zoneWidth / gridWidth) >= 5 * 60 * 1000;
        const QColor color = valid ? QColor(0xb7, 0xc2, 0x45) : QColor(Qt::gray);
        const double x0 = gridLeft + m_zoneX;
        const double selectorHeight = extraHeight + filtersHeight + (filtersHeight > 0 ? 10 : 0);
        painter.fillRect(QRectF(x0, 0, 1, selectorHeight), color);
        painter.fillRect(QRectF(x0 + m_zoneWidth, 0, 1, selectorHeight), color);
        QColor fill = color;
        fill.setAlphaF(0.3);
        painter.fillRect(QRectF(x0, 0, m_zoneWidth, selectorHeight), fill);
    }
}

HistoryChartsModel::HistoryChartsModel(ViewerConnection &connection, HistoryChartWidget *chart,
                                       QObject *parent)
    : QObject(parent)
    , m_connection(connection)
    , m_chart(chart)
{
    m_recallTimer = new QTimer(this);
    m_recallTimer->setSingleShot(true);
    m_recallTimer->setInterval(2500); // forcedRefreshShoter
    connect(m_recallTimer, &QTimer::timeout, this, [this] { request(QStringLiteral("productivity")); });
    connect(&m_connection, &ViewerConnection::historyChartSeriesReceived, this,
            &HistoryChartsModel::onSeries);
}

void HistoryChartsModel::setQuery(quint32 streamId, qint64 beginMs, qint64 endMs, qint64 chartStepMs)
{
    if (streamId == m_streamId && beginMs == m_beginMs && endMs == m_endMs
        && chartStepMs == m_chartStepMs) {
        return;
    }
    m_streamId = streamId;
    m_beginMs = beginMs;
    m_endMs = endMs;
    m_chartStepMs = chartStepMs;
    // HistoryTab rebuilds its ChartsModel on every start/stop/step change:
    // the old series are gone until the new ones arrive.
    m_chart->clearSeries();
    request(QStringLiteral("activity"));
    request(QStringLiteral("productivity"));
}

void HistoryChartsModel::clear()
{
    m_streamId = 0;
    m_pendingTags.clear();
    m_recallTimer->stop();
    m_chart->clearSeries();
}

void HistoryChartsModel::applicationsChanged()
{
    if (m_streamId != 0) {
        m_recallTimer->start();
    }
}

void HistoryChartsModel::request(const QString &kind)
{
    if (m_streamId == 0 || m_endMs <= m_beginMs || m_chartStepMs <= 0) {
        return;
    }
    // ChartsModel.qml granula: K_activity max(chartTimeStep / 5, 60) s,
    // K_byProductivity chartTimeStep s.
    const qint64 granulaMs = kind == QStringLiteral("activity")
                                 ? qMax<qint64>(m_chartStepMs / 5, 60 * 1000)
                                 : m_chartStepMs;
    const QString tag = QStringLiteral("chart%1").arg(++m_nextTag);
    m_pendingTags.insert(kind, tag);
    m_connection.requestChartSeries(m_streamId, kind, m_beginMs, m_endMs, granulaMs, tag);
}

void HistoryChartsModel::onSeries(quint32 streamId, const HistoryChartResult &result)
{
    if (streamId != m_streamId || m_pendingTags.value(result.kind) != result.tag) {
        return;
    }
    m_pendingTags.remove(result.kind);
    if (result.kind == QStringLiteral("activity")) {
        m_chart->setActivity(result);
    } else {
        m_chart->setProductivity(result);
    }
}

namespace {
// SpeedButton.qml:273-289's face: the speed_bg ImageButton (the stylesheet
// draws it) with "<speed>x" (Fonts.rb_medium, white) and triangle_normal.png
// side by side, 5px apart, centered.
class SpeedFaceButton final : public QPushButton
{
public:
    explicit SpeedFaceButton(QWidget *parent) : QPushButton(parent) {}
    void setSpeedText(const QString &text)
    {
        m_text = text;
        update();
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QPushButton::paintEvent(event); // background only: no text/icon set
        static const QPixmap triangle(QStringLiteral(":/history/speedButton/triangle_normal.png"));
        QPainter painter(this);
        QFont font(QStringLiteral("Roboto"));
        font.setPointSize(9);
        painter.setFont(font);
        const int textWidth = painter.fontMetrics().horizontalAdvance(m_text);
        const int x = (width() - (textWidth + 5 + triangle.width())) / 2;
        painter.setPen(Qt::white);
        painter.drawText(QRect(x, 0, textWidth + 1, height()), Qt::AlignLeft | Qt::AlignVCenter, m_text);
        painter.drawPixmap(x + textWidth + 5, (height() - triangle.height()) / 2, triangle);
    }

private:
    QString m_text;
};

// SpeedButton.qml's toolTip: a speedButton/toolTip.png BorderImage (1px
// borders, stretched; the arrow on top) as tall as the image, holding a
// MultiSwitch of the speeds (implicitTextMargin 4) 5px in, vertically
// centered; rowSwitch.width + 16 wide.
// utils/LoadingStatusDialog.qml: a 195x200 box on the window's top layer,
// centered: grayHatching Background with a 1px #62626a radius-4 border, and
// a centered Column (spacing 33) of the title (white, Fonts.rm_big_b) and a
// running Spinner (spinner.png turned 45 degrees every 300 ms).
class LoadingOverlay final : public QWidget
{
public:
    explicit LoadingOverlay(QWidget *parent) : QWidget(parent)
    {
        setFixedSize(195, 200);
        m_timer.setInterval(300);
        QObject::connect(&m_timer, &QTimer::timeout, this, [this] {
            m_angle = (m_angle + 45) % 360;
            update();
        });
        hide();
    }
    void showMessage(const QString &text)
    {
        m_text = text;
        QWidget *window = parentWidget();
        move((window->width() - width()) / 2, (window->height() - height()) / 2);
        raise();
        show();
        m_timer.start();
        update();
    }
    void dismiss()
    {
        m_timer.stop();
        hide();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        static const QPixmap hatching(QStringLiteral(":/activeCell/grayHatching.png"));
        static const QPixmap spinner(QStringLiteral(":/history/spinner/spinner.png"));
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath box;
        box.addRoundedRect(QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5), 4, 4);
        painter.fillPath(box, QBrush(hatching));
        painter.setPen(QColor(0x62, 0x62, 0x6a));
        painter.drawPath(box);
        QFont font(QStringLiteral("Roboto"));
        font.setPointSize(11);
        painter.setFont(font);
        const QFontMetrics metrics(font);
        const int blockHeight = metrics.height() + 33 + spinner.height();
        const int top = (height() - blockHeight) / 2;
        painter.setPen(Qt::white);
        painter.drawText(QRect(0, top, width(), metrics.height()), Qt::AlignHCenter | Qt::AlignVCenter, m_text);
        painter.translate(width() / 2.0, top + metrics.height() + 33 + spinner.height() / 2.0);
        painter.rotate(m_angle);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.drawPixmap(-spinner.width() / 2, -spinner.height() / 2, spinner);
    }

private:
    QString m_text;
    QTimer m_timer;
    int m_angle = 0;
};

class SpeedPopup final : public QWidget
{
public:
    SpeedPopup(const QStringList &speeds, QWidget *parent) : QWidget(parent)
    {
        m_image = QPixmap(QStringLiteral(":/history/speedButton/toolTip.png"));
        // Only the image's opaque part shows (the arrow sits on transparency).
        setStyleSheet(QStringLiteral("background: transparent;"));
        m_switch = new MultiSwitch(this);
        m_switch->setTextMargin(4);
        for (const QString &speed : speeds) {
            m_switch->addTab(speed);
        }
        const QSize hint = m_switch->sizeHint();
        setFixedSize(hint.width() + 16, m_image.height());
        m_switch->setGeometry(5, (height() - hint.height()) / 2, hint.width(), hint.height());
        hide();
    }
    MultiSwitch *multiSwitch() const { return m_switch; }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        const int w = m_image.width();
        const int h = m_image.height();
        const QRect r = rect();
        // 9-slice, border 1 on every side, everything stretched.
        const int xs[] = {0, 1, w - 1, w};
        const int ys[] = {0, 1, h - 1, h};
        const int xt[] = {0, 1, r.width() - 1, r.width()};
        const int yt[] = {0, 1, r.height() - 1, r.height()};
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                painter.drawPixmap(QRect(xt[col], yt[row], xt[col + 1] - xt[col], yt[row + 1] - yt[row]), m_image,
                                   QRect(xs[col], ys[row], xs[col + 1] - xs[col], ys[row + 1] - ys[row]));
            }
        }
    }

private:
    QPixmap m_image;
    MultiSwitch *m_switch = nullptr;
};
} // namespace

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
    tabRow->setSpacing(0);
    tabRow->addStretch(1);
    // Controls/ButtonPlus.qml (small), centered 1px low, then 5px.
    auto *plusHolder = new QWidget(tabStrip);
    auto *addTabButton = new PixmapButton(QStringLiteral(":/controls/buttonPlus/small"), plusHolder);
    plusHolder->setFixedSize(addTabButton->width(), 37);
    addTabButton->move(0, (37 - addTabButton->height()) / 2 + 1);
    addTabButton->setToolTip(QStringLiteral("Add history watching"));
    connect(addTabButton, &QAbstractButton::clicked, this, &HistoryView::openAddTabDialog);
    tabRow->addWidget(plusHolder);
    tabRow->addSpacing(5 + 3);
    m_historyTabBar = new ViewerTabBar(tabStrip);
    m_historyTabBar->setObjectName(QStringLiteral("tabs"));
    m_historyTabBar->setMovable(true);
    tabStrip->installEventFilter(this);
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
        saveTabs(); // tabMove: the new order is saved right away
    });
    tabRow->addWidget(m_historyTabBar, 0, Qt::AlignBottom);
    tabRow->addSpacing(3);
    // Controls/LeftRightButton.qml.
    const auto addArrow = [&](const QString &prefix, int step) {
        auto *arrow = new PixmapButton(prefix, tabStrip, QSize(9, 20));
        connect(arrow, &QAbstractButton::clicked, this, [this, step] {
            const int target = m_historyTabBar->currentIndex() + step;
            if (target >= 0 && target < m_historyTabBar->count()) {
                m_historyTabBar->setCurrentIndex(target);
            }
        });
        tabRow->addWidget(arrow);
    };
    addArrow(QStringLiteral(":/controls/leftRightButton/left"), -1);
    addArrow(QStringLiteral(":/controls/leftRightButton/right"), 1);
    m_tabStrip = tabStrip;
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
        "QScrollBar::handle:horizontal { background: none; min-width: 12px; border-width: 0 5px 0 7px; border-image: url(:/controls/scrollView/h_normal.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::handle:horizontal:hover { border-image: url(:/controls/scrollView/h_hovered.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::handle:horizontal:pressed { border-image: url(:/controls/scrollView/h_pressed.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }"
        "QScrollArea#historyApps { border-left: 2px solid #43444c; border-right: 2px solid #43444c; }"));
    auto *videoLayout = new QVBoxLayout(m_videoPanel);
    videoLayout->setContentsMargins(0, 0, 0, 0);
    videoLayout->setSpacing(0);
    m_videoHeader = new HistoryVideoHeader(m_videoPanel);
    videoLayout->addWidget(m_videoHeader);

    auto *videoBody = new QHBoxLayout;
    m_videoBody = videoBody;
    videoBody->setContentsMargins(0, 0, 0, 0);
    videoBody->setSpacing(0);
    m_monitorStripArea = new QScrollArea(m_videoPanel);
    m_monitorStripArea->setFrameShape(QFrame::NoFrame);
    // Video.qml: the ScrollView reaches 8px below the video (hscrollTopMargin
    // 8), so its scroll bar sits in the gap under the frames instead of
    // taking their height -- the area's own bar stays hidden and
    // m_stripScrollBar (positionOverlays) shows it there.
    m_monitorStripArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_monitorStripArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_monitorStripArea->viewport()->setAutoFillBackground(false);
    m_monitorStripArea->viewport()->installEventFilter(this);
    m_videoStrip = new HistoryVideoStrip;
    m_videoStrip->setScrollBar(m_monitorStripArea->horizontalScrollBar());
    m_monitorStripArea->setWidget(m_videoStrip);
    connect(m_videoStrip, &HistoryVideoStrip::clicked, this, &HistoryView::togglePanelFull);
    videoBody->addWidget(m_monitorStripArea, 1);
    m_stripScrollBar = new QScrollBar(Qt::Horizontal, this);
    m_stripScrollBar->setStyleSheet(QStringLiteral(
        "QScrollBar:horizontal { background: transparent; height: 5px; margin: 0; }"
        "QScrollBar::handle:horizontal { background: none; min-width: 12px; border-width: 0 5px 0 7px;"
        " border-image: url(:/controls/scrollView/h_normal.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::handle:horizontal:hover { border-image: url(:/controls/scrollView/h_hovered.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::handle:horizontal:pressed { border-image: url(:/controls/scrollView/h_pressed.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal { width: 0; }"
        "QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }"));
    {
        QScrollBar *inner = m_monitorStripArea->horizontalScrollBar();
        connect(inner, &QScrollBar::rangeChanged, this, [this](int min, int max) {
            m_stripScrollBar->setRange(min, max);
            m_stripScrollBar->setPageStep(m_monitorStripArea->horizontalScrollBar()->pageStep());
            m_stripScrollBar->setVisible(max > min);
        });
        connect(inner, &QScrollBar::valueChanged, m_stripScrollBar, &QScrollBar::setValue);
        connect(m_stripScrollBar, &QScrollBar::valueChanged, inner, &QScrollBar::setValue);
        m_stripScrollBar->hide();
    }
    // Running applications panel (History.qml `panel`): 420px, laid OVER the
    // video below its 24px header (the frames keep their full width), 2px
    // #43444c lines left/right on the hatching, WebPagesAndPrograms
    // scrolling inside (videoCell.height - 40 tall). It slides in from the
    // video's right edge to width - 420 - 10 in 250 ms (animationPanel);
    // closed by default. Geometry in positionOverlays().
    m_appsPanel = new QFrame(this);
    m_appsPanel->setObjectName(QStringLiteral("historyAppsPanel"));
    m_appsPanel->setFixedWidth(420);
    m_appsPanel->setStyleSheet(QStringLiteral(
        "QFrame#historyAppsPanel { background: #46474e; border-left: 2px solid #43444c;"
        " border-right: 2px solid #43444c; }"));
    auto *appsLayout = new QVBoxLayout(m_appsPanel);
    appsLayout->setContentsMargins(0, 0, 0, 0);
    m_infoArea = new QScrollArea(m_appsPanel);
    m_infoArea->setObjectName(QStringLiteral("historyApps"));
    m_infoArea->setFrameShape(QFrame::NoFrame);
    m_infoArea->setWidgetResizable(true);
    m_infoArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_infoPanel = new HistoryInfoPanel;
    m_infoArea->setWidget(m_infoPanel);
    connect(m_infoPanel, &HistoryInfoPanel::categorizationRequested, this,
            &HistoryView::onCategorizationRequested);
    appsLayout->addWidget(m_infoArea);
    appsLayout->addStretch(1);
    m_appsPanel->hide();
    m_appsAnimation = new QPropertyAnimation(m_appsPanel, "pos", this);
    m_appsAnimation->setDuration(250);
    connect(m_appsAnimation, &QPropertyAnimation::finished, this, [this] {
        if (!m_appsOpen) {
            m_appsPanel->hide();
            return;
        }
        // animationPanel.onStopped: rightPadding = panel.width -- the video's
        // viewport now ends where the panel begins.
        m_videoBody->setContentsMargins(0, 0, m_appsPanel->width(), 0);
    });
    videoLayout->addLayout(videoBody, 1);

    // History.qml statusIcon: anchors.fill the whole Video (its header
    // included), no background, no mouse area.
    m_historyStatusIcon = new BigStatusIcon(m_videoPanel);
    m_historyStatusIcon->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_historyStatusIcon->hide();

    // Video.qml's "excuse", centered over the video area (Fonts.rr_xtra).
    m_statusLabel = new QLabel(m_monitorStripArea);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    // Video.qml:44-48,156 excuse: Fonts.rr_xtra (Roboto 24pt), one line,
    // centered in the video area.
    m_statusLabel->setWordWrap(false);
    m_statusLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_statusLabel->setStyleSheet(QStringLiteral(
        "background: transparent; color: white; font-family: Roboto; font-size: 24pt;"));
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


    // History.qml's sliderAndMeta: a fixed 125px block, leftMargin 12, with
    // column1 (Change period / Video+speed / Text, spacing 10 after a 5px
    // spacer), column2 (the 60px round play button, 11px down), column21
    // (audio, only at Time step = 1s) and then the slider + TimeLine; the
    // panelViolation button sits at its bottom center. Buttons use the
    // original history/*.png assets.
    // qsTr("Change period") -- the running original shows its translation.
    m_changeSettingsButton = new QPushButton(QStringLiteral("Change settings"), this);
    m_changeSettingsButton->setObjectName(QStringLiteral("historyChangeButton"));
    // History.qml:380,395: width = twoButtons.width = exportAviButton.width
    // * 2 + column1.spacing (10).
    const int actionButtonWidth = exportAviButtonWidth();
    m_changeSettingsButton->setFixedSize(actionButtonWidth * 2 + 10, 25);
    connect(m_changeSettingsButton, &QPushButton::clicked, this, &HistoryView::onChangeSettingsClicked);

    const QIcon exportIcon = [] {
        QIcon icon;
        icon.addPixmap(QPixmap(QStringLiteral(":/history/avi_export_normal.png")), QIcon::Normal);
        icon.addPixmap(QPixmap(QStringLiteral(":/history/avi_export_hovered.png")), QIcon::Active);
        icon.addPixmap(QPixmap(QStringLiteral(":/history/avi_export_normal.png")), QIcon::Disabled);
        return icon;
    }();
    // "Video" saves the period as a .webm (VideoSaverSelector).
    m_exportVideoButton = new QPushButton(exportIcon, QStringLiteral("Video"), this);
    m_exportVideoButton->setObjectName(QStringLiteral("historyAssetButton"));
    m_exportVideoButton->setIconSize(QSize(13, 9));
    m_exportVideoButton->setFixedSize(actionButtonWidth, 19);
    connect(m_exportVideoButton, &QPushButton::clicked, this, &HistoryView::onExportVideoClicked);
    // SpeedButton.qml options.
    m_speedCombo = new QComboBox(this);
    m_speedCombo->setObjectName(QStringLiteral("historySpeed"));
    m_speedCombo->addItems({QStringLiteral("0.5x"), QStringLiteral("0.75x"), QStringLiteral("1x"),
                            QStringLiteral("2x"), QStringLiteral("4x"), QStringLiteral("8x"),
                            QStringLiteral("16x")});
    m_speedCombo->setFixedSize(65, 19);
    m_speedCombo->setCurrentIndex(2); // 1x
    connect(m_speedCombo, &QComboBox::currentIndexChanged, this, &HistoryView::onSpeedChanged);
    // SpeedButton.qml:273-289: the face is speed_bg with "<speed>x" and
    // triangle_normal.png centered in a Row (spacing 5); a click opens the
    // speed choice (the original's toolTip.png MultiSwitch, here a menu).
    // The combo stays as the speed model, hidden.
    m_speedCombo->hide();
    m_speedButton = new SpeedFaceButton(this);
    m_speedButton->setObjectName(QStringLiteral("historySpeedButton"));
    m_speedButton->setFixedSize(actionButtonWidth, 19); // History.qml:495
    const auto updateSpeedFace = [this] {
        static_cast<SpeedFaceButton *>(m_speedButton)->setSpeedText(m_speedCombo->currentText());
    };
    updateSpeedFace();
    connect(m_speedCombo, &QComboBox::currentIndexChanged, this, updateSpeedFace);
    {
        QStringList speeds;
        for (int i = 0; i < m_speedCombo->count(); ++i) {
            speeds.append(m_speedCombo->itemText(i));
        }
        auto *popup = new SpeedPopup(speeds, this);
        m_speedPopup = popup;
        popup->multiSwitch()->setCurrentIndex(m_speedCombo->currentIndex());
        connect(popup->multiSwitch(), &MultiSwitch::currentChanged, m_speedCombo, &QComboBox::setCurrentIndex);
        connect(m_speedCombo, &QComboBox::currentIndexChanged, popup->multiSwitch(), &MultiSwitch::setCurrentIndex);
    }
    // onClicked: checked = !checked -- the toolTip shows while checked.
    connect(m_speedButton, &QPushButton::clicked, this, [this] {
        if (m_speedPopup->isVisible()) {
            m_speedPopup->hide();
            return;
        }
        // x = triangle.x - row.width - 51, y = triangle.y + 12 (button
        // coordinates; the Row of "<speed>x", 5, the 8x5 triangle centered).
        QFont font(QStringLiteral("Roboto"));
        font.setPointSize(9);
        const int textWidth = QFontMetrics(font).horizontalAdvance(m_speedCombo->currentText());
        const int rowWidth = textWidth + 5 + 8;
        const int triangleX = (m_speedButton->width() - rowWidth) / 2 + textWidth + 5;
        const int triangleY = (m_speedButton->height() - 5) / 2;
        m_speedPopup->move(m_speedButton->mapTo(this, QPoint(triangleX - rowWidth - 51, triangleY + 12)));
        m_speedPopup->raise();
        m_speedPopup->show();
    });
    // History.qml:62-65: a click anywhere else on History unchecks it.
    qApp->installEventFilter(this);
    m_keylogTableButton = new QPushButton(exportIcon, QStringLiteral("Text"), this);
    m_keylogTableButton->setObjectName(QStringLiteral("historyAssetButton"));
    m_keylogTableButton->setIconSize(QSize(13, 9));
    m_keylogTableButton->setFixedSize(actionButtonWidth, 19); // History.qml:508
    connect(m_keylogTableButton, &QPushButton::clicked, this, &HistoryView::onKeylogTableClicked);

    m_playButton = new QPushButton(this);
    m_playButton->setObjectName(QStringLiteral("historyPlay"));
    m_playButton->setFixedSize(60, 60);
    m_playButton->setIconSize(QSize(23, 30));
    m_playButton->installEventFilter(this); // hover swaps to *_active.png
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
    connect(m_timeline, &TimelineWidget::markerSelected, this, &HistoryView::onTimelineMoved);
    m_applyTimer = new QTimer(this);
    m_applyTimer->setSingleShot(true);
    m_applyTimer->setInterval(0);
    connect(m_applyTimer, &QTimer::timeout, this, [this] { applyMarker(m_pendingApplyMarker); });
    m_frameWatchdog = new QTimer(this);
    m_frameWatchdog->setSingleShot(true);
    m_frameWatchdog->setInterval(10000);
    connect(m_frameWatchdog, &QTimer::timeout, this, [this] {
        qCDebug(lcHistoryTiming, "marker %d: no answer in 10s, releasing the slot",
                m_inFlightMarker);
        releaseFrameSlot();
    });
    m_timeAxis = new TimeAxisWidget(this);

    auto *controlBlock = new QWidget(this);
    controlBlock->setObjectName(QStringLiteral("historyControls"));
    controlBlock->setFixedHeight(kControlBlockHeight);
    controlBlock->setStyleSheet(QStringLiteral(
        // History.qml's root Background: grayHatching shows around sliderAndMeta.
        "QWidget#historyControls { background-image: url(:/activeCell/grayHatching.png); }"
        ".QWidget { background: transparent; }"
        // Button.qml: button/*.png BorderImage, 10px borders; rr_medium_b.
        "QPushButton#historyChangeButton { background: transparent; color: white; font-family: Roboto;"
        "  font-size: 9pt; border-width: 10px; padding: -10px;"
        "  border-image: url(:/activeCell/button/normal.png) 10 10 10 10 stretch stretch; }"
        "QPushButton#historyChangeButton:hover {"
        "  border-image: url(:/activeCell/button/hover.png) 10 10 10 10 stretch stretch; }"
        "QPushButton#historyChangeButton:pressed {"
        "  border-image: url(:/activeCell/button/press.png) 10 10 10 10 stretch stretch; }"
        // ImageButton (5px borders) on speed_bg*.png; the Row 10px in,
        // icon, 5, Fonts.rb_small text.
        "QPushButton#historyAssetButton, QPushButton#historySpeedButton { background: transparent;"
        "  color: white; font-family: Roboto; font-size: 8pt; text-align: left;"
        "  border-width: 5px; padding: -5px -5px -5px 5px;"
        "  border-image: url(:/history/speedButton/speed_bg.png) 5 5 5 5 stretch stretch; }"
        "QPushButton#historySpeedButton { text-align: center; padding: -5px; }"
        "QPushButton#historyAssetButton:hover, QPushButton#historySpeedButton:hover {"
        "  border-image: url(:/history/speedButton/speed_bg_hovered.png) 5 5 5 5 stretch stretch; }"
        "QPushButton#historyAssetButton:pressed, QPushButton#historySpeedButton:pressed {"
        "  border-image: url(:/history/speedButton/speed_bg_pressed.png) 5 5 5 5 stretch stretch; }"
        "QComboBox#historySpeed { border-image: url(:/history/speedButton/speed_bg.png); border: none;"
        "  color: white; font-family: Roboto; font-size: 11px; padding: 0 0 0 10px; }"
        "QComboBox#historySpeed:hover {"
        "  border-image: url(:/history/speedButton/speed_bg_hovered.png); }"
        "QComboBox#historySpeed::drop-down { border: none; width: 16px; }"
        "QComboBox#historySpeed::down-arrow {"
        "  image: url(:/history/speedButton/triangle_normal.png); width: 8px; height: 5px; }"
        "QComboBox#historySpeed::down-arrow:hover {"
        "  image: url(:/history/speedButton/triangle_hovered.png); }"
        "QPushButton#historyPlay { background: transparent; border-image: url(:/history/bg_play.png);"
        "  border: none; padding: 0 0 0 8px; }"
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
    twoButtons->addWidget(m_speedButton);
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
    m_sliderColumn = rightColumn;
    auto *rightColumnLayout = new QVBoxLayout(rightColumn);
    // Its right margin (what is left of the page past widthSlider) is set
    // by applyTimeStep.
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
    m_chartsModel = new HistoryChartsModel(m_connection, m_chart, this);
    root->addWidget(m_chart);
    // animationOpen / animationClose: chartsItem.wantHeight 0 <-> 186 in
    // 250 ms (linear), pushing the controls and the keylogger bar up; the
    // button's text and arrow follow only when it stops.
    m_chartAnimation = new QVariantAnimation(this);
    m_chartAnimation->setDuration(250);
    connect(m_chartAnimation, &QVariantAnimation::valueChanged, this,
            [this](const QVariant &value) { m_chart->setFixedHeight(value.toInt()); });
    connect(m_chartAnimation, &QVariantAnimation::finished, this, [this] {
        if (m_chartOpening) {
            m_chartOpen = true;
            m_chart->setContentVisible(true);
        } else {
            m_chartOpen = false;
            m_chart->hide();
            m_chart->setFixedHeight(kChartsItemHeight);
        }
        updateViolationToggleText();
    });
    connect(m_chart, &HistoryChartWidget::markerDragged, this, [this](int marker) {
        // HistoryTab.setOnlineMarker: a marker with nothing is ignored.
        if (m_timeline->isAllowedMarker(marker)) {
            requestFrameAtMarker(marker);
        }
    });
    connect(m_chart, &HistoryChartWidget::intervalSelected, this, &HistoryView::onChartInterval);
    updateViolationToggleText();
    applyTimeStep();

    // utils/LoadingStatusDialog.qml equivalent.
    // LoadingStatusDialog lives on the window's top layer; it is created on
    // first use, once this page is inside the main window.

    m_infoTimeout = new QTimer(this);
    m_infoTimeout->setSingleShot(true);
    m_infoTimeout->setInterval(30 * 1000);
    connect(m_infoTimeout, &QTimer::timeout, this, [this] {
        m_infoTimedOut = true;
        checkInfoReady();
    });

    m_loadingDelay = new QTimer(this);
    m_loadingDelay->setSingleShot(true);
    m_loadingDelay->setInterval(200);
    connect(m_loadingDelay, &QTimer::timeout, this, [this] {
        for (quint32 streamId : std::as_const(m_inFlightStreams)) {
            m_videoStrip->setLoading(streamId);
        }
    });

    // videoStore's cleanup (every 60 s): a period's cache file goes once no
    // History tab refers to it any more.
    m_cacheEvictTimer = new QTimer(this);
    m_cacheEvictTimer->setInterval(60 * 1000);
    connect(m_cacheEvictTimer, &QTimer::timeout, this, &HistoryView::evictUnusedCaches);
    connect(this, &QObject::destroyed, this, [page = static_cast<const HistoryView *>(this)] {
        referencedCacheFiles().remove(page);
    });
    m_cacheEvictTimer->start();

    m_playbackTimer = new QTimer(this);
    // History.qml: markersPerSecond = speed -- at 1x one Time step marker
    // per second.
    m_playbackTimer->setInterval(1000);
    connect(m_playbackTimer, &QTimer::timeout, this, &HistoryView::onPlaybackTick);

    // SliderBar.qml's rewind actions (Left/Right = one marker, Ctrl = ten)
    // and History.qml's stopPlay (Space).
    const auto addShortcut = [this](const QKeySequence &keys, Qt::ShortcutContext context, auto slot) {
        auto *shortcut = new QShortcut(keys, this);
        shortcut->setContext(context);
        connect(shortcut, &QShortcut::activated, this, slot);
    };
    // The rewind keys work anywhere in the window while History is shown --
    // no click on the video or Play first (the top panel and the tab strip
    // live outside this page, so focus is often not in it). A window
    // shortcut is only enabled while this page is visible, a modal dialog
    // takes the active window away, and a text field claims Left/Right
    // through ShortcutOverride before the shortcut sees them;
    // rewindAllowed() is the belt to those braces.
    const auto rewind = [this](int markers) {
        return [this, markers] {
            if (rewindAllowed()) {
                stepMarkers(markers);
            }
        };
    };
    addShortcut(QKeySequence(Qt::Key_Right), Qt::WindowShortcut, rewind(1));
    addShortcut(QKeySequence(Qt::CTRL | Qt::Key_Right), Qt::WindowShortcut, rewind(10));
    addShortcut(QKeySequence(Qt::Key_Left), Qt::WindowShortcut, rewind(-1));
    addShortcut(QKeySequence(Qt::CTRL | Qt::Key_Left), Qt::WindowShortcut, rewind(-10));
    addShortcut(QKeySequence(Qt::Key_Space), Qt::WidgetWithChildrenShortcut, [this] { onPlayClicked(); });

    connect(&m_connection, &ViewerConnection::historyFramesReceived, this,
            &HistoryView::onFramesReceived);
    connect(&m_connection, &ViewerConnection::historyFrameReceived, this,
            &HistoryView::onFrameReceived);
    connect(&m_connection, &ViewerConnection::historyAppSegmentsReceived, this,
            &HistoryView::onAppSegmentsReceived);
    connect(&m_connection, &ViewerConnection::historyWebVisitsReceived, this,
            &HistoryView::onWebVisitsReceived);
    connect(&m_connection, &ViewerConnection::historyCategoriesReceived, this,
            &HistoryView::onCategoriesReceived);
    connect(&m_connection, &ViewerConnection::historyEmployeeCategoriesReceived, this,
            &HistoryView::onEmployeeCategoriesReceived);
    connect(&m_connection, &ViewerConnection::historyDepartmentCategoriesReceived, this,
            [this](quint32 streamId, const QList<HistoryDepartmentCategories> &departments) {
                if (streamId != currentStreamId()) {
                    return;
                }
                m_scopes.departments = departments;
                updateInfoPanel();
            });
    connect(&m_connection, &ViewerConnection::keyEventsReceived, this, &HistoryView::onKeyEvents);
    connect(&m_connection, &ViewerConnection::keyEventsFailed, this, &HistoryView::onKeyEventsFailed);
    connect(&m_connection, &ViewerConnection::historyError, this, &HistoryView::onHistoryError);
    connect(&m_connection, &ViewerConnection::historySessionStatesReceived, this,
            [this](quint32 streamId, const QString &day, const QList<QPair<qint64, qint64>> &online,
                   const QList<QPair<qint64, qint64>> &lock, const QList<QPair<qint64, qint64>> &saver,
                   const QList<QPair<qint64, qint64>> &noData) {
                if (day != m_rangeKey || streamId != m_periodAnchor) {
                    return;
                }
                m_onlineRanges = online;
                m_lockRanges = lock;
                m_saverRanges = saver;
                m_noDataRanges = noData;
                m_statesLoaded = true;
                // effectiveRanges: wherever the session was online, its
                // noData stretches included.
                m_timeline->setSessionRanges(online + noData);
                refreshLoadedOverlay();
                const int marker = m_timeline->currentMarker();
                if (marker >= 0) {
                    updateHistoryStatusIcon(m_timeline->timestampForMarker(marker));
                }
                checkInfoReady();
            });
    connect(&m_connection, &ViewerConnection::statusChanged, this,
            [this](const QString &, bool connected) { onConnectionStatus(connected); });
    connect(&m_connection, &ViewerConnection::sessionStarted, this, &HistoryView::onSessionStarted);
    connect(&m_connection, &ViewerConnection::historyFrameMissing, this,
            &HistoryView::onHistoryFrameMissing);
    connect(&m_connection, &ViewerConnection::historySegmentsReceived, this,
            &HistoryView::onSegmentsReceived);
    connect(&m_connection, &ViewerConnection::historyScreensReceived, this,
            &HistoryView::onScreensReceived);
    connect(&m_connection, &ViewerConnection::historySegmentReceived, this,
            &HistoryView::onSegmentReceived);
    connect(&m_connection, &ViewerConnection::historySegmentMissing, this,
            &HistoryView::onSegmentMissing);
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
            addHistoryTab({deviceNames.cbegin().key(), HistoryPeriod::recent(QStringLiteral("d"), 1),
                           m_timeStepMs});
        }
        return;
    }
    // A tab restored from the saved configuration (or one whose employee
    // came back under another session) finds its employee by name.
    bool currentResolved = false;
    for (int i = 0; i < m_historyTabs.size(); ++i) {
        HistoryTab &tab = m_historyTabs[i];
        if (tab.deviceKey != 0 && deviceNames.contains(tab.deviceKey)) {
            tab.employeeName = deviceNames.value(tab.deviceKey);
        } else {
            for (auto it = deviceNames.cbegin(); it != deviceNames.cend(); ++it) {
                if (!tab.employeeName.isEmpty() && it.value() == tab.employeeName) {
                    tab.deviceKey = it.key();
                    currentResolved = currentResolved || i == m_currentTab;
                    break;
                }
            }
        }
        updateHistoryTabText(i);
    }
    if (currentResolved && m_activated && m_currentDeviceKey != m_historyTabs.at(m_currentTab).deviceKey) {
        showHistoryTab(m_currentTab);
        return;
    }
    if (m_reloadPending && !deviceMonitorStreams.value(m_currentDeviceKey).isEmpty()) {
        resumeAfterReconnect();
        return;
    }
    if (m_currentDeviceKey != 0 && deviceNames.contains(m_currentDeviceKey)) {
        // The list is re-sent on every live update, so only act when this
        // employee's screens actually changed -- and then fetch the current
        // moment for them, instead of leaving new ones loading.
        if (deviceMonitorStreams.value(m_currentDeviceKey) != previousStreams
            && m_historyStreams.isEmpty()) {
            // Plugging a screen in or out does not change what was recorded:
            // once listScreens has answered, the period's screens come from
            // the data. Only before that do the connected ones stand in.
            rebuildMonitorStrip();
            requestFrameAtMarker(m_timeline->currentMarker());
        }
    }
}

void HistoryView::activate()
{
    if (m_activated) {
        return;
    }
    if (m_currentTab < 0) {
        // Opened before any employee is known (History restored at start):
        // load as soon as the first tab exists.
        m_activateWhenReady = true;
        return;
    }
    m_activateWhenReady = false;
    m_activated = true;
    showHistoryTab(m_currentTab);
}

void HistoryView::showLoadingDialog(const QString &message)
{
    if (!m_loadingDialog) {
        m_loadingDialog = new LoadingOverlay(window());
    }
    static_cast<LoadingOverlay *>(m_loadingDialog.data())->showMessage(message);
    // History.qml:51,359: sliderAndMeta.enabled = !lockButtons
    // (waitingForLoad) -- every button, the slider and the violation toggle.
    m_controlBlock->setEnabled(false);
}

void HistoryView::hideLoadingDialog()
{
    if (m_loadingDialog) {
        static_cast<LoadingOverlay *>(m_loadingDialog.data())->dismiss();
    }
    m_controlBlock->setEnabled(true);
}

void HistoryView::openForDevice(quint32 deviceKey, const QString &day)
{
    const QDate date =
        day.isEmpty() ? QDate::currentDate() : QDate::fromString(day, QStringLiteral("yyyyMMdd"));
    const HistoryPeriod period = HistoryPeriod::recent(
        QStringLiteral("d"), HistoryPeriod::getMultiplier(QStringLiteral("d"), QDateTime(date, QTime(0, 0))));
    m_activated = true;
    // Like GoToHistoryDialog: the current tab if it's already this
    // employee, otherwise a new tab for them.
    if (m_currentTab >= 0 && m_historyTabs.at(m_currentTab).deviceKey == deviceKey) {
        m_historyTabs[m_currentTab].period = period;
        m_resumeMomentMs = -1;
        m_resumePlaying = false;
        saveTabs();
        jumpTo(deviceKey, period);
        return;
    }
    // historyPanel.addTab(0, ...): a jump opens its tab first in the row.
    addHistoryTab({deviceKey, period, m_timeStepMs}, 0);
}

void HistoryView::openForInterval(quint32 deviceKey, const QDateTime &begin, const QDateTime &end,
                                  qint64 timeStepMs)
{
    HistoryPeriod period;
    period.type = QStringLiteral("custom");
    period.customBegin = begin;
    period.customEnd = end;
    period.recentBegin = begin;
    period.recentEnd = end;
    m_activated = true;
    addHistoryTab({deviceKey, period, timeStepMs > 0 ? timeStepMs : m_timeStepMs}, 0);
}

void HistoryView::setDeviceIdentity(quint32 deviceKey, const SessionIdentity &identity)
{
    const bool viewWasOff = m_identities.contains(deviceKey) && !m_identities.value(deviceKey).keyloggerView;
    m_identities.insert(deviceKey, identity);
    if (deviceKey == m_currentDeviceKey) {
        m_keystream->setViewDisabled(!identity.keyloggerView);
        // Keys shown again: the period's keyboardEvents are asked for anew
        // (the earlier answer was the refusal).
        if (viewWasOff && identity.keyloggerView && !m_rangeKey.isEmpty() && m_periodAnchor != 0) {
            m_keyEventsSettled = false;
            m_keyEventsTag = QStringLiteral("history-keylogger-%1").arg(++m_keyEventsGeneration);
            m_connection.requestKeyEvents(m_periodAnchor, periodStartMs(), periodStopMs(), m_keyEventsTag);
        }
        m_timeline->setSessionLabel(m_identities.value(deviceKey).displayText(),
                                    !m_deviceMonitorStreams.value(deviceKey).isEmpty());
    }
}

void HistoryView::jumpTo(quint32 deviceKey, const HistoryPeriod &period)
{
    m_period = period;
    if (deviceKey != m_currentDeviceKey) {
        switchDevice(deviceKey);
    }
    loadPeriod();
}

void HistoryView::showPeriodMoment()
{
    // history/video/Header.qml:40-45: with no current player block the
    // header reads "( <start> - <stop> )" of the period.
    if (!m_period.begin().isValid() || !m_period.end().isValid()) {
        m_videoHeader->setMoment(QString());
        return;
    }
    m_videoHeader->setMoment(QStringLiteral("( %1 - %2 )")
                                 .arg(headerMomentText(m_period.begin()), headerMomentText(m_period.end())));
}

void HistoryView::showNoData()
{
    m_infoPending = false;
    m_infoTimeout->stop();
    m_playbackTimer->stop();
    setPlaying(false);
    ++m_frameGeneration;
    m_frameRequestInFlight = false;
    m_desiredMarker = -1;
    m_inFlightMarker = -1;
    m_inFlightRequestId = 0;
    m_inFlightStreams.clear();
    m_timestamps.clear();
    m_timeline->setTimestamps({});
    clearFrameCache();
    m_segmentStore.close();
    m_segmentStore.clear();
    m_cacheTag.clear();
    m_videoSegments.clear();
    m_emptyRanges.clear();
    m_segmentRequestInFlight = false;
    m_cacheRangeKey.clear();
    m_videoStrip->setStreams({});
    showPeriodMoment();
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
    const HistoryTab &tab = m_historyTabs.at(index);
    const QString name = m_deviceNames.value(tab.deviceKey,
                                             tab.employeeName.isEmpty() ? QStringLiteral("Employee %1").arg(tab.deviceKey)
                                                                        : tab.employeeName);
    m_historyTabBar->setTabText(index, name);
    m_historyTabBar->setTabToolTip(index, name);
}

void HistoryView::storeCurrentTab()
{
    if (m_currentTab < 0 || m_currentTab >= m_historyTabs.size()) {
        return;
    }
    HistoryTab &tab = m_historyTabs[m_currentTab];
    if (m_currentDeviceKey != 0) {
        tab.deviceKey = m_currentDeviceKey;
        tab.employeeName = m_deviceNames.value(m_currentDeviceKey, tab.employeeName);
    }
    tab.period = m_period;
    tab.timeStepMs = m_timeStepMs;
    tab.speedIndex = m_speedCombo->currentIndex();
    // Its View.History lives on while another tab is shown: the moment, the
    // play state and the panels come back with it.
    const int marker = m_timeline->currentMarker();
    tab.positionMs = marker >= 0 && m_timeline->hasData() ? m_timeline->timestampForMarker(marker) : -1;
    tab.playing = m_playbackTimer->isActive();
    tab.chartOpen = m_chartAnimation->state() == QAbstractAnimation::Running ? m_chartOpening : m_chartOpen;
    tab.appsOpen = m_appsOpen;
    tab.panelFull = m_panelFull;
}

void HistoryView::saveTabs() const
{
    // HistoryContainer (one list, every window) + each tab's Config:
    // employee, period, Time step -- written on every change.
    QJsonArray tabs;
    for (const HistoryTab &tab : m_historyTabs) {
        const auto ms = [](const QDateTime &moment) {
            return moment.isValid() ? static_cast<double>(moment.toMSecsSinceEpoch()) : 0.0;
        };
        tabs.append(QJsonObject{{QStringLiteral("id"), tab.id},
                                {QStringLiteral("employee"), m_deviceNames.value(tab.deviceKey, tab.employeeName)},
                                {QStringLiteral("type"), tab.period.type},
                                {QStringLiteral("recentType"), tab.period.recentType},
                                {QStringLiteral("recentMultiplier"), tab.period.recentMultiplier},
                                {QStringLiteral("recentBegin"), ms(tab.period.recentBegin)},
                                {QStringLiteral("recentEnd"), ms(tab.period.recentEnd)},
                                {QStringLiteral("customBegin"), ms(tab.period.customBegin)},
                                {QStringLiteral("customEnd"), ms(tab.period.customEnd)},
                                {QStringLiteral("timeStep"), static_cast<double>(tab.timeStepMs)}});
    }
    QSettings settings;
    settings.setValue(QStringLiteral("history/tabs"),
                      QString::fromUtf8(QJsonDocument(tabs).toJson(QJsonDocument::Compact)));
    if (!m_currentTabKey.isEmpty() && m_currentTab >= 0 && m_currentTab < m_historyTabs.size()) {
        settings.setValue(m_currentTabKey, m_historyTabs.at(m_currentTab).id);
    }
}

void HistoryView::restoreTabs(const QString &currentTabKey)
{
    m_currentTabKey = currentTabKey;
    QSettings settings;
    const QJsonArray tabs =
        QJsonDocument::fromJson(settings.value(QStringLiteral("history/tabs")).toString().toUtf8()).array();
    const QString currentId = settings.value(currentTabKey).toString();
    const auto moment = [](const QJsonValue &value) {
        const qint64 ms = static_cast<qint64>(value.toDouble());
        return ms > 0 ? QDateTime::fromMSecsSinceEpoch(ms) : QDateTime();
    };
    int current = 0;
    const QSignalBlocker blocker(m_historyTabBar);
    for (const QJsonValue &value : tabs) {
        const QJsonObject object = value.toObject();
        HistoryTab tab;
        tab.id = object.value(QStringLiteral("id")).toString();
        tab.employeeName = object.value(QStringLiteral("employee")).toString();
        // getInterval() returns the stored dates as they are: a restored
        // "Today" stays on the day it was chosen.
        tab.period.type = object.value(QStringLiteral("type")).toString(QStringLiteral("recent"));
        tab.period.recentType = object.value(QStringLiteral("recentType")).toString(QStringLiteral("d"));
        tab.period.recentMultiplier = object.value(QStringLiteral("recentMultiplier")).toInt(1);
        tab.period.recentBegin = moment(object.value(QStringLiteral("recentBegin")));
        tab.period.recentEnd = moment(object.value(QStringLiteral("recentEnd")));
        tab.period.customBegin = moment(object.value(QStringLiteral("customBegin")));
        tab.period.customEnd = moment(object.value(QStringLiteral("customEnd")));
        tab.timeStepMs = static_cast<qint64>(object.value(QStringLiteral("timeStep")).toDouble(5 * 60 * 1000));
        if (tab.id.isEmpty() || !tab.period.begin().isValid() || !tab.period.end().isValid()) {
            continue;
        }
        if (tab.id == currentId) {
            current = int(m_historyTabs.size());
        }
        m_historyTabs.append(tab);
        m_historyTabBar->addTab(QString());
        updateHistoryTabText(int(m_historyTabs.size()) - 1);
    }
    if (!m_historyTabs.isEmpty()) {
        m_currentTab = current;
        m_historyTabBar->setCurrentIndex(current);
    }
}

void HistoryView::addHistoryTab(const HistoryTab &tab, int atIndex)
{
    storeCurrentTab();
    HistoryTab added = tab;
    if (added.id.isEmpty()) {
        // activity/History.qml addTab: "h" + random digits.
        added.id = QStringLiteral("h%1").arg(QRandomGenerator::global()->bounded(100000000, 999999999));
    }
    if (added.employeeName.isEmpty()) {
        added.employeeName = m_deviceNames.value(added.deviceKey);
    }
    const int index = atIndex < 0 || atIndex > m_historyTabs.size() ? int(m_historyTabs.size()) : atIndex;
    m_historyTabs.insert(index, added);
    {
        const QSignalBlocker blocker(m_historyTabBar);
        m_historyTabBar->insertTab(index, QString());
        updateHistoryTabText(index);
        m_historyTabBar->setCurrentIndex(index);
    }
    // The new tab is made current at once; it starts with its own player.
    m_currentTab = index;
    m_resumeMomentMs = -1;
    m_resumePlaying = false;
    saveTabs();
    if (m_activated) {
        // A new tab is a new View.History: its panels start closed.
        applyPanelState(false, false, false);
        showHistoryTab(m_currentTab);
    } else if (m_activateWhenReady) {
        activate();
    }
}

void HistoryView::showHistoryTab(int index)
{
    if (index < 0 || index >= m_historyTabs.size()) {
        return;
    }
    const bool switching = index != m_currentTab;
    if (switching) {
        storeCurrentTab();
        m_currentTab = index;
        // The tab put back at its own moment, playing again if it was.
        m_resumeMomentMs = m_historyTabs.at(index).positionMs;
        m_resumePlaying = m_historyTabs.at(index).playing;
        saveTabs();
    }
    if (!m_activated) {
        return;
    }
    const HistoryTab tab = m_historyTabs.at(index);
    m_timeStepMs = tab.timeStepMs;
    applyTimeStep();
    m_speedCombo->setCurrentIndex(tab.speedIndex);
    if (switching) {
        applyPanelState(tab.chartOpen, tab.appsOpen, tab.panelFull);
    }
    if (tab.deviceKey == 0 || !m_deviceNames.contains(tab.deviceKey)) {
        // A restored tab whose employee is not known (yet): setDevices loads
        // it as soon as the employee shows up.
        m_videoHeader->setName(tab.employeeName);
        return;
    }
    jumpTo(tab.deviceKey, tab.period);
}

void HistoryView::openAddTabDialog()
{
    HistoryChoiceDialog dialog(HistoryChoiceDialog::Mode::Add, employeeList(), 0,
                               HistoryPeriod::recent(QStringLiteral("d"), 1), m_timeStepMs, this);
    dialog.move(mapToGlobal(rect().center()) - QPoint(dialog.width() / 2, dialog.height() / 2));
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    m_activated = true;
    // HistoryPanel: the "+" dialog inserts at the current tab's place.
    addHistoryTab({dialog.employee(), dialog.period(), dialog.timeStepMs()}, qMax(0, m_currentTab));
}

void HistoryView::closeHistoryTab(int index)
{
    if (index < 0 || index >= m_historyTabs.size()) {
        return;
    }
    // activity/History.qml onTabDel: a tab whose video is being saved stays.
    if (isVideoSaving()
        && cacheTag(m_historyTabs.at(index).deviceKey, m_historyTabs.at(index).period) == m_videoSaverTag) {
        QMessageBox::critical(this, QStringLiteral("Error"),
                              QStringLiteral("Cannot delete this tab, because video saving in progress"));
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
    saveTabs();
    if (m_historyTabs.isEmpty()) {
        m_currentTab = -1;
        m_currentDeviceKey = 0;
        m_period = HistoryPeriod();
        m_rangeKey.clear();
        m_chartsModel->clear();
        showNoData();
        // HistoryPanel.openPanelName(): with no tabs left, ask for one.
        openAddTabDialog();
        return;
    }
    // The closed tab was not necessarily the current one: only a closed
    // current tab hands over to its neighbour.
    if (index != m_currentTab) {
        if (index < m_currentTab) {
            --m_currentTab;
        }
        const QSignalBlocker blocker(m_historyTabBar);
        m_historyTabBar->setCurrentIndex(m_currentTab);
        saveTabs();
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
    // The anchor for every session-wide query (programs, sites, keystrokes,
    // charts, categories -- the host keys all of them by the employee, so any
    // of their screens answers the same). It used to be "the first monitor in
    // the list", but that list arrives in the host's hash order: once a second
    // screen was plugged in, "first" could become the new screen, and History
    // built its whole timeline from that screen alone -- which is why the
    // single-screen stretch before it vanished. The lowest id is stable.
    const QList<quint32> streams = m_deviceMonitorStreams.value(m_currentDeviceKey);
    return streams.isEmpty() ? 0 : *std::min_element(streams.cbegin(), streams.cend());
}

QList<quint32> HistoryView::historyStreams() const
{
    if (!m_historyStreams.isEmpty()) {
        return m_historyStreams;
    }
    QList<quint32> streams = m_deviceMonitorStreams.value(m_currentDeviceKey);
    std::sort(streams.begin(), streams.end());
    return streams;
}

void HistoryView::rebuildMonitorStrip()
{
    m_videoStrip->setStreams(historyStreams());
    positionOverlays();
}

void HistoryView::positionOverlays()
{
    // Video.qml: the frames take the whole height (H - 24 - 5); the scroll
    // bar lives in the gap below (m_stripScrollBar), so it appearing can't
    // shrink them.
    m_videoStrip->setViewSize(QSize(m_monitorStripArea->viewport()->width(), m_monitorStripArea->height()));
    m_statusLabel->setGeometry(m_monitorStripArea->rect());
    m_historyStatusIcon->setGeometry(m_videoPanel->rect());
    // Right edge 1px inside the video's right border, top on the header's
    // last line (videoCell 7px inset + 24px header - 1).
    const QSize hint = m_toggleAppsButton->sizeHint();
    const QRect video(m_videoPanel->mapTo(this, QPoint(0, 0)), m_videoPanel->size());
    // panel: top at the video header's bottom, height = History - 48 -
    // sliderAndMeta - chartsItem - keylogger; the scroll is videoCell - 40.
    const int panelHeight = qMax(0, height() - 48 - (m_controlBlock->isVisible() ? m_controlBlock->height() : 0)
                                        - (m_chart->isVisible() ? m_chart->height() : 0) - m_keystream->height());
    const int panelX = m_appsAnimation->state() == QAbstractAnimation::Running ? m_appsPanel->x()
                     : m_appsOpen ? width() - m_appsPanel->width() - 10
                                  : video.right() + 1;
    m_appsPanel->setGeometry(panelX, video.top() + 24, m_appsPanel->width(), panelHeight);
    // The strip's bar in the 8px+ gap under the video (hscroll margins 2/4).
    m_stripScrollBar->setGeometry(video.left() + 2, video.bottom() + 3, video.width() - 2, 5);
    m_stripScrollBar->raise();
    m_infoArea->setFixedHeight(qMax(0, video.height() - 40));
    m_appsPanel->raise();
    m_toggleAppsButton->setGeometry(video.right() - hint.width(), video.top() + 24 - 1, hint.width(), 25);
    m_toggleAppsButton->raise();
}

void HistoryView::updateStatusVisibility()
{
    // exportTextButton.enabled: !!historyModel.currentActiveSession.
    m_keylogTableButton->setEnabled(m_timeline->hasData());
    m_statusLabel->setVisible(!m_statusLabel->text().isEmpty());
    m_statusLabel->raise();
    updateAppsButton();
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
    if (event->type() == QEvent::MouseButtonPress && m_speedPopup && m_speedPopup->isVisible()) {
        auto *widget = qobject_cast<QWidget *>(watched);
        if (widget && !m_speedPopup->isAncestorOf(widget) && widget != m_speedPopup && widget != m_speedButton) {
            m_speedPopup->hide();
        }
    }
    if (watched == m_tabStrip && event->type() == QEvent::Resize) {
        // Tabs.qml lv.maxWidth: the strip minus the plus (13 + 5), the
        // arrows (18) and the Row's spacing/margin.
        m_historyTabBar->setAvailableWidth(qMax(100, m_tabStrip->width() - 18 - 18 - 3 * 3));
    }
    if (watched == m_playButton && m_playIcons.size() == 2
        && (event->type() == QEvent::Enter || event->type() == QEvent::Leave)) {
        m_playButton->setIcon(m_playIcons.at(event->type() == QEvent::Enter ? 1 : 0));
    }
    if (m_monitorStripArea && watched == m_monitorStripArea->viewport() && event->type() == QEvent::Resize) {
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
    // SliderBar's knob tooltip: Session.getDisplayText() (domain/user@computer).
    m_timeline->setSessionLabel(m_identities.contains(deviceKey) ? m_identities.value(deviceKey).displayText()
                                                                 : m_deviceNames.value(deviceKey),
                                !m_deviceMonitorStreams.value(deviceKey).isEmpty());
    rebuildMonitorStrip();
    m_scopes = CategoryScopes();
    m_chartsModel->clear();

    const quint32 streamId = currentStreamId();
    if (streamId != 0) {
        m_connection.requestCategories(streamId);
    }
}

void HistoryView::loadPeriod()
{
    m_playbackTimer->stop();
    setPlaying(false);
    // A new interval: every answer still on its way belongs to the old one
    // and is dropped on arrival (see isCurrentGeneration).
    ++m_frameGeneration;
    m_frameRequestInFlight = false;
    m_desiredMarker = -1;
    m_inFlightMarker = -1;
    m_inFlightRequestId = 0;
    m_inFlightStreams.clear();
    m_downloadComplete = false;
    m_infoPending = true;
    m_screensAnswered = false;
    m_infoTimedOut = false;
    m_infoTimeout->start();
    m_noDataRanges.clear();
    m_timeline->setSessionRanges({});
    m_segmentRequestInFlight = false;
    m_videoSegments.clear();
    m_emptyRanges.clear();
    m_historyStreams.clear();
    m_framesByStream.clear();
    m_timestamps.clear();
    m_timeline->setTimestamps({});
    // Only a real change of interval or employee invalidates what has been
    // downloaded; re-opening the same period (a new Time step, say) keeps it,
    // so the blue overlay and instant seeking survive.
    const QString nextRangeKey = ViewerConnection::rangeKey(periodStartMs(), periodStopMs());
    if (nextRangeKey != m_cacheRangeKey || m_currentDeviceKey != m_cacheDeviceKey) {
        clearFrameCache();
        // Each period has its own cache file; what it holds is known (and
        // blue) at once, from an earlier visit or an earlier run.
        const QString tag = cacheTag(m_currentDeviceKey, m_period);
        if (tag != m_cacheTag) {
            m_cacheTag = tag;
            QDir().mkpath(HistorySegmentStore::cacheDirectory());
            if (!m_segmentStore.open(HistorySegmentStore::cacheFileForTag(tag))) {
                m_segmentStore.clear();
            }
        }
    }
    m_cacheRangeKey = nextRangeKey;
    m_cacheDeviceKey = m_currentDeviceKey;
    m_videoStrip->setAllLoading();
    m_keystream->setText(QString(), QString());
    m_webVisits.clear();
    m_appSegments.clear();
    m_keyEvents.clear();
    m_keyLegacy.clear();
    m_keyEventsSettled = false;
    m_infoPanel->setItems({}, {});

    const qint64 start = periodStartMs();
    const qint64 stop = periodStopMs();
    // No moment until the first marker lands (applyMarker): the period.
    showPeriodMoment();
    m_rangeKey = ViewerConnection::rangeKey(start, stop);
    m_timeline->setRange(start, stop);
    m_timeAxis->setRange(start, stop);
    m_chart->setRange(start, stop);
    const qint64 chartStep = HistoryChartWidget::chartStepMs(stop - start, m_timeStepMs);
    m_chart->setChartStepMs(chartStep);

    const quint32 streamId = currentStreamId();
    if (streamId == 0 || stop <= start) {
        m_chartsModel->clear();
        showNoData();
        return;
    }
    m_statusLabel->clear();
    updateStatusVisibility();
    showLoadingDialog(QStringLiteral("Downloading..."));
    // Timeline first: the host answers queries serially, so ask for the
    // frame list (which populates the slider and is cheap) before the
    // heavier chart/app/keystroke queries -- the timeline loads first, as in
    // the original, instead of waiting behind them.
    // Which of the employee's screens recorded anything in the period comes
    // first; their frame lists and video runs are asked for as soon as it
    // answers (onScreensReceived). A screen unplugged since, or one plugged
    // in only later, still shows for its own stretch.
    m_periodAnchor = streamId;
    m_connection.requestHistoryScreens(streamId, start, stop);
    m_statesLoaded = false;
    m_onlineRanges.clear();
    m_lockRanges.clear();
    m_saverRanges.clear();
    m_historyStatusIcon->setKind(QString());
    m_historyStatusIcon->hide();
    m_connection.requestSessionStates(streamId, start, stop);
    m_connection.requestHistoryAppSegments(streamId, start, stop);
    m_connection.requestWebVisits(streamId, start, stop);
    // historyModel.keyboardEvents: this period's keyboard events.
    m_keyEventsTag = QStringLiteral("history-keylogger-%1").arg(++m_keyEventsGeneration);
    m_connection.requestKeyEvents(streamId, start, stop, m_keyEventsTag);
    m_chartsModel->setQuery(streamId, start, stop, chartStep);
}

void HistoryView::onTimelineMoved(int marker)
{
    requestFrameAtMarker(marker);
}

bool HistoryView::rewindAllowed() const
{
    if (!isVisible() || !m_timeline->isEnabled() || QApplication::activeModalWidget()
        || QApplication::activePopupWidget()) {
        return false;
    }
    QWidget *focus = QApplication::focusWidget();
    if (auto *line = qobject_cast<QLineEdit *>(focus)) {
        return line->isReadOnly();
    }
    if (auto *text = qobject_cast<QTextEdit *>(focus)) {
        return text->isReadOnly();
    }
    if (auto *plain = qobject_cast<QPlainTextEdit *>(focus)) {
        return plain->isReadOnly();
    }
    return !qobject_cast<QAbstractSpinBox *>(focus);
}

void HistoryView::stepMarkers(int markers)
{
    // One step = one marker of the Time step grid (SliderBar.qml's
    // rewindLeft/rewindRight). Like setOnlineMarkerForRewind, a target with
    // nothing recorded lands on the next recorded range in that direction.
    const int current = m_timeline->currentMarker();
    if (current < 0 || markers == 0) {
        return;
    }
    const int target = qBound(0, current + markers, m_timeline->markersAmount() - 1);
    const int next = m_timeline->rewindTarget(current, target, markers > 0);
    if (next >= 0 && next != current) {
        requestFrameAtMarker(next);
    }
}

quint64 HistoryView::allocateRequestId()
{
    return (quint64(m_frameGeneration) << 32) | ++m_frameSequence;
}

QImage HistoryView::cachedFrame(quint32 streamId, qint64 timestampMs) const
{
    return m_frameCache.value(qMakePair(streamId, timestampMs));
}

void HistoryView::cacheFrame(quint32 streamId, qint64 timestampMs, const QImage &image)
{
    // A bounded cache: the real progress of a day is kept here (and drawn as
    // the blue overlay), but a long period cannot grow it without limit.
    constexpr qint64 kCacheBudgetBytes = 96 * 1024 * 1024;
    const QPair<quint32, qint64> key(streamId, timestampMs);
    if (m_frameCache.contains(key) || image.isNull()) {
        return;
    }
    m_frameCache.insert(key, image);
    m_frameCacheOrder.append(key);
    m_frameCacheBytes += image.sizeInBytes();
    while (m_frameCacheBytes > kCacheBudgetBytes && m_frameCacheOrder.size() > 1) {
        const QPair<quint32, qint64> oldest = m_frameCacheOrder.takeFirst();
        m_frameCacheBytes -= m_frameCache.take(oldest).sizeInBytes();
        // Its marker is no longer instantly available, but it is still
        // recorded: the overlay keeps it only while the primary stream's
        // copy is cached.
        if (oldest.first == currentStreamId()) {
            m_loadedTimestamps.remove(oldest.second);
        }
    }
}

void HistoryView::clearFrameCache()
{
    m_frameCache.clear();
    m_frameCacheOrder.clear();
    m_frameCacheBytes = 0;
    m_loadedTimestamps.clear();
    m_timeline->clearLoaded();
}

void HistoryView::noteFrameShown(qint64 timestampMs)
{
    // SliderBar.qml's loadedRanges: this one marker, not the whole period.
    m_loadedTimestamps.insert(timestampMs);
    m_timeline->markLoaded(m_timeline->markerForTimestamp(timestampMs));
}

void HistoryView::refreshLoadedOverlay()
{
    // A new Time step renumbers every marker, so the overlay is re-derived
    // from what is actually in the cache instead of being thrown away.
    m_timeline->clearLoaded();
    for (qint64 timestampMs : std::as_const(m_loadedTimestamps)) {
        m_timeline->markLoaded(m_timeline->markerForTimestamp(timestampMs));
    }
    // And every screen's downloaded VP8 stretch -- these are most of what
    // is loaded.
    for (quint32 streamId : historyStreams()) {
        for (const QPair<qint64, qint64> &span : m_segmentStore.spans(streamId)) {
            m_timeline->markLoadedRange(span.first, span.second);
        }
    }
}

void HistoryView::requestFrameAtMarker(int marker)
{
    if (marker < 0 || marker >= m_timeline->markersAmount() || !m_timeline->hasData()) {
        return;
    }
    // The handle follows the mouse immediately; everything expensive is
    // coalesced into one applyMarker() per event-loop pass, for whichever
    // position is the latest by then.
    m_timeline->setCurrentMarker(marker);
    m_pendingApplyMarker = marker;
    if (!m_applyTimer->isActive()) {
        m_applyTimer->start();
    }
}

void HistoryView::applyMarker(int marker)
{
    if (marker < 0 || marker >= m_timeline->markersAmount() || !m_timeline->hasData()) {
        return;
    }
    const qint64 timestampMs = m_timeline->timestampForMarker(marker);
    if (timestampMs < 0) {
        return;
    }
    QElapsedTimer applyTimer;
    applyTimer.start();
    ++m_seekOrdinal;
    // The handle, the moment, the running-apps panel, the keystream and the
    // chart marker all move now -- the UI never waits on the host.
    const QDateTime moment = QDateTime::fromMSecsSinceEpoch(timestampMs);
    m_videoHeader->setMoment(QStringLiteral("( %1 )").arg(headerMomentText(moment)));
    updateKeystream(timestampMs);
    updateHistoryStatusIcon(timestampMs);
    m_timeline->setCurrentMarker(marker);
    updateInfoPanel();
    m_chart->setCurrentPositionMs(timestampMs);

    // Anything already held -- a downloaded VP8 run decoded locally, or a
    // JPEG fetched earlier -- is drawn straight away, with no request at all.
    // This is the whole point of the segment store: scrubbing over
    // downloaded ground is as smooth as a video player.
    // Nothing is asked for per seek: a moment not downloaded yet shows
    // Loading until the background download (re-ordered around the new
    // position right here) reaches it.
    const bool needsFetch = showFromStore(timestampMs);
    m_desiredMarker = marker;
    dispatchPrefetch();
    if (!needsFetch) {
        qCDebug(lcHistoryTiming, "marker %d: served locally in %lld ms", marker,
                applyTimer.elapsed());
        return;
    }

    m_seekTimer.start();
    if (!m_frameRequestInFlight) {
        dispatchFrameRequest();
    }
}

void HistoryView::dispatchFrameRequest()
{
    if (m_desiredMarker < 0 || m_desiredMarker >= m_timeline->markersAmount()) {
        m_frameRequestInFlight = false;
        return;
    }
    const qint64 timestampMs = m_timeline->timestampForMarker(m_desiredMarker);
    if (timestampMs < 0) {
        m_frameRequestInFlight = false;
        return;
    }
    m_inFlightMarker = m_desiredMarker;
    m_inFlightOrdinal = m_seekOrdinal;
    m_inFlightRequestId = allocateRequestId();
    m_inFlightStreams.clear();
    // Every screen of the device is shown at once (see rebuildMonitorStrip),
    // but only the ones not already cached are asked for.
    for (quint32 streamId : historyStreams()) {
        if (screenState(streamId, timestampMs) != ScreenState::Fetch) {
            continue;
        }
        m_inFlightStreams.insert(streamId);
        m_connection.requestHistoryFrame(streamId, timestampMs, m_inFlightRequestId);
    }
    if (m_inFlightStreams.isEmpty()) {
        m_frameRequestInFlight = false;
        return;
    }
    m_frameRequestInFlight = true;
    m_frameWatchdog->start();
    m_loadingDelay->start();
}

void HistoryView::releaseFrameSlot()
{
    m_frameWatchdog->stop();
    m_inFlightStreams.clear();
    m_frameRequestInFlight = false;
    if (m_desiredMarker != m_inFlightMarker) {
        dispatchFrameRequest();
    }
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
    setPlaying(true);
    m_playbackTimer->start();
    if (m_timeline->currentMarker() >= m_timeline->lastAllowedMarker()) {
        requestFrameAtMarker(m_timeline->nearestAllowedMarker(0, true));
    } else {
        // The player's first step is taken at once, the next ones on the
        // timer.
        onPlaybackTick();
    }
}

void HistoryView::onPlaybackTick()
{
    // One marker per tick, skipping over the stretches with no recording
    // instead of stalling on them.
    const int current = m_timeline->currentMarker();
    const int last = m_timeline->lastAllowedMarker();
    if (current < 0 || current >= last) {
        m_playbackTimer->stop();
        setPlaying(false);
        return;
    }
    const int next = m_timeline->nearestAllowedMarker(current + 1, true);
    if (next <= current) {
        m_playbackTimer->stop();
        setPlaying(false);
        return;
    }
    // The player waits while the next moment is still Loading on any screen
    // -- playback never runs ahead of the download.
    if (!markerReady(next)) {
        dispatchPrefetch();
        return;
    }
    requestFrameAtMarker(next);
}

void HistoryView::onToggleViolationPanel()
{
    if (m_chartAnimation->state() == QAbstractAnimation::Running) {
        return;
    }
    m_chartOpening = !m_chartOpen;
    if (m_chartOpening) {
        // The content is created only once the panel is open.
        m_chart->setContentVisible(false);
        m_chart->setFixedHeight(0);
        m_chart->setVisible(!m_panelFull);
        m_chartAnimation->setStartValue(0);
        m_chartAnimation->setEndValue(kChartsItemHeight);
    } else {
        m_chartAnimation->setStartValue(m_chart->height());
        m_chartAnimation->setEndValue(0);
    }
    m_chartAnimation->start();
}

bool runGoToHistoryDialog(QWidget *parent, const QString &title, qint64 beginMs, qint64 endMs, qint64 currentStepMs,
                          qint64 *stepMs)
{
    // trackerQuadratorActiveCell/filters/GoToHistoryDialog.qml: the
    // selected range (read-only) and a Time step for it.
    const QDateTime begin = QDateTime::fromMSecsSinceEpoch(beginMs);
    const QDateTime end = QDateTime::fromMSecsSinceEpoch(endMs);
    QDialog dialog(parent);
    dialog.setWindowTitle(title);
    dialog.setModal(true);
    auto *layout = new QVBoxLayout(&dialog);
    layout->addWidget(new QLabel(QStringLiteral("Time range: "), &dialog));
    auto *range = new QWidget(&dialog);
    auto *rangeRow = new QHBoxLayout(range);
    rangeRow->setContentsMargins(0, 0, 0, 0);
    auto *from = new QDateTimeEdit(begin, range);
    auto *to = new QDateTimeEdit(end, range);
    from->setDisplayFormat(QStringLiteral("dd.MM.yyyy HH:mm"));
    to->setDisplayFormat(QStringLiteral("dd.MM.yyyy HH:mm"));
    rangeRow->addWidget(new QLabel(QStringLiteral("from: "), range));
    rangeRow->addWidget(from);
    rangeRow->addWidget(new QLabel(QStringLiteral("to: "), range));
    rangeRow->addWidget(to);
    range->setEnabled(false); // enabled: false; opacity: 0.5
    auto *dim = new QGraphicsOpacityEffect(range);
    dim->setOpacity(0.5);
    range->setGraphicsEffect(dim);
    layout->addWidget(range);
    auto *stepRow = new QHBoxLayout;
    stepRow->addWidget(new QLabel(QStringLiteral("Time step: "), &dialog));
    auto *steps = new QComboBox(&dialog);
    steps->setFixedWidth(130);
    // TimeStepComboBox: the steps that fit the range (10 .. maxSteps of
    // them), the one closest to the current step selected.
    const qint64 target = endMs - beginMs;
    const double maxSteps = target <= 86400000LL ? 86400 : (target <= 8 * 86400000LL ? 2304 : 300);
    int bestIndex = -1;
    qint64 bestDistance = std::numeric_limits<qint64>::max();
    for (qint64 value : kTimeSteps) {
        const double count = static_cast<double>(target) / value;
        if (count < 10 || count > maxSteps) {
            continue;
        }
        steps->addItem(timeStepText(value), value);
        if (qAbs(value - currentStepMs) < bestDistance) {
            bestDistance = qAbs(value - currentStepMs);
            bestIndex = steps->count() - 1;
        }
    }
    if (bestIndex >= 0) {
        steps->setCurrentIndex(bestIndex);
    }
    stepRow->addWidget(steps);
    stepRow->addStretch(1);
    layout->addLayout(stepRow);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    QObject::connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout->addWidget(buttons);
    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }
    *stepMs = steps->currentIndex() >= 0 ? steps->currentData().toLongLong() : currentStepMs;
    return true;
}

void HistoryView::onChartInterval(qint64 beginMs, qint64 endMs)
{
    // History.qml onWantGoToHistory -> GoToHistoryDialog titled "Change
    // period"; OK opens a new History tab for that custom period, first in
    // the row.
    qint64 step = m_timeStepMs;
    if (!runGoToHistoryDialog(this, QStringLiteral("Change period"), beginMs, endMs, m_timeStepMs, &step)
        || m_currentDeviceKey == 0) {
        return;
    }
    HistoryPeriod period = m_period;
    period.type = QStringLiteral("custom");
    period.customBegin = QDateTime::fromMSecsSinceEpoch(beginMs);
    period.customEnd = QDateTime::fromMSecsSinceEpoch(endMs);
    period.recentBegin = period.customBegin;
    period.recentEnd = period.customEnd;
    addHistoryTab({m_currentDeviceKey, period, step}, 0);
}

void HistoryView::setChartOpen(bool open)
{
    m_chartAnimation->stop();
    m_chartOpen = open;
    m_chart->setContentVisible(true);
    m_chart->setFixedHeight(kChartsItemHeight);
    m_chart->setVisible(open && !m_panelFull);
    updateViolationToggleText();
}

void HistoryView::setAppsOpen(bool open)
{
    m_appsAnimation->stop();
    m_appsOpen = open;
    m_toggleAppsButton->setIcon(QIcon(open
        ? QStringLiteral(":/history/video/buttonRunningApp/applications_arrow_right.png")
        : QStringLiteral(":/history/video/buttonRunningApp/applications_arrow_left.png")));
    m_videoBody->setContentsMargins(0, 0, open ? m_appsPanel->width() : 0, 0);
    positionOverlays();
    m_appsPanel->setVisible(open);
    if (open) {
        m_appsPanel->raise();
        m_toggleAppsButton->raise();
    }
}

void HistoryView::applyPanelState(bool chartOpen, bool appsOpen, bool panelFull)
{
    setChartOpen(chartOpen);
    setAppsOpen(appsOpen);
    if (panelFull != m_panelFull) {
        togglePanelFull();
    }
}

void HistoryView::togglePanelFull()
{
    // chartsItem/sliderAndMeta heights are `panelFull ? 0 : ...`; the
    // keylogger bar drops to the page bottom.
    m_panelFull = !m_panelFull;
    m_controlBlock->setVisible(!m_panelFull);
    m_chart->setVisible(m_chartOpen && !m_panelFull);
    // Over the whole window: no History tab strip above it either.
    m_tabStrip->setVisible(!m_panelFull);
    emit panelFullChanged(m_panelFull);
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
    m_playIcons = {QIcon(QPixmap(playing ? QStringLiteral(":/history/pause_normal.png")
                                         : QStringLiteral(":/history/play_normal.png"))),
                   QIcon(QPixmap(playing ? QStringLiteral(":/history/pause_active.png")
                                         : QStringLiteral(":/history/play_active.png")))};
    m_playButton->setIcon(m_playIcons.at(m_playButton->underMouse() ? 1 : 0));
    // play_normal is nudged 4px right of center, pause_normal 2px
    // (History.qml's horizontalCenterOffset).
    m_playButton->setStyleSheet(
        QStringLiteral("QPushButton { background: transparent; border-image: url(:/history/bg_play.png);"
                       " border: none; padding: 0 0 0 %1px; }")
            .arg(playing ? 4 : 8));
}

void HistoryView::applyTimeStep()
{
    // Audio.qml (column21) only exists at Time step = 1s, and shifts both
    // the slider/TimeLine (12 leftMargin + column1 + 5 + column2 60 + 5
    // [+ column21 39 + 5] + 1 + MultiSessionsSlider spacing 10) and the chart
    // grid (Filters leftMargin 10 + widthActionButtons = column1 + column2
    // + 20 + 12 [+ column21], History.qml:363). column1 is as wide as
    // changeButton (2 * exportAviButton.width + 10).
    const bool audioVisible = m_timeStepMs <= 1000;
    m_muteButton->setVisible(audioVisible);
    const int audioWidth = audioVisible ? 39 : 0;
    const int column1Width = m_changeSettingsButton->width();
    const int column2Width = 60;
    const int leftWidth = 12 + column1Width + 5 + column2Width + 5 + (audioVisible ? audioWidth + 5 : 0) + 1 + 10;
    m_leftColumn->setFixedWidth(leftWidth);
    m_chart->setGridLeft(10 + column1Width + column2Width + 20 + 12 + audioWidth);
    // History.qml:658-660 widthSlider = sliderAndMeta.width - column1 -
    // column2 [- column21] - 50 (single session): the TimeLine stops
    // 50 - (leftWidth - column1 - column2 - column21) px short of the page's
    // right edge -- 17, or 12 at Time step = 1s; the bar reaches it
    // (TimelineWidget::barWidth, measured).
    const int rightGap = 50 - (leftWidth - column1Width - column2Width - audioWidth);
    m_sliderColumn->layout()->setContentsMargins(0, 0, qMax(0, rightGap), 0);
    m_timeline->setStepMs(m_timeStepMs);
    // The marker grid was renumbered: put the blue overlay (and the handle)
    // back where the cache says they belong.
    refreshLoadedOverlay();
    m_timeAxis->setStepMs(m_timeStepMs);
    m_chart->setMarkerStepMs(m_timeStepMs);
}

void HistoryView::onSpeedChanged(int index)
{
    // SpeedButton.qml's 0.5x/0.75x/1x/2x/4x/8x/16x are markersPerSecond:
    // one marker every 1000 / speed ms.
    static const double multipliers[] = {0.5, 0.75, 1.0, 2.0, 4.0, 8.0, 16.0};
    const double multiplier = multipliers[qBound(0, index, 6)];
    m_playbackTimer->setInterval(qMax(20, static_cast<int>(1000.0 / multiplier)));
}

namespace {
// ProgressBar { kind: "big" } in VideoSaverProgress: 250 wide, 0..100.
class SaverProgressBar final : public QWidget
{
public:
    explicit SaverProgressBar(QWidget *parent)
        : QWidget(parent)
    {
        setFixedSize(250, kProgressHeight);
    }
    void setValue(int value)
    {
        if (value != m_value) {
            m_value = value;
            update();
        }
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF bar(0, 0, width(), height());
        QPainterPath pill;
        pill.addRoundedRect(bar, height() / 2.0, height() / 2.0);
        painter.setClipPath(pill);
        painter.fillRect(bar, QColor(0x43, 0x44, 0x4b));
        painter.fillRect(QRectF(0, 0, width() * qBound(0, m_value, 100) / 100.0, height()), QColor(0x6f, 0x71, 0x7f));
        painter.setClipping(false);
        painter.setPen(Qt::white);
        QFont font(QStringLiteral("Roboto"));
        font.setPixelSize(13);
        painter.setFont(font);
        painter.drawText(bar, Qt::AlignCenter, QStringLiteral("%1%").arg(m_value));
    }

private:
    int m_value = 0;
};

// Keystrokes without the bracketed special keys (Result.cutSystemKeys:
// "without system buttons").
QString cutSystemKeys(const QString &text)
{
    static const QRegularExpression kSystemKey(QStringLiteral("<[A-Z][A-Z0-9_]*>"));
    QString out = text;
    out.remove(kSystemKey);
    return out;
}
} // namespace

QString HistoryView::sessionMarker() const
{
    // KeyloggerExport.qml / VideoSaverSelector: domain\user@computer.
    const SessionIdentity identity = m_identities.value(m_currentDeviceKey);
    if (identity.isValid()) {
        return identity.exportMarker();
    }
    return m_deviceNames.value(m_currentDeviceKey, QStringLiteral("Employee %1").arg(m_currentDeviceKey));
}

void HistoryView::onKeylogTableClicked()
{
    // History.qml exportTextButton -> KeyloggerExporter.beginExport(session,
    // employee, historyModel.start, historyModel.stop): the session's
    // keystrokes over the whole period.
    if (m_textExport.active || !m_timeline->hasData()) {
        return;
    }
    const QString employee = m_deviceNames.value(m_currentDeviceKey).trimmed();
    const QString stem = QStringLiteral("%1_%2_%3").arg(employee, dateToStr(m_period.begin()),
                                                        dateToStr(m_period.end()));
    // utils/KeyloggerExport.qml.
    SaveDialog dialog(QStringLiteral("Save keystrokes"), QStringLiteral("csv"),
                             QStringLiteral("keyloggerExport"), stem + QStringLiteral(".csv"), this);
    dialog.addRow(QStringLiteral("Session:"), sessionMarker());
    auto *format = new MultiSwitch(&dialog);
    format->addTab(QStringLiteral("csv"));
    format->addTab(QStringLiteral("xlsx"));
    format->setCurrentIndex(0);
    connect(format, &MultiSwitch::currentChanged, &dialog, [&dialog, stem](int index) {
        const QString extension = index == 1 ? QStringLiteral("xlsx") : QStringLiteral("csv");
        dialog.setExtension(extension);
        dialog.setInitialName(stem + QLatin1Char('.') + extension);
    });
    dialog.addRow(QStringLiteral("File format:"), format);
    auto *withoutSystemKeys = new QCheckBox(QStringLiteral("without system buttons"), &dialog);
    dialog.addWidget(withoutSystemKeys);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    m_textExport = TextExportState();
    m_textExport.active = true;
    m_textExport.path = dialog.path();
    m_textExport.xlsx = format->currentIndex() == 1;
    m_textExport.cutSystemKeys = withoutSystemKeys->isChecked();
    m_textExport.rangeKey = m_rangeKey;
    m_textExport.streamId = currentStreamId();
    // GenericBox "In progress..." with a Spinner and Cancel.
    auto *progress = new QProgressDialog(QStringLiteral("In progress..."), QStringLiteral("Cancel"), 0, 0, this);
    progress->setWindowTitle(QStringLiteral("In progress..."));
    progress->setModal(true);
    progress->setMinimumDuration(0);
    progress->setAttribute(Qt::WA_DeleteOnClose);
    connect(progress, &QProgressDialog::canceled, this, [this] {
        // onCancel: state 0 -- whatever still comes for it is ignored.
        m_textExport.active = false;
        m_textExport.tag.clear();
        if (m_textExport.progress) {
            m_textExport.progress->close();
        }
    });
    m_textExport.progress = progress;
    progress->show();
    // KeyloggerExporter's Selector for the session over the period.
    m_textExport.tag = QStringLiteral("history-export-%1").arg(++m_keyEventsGeneration);
    m_connection.requestKeyEvents(m_textExport.streamId, periodStartMs(), periodStopMs(), m_textExport.tag);
}

void HistoryView::finishTextExport(const KeyEventsReply &reply)
{
    m_textExport.active = false;
    if (m_textExport.progress) {
        QPointer<QDialog> progress = m_textExport.progress;
        m_textExport.progress = nullptr;
        progress->close();
    }
    // KeyloggerExporter's Result: showSites, showExecutables, showBinForSites
    // = HistoryTab.showBinForSite (off only for History by a web page),
    // appFilter = appFilterName, no keyword, the dialog's cutSystemKeys.
    Keylogger::ResultOptions options;
    options.cutSystemKeys = m_textExport.cutSystemKeys;
    options.showSites = true;
    options.showExecutables = true;
    options.showBinForSites = !(m_appFilter.isSet() && m_appFilter.isWebPage);
    options.appFilter = m_appFilter.name;
    const QList<KeyloggerRow> rows = Keylogger::buildResult(reply.events, reply.legacy, options);
    // trySave: S_loadedComplete and S_loadedPartial both write.
    const QString result = m_textExport.xlsx ? Keylogger::exportToXlsx(rows, m_textExport.path)
                                             : Keylogger::exportToCsv(rows, m_textExport.path);
    if (result != QStringLiteral("success")) {
        // KeyloggerExporter.qml: MessageBox "Keylogger export failed".
        QMessageBox::warning(this, QStringLiteral("Keylogger export failed"), result);
    } else if (!reply.complete) {
        QMessageBox::warning(this, QStringLiteral("Keylogger export incomplete"),
                             reply.partialReason.isEmpty()
                                 ? QStringLiteral("Only part of the period's keystrokes could be loaded; the file "
                                                  "holds what was.")
                                 : QStringLiteral("Only part of the period's keystrokes could be loaded (%1); the "
                                                  "file holds what was.")
                                       .arg(reply.partialReason));
    }
}

void HistoryView::failTextExport(const QString &message)
{
    if (!m_textExport.active) {
        return;
    }
    m_textExport.active = false;
    m_textExport.tag.clear();
    if (m_textExport.progress) {
        QPointer<QDialog> progress = m_textExport.progress;
        m_textExport.progress = nullptr;
        progress->close();
    }
    // KeyloggerExporter state 5: the error MessageBox.
    QMessageBox::warning(this, QStringLiteral("Keylogger export failed"),
                         message.isEmpty() ? QStringLiteral("The keystrokes could not be loaded.") : message);
}

void HistoryView::onExportVideoClicked()
{
    // History.qml exportAviButton -> VideoSaverSelector (a SaveDialog for a
    // .webm) -> VideoSaverProgress (PlayerVideoSaver.start(destination, mps,
    // "History of <session>", quallity)).
    if (isVideoSaving()) {
        showVideoSaverBox();
        return;
    }
    static const double kSpeeds[] = {0.5, 0.75, 1.0, 2.0, 4.0, 8.0, 16.0};
    const double mps = kSpeeds[qBound(0, m_speedCombo->currentIndex(), 6)];
    const QString employee = m_deviceNames.value(m_currentDeviceKey).trimmed();
    const QString initial = QStringLiteral("%1_%2 - %3").arg(employee, dateToStr(m_period.begin()),
                                                             dateToStr(m_period.end()));
    SaveDialog dialog(QStringLiteral("Video saver"), QStringLiteral("webm"), QStringLiteral("videoSave"),
                             initial, this);
    dialog.addRow(QStringLiteral("Session: "), sessionMarker());
    dialog.addRow(QStringLiteral("Time acceleration factor: "), m_speedCombo->currentText());
    auto *quality = new MultiSwitch(&dialog);
    quality->addTab(QStringLiteral("Low"));
    quality->addTab(QStringLiteral("Medium"));
    quality->addTab(QStringLiteral("High"));
    quality->setCurrentIndex(1); // Q_good
    dialog.addRow(QStringLiteral("Quallity:"), quality);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    m_videoSaverJob = HistoryVideoJob();
    m_videoSaverJob.destination = dialog.path();
    m_videoSaverJob.mps = mps;
    m_videoSaverJob.trackName = QStringLiteral("History of %1").arg(sessionMarker());
    m_videoSaverJob.quality = quality->currentIndex() == 0 ? WebmWriter::Quality::Poor
                            : quality->currentIndex() == 2 ? WebmWriter::Quality::Best
                                                           : WebmWriter::Quality::Good;
    m_videoSaverTag = m_cacheTag;
    if (m_videoSaver) {
        m_videoSaver->deleteLater();
    }
    m_videoSaver = new HistoryVideoSaver(this);
    connect(m_videoSaver, &HistoryVideoSaver::stateChanged, this, &HistoryView::onVideoSaverStateChanged);
    connect(m_videoSaver, &HistoryVideoSaver::progressChanged, this, [this](double progress) {
        if (m_videoSaverBar) {
            static_cast<SaverProgressBar *>(m_videoSaverBar)->setValue(qRound(progress * 100));
        }
    });
    showVideoSaverBox();
    if (!tryStartVideoJob()) {
        // S_waiting: the saver starts once the block has nothing left to
        // download (onUpdatedLoadedRanges).
        m_videoSaver->wait();
    }
}

bool HistoryView::tryStartVideoJob()
{
    if (!m_videoSaver || (m_videoSaver->state() != HistoryVideoSaver::State::Ready
                          && m_videoSaver->state() != HistoryVideoSaver::State::Waiting)) {
        return false;
    }
    // The period has to be the one shown (its download only runs then) and
    // completely downloaded.
    if (m_cacheTag != m_videoSaverTag || !m_downloadComplete || !m_timeline->hasData()) {
        return false;
    }
    HistoryVideoJob job = m_videoSaverJob;
    job.cacheFile = HistorySegmentStore::cacheFileForTag(m_cacheTag);
    job.streams = historyStreams();
    // One picture per marker that has one on any screen (FUN_1402a8430).
    for (int marker = 0; marker < m_timeline->markersAmount(); ++marker) {
        if (!m_timeline->isAllowedMarker(marker)) {
            continue;
        }
        if (!markerReady(marker)) {
            return false; // still Loading somewhere: keep waiting
        }
        const qint64 timestampMs = m_timeline->timestampForMarker(marker);
        HistoryVideoJob::Marker entry;
        for (quint32 streamId : std::as_const(job.streams)) {
            qint64 lookupMs = timestampMs;
            if (screenState(streamId, timestampMs, &lookupMs) == ScreenState::Ready
                && m_segmentStore.covers(streamId, lookupMs)) {
                entry.lookupMs.insert(streamId, lookupMs);
            }
        }
        if (!entry.lookupMs.isEmpty()) {
            job.markers.append(entry);
        }
    }
    m_videoSaver->startJob(job);
    return true;
}

void HistoryView::showVideoSaverBox()
{
    if (m_videoSaverBox) {
        m_videoSaverBox->show();
        m_videoSaverBox->raise();
        return;
    }
    // VideoSaverProgress.qml: a modal GenericBox "Video saver", not
    // closable, with the progress bar and Cancel.
    auto *box = new QDialog(this, Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    box->setWindowTitle(QStringLiteral("Video saver"));
    box->setModal(true);
    auto *layout = new QVBoxLayout(box);
    auto *bar = new SaverProgressBar(box);
    bar->setValue(m_videoSaver ? qRound(m_videoSaver->progress() * 100) : 0);
    m_videoSaverBar = bar;
    layout->addWidget(bar, 0, Qt::AlignHCenter);
    auto *buttons = new QDialogButtonBox(box);
    buttons->addButton(QStringLiteral("Cancel"), QDialogButtonBox::RejectRole);
    layout->addWidget(buttons);
    connect(buttons, &QDialogButtonBox::rejected, this, [this] {
        // cancelEncodingWithConfirmation(): while busy the box hides and a
        // ConfirmBox asks; Cancel there brings the box back.
        if (!isVideoSaving()) {
            if (m_videoSaverBox) {
                m_videoSaverBox->deleteLater();
            }
            return;
        }
        m_videoSaverBox->hide();
        const auto answer = QMessageBox::question(this, QStringLiteral("Video saver"),
                                                  QStringLiteral("Video saving will be interrupted."),
                                                  QMessageBox::Ok | QMessageBox::Cancel);
        if (answer == QMessageBox::Ok) {
            if (m_videoSaver) {
                m_videoSaver->cancel();
            }
        } else if (m_videoSaverBox) {
            m_videoSaverBox->show();
        }
    });
    m_videoSaverBox = box;
    box->show();
}

void HistoryView::onVideoSaverStateChanged()
{
    if (!m_videoSaver) {
        return;
    }
    switch (m_videoSaver->state()) {
    case HistoryVideoSaver::State::Done:
    case HistoryVideoSaver::State::Canceled:
        // S_done / S_canceled: the box goes away, nothing else is said.
        if (m_videoSaverBox) {
            m_videoSaverBox->deleteLater();
        }
        m_videoSaverBar = nullptr;
        break;
    case HistoryVideoSaver::State::Error: {
        if (m_videoSaverBox) {
            m_videoSaverBox->hide();
            m_videoSaverBox->deleteLater();
        }
        m_videoSaverBar = nullptr;
        const QString message = m_videoSaver->errorString();
        QMessageBox::critical(this, QStringLiteral("Error"), message);
        break;
    }
    default:
        break;
    }
}

void HistoryView::onToggleRunningApps()
{
    // History.qml's appButton (checkable, closed by default): the arrow
    // flips and animationPanel moves the panel in from the video's right
    // edge, or back out of it.
    m_appsOpen = !m_appsOpen;
    m_toggleAppsButton->setIcon(QIcon(m_appsOpen
        ? QStringLiteral(":/history/video/buttonRunningApp/applications_arrow_right.png")
        : QStringLiteral(":/history/video/buttonRunningApp/applications_arrow_left.png")));
    const QRect video(m_videoPanel->mapTo(this, QPoint(0, 0)), m_videoPanel->size());
    const int openX = width() - m_appsPanel->width() - 10;
    const int closedX = video.right() + 1;
    m_appsAnimation->stop();
    if (!m_appsOpen) {
        // rightPadding drops to 0 as soon as the button is unchecked.
        m_videoBody->setContentsMargins(0, 0, 0, 0);
    }
    if (m_appsOpen) {
        positionOverlays();
        m_appsPanel->move(closedX, m_appsPanel->y());
        m_appsPanel->show();
        m_toggleAppsButton->raise();
        m_appsAnimation->setEndValue(QPoint(openX, m_appsPanel->y()));
    } else {
        m_appsAnimation->setEndValue(QPoint(closedX, m_appsPanel->y()));
    }
    m_appsAnimation->setStartValue(m_appsPanel->pos());
    m_appsAnimation->start();
}

void HistoryView::updateHistoryStatusIcon(qint64 momentMs)
{
    // History.qml statusIcon.kind from infoFrames[0]: noData, then inLock,
    // then inSaver, else none. ("videoWatchDisabled" never applies here.)
    const auto inRanges = [momentMs](const QList<QPair<qint64, qint64>> &ranges) {
        for (const auto &range : ranges) {
            if (momentMs >= range.first && momentMs < range.second) {
                return true;
            }
        }
        return false;
    };
    QString kind;
    if (m_statesLoaded && momentMs >= 0) {
        // noData only where the session was there without data
        // (online_session_nodata); an offline stretch has no infoFrame.
        if (inRanges(m_noDataRanges)) {
            kind = QStringLiteral("noData");
        } else if (inRanges(m_lockRanges)) {
            kind = QStringLiteral("lock");
        } else if (inRanges(m_saverRanges)) {
            kind = QStringLiteral("saver");
        }
    }
    m_historyStatusIcon->setKind(kind);
    m_historyStatusIcon->setVisible(!kind.isEmpty());
    m_historyStatusIcon->raise();
    // History.qml:145-149: videoFrames = [] while the icon shows -- the
    // screens disappear, Video's #45464d stays behind the icon.
    m_videoStrip->setVisible(kind.isEmpty());
    m_stripScrollBar->setVisible(kind.isEmpty()
                                 && m_monitorStripArea->horizontalScrollBar()->maximum() > 0);
    // History.qml:172-178: a status icon takes the view out of panelFull.
    if (!kind.isEmpty() && m_panelFull) {
        togglePanelFull();
    }
}

void HistoryView::updateAppsButton()
{
    // appButton.enabled: historyModel.videoFrames && historyModel.isOnline.
    m_toggleAppsButton->setEnabled(m_timeline->hasData() && m_connection.isAuthenticated());
}

void HistoryView::onChangeSettingsClicked()
{
    // History.qml:383-389: no period change while the video is encoded.
    if (isVideoSaving()) {
        QMessageBox::critical(this, QStringLiteral("Error"), QStringLiteral("Can't change period during video encoding"));
        return;
    }
    HistoryChoiceDialog dialog(HistoryChoiceDialog::Mode::Change, employeeList(), m_currentDeviceKey,
                               m_period, m_timeStepMs, this);
    dialog.move(mapToGlobal(rect().center()) - QPoint(dialog.width() / 2, dialog.height() / 2));
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    m_timeStepMs = dialog.timeStepMs();
    applyTimeStep();
    // History.qml:259-262: appButton.checked = false; markersPerSecond = 0;
    // a new period opens at its start.
    setAppsOpen(false);
    m_resumeMomentMs = -1;
    m_resumePlaying = false;
    if (m_currentTab >= 0) {
        HistoryTab &tab = m_historyTabs[m_currentTab];
        tab.deviceKey = dialog.employee();
        tab.employeeName = m_deviceNames.value(dialog.employee());
        tab.period = dialog.period();
        tab.timeStepMs = m_timeStepMs;
        updateHistoryTabText(m_currentTab);
    }
    saveTabs();
    jumpTo(dialog.employee(), dialog.period());
}

void HistoryView::onFramesReceived(quint32 streamId, const QString &day,
                                   const QList<qint64> &timestamps)
{
    if (day != m_rangeKey || !historyStreams().contains(streamId)) {
        return;
    }
    m_framesByStream.insert(streamId, timestamps);
    mergeFrameLists();
    if (m_infoPending) {
        checkInfoReady();
        return;
    }
    // A list arriving after the information (a reconnect): this screen's
    // state at the current moment.
    if (m_timeline->currentMarker() < 0 && m_timeline->hasData()) {
        startPlayer();
        return;
    }
    const qint64 moment = m_timeline->timestampForMarker(m_timeline->currentMarker());
    if (moment >= 0 && showFromStore(moment) && !m_frameRequestInFlight) {
        dispatchFrameRequest();
    }
}

void HistoryView::checkInfoReady()
{
    if (!m_infoPending) {
        return;
    }
    // LoadInfoDone: the screens, every screen's frame list and the session
    // states are known (or the information timed out).
    bool framesKnown = m_screensAnswered;
    for (quint32 streamId : historyStreams()) {
        framesKnown = framesKnown && m_framesByStream.contains(streamId);
    }
    if (!(framesKnown && m_statesLoaded) && !m_infoTimedOut) {
        return;
    }
    m_infoPending = false;
    m_infoTimeout->stop();
    hideLoadingDialog();
    if (!m_timeline->hasData()) {
        // Video.qml excuse: !isOnline && !waitingForLoad.
        m_statusLabel->setText(QStringLiteral("No information for selected period"));
        updateStatusVisibility();
        m_videoStrip->setStreams({});
        return;
    }
    m_statusLabel->clear();
    updateStatusVisibility();
    rebuildMonitorStrip();
    startPlayer();
}

void HistoryView::startPlayer()
{
    // FUN_140272ef0: setMarker(0) snaps to the first recorded moment -- or,
    // for a tab shown again / a period reloaded after a reconnect, the
    // moment it was at, playing again if it was.
    int marker = -1;
    if (m_resumeMomentMs >= 0) {
        marker = m_timeline->nearestAllowedMarker(m_timeline->markerForTimestamp(m_resumeMomentMs), false);
    } else if (m_resumeMarker >= 0) {
        marker = m_timeline->nearestAllowedMarker(m_resumeMarker, false);
    }
    m_resumeMomentMs = -1;
    m_resumeMarker = -1;
    if (marker < 0) {
        marker = m_timeline->nearestAllowedMarker(0, true);
    }
    requestFrameAtMarker(marker);
    if (m_resumePlaying) {
        m_resumePlaying = false;
        setPlaying(true);
        m_playbackTimer->start();
    }
}

void HistoryView::onConnectionStatus(bool connected)
{
    if (connected || m_connectionLost || m_rangeKey.isEmpty()) {
        return;
    }
    m_connectionLost = true;
    // The export's Selector will not be answered either.
    if (m_textExport.active) {
        failTextExport(QStringLiteral("The connection to the host was lost."));
    }
    // Nothing sent before the drop will be answered: the request slots are
    // freed (the download in flight is asked for again after the reconnect),
    // while the player keeps its moment, its play state, the cache and the
    // blue ranges -- what is downloaded keeps playing, the rest waits as
    // Loading. Only a period whose timeline never arrived is reloaded.
    if (!m_timeline->hasData()) {
        if (m_resumeMarker < 0) {
            m_resumeMarker = m_timeline->currentMarker();
            m_resumePlaying = m_playbackTimer->isActive();
        }
        m_playbackTimer->stop();
        setPlaying(false);
    }
    m_frameWatchdog->stop();
    m_frameRequestInFlight = false;
    m_inFlightStreams.clear();
    m_inFlightMarker = -1;
    m_inFlightRequestId = 0;
    m_segmentRequestInFlight = false;
    hideLoadingDialog();
    updateAppsButton();
}

void HistoryView::onSessionStarted()
{
    if (!m_connectionLost) {
        return;
    }
    m_connectionLost = false;
    if (!m_activated || m_currentTab < 0) {
        m_resumeMarker = -1;
        m_resumePlaying = false;
        return;
    }
    // The device list comes back with the agent's hello (setDevices); the
    // period is reloaded once this employee's screens are known again.
    m_reloadPending = true;
    if (!m_deviceMonitorStreams.value(m_currentDeviceKey).isEmpty()) {
        QTimer::singleShot(0, this, [this] {
            if (m_reloadPending) {
                resumeAfterReconnect();
            }
        });
    }
}

void HistoryView::resumeAfterReconnect()
{
    m_reloadPending = false;
    if (m_timeline->hasData() && !m_rangeKey.isEmpty()) {
        // No reload: the queue picks up around the current moment.
        m_segmentRequestInFlight = false;
        m_resumeMarker = -1;
        m_resumePlaying = false;
        if (!m_statesLoaded) {
            m_connection.requestSessionStates(m_periodAnchor, periodStartMs(), periodStopMs());
        }
        // historyModel.keyboardEvents never came: a new request (the old
        // tag is dropped, so a late answer cannot overwrite it).
        if (!m_keyEventsSettled) {
            m_keyEventsTag = QStringLiteral("history-keylogger-%1").arg(++m_keyEventsGeneration);
            m_connection.requestKeyEvents(m_periodAnchor, periodStartMs(), periodStopMs(), m_keyEventsTag);
        }
        for (quint32 screen : historyStreams()) {
            if (!m_videoSegments.contains(screen)) {
                m_connection.requestHistorySegments(screen, periodStartMs(), periodStopMs());
            }
        }
        dispatchPrefetch();
        const int marker = m_timeline->currentMarker();
        if (marker >= 0) {
            requestFrameAtMarker(marker);
        }
        updateAppsButton();
        return;
    }
    const int marker = m_resumeMarker;
    const bool playing = m_resumePlaying;
    loadPeriod();
    m_resumeMarker = marker;
    m_resumePlaying = playing;
}

void HistoryView::mergeFrameLists()
{
    // SliderBar.qml's effectiveRanges belong to the session block, not to a
    // screen: the slider shows wherever ANY screen recorded something.
    QList<qint64> merged;
    for (auto it = m_framesByStream.cbegin(); it != m_framesByStream.cend(); ++it) {
        merged += it.value();
    }
    std::sort(merged.begin(), merged.end());
    merged.erase(std::unique(merged.begin(), merged.end()), merged.end());
    m_timestamps = merged;
    const int marker = m_timeline->currentMarker();
    m_timeline->setTimestamps(m_timestamps);
    if (marker >= 0) {
        m_timeline->setCurrentMarker(marker);
    }
    // Whatever is already downloaded is blue again at once; everything else
    // stays gray until its frames actually arrive.
    refreshLoadedOverlay();
}

void HistoryView::onScreensReceived(quint32 streamId, const QString &day,
                                    const QList<QPair<quint32, QString>> &screens)
{
    if (day != m_rangeKey || streamId != m_periodAnchor) {
        return;
    }
    m_historyStreams.clear();
    for (const QPair<quint32, QString> &screen : screens) {
        m_historyStreams.append(screen.first);
    }
    m_screensAnswered = true;
    if (m_historyStreams.isEmpty()) {
        m_infoPending = false;
        m_infoTimeout->stop();
        m_statusLabel->setText(QStringLiteral("No information for selected period"));
        updateStatusVisibility();
        m_videoStrip->setStreams({});
        hideLoadingDialog();
        return;
    }
    rebuildMonitorStrip();
    // Now the video side of the period, for every screen that has any.
    const qint64 start = periodStartMs();
    const qint64 stop = periodStopMs();
    for (quint32 screen : std::as_const(m_historyStreams)) {
        m_connection.requestHistoryFrames(screen, start, stop);
        m_connection.requestHistorySegments(screen, start, stop);
    }
}

void HistoryView::onFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image,
                                  quint64 requestId)
{
    // An answer prepared for a period or an employee the user has already
    // left must never reach the screen.
    if (!isCurrentGeneration(requestId)) {
        qCDebug(lcHistoryTiming, "dropping frame of generation %u (now %u)",
                quint32(requestId >> 32), m_frameGeneration);
        return;
    }
    if (!historyStreams().contains(streamId)) {
        return;
    }
    // Keep it even if it is not the position we are on any more: it is a real
    // downloaded frame, and the next seek back here will be instant.
    cacheFrame(streamId, timestampMs, image);

    // An answer to a round that is no longer the one in flight: kept in the
    // cache, nothing else.
    if (requestId != m_inFlightRequestId) {
        return;
    }
    // Show it as long as nothing newer is already on screen. While "next" is
    // held down the user runs ahead of the host, and every frame that comes
    // back is still a position they are scrubbing through -- it belongs on
    // screen, exactly like dragging a video player's scrubber. Only an answer
    // older than what is displayed (a late one for a position already
    // overtaken by a cached frame) is dropped.
    if (m_inFlightOrdinal >= m_displayedOrdinal) {
        m_displayedOrdinal = m_inFlightOrdinal;
        m_videoStrip->setFrame(streamId, image);
        if (streamId == currentStreamId()) {
            noteFrameShown(timestampMs);
            if (m_seekTimer.isValid()) {
                qCDebug(lcHistoryTiming, "marker %d: %lld ms from cursor to picture",
                        m_inFlightMarker, m_seekTimer.elapsed());
            }
        }
    }
    m_inFlightStreams.remove(streamId);
    if (m_inFlightStreams.isEmpty()) {
        // The whole round is answered: if the user has moved on meanwhile,
        // fetch the latest position only -- never the ones it passed over.
        releaseFrameSlot();
    }
}

bool HistoryView::isEmptyRange(quint32 streamId, qint64 timestampMs) const
{
    for (const QPair<qint64, qint64> &range : m_emptyRanges.value(streamId)) {
        if (timestampMs >= range.first && timestampMs < range.second) {
            return true;
        }
    }
    return false;
}

void HistoryView::onSegmentsReceived(quint32 streamId, const QString &day,
                                     const QList<HistoryVideoSegmentInfo> &segments)
{
    if (day != m_rangeKey || !historyStreams().contains(streamId)) {
        return;
    }
    m_videoSegments.insert(streamId, segments);
    dispatchPrefetch();
    // Which of this screen's moments are Loading and which have no video
    // is known now.
    const qint64 moment = m_timeline->timestampForMarker(m_timeline->currentMarker());
    if (moment >= 0 && showFromStore(moment) && !m_frameRequestInFlight) {
        dispatchFrameRequest();
    }
}

void HistoryView::dispatchPrefetch()
{
    // One run in flight at a time: the host serves frames the user is waiting
    // on ahead of this anyway, and a single outstanding request keeps the
    // order "nearest to the cursor first" meaningful.
    if (m_segmentRequestInFlight || m_rangeKey.isEmpty() || !m_connection.isAuthenticated()) {
        return;
    }
    const qint64 periodStart = periodStartMs();
    const qint64 periodStop = periodStopMs();
    const int marker = m_timeline->currentMarker();
    const qint64 around = marker >= 0 ? m_timeline->timestampForMarker(marker) : periodStart;

    quint32 bestStream = 0;
    qint64 bestSequence = 0;
    qint64 bestFrom = 0;
    qint64 bestTo = 0;
    quint64 bestDistance = std::numeric_limits<quint64>::max();

    for (quint32 streamId : historyStreams()) {
        for (const HistoryVideoSegmentInfo &run : m_videoSegments.value(streamId)) {
            const qint64 start = qMax(run.beginMs, periodStart);
            const qint64 stop = qMin(run.endMs + 1, periodStop);
            if (stop <= start) {
                continue;
            }
            qint64 gap = 0;
            if (!m_segmentStore.nextGap(streamId, start, stop, around < 0 ? start : around, &gap)) {
                continue;
            }
            if (isEmptyRange(streamId, gap)) {
                continue;
            }
            // FUN_14025e8c0's order: (uint64)(segment.stop - wantedPos)
            // ascending -- from the playhead forward, then wrapping round to
            // the period's start; every screen alike.
            const quint64 distance = quint64(gap - (around < 0 ? start : around));
            if (distance < bestDistance) {
                bestDistance = distance;
                bestStream = streamId;
                bestSequence = run.sequenceId;
                bestFrom = gap;
                bestTo = stop;
            }
        }
    }
    if (bestStream == 0) {
        // Everything in the period is held -- "downloaded", once every
        // screen's run list is known.
        bool listsKnown = true;
        for (quint32 streamId : historyStreams()) {
            listsKnown = listsKnown && m_videoSegments.contains(streamId);
        }
        m_downloadComplete = listsKnown && !historyStreams().isEmpty();
        if (m_downloadComplete && m_videoSaver && m_videoSaver->state() == HistoryVideoSaver::State::Waiting) {
            tryStartVideoJob();
        }
        return;
    }
    m_downloadComplete = false;
    m_segmentRequestInFlight = true;
    m_segmentStreamId = bestStream;
    m_segmentFromMs = bestFrom;
    m_segmentToMs = bestTo;
    m_segmentRequestId = allocateRequestId();
    m_connection.requestHistorySegment(bestStream, bestSequence, bestFrom, bestTo,
                                       m_segmentRequestId);
}

void HistoryView::onSegmentReceived(quint32 streamId, quint64 requestId,
                                    const ViewerProtocol::HistorySegmentPayload &segment)
{
    const bool wasOurs = requestId == m_segmentRequestId && m_segmentRequestInFlight;
    if (wasOurs) {
        m_segmentRequestInFlight = false;
    }
    // A run downloaded for the period we have since left is of no use.
    if (!isCurrentGeneration(requestId)
        || !historyStreams().contains(streamId)) {
        if (wasOurs) {
            dispatchPrefetch();
        }
        return;
    }
    // A complete answer (no hasMore) that stops short of what was asked:
    // nothing was recorded between its last frame and the end of the asked
    // stretch. Remember it, or the same gap would be asked for again and
    // again (the answer is a chunk already held) and the period would never
    // count as downloaded.
    if (wasOurs && streamId == m_segmentStreamId && !segment.hasMore && !segment.frames.isEmpty()) {
        qint64 last = segment.frames.first().timestampMs;
        for (const ViewerProtocol::SegmentFrame &frame : segment.frames) {
            last = qMax(last, frame.timestampMs);
        }
        const qint64 tail = qMax(m_segmentFromMs, last + 1);
        if (tail < m_segmentToMs) {
            m_emptyRanges[streamId].append({tail, m_segmentToMs});
        }
    }
    if (m_segmentStore.addSegment(streamId, segment)) {
        const int marker = m_timeline->currentMarker();
        const qint64 around = marker >= 0 ? m_timeline->timestampForMarker(marker) : periodStartMs();
        m_segmentStore.trim(around);
        // SliderBar.qml's loadedRanges: the union of every screen's
        // downloaded segments -- the whole new stretch turns blue at once.
        m_timeline->markLoadedRange(segment.frames.first().timestampMs,
                                    segment.frames.last().timestampMs);
        qCDebug(lcHistoryTiming, "segment: %lld frames for screen %u, %lld KB held",
                qint64(segment.frames.size()), streamId, m_segmentStore.bytes() / 1024);
        // The position the user is on may have just become available.
        const qint64 moment = m_timeline->timestampForMarker(m_timeline->currentMarker());
        if (moment >= 0) {
            showFromStore(moment);
        }
    }
    if (wasOurs) {
        dispatchPrefetch();
    }
}

void HistoryView::onSegmentMissing(quint32 streamId, quint64 requestId)
{
    if (requestId != m_segmentRequestId || !m_segmentRequestInFlight) {
        return;
    }
    m_segmentRequestInFlight = false;
    // Nothing recorded from there to the end of that run: remember it, or the
    // prefetch would keep picking the same gap forever.
    if (isCurrentGeneration(requestId) && streamId == m_segmentStreamId) {
        m_emptyRanges[streamId].append({m_segmentFromMs, m_segmentToMs});
    }
    dispatchPrefetch();
}

HistoryView::ScreenState HistoryView::screenState(quint32 streamId, qint64 timestampMs, qint64 *lookupMs) const
{
    if (lookupMs) {
        *lookupMs = timestampMs;
    }
    // A screen holds its last picture up to a Time step past its run's end
    // (the markers come from every screen's frames, not exactly this one's).
    const qint64 hold = qMax<qint64>(2000, m_timeStepMs);
    const auto runs = m_videoSegments.constFind(streamId);
    if (runs != m_videoSegments.cend()) {
        for (const HistoryVideoSegmentInfo &run : runs.value()) {
            if (timestampMs < run.beginMs || timestampMs > run.endMs + hold) {
                continue;
            }
            const qint64 at = qMin(timestampMs, run.endMs);
            if (lookupMs) {
                *lookupMs = at;
            }
            if (m_segmentStore.covers(streamId, at)) {
                return ScreenState::Ready;
            }
            // Nothing recorded there: the screen still shows its last
            // picture before the stretch (the latest image at or before the
            // moment), if that one is held.
            for (const QPair<qint64, qint64> &range : m_emptyRanges.value(streamId)) {
                if (at < range.first || at >= range.second) {
                    continue;
                }
                const qint64 before = range.first - 1;
                if (before >= run.beginMs && m_segmentStore.covers(streamId, before)) {
                    if (lookupMs) {
                        *lookupMs = before;
                    }
                    return ScreenState::Ready;
                }
                return ScreenState::Offline;
            }
            return ScreenState::Loading;
        }
    }
    if (m_segmentStore.covers(streamId, timestampMs) || !cachedFrame(streamId, timestampMs).isNull()) {
        return ScreenState::Ready;
    }
    if (runs == m_videoSegments.cend() || !m_framesByStream.contains(streamId)) {
        return ScreenState::Loading; // not known yet
    }
    // History from before the VP8 runs: a single picture, fetched on demand
    // where the screen's frame list has one near the moment.
    const QList<qint64> frames = m_framesByStream.value(streamId);
    const auto near = std::lower_bound(frames.cbegin(), frames.cend(), timestampMs - hold);
    if (near != frames.cend() && *near <= timestampMs + hold) {
        return ScreenState::Fetch;
    }
    return ScreenState::Offline;
}

bool HistoryView::markerReady(int marker) const
{
    const qint64 timestampMs = m_timeline->timestampForMarker(marker);
    if (timestampMs < 0) {
        return true;
    }
    for (quint32 streamId : historyStreams()) {
        if (screenState(streamId, timestampMs) == ScreenState::Loading) {
            return false;
        }
    }
    return true;
}

bool HistoryView::showFromStore(qint64 timestampMs)
{
    bool needsFetch = false;
    for (quint32 streamId : historyStreams()) {
        qint64 lookupMs = timestampMs;
        const ScreenState state = screenState(streamId, timestampMs, &lookupMs);
        if (state == ScreenState::Fetch) {
            needsFetch = true;
            continue;
        }
        if (state == ScreenState::Loading) {
            m_videoStrip->setLoading(streamId);
            continue;
        }
        if (state == ScreenState::Offline) {
            m_videoStrip->setOffline(streamId);
            continue;
        }
        // Downloaded VP8 first -- decoding a delta locally is far cheaper
        // than a round trip for a JPEG.
        QImage image = m_segmentStore.frameAt(streamId, lookupMs);
        if (image.isNull()) {
            image = cachedFrame(streamId, timestampMs);
        }
        if (image.isNull()) {
            m_videoStrip->setLoading(streamId);
            continue;
        }
        m_videoStrip->setFrame(streamId, image);
        m_displayedOrdinal = m_seekOrdinal;
        noteFrameShown(timestampMs);
    }
    return needsFetch;
}

QString HistoryView::cacheTag(quint32 deviceKey, const HistoryPeriod &period) const
{
    // The employee and where the period starts: a "today" period keeps its
    // file while the day goes on (its end is "now").
    const QString who = m_deviceNames.value(deviceKey, QString::number(deviceKey));
    return who + QLatin1Char('|') + QString::number(period.begin().toMSecsSinceEpoch());
}

void HistoryView::evictUnusedCaches()
{
    QSet<QString> mine;
    if (!m_cacheTag.isEmpty()) {
        mine.insert(QFileInfo(HistorySegmentStore::cacheFileForTag(m_cacheTag)).fileName());
    }
    for (const HistoryTab &tab : std::as_const(m_historyTabs)) {
        mine.insert(QFileInfo(HistorySegmentStore::cacheFileForTag(cacheTag(tab.deviceKey, tab.period))).fileName());
    }
    referencedCacheFiles().insert(this, mine);
    QSet<QString> referenced;
    for (const QSet<QString> &files : std::as_const(referencedCacheFiles())) {
        referenced.unite(files);
    }
    const QDir dir(HistorySegmentStore::cacheDirectory());
    const QStringList files = dir.entryList({QStringLiteral("vhdb.*.sqlite")}, QDir::Files);
    for (const QString &file : files) {
        if (!referenced.contains(file)) {
            QFile::remove(dir.filePath(file)); // fails harmlessly while open
        }
    }
}

void HistoryView::onHistoryFrameMissing(quint32 streamId, quint64 requestId)
{
    if (!isCurrentGeneration(requestId) || requestId != m_inFlightRequestId) {
        return;
    }
    // PlayerVideoFrame.Offline: that screen has nothing near this moment --
    // but only while nothing newer is already on screen (see onFrameReceived).
    if (m_inFlightOrdinal >= m_displayedOrdinal
        && historyStreams().contains(streamId)) {
        m_videoStrip->setOffline(streamId);
    }
    m_inFlightStreams.remove(streamId);
    if (m_inFlightStreams.isEmpty()) {
        releaseFrameSlot();
    }
}

void HistoryView::onAppSegmentsReceived(quint32 streamId, const QString &day,
                                        const QList<HistoryAppSegment> &segments)
{
    if (streamId != currentStreamId() || day != m_rangeKey) {
        return;
    }
    m_appSegments = segments;
    updateInfoPanel();
}

void HistoryView::onWebVisitsReceived(quint32 streamId, const QString &day,
                                      const QList<HistoryAppSegment> &visits)
{
    if (streamId != currentStreamId() || day != m_rangeKey) {
        return;
    }
    m_webVisits = visits;
    updateInfoPanel();
}

void HistoryView::updateInfoPanel()
{
    // infoFrame for the current marker: what was used during this Time step,
    // each resource's share of it, and which one was in use at the moment.
    const qint64 moment = m_timeline->timestampForMarker(m_timeline->currentMarker());
    if (moment < 0) {
        m_infoPanel->setItems({}, {});
        return;
    }
    const qint64 rangeStart = periodStartMs();
    const qint64 step = qMax<qint64>(1000, m_timeStepMs);
    const qint64 windowStart = rangeStart + (moment - rangeStart) / step * step;
    const qint64 windowEnd = windowStart + step;
    const auto build = [&](const QList<HistoryAppSegment> &segments, bool webPages) {
        QHash<QString, qint64> usedMs;
        // The title shown for a resource is the one it had AT the current
        // moment -- not the latest across the whole period. A resource can
        // carry different window titles over time (now one row per title, see
        // noteApplication), and the original's infoFrame shows the title of
        // the segment covering the marker. momentTitle wins; windowTitle (the
        // last title overlapping the visible step) is the fallback when the
        // moment itself lands in a gap between this resource's segments.
        QHash<QString, QString> momentTitle;
        QHash<QString, QString> windowTitle;
        QString active;
        qint64 totalMs = 0;
        for (const HistoryAppSegment &segment : segments) {
            const qint64 overlap = qMin(segment.endMs, windowEnd) - qMax(segment.startMs, windowStart);
            if (overlap > 0) {
                usedMs[segment.application] += overlap;
                totalMs += overlap;
                if (!segment.title.isEmpty()) {
                    windowTitle[segment.application] = segment.title;
                }
            }
            if (segment.startMs <= moment && moment < segment.endMs) {
                active = segment.application;
                momentTitle[segment.application] = segment.title;
            }
        }
        QList<HistoryInfoPanel::Item> items;
        for (auto it = usedMs.cbegin(); it != usedMs.cend(); ++it) {
            const QString title =
                momentTitle.contains(it.key()) ? momentTitle.value(it.key()) : windowTitle.value(it.key());
            items.append({it.key(), title.isEmpty() ? QStringLiteral("No title") : title,
                          100.0 * it.value() / qMax<qint64>(1, totalMs),
                          m_scopes.resolve(it.key(), webPages), it.key() == active, webPages});
        }
        std::sort(items.begin(), items.end(), [](const HistoryInfoPanel::Item &a, const HistoryInfoPanel::Item &b) {
            return a.percent > b.percent;
        });
        return items;
    };
    m_infoPanel->setItems(build(m_webVisits, true), build(m_appSegments, false));
}

bool runCategorizationDialog(QWidget *parent, ViewerConnection &connection, quint32 streamId,
                             const QString &resource, bool webPage, const QString &employeeName,
                             CategoryScopes &scopes)
{
    const QString key = CategoryScopes::key(resource, webPage);
    QList<CategorizationDialog::Row> rows;
    rows.append({QStringLiteral("All employees"), false, false, scopes.organization.value(key)});
    for (const HistoryDepartmentCategories &department : std::as_const(scopes.departments)) {
        rows.append({department.name, false, false, department.categories.value(key)});
    }
    rows.append({employeeName, true, false, scopes.employee.value(key)});
    CategorizationDialog dialog(key, rows, parent);
    dialog.move(parent->mapToGlobal(parent->rect().center()) - QPoint(dialog.width() / 2, dialog.height() / 2));
    if (dialog.exec() != QDialog::Accepted) {
        return false;
    }
    const auto unchanged = [](const QString &before, const QString &after) {
        return (before.isEmpty() ? QStringLiteral("none") : before) == after;
    };
    bool changed = false;
    // insert / update / delete: only the scopes whose rule was edited.
    const QString organization = dialog.category(0);
    if (!unchanged(scopes.organization.value(key), organization)) {
        scopes.organization.insert(key, organization);
        connection.setAppCategory(streamId, key, organization);
        changed = true;
    }
    for (int i = 0; i < scopes.departments.size(); ++i) {
        HistoryDepartmentCategories &department = scopes.departments[i];
        const QString category = dialog.category(i + 1);
        if (unchanged(department.categories.value(key), category)) {
            continue;
        }
        if (category == QStringLiteral("none")) {
            department.categories.remove(key);
        } else {
            department.categories.insert(key, category);
        }
        connection.setDepartmentCategory(streamId, department.id, key, category);
        changed = true;
    }
    const QString employee = dialog.category(int(scopes.departments.size()) + 1);
    if (!unchanged(scopes.employee.value(key), employee)) {
        if (employee == QStringLiteral("none")) {
            scopes.employee.remove(key);
        } else {
            scopes.employee.insert(key, employee);
        }
        connection.setAppCategory(streamId, key, employee, true);
        changed = true;
    }
    return changed;
}

void HistoryView::onCategorizationRequested(const QString &resource, bool webPage)
{
    if (runCategorizationDialog(this, m_connection, currentStreamId(), resource, webPage,
                                m_deviceNames.value(m_currentDeviceKey), m_scopes)) {
        m_chartsModel->applicationsChanged();
        updateInfoPanel();
    }
}

void HistoryView::onEmployeeCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_scopes.employee = categories;
    updateInfoPanel();
}

void HistoryView::onCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_scopes.organization = categories;
    updateInfoPanel();
}

void HistoryView::onKeyEvents(quint32 streamId, const QString &tag, const KeyEventsReply &reply)
{
    if (m_textExport.active && streamId == m_textExport.streamId && tag == m_textExport.tag) {
        finishTextExport(reply);
        return;
    }
    if (m_keyEventsTag.isEmpty() || tag != m_keyEventsTag) {
        return; // another Selector's (the Tracker's, or an older period's)
    }
    m_keyEventsSettled = true;
    m_keyEvents = reply.events;
    m_keyLegacy = reply.legacy;
    const qint64 moment = m_timeline->timestampForMarker(m_timeline->currentMarker());
    updateKeystream(moment);
}

void HistoryView::updateKeystream(qint64 timestampMs)
{
    // History.qml's two Keystreams over historyModel.keyboardEvents: the
    // events up to the moment (v.moment <= moment) in white, the ones after
    // it in gray -- each its own Translator (the QML path: nothing cut).
    // The earlier keylogger's chunks lead, as their saved text at their
    // save moment.
    m_keystream->setViewDisabled(!m_identities.value(m_currentDeviceKey).keyloggerView);
    const QPair<QString, QString> text = Keylogger::splitKeystream(m_keyEvents, m_keyLegacy, timestampMs);
    m_keystream->setText(text.first, text.second);
}

void HistoryView::onKeyEventsFailed(quint32 streamId, const QString &tag, const QString &message)
{
    if (m_textExport.active && streamId == m_textExport.streamId && tag == m_textExport.tag) {
        failTextExport(message);
        return;
    }
    if (!m_keyEventsTag.isEmpty() && tag == m_keyEventsTag) {
        // historyModel.keyboardEvents stays empty; the bar says nothing.
        m_keyEventsSettled = true;
        m_keyEvents.clear();
        m_keyLegacy.clear();
        updateKeystream(m_timeline->timestampForMarker(m_timeline->currentMarker()));
    }
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
