#include "devicedetailview.h"
#include "multiswitch.h"

#include "devicetilewidget.h" // statusIconText

#include "historyview.h"
#include "monitorwidget.h"
#include "viewerconnection.h"

#include <QAbstractItemView>
#include <QCheckBox>
#include <QComboBox>
#include <QDate>
#include <QDateTime>
#include <QEvent>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSignalBlocker>
#include <QSizePolicy>
#include <QSettings>
#include <QSlider>
#include <QToolButton>
#include <QStackedWidget>
#include <QStyle>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>

#include <QEnterEvent>
#include <QFontMetrics>
#include <QPixmap>

#include <algorithm>
#include <cmath>
#include <utility>

namespace {

QPixmap acAsset(const QString &path)
{
    return QPixmap(QStringLiteral(":/activeCell/") + path);
}

// A BorderImage with the same border on all four sides, every part
// stretched (Controls/Button.qml:30-33: imgBorder* 10).
void draw9Slice(QPainter &painter, const QRect &target, const QPixmap &pixmap, int border)
{
    if (pixmap.isNull() || target.isEmpty()) {
        return;
    }
    const int sw = pixmap.width();
    const int sh = pixmap.height();
    const int bx = qMin(border, qMin(sw, target.width()) / 2);
    const int by = qMin(border, qMin(sh, target.height()) / 2);
    const int xs[] = {0, bx, sw - bx, sw};
    const int ys[] = {0, by, sh - by, sh};
    const int xt[] = {target.left(), target.left() + bx, target.right() + 1 - bx, target.right() + 1};
    const int yt[] = {target.top(), target.top() + by, target.bottom() + 1 - by, target.bottom() + 1};
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            const QRect dst(xt[col], yt[row], xt[col + 1] - xt[col], yt[row + 1] - yt[row]);
            const QRect src(xs[col], ys[row], xs[col + 1] - xs[col], ys[row + 1] - ys[row]);
            if (!dst.isEmpty() && !src.isEmpty()) {
                painter.drawPixmap(dst, pixmap, src);
            }
        }
    }
}

// Utils/Fonts.qml sizes are point sizes (ssmall 8, smedium 9, sbig 11).
// The *_b variants render at regular weight in the running original (its
// "Roboto Bold" / "Roboto Regular" are face names, not families -- measured
// on the Back button and the caption), so bold only documents the QML name.
QFont roboto(int pointSize, bool bold)
{
    Q_UNUSED(bold)
    QFont f(QStringLiteral("Roboto"));
    f.setPointSize(pointSize);
    return f;
}

// A position QML leaves fractional, snapped the way it lands on screen.
int snapped(qreal value)
{
    return static_cast<int>(std::floor(value + 0.5));
}

// trackerQuadratorActiveCell/ChartTimeModels.qml: the Violations chart's
// Range and Step options (seconds), and which steps each range allows.
const qint64 kViolRangeSec[] = {3600, 14400, 28800, 86400, 432000, 864000};
const char *const kViolRangeText[] = {"1 hour", "4 hours", "8 hours", "1 day", "5 days", "10 days"};
const qint64 kViolStepSec[] = {60,   300,  600,   900,   1200,  1800,
                               3600, 7200, 14400, 28800, 43200, 86400};
const char *const kViolStepText[] = {"1 minute", "5 minutes", "10 minutes", "15 minutes",
                                     "20 minutes", "30 minutes", "1 hour", "2 hours",
                                     "4 hours", "8 hours", "12 hours", "1 day"};
// timeRangeInit: step indices valid for each range index.
const QList<int> kViolRangeSteps[] = {
    {0, 1, 3}, {1, 3, 5, 6}, {2, 3, 5, 6}, {4, 5, 6}, {7, 8, 9, 10, 11}, {8, 9, 10, 11}};
constexpr int kViolRangeCount = 6;
// Chart.qml labelsAreaLeftMargin / labelsAreaRightMargin.
constexpr int kViolGridLeft = 210;
constexpr int kViolGridRight = 25;

} // namespace

// Controls/BackButton.qml (Button.qml + arrow.png): button/*.png BorderImage
// (border 10) with the arrow icon and white "Back" (Fonts.rr_medium_b).
class BackButton final : public QPushButton
{
public:
    explicit BackButton(QWidget *parent) : QPushButton(parent)
    {
        setCursor(Qt::PointingHandCursor);
        m_normal = acAsset(QStringLiteral("button/normal.png"));
        m_hover = acAsset(QStringLiteral("button/hover.png"));
        m_press = acAsset(QStringLiteral("button/press.png"));
        m_arrow = acAsset(QStringLiteral("arrow.png"));
        m_font = roboto(9, true);
        // ImageButton.qml:76-77 implicit size = max(background, label);
        // :180-181 label = Row(arrow, 5 = (10 + 10) / 4 spacing, text) + 23
        // wide, + 11 high.
        const QFontMetrics metrics(m_font);
        const int textW = metrics.horizontalAdvance(tr("Back"));
        setFixedSize(qMax(m_normal.width(), m_arrow.width() + 5 + textW + 23),
                     qMax(m_normal.height(), qMax(m_arrow.height(), metrics.height()) + 11));
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        const QPixmap &bg = isDown() ? m_press : (m_hovered ? m_hover : m_normal);
        draw9Slice(painter, rect(), bg, 10);
        // ImageButton.qml:194-215: the Row centered, both children
        // vertically centered in it.
        const int textW = QFontMetrics(m_font).horizontalAdvance(tr("Back"));
        const int content = m_arrow.width() + 5 + textW;
        const int x = (width() - content) / 2;
        painter.drawPixmap(x, (height() - m_arrow.height()) / 2, m_arrow);
        painter.setFont(m_font);
        painter.setPen(QColor(0xf7, 0xf7, 0xf7));
        painter.drawText(QRect(x + m_arrow.width() + 5, 0, textW + 2, height()),
                         Qt::AlignLeft | Qt::AlignVCenter, tr("Back"));
    }
    void enterEvent(QEnterEvent *event) override
    {
        m_hovered = true;
        update();
        QPushButton::enterEvent(event);
    }
    void leaveEvent(QEvent *event) override
    {
        m_hovered = false;
        update();
        QPushButton::leaveEvent(event);
    }

private:
    QPixmap m_normal, m_hover, m_press, m_arrow;
    QFont m_font;
    bool m_hovered = false;
};

// StatusUser.qml: info_bg.png BorderImage (border 5, tiled) with the warning
// icon and "Not active: HH:MM:SS" (or "Active"), width = content + 20, left
// margin 10. DeviceDetailView places it in the header (Windows.qml:31-39).
class StatusUserBox final : public QWidget
{
public:
    explicit StatusUserBox(QWidget *parent) : QWidget(parent)
    {
        m_bg = acAsset(QStringLiteral("StatusUser/info_bg.png"));
        m_notActive = acAsset(QStringLiteral("StatusUser/info_not_active.png"));
        m_active = acAsset(QStringLiteral("StatusUser/info_active.png"));
        // StatusUser.qml:181 Fonts.rr_medium.
        m_font = roboto(9, false);
        // No height set: the BorderImage's implicit (source) height.
        setFixedHeight(m_bg.height());
        hide();
    }

