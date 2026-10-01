#include "devicedetailview.h"

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
#include <utility>

namespace {

QPixmap acAsset(const QString &path)
{
    return QPixmap(QStringLiteral(":/activeCell/") + path);
}

// A horizontal 3-slice of a BorderImage: fixed-width left/right caps, the
// middle stretched -- used for the button and pick-bar backgrounds, whose
// QML borders are horizontal only (height is left at the source height).
void draw3Slice(QPainter &painter, const QRect &target, const QPixmap &pixmap, int left, int right)
{
    if (pixmap.isNull() || target.width() <= 0) {
        return;
    }
    const int h = pixmap.height();
    left = qMin(left, target.width() / 2);
    right = qMin(right, target.width() - left);
    painter.drawPixmap(QRect(target.left(), target.top(), left, target.height()), pixmap,
                       QRect(0, 0, left, h));
    painter.drawPixmap(QRect(target.right() - right + 1, target.top(), right, target.height()),
                       pixmap, QRect(pixmap.width() - right, 0, right, h));
    painter.drawPixmap(QRect(target.left() + left, target.top(), target.width() - left - right,
                             target.height()),
                       pixmap, QRect(left, 0, pixmap.width() - left - right, h));
}

QFont robotoBold(int px)
{
    QFont f(QStringLiteral("Roboto"));
    f.setPixelSize(px);
    f.setBold(true);
    return f;
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
        setFixedHeight(23);
        m_normal = acAsset(QStringLiteral("button/normal.png"));
        m_hover = acAsset(QStringLiteral("button/hover.png"));
        m_press = acAsset(QStringLiteral("button/press.png"));
        m_arrow = acAsset(QStringLiteral("arrow.png"));
        m_font = robotoBold(12);
        const int textW = QFontMetrics(m_font).horizontalAdvance(tr("Back"));
        setFixedWidth(m_arrow.width() + 6 + textW + 24);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        const QPixmap &bg = isDown() ? m_press : (m_hovered ? m_hover : m_normal);
        draw3Slice(painter, rect(), bg, 10, 10);
        const int textW = QFontMetrics(m_font).horizontalAdvance(tr("Back"));
        const int content = m_arrow.width() + 6 + textW;
        const int x = (width() - content) / 2;
        painter.drawPixmap(x, (height() - m_arrow.height()) / 2, m_arrow);
        painter.setFont(m_font);
        painter.setPen(QColor(0xf7, 0xf7, 0xf7));
        painter.drawText(QRect(x + m_arrow.width() + 6, 0, textW + 2, height()),
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

// Widgets/TabView.qml's MultiSwitch: a thin bar (multiSwitch/bg.png, border 4)
// with the pick handle (pick_normal.png) centered over the current tab, and a
// row of icon+label tabs below it. Active/hovered tab shows the "_on" icon and
// white text; others the "_off" icon and #b7b7b7 text (MultiSwitch.qml).
class DetailTabSwitch final : public QWidget
{
    Q_OBJECT

public:
    explicit DetailTabSwitch(QWidget *parent) : QWidget(parent)
    {
        m_bar = acAsset(QStringLiteral("multiSwitch/bg.png"));
        m_pick = acAsset(QStringLiteral("multiSwitch/pick_normal.png"));
        m_font = QFont(QStringLiteral("Roboto"));
        m_font.setPixelSize(11);
        setMouseTracking(true);
        setCursor(Qt::PointingHandCursor);
    }

    void addTab(const QString &text, const QString &onIcon, const QString &offIcon,
                bool selectable = true)
    {
        m_tabs.append({text, acAsset(onIcon), acAsset(offIcon), selectable});
        updateGeometry();
        update();
    }
    int currentIndex() const { return m_current; }
    void setCurrentIndex(int index)
    {
        if (index < 0 || index >= m_tabs.size() || index == m_current) {
            return;
        }
        m_current = index;
        update();
    }

signals:
    void currentChanged(int index);

protected:
    QSize sizeHint() const override { return QSize(totalWidth(), 34); }
    QSize minimumSizeHint() const override { return sizeHint(); }

    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        const QList<int> edges = tabEdges();
        // The pick bar, vertically centered on the 19px pick handle.
        draw3Slice(painter, QRect(0, m_pick.height() / 2 - 2, width(), 4), m_bar, 4, 4);
        if (m_current >= 0 && m_current + 1 < edges.size()) {
            const int cx = (edges[m_current] + edges[m_current + 1]) / 2;
            painter.drawPixmap(cx - m_pick.width() / 2, 0, m_pick);
        }
        painter.setFont(m_font);
        const QFontMetrics metrics(m_font);
        const int rowTop = m_pick.height();
        const int rowHeight = height() - rowTop;
        for (int i = 0; i < m_tabs.size(); ++i) {
            const Tab &tab = m_tabs.at(i);
            const bool lit = i == m_current || i == m_hover;
            const QPixmap &icon = lit ? tab.on : tab.off;
            const int textW = metrics.horizontalAdvance(tab.text);
            const int content = icon.width() + 5 + textW;
            const int startX = edges[i] + (edges[i + 1] - edges[i] - content) / 2;
            painter.drawPixmap(startX, rowTop + (rowHeight - icon.height()) / 2, icon);
            painter.setPen(lit ? QColor(Qt::white) : QColor(0xb7, 0xb7, 0xb7));
            painter.drawText(QRect(startX + icon.width() + 5, rowTop, textW + 2, rowHeight),
                             Qt::AlignLeft | Qt::AlignVCenter, tab.text);
        }
    }

    void mousePressEvent(QMouseEvent *event) override
    {
        const int index = tabAt(event->pos().x());
        if (index >= 0 && m_tabs.at(index).selectable && index != m_current) {
            m_current = index;
            update();
            emit currentChanged(index);
        }
    }
    void mouseMoveEvent(QMouseEvent *event) override
    {
        const int index = tabAt(event->pos().x());
        if (index != m_hover) {
            m_hover = index;
            update();
        }
    }
    void leaveEvent(QEvent *) override
    {
        if (m_hover != -1) {
            m_hover = -1;
            update();
        }
    }

private:
    struct Tab {
        QString text;
        QPixmap on;
        QPixmap off;
        bool selectable = true;
    };
    int tabWidth(const Tab &tab) const
    {
        return QFontMetrics(m_font).horizontalAdvance(tab.text)
            + (tab.on.isNull() ? 0 : tab.on.width() + 5) + 16;
    }
    int totalWidth() const
    {
        int width = 0;
        for (const Tab &tab : m_tabs) {
            width += tabWidth(tab);
        }
        return width;
    }
    QList<int> tabEdges() const
    {
        QList<int> edges;
        int x = 0;
        for (const Tab &tab : m_tabs) {
            edges.append(x);
            x += tabWidth(tab);
        }
        edges.append(x);
        return edges;
    }
    int tabAt(int x) const
    {
        const QList<int> edges = tabEdges();
        for (int i = 0; i + 1 < edges.size(); ++i) {
            if (x >= edges[i] && x < edges[i + 1]) {
                return i;
            }
        }
        return -1;
    }

    QList<Tab> m_tabs;
    int m_current = 0;
    int m_hover = -1;
    QPixmap m_bar;
    QPixmap m_pick;
    QFont m_font;
};

// StatusUser.qml: info_bg.png BorderImage (border 5) with the warning icon and
// "Not active: HH:MM:SS" (or "Active"), width = content + 20, left margin 10.
class StatusUserBox final : public QWidget
{
public:
    explicit StatusUserBox(QWidget *parent) : QWidget(parent)
    {
        m_bg = acAsset(QStringLiteral("StatusUser/info_bg.png"));
        m_notActive = acAsset(QStringLiteral("StatusUser/info_not_active.png"));
        m_active = acAsset(QStringLiteral("StatusUser/info_active.png"));
        m_font = QFont(QStringLiteral("Roboto"));
        m_font.setPixelSize(12);
        setFixedHeight(28);
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
        update();
        show();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        draw3Slice(painter, rect(), m_bg, 5, 5);
        const QPixmap &icon = m_active_ ? m_active : m_notActive;
        painter.drawPixmap(10, (height() - icon.height()) / 2, icon);
        painter.setFont(m_font);
        painter.setPen(Qt::white);
        painter.drawText(QRect(10 + icon.width() + 10, 0, width(), height()),
                         Qt::AlignLeft | Qt::AlignVCenter, m_text);
    }

private:
    QPixmap m_bg, m_notActive, m_active;
    QString m_text;
    QFont m_font;
    bool m_active_ = true;
};

// utils/StatusIcon.qml, typeSize "big": #45464d background (Video.qml),
// tracker/statusIcon/big_<kind>.png centered with a 10px gap, then the
// kind's text (18pt bold #38373d, word-wrapped). Shown instead of the video
// whenever there's nothing live to display (Windows.qml's instantStatusIcon).
class BigStatusIcon final : public QWidget
{
public:
    explicit BigStatusIcon(QWidget *parent) : QWidget(parent) {}

    void setKind(const QString &kind)
    {
        if (m_kind == kind) {
            return;
        }
        m_kind = kind;
        m_icon = kind.isEmpty() ? QPixmap()
                                : QPixmap(QStringLiteral(":/tracker/statusIcon/big_%1.png").arg(kind));
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(0x45, 0x46, 0x4d));
        if (m_icon.isNull()) {
            return;
        }
        const QString text = kindText(m_kind);
        QFont font(QStringLiteral("Roboto"));
        font.setPixelSize(24);
        font.setBold(true);
        const QRect textArea(20, 0, width() - 40, 1000);
        const QRect textBounds =
            text.isEmpty() ? QRect() : QFontMetrics(font).boundingRect(textArea, Qt::TextWordWrap, text);
        const int totalHeight = m_icon.height() + (text.isEmpty() ? 0 : 10 + textBounds.height());
        int y = (height() - totalHeight) / 2;
        painter.drawPixmap((width() - m_icon.width()) / 2, y, m_icon);
        y += m_icon.height() + 10;
        if (!text.isEmpty()) {
            painter.setFont(font);
            painter.setPen(QColor(0x38, 0x37, 0x3d));
            painter.drawText(QRect(20, y, width() - 40, textBounds.height()),
                             Qt::AlignHCenter | Qt::TextWordWrap, text);
        }
    }

private:
    // utils/StatusIcon.qml's text switch, for the kinds this page can show.
    static QString kindText(const QString &kind)
    {
        if (kind == QStringLiteral("offline")) {
            return QStringLiteral("Offline");
        }
        if (kind == QStringLiteral("noSessions")) {
            return QStringLiteral("No session");
        }
        if (kind == QStringLiteral("emptyStream")) {
            return QStringLiteral("No video");
        }
        return QString();
    }

    QString m_kind;
    QPixmap m_icon;
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
        "QScrollBar::handle:vertical { background: #57585f; border-radius: 2px; min-height: 30px; }"
        "QScrollBar::handle:vertical:hover { background: #5f6067; }"
        "QScrollBar:horizontal { background: transparent; height: 11px; margin: 3px 0 3px 0; }"
        "QScrollBar::handle:horizontal { background: #57585f; border-radius: 2px; min-width: 30px; }"
        "QScrollBar::handle:horizontal:hover { background: #5f6067; }"
        "QScrollBar::add-line, QScrollBar::sub-line { width: 0; height: 0; background: none; border: none; }"
        "QScrollBar::add-page, QScrollBar::sub-page { background: none; }"));

    // TabView.qml topLine: 27px, Back, 20px, caption, 20px, MultiSwitch,
    // 20px, headerZone (fills to the right); topMargin 17.
    auto *headerRow = new QWidget(this);
    headerRow->setObjectName(QStringLiteral("detailHeader"));
    auto *headerLayout = new QHBoxLayout(headerRow);
    headerLayout->setContentsMargins(27, 17, 20, 7);
    headerLayout->setSpacing(0);
    auto *backButton = new BackButton(headerRow);
    connect(backButton, &QPushButton::clicked, this, &DeviceDetailView::backRequested);
    headerLayout->addWidget(backButton, 0, Qt::AlignVCenter);
    headerLayout->addSpacing(20);
    // caption: Fonts.rb_big_b (Roboto Bold ~11pt), white.
    m_nameLabel = new QLabel(headerRow);
    m_nameLabel->setFont(robotoBold(15));
    m_nameLabel->setStyleSheet(QStringLiteral("color: white;"));
    m_nameLabel->setToolTip(QStringLiteral("Dublu-click pentru a redenumi (doar local, in Viewer)"));
    m_nameLabel->installEventFilter(this); // catches the double-click, see eventFilter()
    headerLayout->addWidget(m_nameLabel, 0, Qt::AlignVCenter);
    headerLayout->addSpacing(20);

    m_tabSwitch = new DetailTabSwitch(headerRow);
    headerLayout->addWidget(m_tabSwitch, 0, Qt::AlignVCenter);
    headerLayout->addStretch();
    root->addWidget(headerRow);

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
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    m_monitorContainer = new QWidget(scroll);
    m_monitorLayout = new QVBoxLayout(m_monitorContainer);
    m_monitorLayout->setContentsMargins(6, 6, 6, 6);
    m_monitorLayout->setSpacing(6);
    scroll->setWidget(m_monitorContainer);
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
    windowsScroll->setWidgetResizable(true);
    windowsScroll->setFrameShape(QFrame::NoFrame);
    m_windowsContainer = new QWidget(windowsScroll);
    m_windowsLayout = new QVBoxLayout(m_windowsContainer);
    m_windowsLayout->setContentsMargins(6, 6, 6, 6);
    m_windowsLayout->setSpacing(6);
    windowsScroll->setWidget(m_windowsContainer);
    m_programsInnerStack->addWidget(windowsScroll);
    m_programsStatusIcon = new BigStatusIcon(m_programsInnerStack);
    m_programsInnerStack->addWidget(m_programsStatusIcon);
    programsPageLayout->addWidget(m_programsInnerStack);
    m_leftStack->addWidget(programsPage);
    m_programsPage = programsPage;

    // keylogger/Toolbar.qml + keylogger/Table.qml equivalent. The real
    // toolbar also has Web pages/Programs/Both scope buttons and an
    // export -- both already exist as-is in HistoryView (this page is the
    // *live, today-only* keylogger, History's is the full historical one),
    // so they're not duplicated here.
    auto *keyloggerPage = new QWidget(m_leftStack);
    m_keyloggerPage = keyloggerPage;
    auto *keyloggerLayout = new QVBoxLayout(keyloggerPage);
    keyloggerLayout->setContentsMargins(10, 10, 10, 10);
    keyloggerLayout->setSpacing(6);

    auto *keyloggerToolbar = new QWidget(keyloggerPage);
    auto *keyloggerToolbarLayout = new QHBoxLayout(keyloggerToolbar);
    keyloggerToolbarLayout->setContentsMargins(0, 0, 0, 0);
    m_keyloggerSearch = new QLineEdit(keyloggerToolbar);
    m_keyloggerSearch->setPlaceholderText(QStringLiteral("Cauta in fereastra sau text..."));
    connect(m_keyloggerSearch, &QLineEdit::textChanged, this, &DeviceDetailView::filterKeyloggerTable);
    keyloggerToolbarLayout->addWidget(m_keyloggerSearch, 1);
    auto *hideSystemKeysCheck = new QCheckBox(QStringLiteral("Hide system keys"), keyloggerToolbar);
    // Inert: KikiHost's keylogger doesn't tag which characters are
    // control/system keys vs. printable text (see keylogger.cpp) -- there's
    // nothing to filter by yet, so this stays visible but has no effect
    // until that tagging exists.
    hideSystemKeysCheck->setEnabled(false);
    hideSystemKeysCheck->setToolTip(
        QStringLiteral("Grabber-ul nu marcheaza inca ce taste sunt \"de sistem\"."));
    keyloggerToolbarLayout->addWidget(hideSystemKeysCheck);
    keyloggerLayout->addWidget(keyloggerToolbar);

    m_keyloggerTable = new QTableWidget(0, 4, keyloggerPage);
    m_keyloggerTable->setHorizontalHeaderLabels(
        {QStringLiteral("Date"), QStringLiteral("Pressing period"), QStringLiteral("Window"),
         QStringLiteral("Keystrokes")});
    m_keyloggerTable->horizontalHeader()->setStretchLastSection(true);
    m_keyloggerTable->verticalHeader()->hide();
    m_keyloggerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_keyloggerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    // keylogger/Table.qml: no fill -- the table sits on the page's grayHatching
    // background, with #43444c decoration lines and a transparent header;
    // cell text is light gray, the header white.
    m_keyloggerTable->setShowGrid(true);
    m_keyloggerTable->setStyleSheet(QStringLiteral(
        "QTableWidget { background: transparent; gridline-color: #43444c; color: #cccccc; }"
        "QTableWidget::item { background: transparent; }"
        "QHeaderView::section { background: transparent; color: white; border: none;"
        " border-bottom: 1px solid #43444c; padding: 4px; }"
        "QTableCornerButton::section { background: transparent; }"));
    keyloggerLayout->addWidget(m_keyloggerTable, 1);
    m_leftStack->addWidget(keyloggerPage);

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
    connect(m_tabSwitch, &DetailTabSwitch::currentChanged, this, &DeviceDetailView::switchSubTab);

    // Windows.qml's Row: 30px, the video (2/3 of the width), 25px,
    // SessionInfo (width/3 - 85, see resizeEvent), 30px.
    auto *content = new QWidget(this);
    auto *contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(30, 0, 30, 0);
    contentLayout->setSpacing(25);

    // Matches the real viewer's Windows.qml: a compact keylog ticker sits
    // directly under the video on both Monitors and Programs (hidden for
    // Keylogger, which already shows the full log) -- not just History.
    auto *leftColumn = new QWidget(content);
    auto *leftColumnLayout = new QVBoxLayout(leftColumn);
    leftColumnLayout->setContentsMargins(0, 0, 0, 0);
    leftColumnLayout->setSpacing(2);
    leftColumnLayout->addWidget(m_leftStack, 1);
    // Windows.qml's keylogger BorderImage: 25px, keylogger_bg.png tiled,
    // right-aligned white text (Fonts.rr_medium).
    m_keylogTicker = new QLabel(leftColumn);
    m_keylogTicker->setFixedHeight(25);
    m_keylogTicker->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_keylogTicker->setContentsMargins(10, 0, 10, 0);
    QFont tickerFont(QStringLiteral("Roboto"));
    tickerFont.setPixelSize(12);
    m_keylogTicker->setFont(tickerFont);
    m_keylogTicker->setStyleSheet(QStringLiteral(
        "color: white; background-image: url(:/activeCell/keylogger_bg.png);"
        " background-repeat: repeat;"));
    leftColumnLayout->addWidget(m_keylogTicker);
    contentLayout->addWidget(leftColumn, 1);

    // Right column: the StatusUser box on top (right-aligned), the
    // SessionInfo panel below -- as Windows.qml stacks them.
    m_rightColumn = new QWidget(content);
    auto *rightLayout = new QVBoxLayout(m_rightColumn);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(6);
    m_statusUser = new StatusUserBox(m_rightColumn);
    rightLayout->addWidget(m_statusUser, 0, Qt::AlignRight);
    m_infoArea = new QScrollArea(m_rightColumn);
    m_infoArea->setFrameShape(QFrame::NoFrame);
    m_infoArea->setWidgetResizable(true);
    m_infoArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_infoArea->setStyleSheet(QStringLiteral("QScrollArea { background: transparent; }"));
    m_infoPanel = new HistoryInfoPanel;
    m_infoArea->setWidget(m_infoPanel);
    connect(m_infoPanel, &HistoryInfoPanel::categorizationRequested, this,
            &DeviceDetailView::onCategorizationRequested);
    rightLayout->addWidget(m_infoArea, 1);
    contentLayout->addWidget(m_rightColumn);
    root->addWidget(content, 1);

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
                m_categories = categories;
                updateInfoPanel();
                updateViolationsChart(); // efficiency colors depend on categories
            });
    connect(&m_connection, &ViewerConnection::historyEmployeeCategoriesReceived, this,
            [this](quint32 streamId, const QHash<QString, QString> &categories) {
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_employeeCategories = categories;
                updateInfoPanel();
                updateViolationsChart(); // efficiency colors depend on categories
            });
    // Violations chart data (Activity histogram + Efficiency bands), stored
    // per day and combined over the current window.
    connect(&m_connection, &ViewerConnection::historyActivityReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryActivitySample> &samples) {
                if (streamId != m_primaryStreamId || !m_violRequestedDays.contains(day)) {
                    return;
                }
                m_violActivityByDay.insert(day, samples);
                updateViolationsChart();
            });
    connect(&m_connection, &ViewerConnection::historyAppSegmentsReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryAppSegment> &segments) {
                if (streamId != m_primaryStreamId || !m_violRequestedDays.contains(day)) {
                    return;
                }
                m_violSegmentsByDay.insert(day, segments);
                updateViolationsChart();
            });
    connect(&m_connection, &ViewerConnection::historyKeystrokesReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryKeystrokeEntry> &entries) {
                Q_UNUSED(day)
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_keystrokeEntries = entries;
                m_keystrokesLoaded = true;
                rebuildKeyloggerTable();
                // utils/Keystream.qml's two distinct messages: "disabled"
                // isn't a state KikiHost reports (there's no per-device
                // keylogger on/off flag in the protocol -- it's always on),
                // so the only real distinction left is "no data yet" vs.
                // "server confirmed nothing today".
                m_keylogTicker->setText(
                    entries.isEmpty() ? QStringLiteral("Keylogger nu a inregistrat nimic azi.")
                                      : entries.last().text.left(200));
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
    m_leftStack->setCurrentWidget(page);
    const bool videoTab = page == m_programsPage || page == m_monitorsPage;
    // The keylog ticker sits under the video on Programs/Monitors only.
    if (m_keylogTicker) {
        m_keylogTicker->setVisible(videoTab);
    }
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
    // A hidden QStackedWidget page's children don't necessarily get a real
    // layout pass (container width can stay stale/zero) until the page is
    // actually made current.
    QTimer::singleShot(0, this, &DeviceDetailView::resizeMonitorsToFit);
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

    connect(m_violRangeSlider, &QSlider::valueChanged, this, [this](int value) {
        m_violRangeIndex = qBound(0, value, kViolRangeCount - 1);
        applyViolationsStepForRange();
        reloadViolationsData();
    });
    connect(m_violStepSlider, &QSlider::valueChanged, this, [this](int value) {
        const QList<int> &allowed = kViolRangeSteps[m_violRangeIndex];
        if (value >= 0 && value < allowed.size()) {
            m_violStepIndex = allowed.at(value);
            reloadViolationsData();
        }
    });
    connect(shiftLeft, &QToolButton::clicked, this, [this] {
        const qint64 end = m_violWindowEndMs > 0 ? m_violWindowEndMs : QDateTime::currentMSecsSinceEpoch();
        m_violWindowEndMs = end - kViolRangeSec[m_violRangeIndex] * 1000;
        reloadViolationsData();
    });
    connect(shiftRight, &QToolButton::clicked, this, [this] {
        if (m_violWindowEndMs <= 0) {
            return; // already at "now"
        }
        m_violWindowEndMs += kViolRangeSec[m_violRangeIndex] * 1000;
        if (m_violWindowEndMs >= QDateTime::currentMSecsSinceEpoch()) {
            m_violWindowEndMs = 0; // back to live "now"
        }
        reloadViolationsData();
    });

    applyViolationsStepForRange();
}

