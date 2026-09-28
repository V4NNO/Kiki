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
#include <QSizePolicy>
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
    setStyleSheet(QStringLiteral(
        "QWidget#detailView { background-image: url(:/activeCell/grayHatching.png); }"));

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
    auto *windowsScroll = new QScrollArea(programsPage);
    windowsScroll->setWidgetResizable(true);
    windowsScroll->setFrameShape(QFrame::NoFrame);
    m_windowsContainer = new QWidget(windowsScroll);
    m_windowsLayout = new QVBoxLayout(m_windowsContainer);
    m_windowsLayout->setContentsMargins(6, 6, 6, 6);
    m_windowsLayout->setSpacing(6);
    m_programsPlaceholder = new QLabel(QStringLiteral("Se asteapta activitate..."), m_windowsContainer);
    m_programsPlaceholder->setAlignment(Qt::AlignCenter);
    m_programsPlaceholder->setStyleSheet(QStringLiteral("color: #6f747d; font-size: 11pt;"));
    m_windowsLayout->addWidget(m_programsPlaceholder);
    windowsScroll->setWidget(m_windowsContainer);
    programsPageLayout->addWidget(windowsScroll);
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
    // Inert: PersonalHost's keylogger doesn't tag which characters are
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
    keyloggerLayout->addWidget(m_keyloggerTable, 1);
    m_leftStack->addWidget(keyloggerPage);

    // TrackerQuadratorActiveCell.qml's tab order: Programs, Monitors,
    // Violations (shown but not selectable -- no violation engine here),
    // Keylogger. Default is Monitors (currentIndex 1, WOOS-616).
    m_tabSwitch->addTab(QStringLiteral("Programs"), QStringLiteral("tabs/windows_on.png"),
                        QStringLiteral("tabs/windows_off.png"));
    m_tabSwitch->addTab(QStringLiteral("Monitors"), QStringLiteral("tabs/monitor_on.png"),
                        QStringLiteral("tabs/monitor_off.png"));
    m_tabSwitch->addTab(QStringLiteral("Violations"), QStringLiteral("tabs/filters_on.png"),
                        QStringLiteral("tabs/filters_off.png"), false);
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
    m_infoArea->setStyleSheet(QStringLiteral("background: transparent;"));
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
            });
    connect(&m_connection, &ViewerConnection::historyEmployeeCategoriesReceived, this,
            [this](quint32 streamId, const QHash<QString, QString> &categories) {
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_employeeCategories = categories;
                updateInfoPanel();
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
                // isn't a state PersonalHost reports (there's no per-device
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
    // Tab index -> stacked page. Violations (2) has no page and isn't
    // selectable, so it never reaches here.
    QWidget *page = m_monitorsPage;
    if (index == 0) {
        page = m_programsPage;
    } else if (index == 3) {
        page = m_keyloggerPage;
    }
    m_leftStack->setCurrentWidget(page);
    // The keylog ticker sits under the video on Programs/Monitors only; the
    // Keylogger tab shows the full log instead.
    if (m_keylogTicker) {
        m_keylogTicker->setVisible(page != m_keyloggerPage);
    }
    // A hidden QStackedWidget page's children don't necessarily get a real
    // layout pass (container width can stay stale/zero) until the page is
    // actually made current.
    QTimer::singleShot(0, this, &DeviceDetailView::resizeMonitorsToFit);
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
    m_tabSwitch->setCurrentIndex(1);
    switchSubTab(1);
    refreshStats();
}

void DeviceDetailView::refreshWindowPreviews(const QList<MonitorWidget *> &windowPreviews)
{
    m_currentWindowPreviews = windowPreviews;
    layoutPreviewWidgets(m_windowsLayout, m_windowsContainer, windowPreviews);
    m_programsPlaceholder->setVisible(windowPreviews.isEmpty());
    if (windowPreviews.isEmpty()) {
        // layoutPreviewWidgets() cleared the layout, including the
        // placeholder -- put it back since there's nothing else to show.
        m_windowsLayout->addWidget(m_programsPlaceholder);
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
            items.append({entry.application, QStringLiteral("No title"),
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