    // idleText is metadataChanged's raw string ("Idle hh:mm:ss" / "Locked
    // hh:mm:ss" / empty). Empty means the session is active.
    void setIdle(const QString &idleText)
    {
        m_active_ = idleText.isEmpty();
        m_text = m_active_ ? tr("Active")
                           : tr("Not active: %1").arg(idleText.section(QLatin1Char(' '), 1));
        const QPixmap &icon = m_active_ ? m_active : m_notActive;
        setFixedWidth(icon.width() + 10 + QFontMetrics(m_font).horizontalAdvance(m_text) + 20);
        show();
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        // StatusUser.qml:162-165: horizontalTileMode Repeat, border 5 (the
        // box is drawn at the source height, so only the middle tiles).
        constexpr int kBorder = 5;
        if (!m_bg.isNull() && width() > 2 * kBorder) {
            painter.drawPixmap(QRect(0, 0, kBorder, height()), m_bg, QRect(0, 0, kBorder, m_bg.height()));
            painter.drawPixmap(QRect(width() - kBorder, 0, kBorder, height()), m_bg,
                               QRect(m_bg.width() - kBorder, 0, kBorder, m_bg.height()));
            painter.drawTiledPixmap(QRect(kBorder, 0, width() - 2 * kBorder, height()),
                                    m_bg.copy(kBorder, 0, m_bg.width() - 2 * kBorder, m_bg.height()));
        }
        // :167-172 the Row (icon, 10, text) at x 10, vertically centered.
        const QPixmap &icon = m_active_ ? m_active : m_notActive;
        painter.drawPixmap(10, (height() - icon.height()) / 2, icon);
        painter.setFont(m_font);
        painter.setPen(Qt::white);
        // :177 the text's verticalCenterOffset 1.
        painter.drawText(QRect(10 + icon.width() + 10, 1, width(), height()),
                         Qt::AlignLeft | Qt::AlignVCenter, m_text);
    }

private:
    QPixmap m_bg, m_notActive, m_active;
    QString m_text;
    QFont m_font;
    bool m_active_ = true;
};

// Windows.qml:80-128 keylogger: a BorderImage of the 1x10 keylogger_bg.png
// (border top/bottom 2, horizontalTileMode Repeat, verticalTileMode
// Stretch) under the right-aligned keystream text.
class KeylogTicker final : public QLabel
{
public:
    explicit KeylogTicker(QWidget *parent) : QLabel(parent)
    {
        m_bg = QPixmap(QStringLiteral(":/activeCell/keylogger_bg.png"));
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        if (!m_bg.isNull()) {
            QPainter painter(this);
            constexpr int kBorder = 2;
            const int sh = m_bg.height();
            const int sw = m_bg.width();
            const int h = height();
            // The image is one column wide: stretching it across the width is
            // the same as repeating it.
            painter.drawPixmap(QRect(0, 0, width(), kBorder), m_bg, QRect(0, 0, sw, kBorder));
            painter.drawPixmap(QRect(0, kBorder, width(), h - 2 * kBorder), m_bg,
                               QRect(0, kBorder, sw, sh - 2 * kBorder));
            painter.drawPixmap(QRect(0, h - kBorder, width(), kBorder), m_bg,
                               QRect(0, sh - kBorder, sw, kBorder));
        }
        QLabel::paintEvent(event);
    }

private:
    QPixmap m_bg;
};

// windows/Video.qml: a ScrollView whose Column (spacing 10) is the scroll
// width minus the 5px scrollbar and 25px, one windows/video/Monitor.qml per
// stream, each as tall as its width allows (MonitorWidget::heightForWidth).
// The ScrollView itself is only as tall as that content (contentHeight),
// capped by the room above the keylogger line -- see
// DeviceDetailView::updateVideoGeometry().
class MonitorColumn final : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorColumn(QScrollArea *scroll)
        : QWidget(scroll), m_scroll(scroll)
    {
        scroll->setWidget(this);
        scroll->setWidgetResizable(false);
        scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scroll->viewport()->installEventFilter(this);
    }

    void setMonitors(const QList<MonitorWidget *> &monitors)
    {
        for (MonitorWidget *monitor : std::as_const(m_monitors)) {
            if (monitor && monitor->parent() == this) {
                disconnect(monitor, nullptr, this, nullptr);
                monitor->hide();
                monitor->setParent(nullptr);
            }
        }
        m_monitors = monitors;
        for (MonitorWidget *monitor : monitors) {
            monitor->setParent(this);
            connect(monitor, &MonitorWidget::clicked, this, &MonitorColumn::monitorClicked);
            connect(monitor, &MonitorWidget::geometryHintChanged, this, &MonitorColumn::relayout);
            monitor->show();
        }
        relayout();
    }

    const QList<MonitorWidget *> &monitors() const { return m_monitors; }
    int contentHeight() const { return m_contentHeight; }

    void relayout()
    {
        const int columnWidth = qMax(0, m_scroll->width() - 5 - 25);
        int y = 0;
        for (int i = 0; i < m_monitors.size(); ++i) {
            MonitorWidget *monitor = m_monitors.at(i);
            const int h = monitor->heightForWidth(columnWidth);
            monitor->setGeometry(0, y, columnWidth, h);
            y += h + (i + 1 < m_monitors.size() ? 10 : 0);
        }
        const bool changed = y != m_contentHeight;
        m_contentHeight = y;
        resize(m_scroll->viewport()->width(), y);
        if (changed) {
            emit contentHeightChanged();
        }
    }

signals:
    void monitorClicked(quint32 streamId);
    void contentHeightChanged();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == m_scroll->viewport() && event->type() == QEvent::Resize) {
            relayout();
        }
        return QWidget::eventFilter(watched, event);
    }

private:
    QScrollArea *m_scroll;
    QList<MonitorWidget *> m_monitors;
    int m_contentHeight = 0;
};