void DeviceDetailView::applyViolationsStepForRange()
{
    if (!m_violStepSlider || !m_violStepLabels) {
        return;
    }
    const QList<int> &allowed = kViolRangeSteps[m_violRangeIndex];
    // Keep the current step if it's valid for this range, else pick the last
    // (coarsest) allowed step.
    int pos = allowed.indexOf(m_violStepIndex);
    if (pos < 0) {
        pos = allowed.size() - 1;
        m_violStepIndex = allowed.at(pos);
    }
    {
        QSignalBlocker blocker(m_violStepSlider);
        m_violStepSlider->setRange(0, allowed.size() - 1);
        m_violStepSlider->setValue(pos);
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
}

void DeviceDetailView::reloadViolationsData()
{
    if (!m_violChart || m_primaryStreamId == 0) {
        return;
    }
    const qint64 rangeMs = kViolRangeSec[m_violRangeIndex] * 1000;
    const qint64 stepMs = kViolStepSec[m_violStepIndex] * 1000;
    qint64 endMs = m_violWindowEndMs > 0 ? m_violWindowEndMs : QDateTime::currentMSecsSinceEpoch();
    // Align the window edge to a step boundary (in local time) so the axis
    // ticks land on round times -- TrackerQuadratorActiveCell.qml floors
    // "now" to the step the same way.
    const qint64 tzOffMs = QDateTime::currentDateTime().offsetFromUtc() * 1000LL;
    endMs = ((endMs + tzOffMs) / stepMs) * stepMs - tzOffMs;
    const qint64 startMs = endMs - rangeMs;
    m_violChart->setRange(startMs, endMs);
    m_violChart->setStepMs(stepMs);
    m_violAxis->setRange(startMs, endMs);
    m_violAxis->setStepMs(stepMs);

    // Query every day the window touches (data is stored per day); keep only
    // those days so stale ones don't linger.
    m_violRequestedDays.clear();
    QDate day = QDateTime::fromMSecsSinceEpoch(startMs).date();
    const QDate lastDay = QDateTime::fromMSecsSinceEpoch(endMs).date();
    for (; day <= lastDay; day = day.addDays(1)) {
        const QString key = day.toString(QStringLiteral("yyyyMMdd"));
        m_violRequestedDays.insert(key);
        m_connection.requestHistoryActivity(m_primaryStreamId, key);
        m_connection.requestHistoryAppSegments(m_primaryStreamId, key);
    }
    updateViolationsChart();
}

void DeviceDetailView::updateViolationsChart()
{
    if (!m_violChart) {
        return;
    }
    QList<HistoryActivitySample> activity;
    QList<HistoryAppSegment> segments;
    for (const QString &day : std::as_const(m_violRequestedDays)) {
        activity += m_violActivityByDay.value(day);
        segments += m_violSegmentsByDay.value(day);
    }
    QHash<QString, QString> categories = m_categories;
    for (auto it = m_employeeCategories.cbegin(); it != m_employeeCategories.cend(); ++it) {
        categories.insert(it.key(), it.value());
    }
    m_violChart->setActivity(activity);
    m_violChart->setEfficiency(segments, categories);
}

void DeviceDetailView::showDevice(quint32 sessionKey, const QString &displayName,
                                  quint32 primaryStreamId, const QList<MonitorWidget *> &monitors,
                                  const QList<MonitorWidget *> &windowPreviews)
{
    m_sessionKey = sessionKey;
    m_primaryStreamId = primaryStreamId;
    m_nameLabel->setText(displayName);

    m_currentMonitors = monitors;
    layoutPreviewWidgets(m_monitorLayout, m_monitorContainer, monitors);
    updateMonitorStatusIcon();

    refreshWindowPreviews(windowPreviews);

    // m_monitorContainer's/m_windowsContainer's width isn't reliable yet
    // here -- this page may not have been shown/laid out for real by
    // QStackedWidget/QScrollArea the first time a device is opened, so
    // width() can still be whatever default/zero size it had at
    // construction. Defer one event-loop turn, by which point MainWindow
    // has already switched to this page and it's been through a real
    // layout pass.
    QTimer::singleShot(0, this, &DeviceDetailView::resizeMonitorsToFit);

    m_statusUser->hide();
    m_keystrokeEntries.clear();
    m_keystrokesLoaded = false;
    rebuildKeyloggerTable();
    m_keylogTicker->setText(QStringLiteral("Se incarca..."));
    m_lastPrograms.clear();
    m_lastWebPages.clear();
    m_employeeCategories.clear();
    m_activeApplication.clear();
    updateInfoPanel();
    // Reset the Violations chart to a fresh "now"-anchored window.
    m_violWindowEndMs = 0;
    m_violActivityByDay.clear();
    m_violSegmentsByDay.clear();
    m_violRequestedDays.clear();
    if (m_violChart) {
        m_violChart->setActivity({});
        m_violChart->setEfficiency({}, {});
    }
    m_tabSwitch->setCurrentIndex(1);
    switchSubTab(1);
    refreshStats();
}

void DeviceDetailView::refreshWindowPreviews(const QList<MonitorWidget *> &windowPreviews)
{
    m_currentWindowPreviews = windowPreviews;
    layoutPreviewWidgets(m_windowsLayout, m_windowsContainer, windowPreviews);
    // Show the big StatusIcon placeholder (same kind mapping as Monitors)
    // when there are no window previews, else the previews scroll.
    if (windowPreviews.isEmpty() && m_programsInnerStack) {
        QString kind = QStringLiteral("emptyStream");
        if (m_sessionState == QStringLiteral("disconnected")) {
            kind = QStringLiteral("offline");
        } else if (m_sessionState == QStringLiteral("idle") || m_sessionState == QStringLiteral("other")) {
            kind = QStringLiteral("noSessions");
        }
        m_programsStatusIcon->setKind(kind);
        m_programsInnerStack->setCurrentIndex(1);
    } else if (m_programsInnerStack) {
        m_programsInnerStack->setCurrentIndex(0);
    }
    resizeMonitorsToFit();
}

void DeviceDetailView::setDisplayName(const QString &displayName)
{
    m_nameLabel->setText(displayName);
}

void DeviceDetailView::setSessionState(const QString &state)
{
    m_sessionState = state;
    updateMonitorStatusIcon();
}

void DeviceDetailView::updateMonitorStatusIcon()
{
    // Same kind mapping DeviceTileWidget::statusKind() uses, minus "lock"
    // (a locked session still has a real, if frozen, video frame to show --
    // Windows.qml only swaps in the big icon for kinds where there's
    // nothing to draw at all).
    QString kind;
    if (m_sessionState == QStringLiteral("disconnected")) {
        kind = QStringLiteral("offline");
    } else if (m_sessionState == QStringLiteral("idle") || m_sessionState == QStringLiteral("other")) {
        kind = QStringLiteral("noSessions");
    } else if (m_currentMonitors.isEmpty()) {
        kind = QStringLiteral("emptyStream");
    }
    if (!m_monitorInnerStack) {
        return;
    }
    if (kind.isEmpty()) {
        m_monitorInnerStack->setCurrentIndex(0);
    } else {
        m_monitorStatusIcon->setKind(kind);
        m_monitorInnerStack->setCurrentIndex(1);
    }
}

void DeviceDetailView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_rightColumn->setFixedWidth(qMax(200, width() / 3 - 85));
    resizeMonitorsToFit();
}