DeviceDetailView::DeviceDetailView(ViewerConnection &connection, QWidget *parent)
    : QWidget(parent), m_connection(connection)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // TabView.qml: the whole page sits on a grayHatching background.
    setObjectName(QStringLiteral("detailView"));
    setAttribute(Qt::WA_StyledBackground);
    // Controls/ScrollView.qml: a slim (~5px handle), rounded, arrow-less
    // scrollbar over an invisible track (scrollView/*.png, handle ~#57585F),
    // NOT Qt's default wide bar with up/down arrow buttons and a groove. Set
    // on #detailView so it cascades to every QScrollArea in this page (the
    // monitors/programs stacks and the right-hand info panel).
    setStyleSheet(QStringLiteral(
        // TabView.qml sits on a single grayHatching background (#47484f); the
        // whole page -- both the video column and the SessionInfo panel --
        // shares it. Make every structural container transparent so that one
        // background shows through uniformly, instead of MainWindow's default
        // #30323a QWidget fill covering it on the left/gutters. Inputs and the
        // keylogger table keep their own explicit backgrounds (more specific
        // rules below / the global QLineEdit rule).
        "QWidget { background: transparent; }"
        "QWidget#detailView { background-image: url(:/activeCell/grayHatching.png); }"
        "QScrollBar:vertical { background: transparent; width: 11px; margin: 0 3px 0 3px; }"
        "QScrollBar::handle:vertical { background: none; min-height: 12px; border-width: 6px 0 6px 0; border-image: url(:/controls/scrollView/v_normal.png) 6 0 6 0 repeat repeat; }"
        "QScrollBar::handle:vertical:hover { border-image: url(:/controls/scrollView/v_hovered.png) 6 0 6 0 repeat repeat; }"
        "QScrollBar::handle:vertical:pressed { border-image: url(:/controls/scrollView/v_pressed.png) 6 0 6 0 repeat repeat; }"
        "QScrollBar:horizontal { background: transparent; height: 11px; margin: 3px 0 3px 0; }"
        "QScrollBar::handle:horizontal { background: none; min-width: 12px; border-width: 0 5px 0 7px; border-image: url(:/controls/scrollView/h_normal.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::handle:horizontal:hover { border-image: url(:/controls/scrollView/h_hovered.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::handle:horizontal:pressed { border-image: url(:/controls/scrollView/h_pressed.png) 0 5 0 7 repeat repeat; }"
        "QScrollBar::add-line, QScrollBar::sub-line { width: 0; height: 0; background: none; border: none; }"
        "QScrollBar::add-page, QScrollBar::sub-page { background: none; }"));

    // TabView.qml:185-259 topLine: 27px, Back, 20px, caption, 20px,
    // MultiSwitch, 20px, headerZone (fills to the right); topMargin 17.
    // The items carry their own verticalCenterOffsets, so they are placed by
    // hand in layoutHeader().
    m_headerRow = new QWidget(this);
    m_headerRow->setObjectName(QStringLiteral("detailHeader"));
    m_backButton = new BackButton(m_headerRow);
    connect(m_backButton, &QPushButton::clicked, this, &DeviceDetailView::backRequested);
    // TabView.qml:223-233 caption: Fonts.rb_big_b (Roboto Bold 11pt), white.
    m_nameLabel = new QLabel(m_headerRow);
    m_nameLabel->setFont(roboto(11, true));
    // The page stylesheet's font-size would override setFont().
    m_nameLabel->setStyleSheet(QStringLiteral("color: white; font-size: 11pt; font-weight: 400;"));
    m_nameLabel->installEventFilter(this); // catches the double-click, see eventFilter()

    m_tabSwitch = new MultiSwitch(m_headerRow);
    root->addWidget(m_headerRow);

    // Left content area: Programs / Monitors / Keylogger each swap what's
    // shown here via m_leftStack; Violations has no backing feature (no
    // violation-detection system exists anywhere in this project) so its
    // button stays disabled -- a real placeholder, not a stub page.
    m_leftStack = new QStackedWidget(this);

    auto *monitorsPage = new QWidget(m_leftStack);
    auto *monitorsPageLayout = new QVBoxLayout(monitorsPage);
    monitorsPageLayout->setContentsMargins(0, 0, 0, 0);
    // Windows.qml's instantStatusIcon: the big StatusIcon replaces the video
    // entirely (rather than being drawn per-monitor) whenever the session
    // has no live video to show at all -- offline, no session, or simply no
    // monitor streams reported yet.
    m_monitorInnerStack = new QStackedWidget(monitorsPage);
    auto *scroll = new QScrollArea(m_monitorInnerStack);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setStyleSheet(QStringLiteral("QScrollArea { background: transparent; }"));
    scroll->viewport()->setAutoFillBackground(false);
    m_monitorColumn = new MonitorColumn(scroll);
    connect(m_monitorColumn, &MonitorColumn::monitorClicked, this, [this](quint32 streamId) {
        // Monitors: MouseArea.onClicked -> wantShowFullMonitor(sourceId).
        emit fullMonitorRequested(streamId);
    });
    connect(m_monitorColumn, &MonitorColumn::contentHeightChanged, this, &DeviceDetailView::updateVideoGeometry);
    m_monitorInnerStack->addWidget(scroll);
    m_monitorStatusIcon = new BigStatusIcon(m_monitorInnerStack);
    m_monitorInnerStack->addWidget(m_monitorStatusIcon);
    monitorsPageLayout->addWidget(m_monitorInnerStack);
    m_leftStack->addWidget(monitorsPage);
    m_monitorsPage = monitorsPage;

    // Live preview of every currently open window on the device -- same
    // PrintWindow-based technique confirmed present in the reference
    // grabber, generalized from "the foreground window only" to "every open
    // window" (see windowlistcapture.h). Stacked with scroll, same as
    // Monitors. The Programs/Web pages percentage breakdown stays in the
    // persistent right-hand panel below; this tab is specifically about
    // *seeing* current activity, not another copy of the same list.
    auto *programsPage = new QWidget(m_leftStack);
    auto *programsPageLayout = new QVBoxLayout(programsPage);
    programsPageLayout->setContentsMargins(0, 0, 0, 0);
    // Same structure as Monitors: a scroll of previews, or a big StatusIcon
    // placeholder when there's nothing to show (Windows.qml's instantStatusIcon
    // -- not a plain gray text line).
    m_programsInnerStack = new QStackedWidget(programsPage);
    auto *windowsScroll = new QScrollArea(m_programsInnerStack);
    windowsScroll->setFrameShape(QFrame::NoFrame);
    windowsScroll->setStyleSheet(QStringLiteral("QScrollArea { background: transparent; }"));
    windowsScroll->viewport()->setAutoFillBackground(false);
    m_windowsColumn = new MonitorColumn(windowsScroll);
    connect(m_windowsColumn, &MonitorColumn::monitorClicked, this, [this](quint32 streamId) {
        // Programs: MouseArea.onClicked -> wantShowWindow(stream).
        emit windowMonitorRequested(streamId);
    });
    connect(m_windowsColumn, &MonitorColumn::contentHeightChanged, this, &DeviceDetailView::updateVideoGeometry);
    m_programsInnerStack->addWidget(windowsScroll);
    m_programsStatusIcon = new BigStatusIcon(m_programsInnerStack);
    m_programsInnerStack->addWidget(m_programsStatusIcon);
    programsPageLayout->addWidget(m_programsInnerStack);
    m_leftStack->addWidget(programsPage);
    m_programsPage = programsPage;

    // trackerQuadratorActiveCell/Keylogger.qml.
    m_keyloggerPage = new KeyloggerPage(m_connection, m_leftStack);
    connect(m_keyloggerPage, &KeyloggerPage::goToHistory, this,
            [this](quint32, const QDateTime &begin, const QDateTime &end, qint64 timeStepMs) {
                emit historyIntervalRequested(m_sessionKey, begin, end, timeStepMs);
            });
    m_leftStack->addWidget(m_keyloggerPage);

    // Violations: the Activity/Efficiency Chart (Filters.qml in this org-less
    // build).
    auto *violationsPage = new QWidget(m_leftStack);
    buildViolationsPage(violationsPage);
    m_leftStack->addWidget(violationsPage);
    m_violationsPage = violationsPage;

    // TrackerQuadratorActiveCell.qml's tab order: Programs, Monitors,
    // Violations, Keylogger. Default is Monitors (currentIndex 1, WOOS-616).
    m_tabSwitch->addTab(QStringLiteral("Programs"), QStringLiteral("tabs/windows_on.png"),
                        QStringLiteral("tabs/windows_off.png"));
    m_tabSwitch->addTab(QStringLiteral("Monitors"), QStringLiteral("tabs/monitor_on.png"),
                        QStringLiteral("tabs/monitor_off.png"));
    m_tabSwitch->addTab(QStringLiteral("Violations"), QStringLiteral("tabs/filters_on.png"),
                        QStringLiteral("tabs/filters_off.png"));
    m_tabSwitch->addTab(QStringLiteral("Keylogger"), QStringLiteral("tabs/keylogger_on.png"),
                        QStringLiteral("tabs/keylogger_off.png"));
    connect(m_tabSwitch, &MultiSwitch::currentChanged, this, &DeviceDetailView::switchSubTab);

    // Windows.qml's Row: 30px, the video (2/3 of the width), 25px,
    // SessionInfo (width/3 - 85, see resizeEvent), 30px.
    auto *content = new QWidget(this);
    auto *contentLayout = new QHBoxLayout(content);
    // contentVideo / SessionInfo are windows.height - 25 tall.
    contentLayout->setContentsMargins(30, 0, 30, 25);
    contentLayout->setSpacing(25);

    // Matches the real viewer's Windows.qml: a compact keylog ticker sits
    // directly under the video on both Monitors and Programs (hidden for
    // Keylogger, which already shows the full log) -- not just History.
    auto *leftColumn = new QWidget(content);
    auto *leftColumnLayout = new QVBoxLayout(leftColumn);
    leftColumnLayout->setContentsMargins(0, 0, 0, 0);
    // ColumnLayout spacing 10: Video, the keylogger line, then a filler.
    leftColumnLayout->setSpacing(10);
    leftColumnLayout->addWidget(m_leftStack, 1);
    // Windows.qml:80-101 keylogger BorderImage: 25px (Layout.preferredHeight),
    // keylogger_bg.png stretched vertically, the right-aligned white
    // Keystream (Fonts.rr_medium, 10px side margins, NoWrap, clip).
    m_keylogTicker = new KeylogTicker(leftColumn);
    m_keylogTicker->setFixedHeight(25);
    m_keylogTicker->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    m_keylogTicker->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    // textFormat: Text.PlainText -- "<CTRL + C>" is text, not a tag.
    m_keylogTicker->setTextFormat(Qt::PlainText);
    m_keylogTicker->setContentsMargins(10, 0, 10, 0);
    m_keylogTicker->setFont(roboto(9, false));
    m_keylogTicker->setStyleSheet(QStringLiteral("color: white;"));
    leftColumnLayout->addWidget(m_keylogTicker);
    leftColumnLayout->addStretch(0);
    m_leftColumn = leftColumn;
    m_leftColumn->installEventFilter(this);
    contentLayout->addWidget(leftColumn, 1);

    // Right column: the SessionInfo panel (Windows.qml:137-144).
    m_rightColumn = new QWidget(content);
    auto *rightLayout = new QVBoxLayout(m_rightColumn);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);
    m_infoArea = new QScrollArea(m_rightColumn);
    m_infoArea->setFrameShape(QFrame::NoFrame);
    m_infoArea->setWidgetResizable(true);
    m_infoArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_infoArea->setStyleSheet(QStringLiteral("QScrollArea { background: transparent; }"));
    m_infoPanel = new HistoryInfoPanel;
    m_infoPanel->setInset(0); // SessionInfo: rows are infoWidth wide, from x 0
    m_infoArea->setWidget(m_infoPanel);
    connect(m_infoPanel, &HistoryInfoPanel::categorizationRequested, this,
            &DeviceDetailView::onCategorizationRequested);
    rightLayout->addWidget(m_infoArea, 1);
    contentLayout->addWidget(m_rightColumn);
    root->addWidget(content, 1);

    // Windows.qml:31-39: StatusUser is parented to the TabView's headerZone,
    // not to the content -- see layoutHeader() for its geometry.
    m_statusUser = new StatusUserBox(m_headerRow);

    // Default to Monitors (WOOS-616) now that the ticker exists.
    m_tabSwitch->setCurrentIndex(1);
    switchSubTab(1);

    connect(&m_connection, &ViewerConnection::metadataChanged, this,
            [this](quint32 streamId, const QString &application, const QString &idleText) {
                if (streamId != m_primaryStreamId) {
                    return;
                }
                if (application != m_activeApplication) {
                    m_activeApplication = application;
                    updateInfoPanel();
                }
                // StatusUser.qml shows the box only while idle/locked
                // (runTimer4LockSaver); an active session hides it.
                if (idleText.isEmpty()) {
                    m_statusUser->hide();
                } else {
                    m_statusUser->setIdle(idleText);
                    layoutHeader();
                }
            });
    connect(&m_connection, &ViewerConnection::historyRunningApplicationsReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryAppUsage> &applications) {
                Q_UNUSED(day)
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_lastPrograms = applications;
                updateInfoPanel();
            });
    connect(&m_connection, &ViewerConnection::historyWebPagesReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryAppUsage> &pages) {
                Q_UNUSED(day)
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_lastWebPages = pages;
                updateInfoPanel();
            });
    connect(&m_connection, &ViewerConnection::historyCategoriesReceived, this,
            [this](quint32 streamId, const QHash<QString, QString> &categories) {
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_scopes.organization = categories;
                updateInfoPanel();
            });
    connect(&m_connection, &ViewerConnection::historyDepartmentCategoriesReceived, this,
            [this](quint32 streamId, const QList<HistoryDepartmentCategories> &departments) {
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_scopes.departments = departments;
                updateInfoPanel();
            });
    connect(&m_connection, &ViewerConnection::historyEmployeeCategoriesReceived, this,
            [this](quint32 streamId, const QHash<QString, QString> &categories) {
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_scopes.employee = categories;
                updateInfoPanel();
            });


    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(30000);
    connect(m_refreshTimer, &QTimer::timeout, this, &DeviceDetailView::refreshStats);
}

void DeviceDetailView::switchSubTab(int index)
{
    // Tab index -> stacked page.
    QWidget *page = m_monitorsPage;
    if (index == 0) {
        page = m_programsPage;
    } else if (index == 2) {
        page = m_violationsPage;
    } else if (index == 3) {
        page = m_keyloggerPage;
    }
    // The Keylogger's Selector only lives while its tab is shown.
    m_keyloggerPage->setActive(page == m_keyloggerPage);
    m_leftStack->setCurrentWidget(page);
    const bool videoTab = page == m_programsPage || page == m_monitorsPage;
    // SessionInfo (Web pages/Programs) belongs to Windows.qml (Programs/
    // Monitors); Violations (Chart) and Keylogger span the full width.
    if (m_rightColumn) {
        m_rightColumn->setVisible(videoTab);
    }
    if (index == 2) {
        // (Re)load the Activity/Efficiency data for the current window when
        // the Violations tab is opened.
        reloadViolationsData();
    }
    updateMonitorStatusIcon();
    emit streamsChanged();
}

namespace {
// A thin, arrow-less slider matching Controls/Slider.qml's look.
QString violSliderQss()
{
    return QStringLiteral(
        "QSlider::groove:horizontal { height: 2px; background: #43444c; margin: 0 8px; }"
        "QSlider::handle:horizontal { width: 13px; height: 13px; margin: -6px -7px;"
        " border-radius: 7px; background: #b7b7b7; }"
        "QSlider::handle:horizontal:hover { background: #ffffff; }");
}
// A row of evenly-distributed labels beneath a slider.
QWidget *makeLabelRow(const QStringList &texts, QWidget *parent)
{
    auto *row = new QWidget(parent);
    auto *lay = new QHBoxLayout(row);
    lay->setContentsMargins(4, 0, 4, 0);
    lay->setSpacing(0);
    for (int i = 0; i < texts.size(); ++i) {
        auto *label = new QLabel(texts.at(i), row);
        label->setStyleSheet(QStringLiteral("color: #8f9298; font-size: 8pt;"));
        Qt::Alignment align = Qt::AlignHCenter;
        if (i == 0) {
            align = Qt::AlignLeft;
        } else if (i == texts.size() - 1) {
            align = Qt::AlignRight;
        }
        label->setAlignment(align | Qt::AlignVCenter);
        lay->addWidget(label, 1);
    }
    return row;
}
} // namespace