void DeviceDetailView::resizeMonitorsToFit()
{
    resizePreviewWidgetsToFit(m_monitorContainer, m_currentMonitors);
    resizePreviewWidgetsToFit(m_windowsContainer, m_currentWindowPreviews);
}

void DeviceDetailView::layoutPreviewWidgets(QVBoxLayout *layout, QWidget *container,
                                            const QList<MonitorWidget *> &widgets)
{
    QLayoutItem *item = nullptr;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->setParent(nullptr);
        }
        delete item;
    }
    for (MonitorWidget *widget : widgets) {
        widget->setParent(container);
        // MonitorWidget's own sizeHint (335x205) is sized for the small
        // Tracker grid tiles it used to live in; here it's the only thing
        // (or one of a few) in a much wider pane and should look like the
        // reference UI's large screen/window, not a tiny tile with empty
        // space around it. Expanding + a 16:9-ish minimum height (kept in
        // sync by resizePreviewWidgetsToFit()) makes it fill the available
        // width; the QScrollArea it's inside handles several of these
        // stacked together being taller than the viewport.
        widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        widget->show();
        layout->addWidget(widget);
    }
    layout->addStretch();
}

void DeviceDetailView::resizePreviewWidgetsToFit(QWidget *container, const QList<MonitorWidget *> &widgets)
{
    if (widgets.isEmpty() || !container) {
        return;
    }
    const int minHeight = qMax(240, container->width() * 9 / 16);
    for (MonitorWidget *widget : widgets) {
        widget->setMinimumHeight(minHeight);
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
    m_connection.requestKeystrokes(m_primaryStreamId, today);
    m_connection.requestCategories(m_primaryStreamId);
}

void DeviceDetailView::updateInfoPanel()
{
    // Today's totals per resource, each resource's share of them; the
    // application currently in front is the active one.
    QHash<QString, QString> categories = m_categories;
    for (auto it = m_employeeCategories.cbegin(); it != m_employeeCategories.cend(); ++it) {
        categories.insert(it.key(), it.value());
    }
    const auto build = [&](const QList<HistoryAppUsage> &entries, bool matchActive) {
        qint64 totalMs = 0;
        for (const HistoryAppUsage &entry : entries) {
            totalMs += entry.totalMs;
        }
        QList<HistoryInfoPanel::Item> items;
        for (const HistoryAppUsage &entry : entries) {
            items.append({entry.application,
                          entry.title.isEmpty() ? QStringLiteral("No title") : entry.title,
                          100.0 * entry.totalMs / qMax<qint64>(1, totalMs),
                          categories.value(entry.application, entry.category),
                          matchActive && entry.application == m_activeApplication});
        }
        std::sort(items.begin(), items.end(), [](const HistoryInfoPanel::Item &a, const HistoryInfoPanel::Item &b) {
            return a.percent > b.percent;
        });
        return items;
    };
    m_infoPanel->setItems(build(m_lastWebPages, false), build(m_lastPrograms, true));
}

void DeviceDetailView::onCategorizationRequested(const QString &resource)
{
    CategorizationDialog dialog(resource, m_categories.value(resource), m_employeeCategories.value(resource),
                                m_nameLabel->text(), this);
    dialog.move(mapToGlobal(rect().center()) - QPoint(dialog.width() / 2, dialog.height() / 2));
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    if (dialog.category() != m_categories.value(resource, QStringLiteral("none"))) {
        m_categories[resource] = dialog.category();
        m_connection.setAppCategory(m_primaryStreamId, resource, dialog.category());
    }
    if (dialog.employeeCategory() != m_employeeCategories.value(resource, QStringLiteral("none"))) {
        if (dialog.employeeCategory() == QStringLiteral("none")) {
            m_employeeCategories.remove(resource);
        } else {
            m_employeeCategories[resource] = dialog.employeeCategory();
        }
        m_connection.setAppCategory(m_primaryStreamId, resource, dialog.employeeCategory(), true);
    }
    updateInfoPanel();
}

void DeviceDetailView::rebuildKeyloggerTable()
{
    // Groups consecutive keystrokes in the same window into one row (same
    // "Pressing period" grouping HistoryView's keylogger export table
    // uses), a >2 minute gap starts a new row even for the same window.
    m_keyloggerTable->setRowCount(0);
    if (!m_keystrokesLoaded) {
        m_keyloggerTable->setRowCount(1);
        auto *loading = new QTableWidgetItem(QStringLiteral("Se incarca..."));
        loading->setFlags(loading->flags() & ~Qt::ItemIsEditable);
        m_keyloggerTable->setItem(0, 0, loading);
        m_keyloggerTable->setSpan(0, 0, 1, 4);
        return;
    }
    if (m_keystrokeEntries.isEmpty()) {
        m_keyloggerTable->setRowCount(1);
        auto *empty = new QTableWidgetItem(QStringLiteral("Keylogger nu a inregistrat nimic azi."));
        empty->setFlags(empty->flags() & ~Qt::ItemIsEditable);
        m_keyloggerTable->setItem(0, 0, empty);
        m_keyloggerTable->setSpan(0, 0, 1, 4);
        return;
    }

    constexpr qint64 kGroupGapMs = 2 * 60 * 1000;
    struct Group { qint64 startMs; qint64 endMs; QString windowTitle; QString text; };
    QList<Group> groups;
    for (const HistoryKeystrokeEntry &entry : std::as_const(m_keystrokeEntries)) {
        if (!groups.isEmpty() && groups.last().windowTitle == entry.windowTitle
            && entry.timestampMs - groups.last().endMs <= kGroupGapMs) {
            groups.last().endMs = entry.timestampMs;
            groups.last().text += entry.text;
        } else {
            groups.append({entry.timestampMs, entry.timestampMs, entry.windowTitle, entry.text});
        }
    }

    m_keyloggerTable->setRowCount(groups.size());
    for (int row = 0; row < groups.size(); ++row) {
        const Group &group = groups.at(row);
        const QDateTime start = QDateTime::fromMSecsSinceEpoch(group.startMs);
        const QDateTime end = QDateTime::fromMSecsSinceEpoch(group.endMs);
        auto *dateItem = new QTableWidgetItem(start.toString(QStringLiteral("dd.MM.yyyy")));
        auto *periodItem = new QTableWidgetItem(QStringLiteral("%1 - %2").arg(
            start.toString(QStringLiteral("HH:mm:ss")), end.toString(QStringLiteral("HH:mm:ss"))));
        auto *windowItem = new QTableWidgetItem(group.windowTitle);
        auto *textItem = new QTableWidgetItem(group.text);
        for (QTableWidgetItem *item : {dateItem, periodItem, windowItem, textItem}) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
        m_keyloggerTable->setItem(row, 0, dateItem);
        m_keyloggerTable->setItem(row, 1, periodItem);
        m_keyloggerTable->setItem(row, 2, windowItem);
        m_keyloggerTable->setItem(row, 3, textItem);
    }
    filterKeyloggerTable();
}

void DeviceDetailView::filterKeyloggerTable()
{
    const QString needle = m_keyloggerSearch ? m_keyloggerSearch->text().trimmed() : QString();
    for (int row = 0; row < m_keyloggerTable->rowCount(); ++row) {
        if (needle.isEmpty()) {
            m_keyloggerTable->setRowHidden(row, false);
            continue;
        }
        const QTableWidgetItem *windowItem = m_keyloggerTable->item(row, 2);
        const QTableWidgetItem *textItem = m_keyloggerTable->item(row, 3);
        const bool matches =
            (windowItem && windowItem->text().contains(needle, Qt::CaseInsensitive))
            || (textItem && textItem->text().contains(needle, Qt::CaseInsensitive));
        m_keyloggerTable->setRowHidden(row, !matches);
    }
}

bool DeviceDetailView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_nameLabel && event->type() == QEvent::MouseButtonDblClick) {
        emit renameRequested(m_sessionKey, m_nameLabel->text());
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

// DetailTabSwitch declares Q_OBJECT in this .cpp, so AUTOMOC needs this.
#include "devicedetailview.moc"