void DeviceDetailView::buildViolationsPage(QWidget *page)
{
    auto *lay = new QVBoxLayout(page);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    // Time axis (dates/times), aligned to the chart grid (leftMargin 210,
    // rightMargin 25), with the shift-left/right buttons at its ends.
    auto *axisRow = new QWidget(page);
    auto *axisLay = new QHBoxLayout(axisRow);
    axisLay->setContentsMargins(kViolGridLeft - 18, 0, kViolGridRight - 18, 0);
    axisLay->setSpacing(0);
    auto *shiftLeft = new QToolButton(axisRow);
    shiftLeft->setText(QStringLiteral("‹"));
    shiftLeft->setFixedWidth(18);
    shiftLeft->setStyleSheet(QStringLiteral(
        "QToolButton { color: #b7b7b7; background: transparent; border: none; font-size: 14pt; }"
        "QToolButton:hover { color: white; }"));
    axisLay->addWidget(shiftLeft);
    m_violAxis = new TimeAxisWidget(axisRow);
    m_violAxis->setFixedHeight(30);
    axisLay->addWidget(m_violAxis, 1);
    auto *shiftRight = new QToolButton(axisRow);
    m_violShiftRight = shiftRight;
    shiftRight->setText(QStringLiteral("›"));
    shiftRight->setFixedWidth(18);
    shiftRight->setStyleSheet(shiftLeft->styleSheet());
    axisLay->addWidget(shiftRight);
    axisRow->setFixedHeight(30);
    lay->addWidget(axisRow);

    // The Activity/Efficiency chart, filling the rest.
    m_violChart = new HistoryChartWidget(page);
    m_violChart->setFillMode(true);
    m_violChart->setGridLeft(kViolGridLeft);
    lay->addWidget(m_violChart, 1);
    m_violChartsModel = new HistoryChartsModel(m_connection, m_violChart, this);

    // Range + Step selectors at the bottom (Control.qml).
    auto *controls = new QWidget(page);
    auto *cLay = new QHBoxLayout(controls);
    cLay->setContentsMargins(kViolGridLeft, 6, kViolGridRight, 10);
    cLay->setSpacing(30);

    // Range block.
    auto *rangeBlock = new QWidget(controls);
    auto *rangeOuter = new QHBoxLayout(rangeBlock);
    rangeOuter->setContentsMargins(0, 0, 0, 0);
    rangeOuter->setSpacing(8);
    auto *rangeTitle = new QLabel(QStringLiteral("Range"), rangeBlock);
    rangeTitle->setStyleSheet(QStringLiteral("color: white; font-size: 9pt;"));
    rangeOuter->addWidget(rangeTitle, 0, Qt::AlignVCenter);
    auto *rangeCol = new QWidget(rangeBlock);
    auto *rangeColLay = new QVBoxLayout(rangeCol);
    rangeColLay->setContentsMargins(0, 0, 0, 0);
    rangeColLay->setSpacing(2);
    m_violRangeSlider = new QSlider(Qt::Horizontal, rangeCol);
    m_violRangeSlider->setStyleSheet(violSliderQss());
    m_violRangeSlider->setRange(0, kViolRangeCount - 1);
    m_violRangeSlider->setValue(m_violRangeIndex);
    m_violRangeSlider->setPageStep(1);
    rangeColLay->addWidget(m_violRangeSlider);
    QStringList rangeTexts;
    for (const char *t : kViolRangeText) {
        rangeTexts << QString::fromLatin1(t);
    }
    rangeColLay->addWidget(makeLabelRow(rangeTexts, rangeCol));
    rangeOuter->addWidget(rangeCol, 1);
    cLay->addWidget(rangeBlock, 3);

    // Step block.
    auto *stepBlock = new QWidget(controls);
    auto *stepOuter = new QHBoxLayout(stepBlock);
    stepOuter->setContentsMargins(0, 0, 0, 0);
    stepOuter->setSpacing(8);
    auto *stepTitle = new QLabel(QStringLiteral("Step"), stepBlock);
    stepTitle->setStyleSheet(QStringLiteral("color: white; font-size: 9pt;"));
    stepOuter->addWidget(stepTitle, 0, Qt::AlignVCenter);
    auto *stepCol = new QWidget(stepBlock);
    auto *stepColLay = new QVBoxLayout(stepCol);
    stepColLay->setContentsMargins(0, 0, 0, 0);
    stepColLay->setSpacing(2);
    m_violStepSlider = new QSlider(Qt::Horizontal, stepCol);
    m_violStepSlider->setStyleSheet(violSliderQss());
    m_violStepSlider->setPageStep(1);
    stepColLay->addWidget(m_violStepSlider);
    m_violStepLabels = new QWidget(stepCol);
    new QHBoxLayout(m_violStepLabels);
    stepColLay->addWidget(m_violStepLabels);
    stepOuter->addWidget(stepCol, 1);
    cLay->addWidget(stepBlock, 2);

    lay->addWidget(controls);

    // ChartsModel.qml onChartTimeRangeChanged: back to the live window, the
    // step re-picked for the new range.
    connect(m_violRangeSlider, &QSlider::valueChanged, this, [this](int value) {
        m_violRangeIndex = qBound(0, value, kViolRangeCount - 1);
        m_violLive = true;
        applyViolationsStepForRange();
        reloadViolationsData();
    });
    connect(m_violStepSlider, &QSlider::valueChanged, this, [this](int value) {
        const QList<int> &allowed = kViolRangeSteps[m_violRangeIndex];
        if (value >= 0 && value < allowed.size()) {
            setViolationsStepIndex(allowed.at(value));
            saveViolationsState();
            reloadViolationsData();
        }
    });
    // chartTimeWantLeft / chartTimeWantRight: one range back / forward (not
    // past now), leaving the live window.
    connect(shiftLeft, &QToolButton::clicked, this, [this] {
        m_violLive = false;
        m_violBeginMs -= kViolRangeSec[m_violRangeIndex] * 1000;
        reloadViolationsData();
    });
    connect(shiftRight, &QToolButton::clicked, this, [this] {
        m_violLive = false;
        m_violBeginMs = qMin(m_violBeginMs + kViolRangeSec[m_violRangeIndex] * 1000,
                             QDateTime::currentMSecsSinceEpoch());
        reloadViolationsData();
    });
    // chartTimeAutoMoveCounter: the live window follows now every second.
    m_violAutoMoveTimer = new QTimer(this);
    m_violAutoMoveTimer->setInterval(1000);
    connect(m_violAutoMoveTimer, &QTimer::timeout, this, [this] {
        if (m_leftStack->currentWidget() == m_violationsPage) {
            reloadViolationsData();
        }
    });
    m_violAutoMoveTimer->start();

    // TrackerQuadratorActiveCell.load(): chartTimeRangeIndex (the step
    // picked for it), then chartTimeStepIndex -- both "|| 0".
    const QSettings settings;
    m_violRangeIndex = qBound(0, settings.value(QStringLiteral("activeCellState/chartTimeRangeIndex"), 0).toInt(),
                              kViolRangeCount - 1);
    const int savedStep = settings.value(QStringLiteral("activeCellState/chartTimeStepIndex"), 0).toInt();
    {
        const QSignalBlocker blocker(m_violRangeSlider);
        m_violRangeSlider->setValue(m_violRangeIndex);
    }
    applyViolationsStepForRange();
    const QList<int> &allowed = kViolRangeSteps[m_violRangeIndex];
    const int stepPosition = qBound(0, savedStep, int(allowed.size()) - 1);
    setViolationsStepIndex(allowed.at(stepPosition));
    {
        const QSignalBlocker blocker(m_violStepSlider);
        m_violStepSlider->setValue(stepPosition);
    }
    saveViolationsState();
}

void DeviceDetailView::setViolationsStepIndex(int stepIndex)
{
    // ChartsModel.qml onChartTimeStepChanged: the marks the chosen step
    // gives become the target the next range change aims for.
    if (stepIndex == m_violStepIndex) {
        return;
    }
    m_violStepIndex = stepIndex;
    m_violLive = true;
    m_violLastMarks = static_cast<double>(kViolRangeSec[m_violRangeIndex]) / kViolStepSec[stepIndex];
}

void DeviceDetailView::saveViolationsState() const
{
    QSettings settings;
    settings.setValue(QStringLiteral("activeCellState/chartTimeRangeIndex"), m_violRangeIndex);
    settings.setValue(QStringLiteral("activeCellState/chartTimeStepIndex"),
                      kViolRangeSteps[m_violRangeIndex].indexOf(m_violStepIndex));
}

void DeviceDetailView::applyViolationsStepForRange()
{
    if (!m_violStepSlider || !m_violStepLabels) {
        return;
    }
    // ChartsModel.qml updateChartTimeStep(): the steps timeRangeInit allows
    // for this range, and of them the first whose marks (range / step) are
    // nearest lastChartTimeMarks.
    const QList<int> &allowed = kViolRangeSteps[m_violRangeIndex];
    const double rangeValue = kViolRangeSec[m_violRangeIndex];
    int nearest = -1;
    double nearestDelta = 1e100;
    for (int i = 0; i < allowed.size(); ++i) {
        const double delta = std::abs(m_violLastMarks - rangeValue / kViolStepSec[allowed.at(i)]);
        if (delta < nearestDelta) {
            nearest = i;
            nearestDelta = delta;
        }
    }
    if (nearest >= 0) {
        setViolationsStepIndex(allowed.at(nearest));
    }
    {
        const QSignalBlocker blocker(m_violStepSlider);
        m_violStepSlider->setRange(0, allowed.size() - 1);
        m_violStepSlider->setValue(qMax(0, int(allowed.indexOf(m_violStepIndex))));
    }
    // Rebuild the step labels for the allowed set.
    auto *oldLay = m_violStepLabels->layout();
    if (oldLay) {
        QLayoutItem *item = nullptr;
        while ((item = oldLay->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLay;
    }
    QStringList texts;
    for (int idx : allowed) {
        texts << QString::fromLatin1(kViolStepText[idx]);
    }
    auto *built = makeLabelRow(texts, m_violStepLabels);
    auto *host = new QVBoxLayout(m_violStepLabels);
    host->setContentsMargins(0, 0, 0, 0);
    host->addWidget(built);
    saveViolationsState();
}

void DeviceDetailView::reloadViolationsData()
{
    if (!m_violChart || m_violStepIndex < 0) {
        return;
    }
    const qint64 rangeMs = kViolRangeSec[m_violRangeIndex] * 1000;
    const qint64 stepMs = kViolStepSec[m_violStepIndex] * 1000;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    // chartTimeBeginAmortisseur: live, the window ends at now floored to the
    // step in local time (now - ((now - tzOffset) % step)).
    if (m_violLive || m_violBeginMs == 0) {
        const qint64 tzOffMs = QDateTime::currentDateTime().offsetFromUtc() * 1000LL;
        const qint64 alignedNow = now - ((now + tzOffMs) % stepMs);
        m_violBeginMs = alignedNow - rangeMs;
    }
    const qint64 beginMs = m_violBeginMs;
    const qint64 endMs = beginMs + rangeMs;
    m_violChart->setRange(beginMs, endMs);
    m_violChart->setChartStepMs(stepMs);
    m_violAxis->setRange(beginMs, endMs);
    m_violAxis->setStepMs(stepMs);
    // Filters.qml: the right shift is enabled while now > timeEnd + timeStep.
    if (m_violShiftRight) {
        m_violShiftRight->setEnabled(now > endMs + stepMs);
    }
    if (m_primaryStreamId != 0) {
        m_violChartsModel->setQuery(m_primaryStreamId, beginMs, endMs, stepMs);
    }
}

void DeviceDetailView::showDevice(quint32 sessionKey, const QString &displayName,
                                  quint32 primaryStreamId, const QList<MonitorWidget *> &monitors,
                                  const QList<MonitorWidget *> &windowPreviews)
{
    m_sessionKey = sessionKey;
    m_primaryStreamId = primaryStreamId;
    m_nameLabel->setText(QFontMetrics(m_nameLabel->font()).elidedText(displayName, Qt::ElideRight, 500));
    layoutHeader();

    setMonitors(monitors);
    refreshWindowPreviews(windowPreviews);

    m_statusUser->hide();
    m_displayName = displayName;
    m_keylogTicker->clear();
    m_keyloggerPage->setSession(sessionKey, primaryStreamId, displayName, m_identity);
    m_lastPrograms.clear();
    m_lastWebPages.clear();
    m_scopes = CategoryScopes();
    m_activeApplication.clear();
    updateInfoPanel();
    // Reset the Violations chart to a fresh "now"-anchored window.
    m_violLive = true;
    m_violBeginMs = 0;
    if (m_violChartsModel) {
        m_violChartsModel->clear();
    }
    m_tabSwitch->setCurrentIndex(1);
    switchSubTab(1);
    refreshStats();
}

void DeviceDetailView::refreshWindowPreviews(const QList<MonitorWidget *> &windowPreviews)
{
    m_currentWindowPreviews = windowPreviews;
    m_windowsColumn->setMonitors(windowPreviews);
    updateMonitorStatusIcon();
    emit streamsChanged();
}

void DeviceDetailView::setMonitors(const QList<MonitorWidget *> &monitors)
{
    m_currentMonitors = monitors;
    m_monitorColumn->setMonitors(monitors);
    updateMonitorStatusIcon();
    emit streamsChanged();
}

void DeviceDetailView::setNodeOnline(bool online)
{
    if (m_nodeOnline != online) {
        m_nodeOnline = online;
        updateMonitorStatusIcon();
        emit streamsChanged();
    }
}

void DeviceDetailView::setActivityState(bool locked, bool saver)
{
    if (m_locked != locked || m_saver != saver) {
        m_locked = locked;
        m_saver = saver;
        updateMonitorStatusIcon();
    }
}

void DeviceDetailView::setNoData(bool noData)
{
    if (m_noData != noData) {
        m_noData = noData;
        updateMonitorStatusIcon();
    }
}

QString DeviceDetailView::cellStatusKind() const
{
    // The cell's StatusIcon.kind (same order as DeviceTileWidget's).
    if (!m_nodeOnline) {
        return QStringLiteral("nodeOffline");
    }
    if (m_sessionState == QStringLiteral("disconnected")
        || (m_currentMonitors.isEmpty() && m_sessionState.isEmpty())) {
        return QStringLiteral("offline");
    }
    if (m_sessionState == QStringLiteral("idle") || m_sessionState == QStringLiteral("other")
        || m_currentMonitors.isEmpty()) {
        return QStringLiteral("noSessions");
    }
    if (m_noData) {
        return QStringLiteral("noData");
    }
    if (m_locked) {
        return QStringLiteral("lock");
    }
    if (m_saver) {
        return QStringLiteral("saver");
    }
    return QString();
}

bool DeviceDetailView::showsVideo() const
{
    // Windows.qml's instantStatusIcon is active for every kind but null /
    // saver / lock / noData, or when the selector has no stream at all.
    const QString kind = cellStatusKind();
    const bool kindHidesVideo = !kind.isEmpty() && kind != QStringLiteral("saver")
                             && kind != QStringLiteral("lock") && kind != QStringLiteral("noData");
    const bool programs = m_leftStack->currentWidget() == m_programsPage;
    const bool hasStreams = programs ? !m_currentWindowPreviews.isEmpty() : !m_currentMonitors.isEmpty();
    return !kindHidesVideo && hasStreams;
}

QList<quint32> DeviceDetailView::wantedStreams() const
{
    // windows/video/Monitor.qml: fps 2 for every stream in the column.
    if (!isVisible() || !showsVideo()) {
        return {};
    }
    QList<quint32> streams;
    const bool programs = m_leftStack->currentWidget() == m_programsPage;
    if (!programs && m_leftStack->currentWidget() != m_monitorsPage) {
        return {};
    }
    for (MonitorWidget *monitor : programs ? m_currentWindowPreviews : m_currentMonitors) {
        streams.append(monitor->streamId());
    }
    return streams;
}

void DeviceDetailView::setDisplayName(const QString &displayName)
{
    m_nameLabel->setText(QFontMetrics(m_nameLabel->font()).elidedText(displayName, Qt::ElideRight, 500));
    layoutHeader();
}

void DeviceDetailView::setSessionState(const QString &state)
{
    m_sessionState = state;
    updateMonitorStatusIcon();
}

void DeviceDetailView::updateMonitorStatusIcon()
{
    if (!m_monitorInnerStack || !m_programsInnerStack) {
        return;
    }
    // The icon shows the cell's kind, or "No video" (emptyStream) when the
    // kind is null but there is no stream to show.
    QString kind = cellStatusKind();
    const QString shown = kind.isEmpty() ? QStringLiteral("emptyStream") : kind;
    const bool hideKind = !kind.isEmpty() && kind != QStringLiteral("saver") && kind != QStringLiteral("lock")
                       && kind != QStringLiteral("noData");
    const bool monitorsIcon = hideKind || m_currentMonitors.isEmpty();
    const bool programsIcon = hideKind || m_currentWindowPreviews.isEmpty();
    m_monitorStatusIcon->setKind(shown);
    m_programsStatusIcon->setKind(shown);
    m_monitorInnerStack->setCurrentIndex(monitorsIcon ? 1 : 0);
    m_programsInnerStack->setCurrentIndex(programsIcon ? 1 : 0);
    updateVideoGeometry();
}

void DeviceDetailView::updateVideoGeometry()
{
    if (!m_leftColumn || !m_leftStack) {
        return;
    }
    QWidget *page = m_leftStack->currentWidget();
    const bool videoTab = page == m_monitorsPage || page == m_programsPage;
    auto *layout = static_cast<QVBoxLayout *>(m_leftColumn->layout());
    const bool video = videoTab && showsVideo();
    // keylogger: visible with the video only.
    m_keylogTicker->setVisible(video);
    if (!video) {
        // Other tabs, and the StatusIcon (anchors.fill contentVideo), take
        // the whole column.
        m_leftStack->setMinimumHeight(0);
        m_leftStack->setMaximumHeight(QWIDGETSIZE_MAX);
        layout->setStretch(0, 1);
        layout->setStretch(2, 0);
        return;
    }
    // Layout.preferredHeight: min(contentHeight, contentVideo.height -
    // keylogger.height) -- the keylogger line sits right under the video.
    const MonitorColumn *column = page == m_programsPage ? m_windowsColumn : m_monitorColumn;
    const int available = m_leftColumn->height() - m_keylogTicker->height() - layout->spacing();
    m_leftStack->setFixedHeight(qMax(0, qMin(column->contentHeight(), available)));
    layout->setStretch(0, 0);
    layout->setStretch(2, 1);
}

void DeviceDetailView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_rightColumn->setFixedWidth(qMax(200, width() / 3 - 85));
    layoutHeader();
    updateVideoGeometry();
}

void DeviceDetailView::layoutHeader()
{
    // TabView.qml:185-259: topLine is a RowLayout 17px from the top, as tall
    // as its tallest item, whose items are vertically centered with their
    // own offsets: Back -4.5 (-4 when the row height is odd), caption -4,
    // MultiSwitch -1. 27px, Back, 20px, caption (<= 500 wide), 20px,
    // MultiSwitch, 20px, headerZone (fills the rest, full row height), with
    // RowLayout's default spacing of 5 between every two of those items. The
    // content starts 7px under the row (:265-267).
    constexpr int kTop = 17;
    constexpr int kSpacing = 5;
    const QFontMetrics captionMetrics(m_nameLabel->font());
    const int captionWidth = qMin(500, captionMetrics.horizontalAdvance(m_nameLabel->text()));
    const QSize back = m_backButton->size();
    const QSize tabs = m_tabSwitch->sizeHint();
    const int captionHeight = captionMetrics.height();
    const int rowHeight = qMax(back.height(), qMax(captionHeight, tabs.height()));
    const qreal center = kTop + rowHeight / 2.0;
    const qreal backOffset = rowHeight % 2 == 0 ? -4.5 : -4.0;
    int x = 27 + kSpacing;
    m_backButton->move(x, snapped(center - back.height() / 2.0 + backOffset));
    x += back.width() + kSpacing + 20 + kSpacing;
    m_nameLabel->setGeometry(x, snapped(center - captionHeight / 2.0 - 4), captionWidth, captionHeight);
    x += captionWidth + kSpacing + 20 + kSpacing;
    m_tabSwitch->setGeometry(x, snapped(center - tabs.height() / 2.0 - 1), tabs.width(), tabs.height());
    m_headerRow->setFixedHeight(kTop + rowHeight + 7);
    // Windows.qml:31-39 StatusUser in headerZone: top -2, right margin
    // sessionInfo.width - width + 30, i.e. its left edge sits 30px left of
    // the SessionInfo column's width from the right.
    if (m_statusUser) {
        const int statusX = width() - m_rightColumn->width() - 30;
        m_statusUser->move(qMax(x + tabs.width() + kSpacing + 20 + kSpacing, statusX), kTop - 2);
        m_statusUser->raise();
    }
}

void DeviceDetailView::activate()
{
    m_refreshTimer->start();
    refreshStats();
}

void DeviceDetailView::deactivate()
{
    m_refreshTimer->stop();
}

void DeviceDetailView::refreshStats()
{
    if (m_primaryStreamId == 0) {
        return;
    }
    const QString today = QDate::currentDate().toString(QStringLiteral("yyyyMMdd"));
    m_connection.requestRunningApplications(m_primaryStreamId, today);
    m_connection.requestWebPages(m_primaryStreamId, today);
    m_connection.requestCategories(m_primaryStreamId);
}

void DeviceDetailView::updateInfoPanel()
{
    // Today's totals per resource, each resource's share of them; the
    // application currently in front is the active one.
    const auto build = [&](const QList<HistoryAppUsage> &entries, bool matchActive, bool webPages) {
        qint64 totalMs = 0;
        for (const HistoryAppUsage &entry : entries) {
            totalMs += entry.totalMs;
        }
        QList<HistoryInfoPanel::Item> items;
        for (const HistoryAppUsage &entry : entries) {
            // A web page by its site, a program by its executable; the
            // nearest scope with a rule decides.
            const QString resolved = m_scopes.resolve(entry.application, webPages);
            items.append({entry.application,
                          entry.title.isEmpty() ? QStringLiteral("No title") : entry.title,
                          100.0 * entry.totalMs / qMax<qint64>(1, totalMs),
                          resolved.isEmpty() ? QStringLiteral("none") : resolved,
                          matchActive && entry.application == m_activeApplication, webPages});
        }
        std::sort(items.begin(), items.end(), [](const HistoryInfoPanel::Item &a, const HistoryInfoPanel::Item &b) {
            return a.percent > b.percent;
        });
        return items;
    };
    m_infoPanel->setItems(build(m_lastWebPages, false, true), build(m_lastPrograms, true, false));
}

void DeviceDetailView::onCategorizationRequested(const QString &resource, bool webPage)
{
    if (!runCategorizationDialog(this, m_connection, m_primaryStreamId, resource, webPage,
                                 m_nameLabel->text(), m_scopes)) {
        return;
    }
    if (m_violChartsModel) {
        m_violChartsModel->applicationsChanged();
    }
    updateInfoPanel();
}

void DeviceDetailView::setSessionIdentity(const SessionIdentity &identity)
{
    m_identity = identity;
    if (m_sessionKey != 0) {
        m_keyloggerPage->setSession(m_sessionKey, m_primaryStreamId, m_displayName, identity);
    }
}

void DeviceDetailView::setLiveKeyEvents(const QList<KeyEvent> &events)
{
    // Keystream.qml: Translator { input: keyboardEvents } -- the QML path,
    // nothing cut, each event's own key name.
    m_keylogTicker->setText(Keylogger::translate(events, false, false, false));
}

bool DeviceDetailView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_nameLabel && event->type() == QEvent::MouseButtonDblClick) {
        emit renameRequested(m_sessionKey, m_nameLabel->text());
        return true;
    }
    if (watched == m_leftColumn && event->type() == QEvent::Resize) {
        updateVideoGeometry();
    }
    return QWidget::eventFilter(watched, event);
}


// MonitorColumn declares Q_OBJECT in this .cpp, so AUTOMOC needs this.
#include "devicedetailview.moc"
