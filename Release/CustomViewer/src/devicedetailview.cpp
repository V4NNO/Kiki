#include "devicedetailview.h"

#include "efficiencycategorybutton.h"
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

#include <utility>

namespace {
QString formatDuration(qint64 ms)
{
    const qint64 totalMinutes = ms / 60000;
    const qint64 hours = totalMinutes / 60;
    const qint64 minutes = totalMinutes % 60;
    if (hours > 0) {
        return QStringLiteral("%1h %2m").arg(hours).arg(minutes);
    }
    if (minutes > 0) {
        return QStringLiteral("%1m").arg(minutes);
    }
    return QStringLiteral("<1m");
}

// Builds a "heading + rows container" section used both in the compact
// side panel and the bigger standalone Programs page; returns the
// QVBoxLayout callers should feed rows into.
QVBoxLayout *buildUsageSection(QWidget *parent, QVBoxLayout *parentLayout, const QString &heading)
{
    auto *headingLabel = new QLabel(heading, parent);
    headingLabel->setStyleSheet(QStringLiteral("color: #9fa5ae; font-weight: 700; letter-spacing: 1px;"));
    parentLayout->addWidget(headingLabel);
    auto *container = new QWidget(parent);
    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);
    parentLayout->addWidget(container);
    return layout;
}
}

UsagePercentRow::UsagePercentRow(const QString &title, const QString &subtitle, double fraction,
                                 QWidget *parent)
    : QWidget(parent), m_title(title), m_subtitle(subtitle), m_fraction(qBound(0.0, fraction, 1.0))
{
}

void UsagePercentRow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(QColor(225, 231, 235));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 9));
    const QString percentText = QStringLiteral("%1%").arg(m_fraction * 100.0, 0, 'f', 1);
    const int percentWidth = 46;
    painter.drawText(QRect(0, 2, width() - percentWidth, 16), Qt::AlignLeft | Qt::AlignVCenter,
                     painter.fontMetrics().elidedText(m_title, Qt::ElideRight,
                                                       width() - percentWidth - 4));
    painter.setPen(QColor(33, 183, 128));
    painter.drawText(QRect(width() - percentWidth, 2, percentWidth, 16),
                     Qt::AlignRight | Qt::AlignVCenter, percentText);

    painter.setPen(QColor(158, 164, 173));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 8));
    painter.drawText(QRect(0, 19, width(), 14), Qt::AlignLeft | Qt::AlignVCenter,
                     painter.fontMetrics().elidedText(m_subtitle, Qt::ElideRight, width()));

    const QRect barRect(0, 37, width(), 4);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(62, 64, 73));
    painter.drawRoundedRect(barRect, 2, 2);
    if (m_fraction > 0.0) {
        painter.setBrush(QColor(29, 160, 111));
        painter.drawRoundedRect(QRect(barRect.x(), barRect.y(),
                                      qMax(3, static_cast<int>(barRect.width() * m_fraction)),
                                      barRect.height()),
                                2, 2);
    }
}

QSize UsagePercentRow::sizeHint() const
{
    return QSize(260, 44);
}

TriLineWidget::TriLineWidget(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(28);
}

void TriLineWidget::setFractions(double productive, double neutral, double unproductive, double none)
{
    m_productive = qBound(0.0, productive, 1.0);
    m_neutral = qBound(0.0, neutral, 1.0);
    m_unproductive = qBound(0.0, unproductive, 1.0);
    m_none = qBound(0.0, none, 1.0);
    update();
}

void TriLineWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    const QRect barRect(0, 16, width(), 8);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0x3e, 0x40, 0x49));
    painter.drawRoundedRect(barRect, 3, 3);

    const double total = m_productive + m_neutral + m_unproductive + m_none;
    if (total <= 0.0) {
        painter.setPen(QColor(0x6f, 0x74, 0x7d));
        painter.setFont(QFont(QStringLiteral("Segoe UI"), 8));
        painter.drawText(rect(), Qt::AlignLeft | Qt::AlignTop, QStringLiteral("Fara date inca astazi."));
        return;
    }

    int x = barRect.x();
    const struct { double fraction; QColor color; } segments[] = {
        {m_productive, EfficiencyCategoryButton::color(QStringLiteral("productive"))},
        {m_neutral, EfficiencyCategoryButton::color(QStringLiteral("neutral"))},
        {m_unproductive, EfficiencyCategoryButton::color(QStringLiteral("unproductive"))},
        {m_none, EfficiencyCategoryButton::color(QStringLiteral("none"))},
    };
    for (const auto &segment : segments) {
        if (segment.fraction <= 0.0) {
            continue;
        }
        const int segmentWidth = qMax(1, static_cast<int>(barRect.width() * segment.fraction));
        painter.setBrush(segment.color);
        painter.drawRect(QRect(x, barRect.y(), segmentWidth, barRect.height()));
        x += segmentWidth;
    }

    painter.setPen(QColor(0xc9, 0xcd, 0xd3));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 8));
    painter.drawText(QRect(0, 0, width(), 14), Qt::AlignLeft | Qt::AlignTop,
                     QStringLiteral("Productiv %1%  ·  Neutru %2%  ·  Neproductiv %3%")
                         .arg(m_productive * 100.0, 0, 'f', 0)
                         .arg(m_neutral * 100.0, 0, 'f', 0)
                         .arg(m_unproductive * 100.0, 0, 'f', 0));
}

QSize TriLineWidget::sizeHint() const
{
    return QSize(260, 28);
}

DeviceDetailView::DeviceDetailView(ViewerConnection &connection, QWidget *parent)
    : QWidget(parent), m_connection(connection)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    auto *headerRow = new QWidget(this);
    headerRow->setObjectName(QStringLiteral("detailHeader"));
    auto *headerLayout = new QHBoxLayout(headerRow);
    headerLayout->setContentsMargins(10, 8, 10, 8);
    auto *backButton = new QPushButton(QStringLiteral("←  Back"), headerRow);
    backButton->setObjectName(QStringLiteral("flatButton"));
    connect(backButton, &QPushButton::clicked, this, &DeviceDetailView::backRequested);
    m_nameLabel = new QLabel(headerRow);
    m_nameLabel->setStyleSheet(QStringLiteral("font-size: 12pt; font-weight: 700; padding-left: 10px;"));
    m_nameLabel->setToolTip(QStringLiteral("Dublu-click pentru a redenumi (doar local, in Viewer)"));
    m_nameLabel->installEventFilter(this); // catches the double-click, see eventFilter()
    headerLayout->addWidget(backButton);
    headerLayout->addWidget(m_nameLabel);

    // SessionPicker.qml equivalent -- see the member comment in the header
    // for why this is a single disabled entry rather than a real picker.
    m_sessionPicker = new QComboBox(headerRow);
    m_sessionPicker->setFixedWidth(160);
    m_sessionPicker->addItem(QStringLiteral("Sesiune principala"));
    m_sessionPicker->setEnabled(false);
    m_sessionPicker->setToolTip(
        QStringLiteral("O singura sesiune per device -- grabber-ul nu raporteaza inca "
                       "mai multe sesiuni simultane ale aceluiasi angajat."));
    headerLayout->addSpacing(10);
    headerLayout->addWidget(m_sessionPicker);
    headerLayout->addStretch();

    m_goToHistoryButton = new QPushButton(QStringLiteral("Go to History"), headerRow);
    m_goToHistoryButton->setObjectName(QStringLiteral("flatButton"));
    connect(m_goToHistoryButton, &QPushButton::clicked, this, [this]() {
        emit goToHistoryRequested(m_sessionKey);
    });
    headerLayout->addWidget(m_goToHistoryButton);

    // StatusUser.qml equivalent -- moved here from the stats panel (see the
    // member comment). ViolationsTimer.qml equivalent sits right next to it
    // in the real header too, hence the pairing here.
    m_statusUserLabel = new QLabel(headerRow);
    m_statusUserLabel->setStyleSheet(QStringLiteral("color: #f2a4a4; padding: 0 8px; font-weight: 600;"));
    m_statusUserLabel->hide();
    headerLayout->addWidget(m_statusUserLabel);
    m_violationsTimerLabel = new QLabel(QStringLiteral("Violari: —"), headerRow);
    m_violationsTimerLabel->setStyleSheet(QStringLiteral("color: #6f747d; padding: 0 8px;"));
    m_violationsTimerLabel->setToolTip(
        QStringLiteral("Nu exista un motor de violari/reguli in grabber inca."));
    headerLayout->addWidget(m_violationsTimerLabel);
    root->addWidget(headerRow);

    // Left content area: Programs / Monitors / Keylogger each swap what's
    // shown here via m_leftStack; Violations has no backing feature (no
    // violation-detection system exists anywhere in this project) so its
    // button stays disabled -- a real placeholder, not a stub page.
    m_leftStack = new QStackedWidget(this);

    auto *monitorsPage = new QWidget(m_leftStack);
    auto *monitorsPageLayout = new QVBoxLayout(monitorsPage);
    monitorsPageLayout->setContentsMargins(0, 0, 0, 0);
    auto *scroll = new QScrollArea(monitorsPage);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    m_monitorContainer = new QWidget(scroll);
    m_monitorLayout = new QVBoxLayout(m_monitorContainer);
    m_monitorLayout->setContentsMargins(6, 6, 6, 6);
    m_monitorLayout->setSpacing(6);
    scroll->setWidget(m_monitorContainer);
    monitorsPageLayout->addWidget(scroll);
    m_leftStack->addWidget(monitorsPage);

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

    auto *subNav = new QWidget(this);
    auto *subNavLayout = new QHBoxLayout(subNav);
    subNavLayout->setContentsMargins(10, 0, 10, 4);
    m_programsTabButton = new QPushButton(QStringLiteral("Programs"), subNav);
    m_monitorsTabButton = new QPushButton(QStringLiteral("Monitors"), subNav);
    auto *violationsTabButton = new QPushButton(QStringLiteral("Violations"), subNav);
    violationsTabButton->setObjectName(QStringLiteral("navButton"));
    violationsTabButton->setEnabled(false); // no violation-detection feature exists
    m_keyloggerTabButton = new QPushButton(QStringLiteral("Keylogger"), subNav);
    connect(m_programsTabButton, &QPushButton::clicked, this,
            [this, programsPage] { switchSubTab(programsPage, m_programsTabButton); });
    connect(m_monitorsTabButton, &QPushButton::clicked, this,
            [this, monitorsPage] { switchSubTab(monitorsPage, m_monitorsTabButton); });
    connect(m_keyloggerTabButton, &QPushButton::clicked, this,
            [this, keyloggerPage] { switchSubTab(keyloggerPage, m_keyloggerTabButton); });
    subNavLayout->addWidget(m_programsTabButton);
    subNavLayout->addWidget(m_monitorsTabButton);
    subNavLayout->addWidget(violationsTabButton);
    subNavLayout->addWidget(m_keyloggerTabButton);
    subNavLayout->addStretch();
    root->addWidget(subNav);
    switchSubTab(monitorsPage, m_monitorsTabButton);

    auto *content = new QWidget(this);
    auto *contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    // Matches the real viewer's Windows.qml: a compact keylog ticker sits
    // directly under the video on both Monitors and Programs (hidden for
    // Keylogger, which already shows the full log) -- not just History.
    auto *leftColumn = new QWidget(content);
    auto *leftColumnLayout = new QVBoxLayout(leftColumn);
    leftColumnLayout->setContentsMargins(0, 0, 0, 0);
    leftColumnLayout->setSpacing(2);
    leftColumnLayout->addWidget(m_leftStack, 1);
    m_keylogTicker = new QLabel(leftColumn);
    m_keylogTicker->setFixedHeight(24);
    m_keylogTicker->setAlignment(Qt::AlignCenter);
    m_keylogTicker->setStyleSheet(QStringLiteral("color: #9fa5ae; background: #24272e;"));
    leftColumnLayout->addWidget(m_keylogTicker);
    contentLayout->addWidget(leftColumn, 1);

    auto *statsPanel = new QWidget(content);
    statsPanel->setFixedWidth(300);
    statsPanel->setObjectName(QStringLiteral("statsPanel"));
    auto *statsLayout = new QVBoxLayout(statsPanel);
    statsLayout->setContentsMargins(12, 12, 12, 12);
    statsLayout->setSpacing(14);

    // SessionInfo.qml's TriLine equivalent -- aggregate today's Programs +
    // Web pages split by efficiency category, above the two lists below.
    m_triLine = new TriLineWidget(statsPanel);
    statsLayout->addWidget(m_triLine);

    m_webPagesLayout = buildUsageSection(statsPanel, statsLayout, QStringLiteral("Web pages"));
    m_programsLayout = buildUsageSection(statsPanel, statsLayout, QStringLiteral("Programs"));

    statsLayout->addStretch();
    contentLayout->addWidget(statsPanel);
    root->addWidget(content, 1);

    connect(&m_connection, &ViewerConnection::metadataChanged, this,
            [this](quint32 streamId, const QString &, const QString &idleText) {
                if (streamId != m_primaryStreamId) {
                    return;
                }
                if (idleText.isEmpty()) {
                    m_statusUserLabel->hide();
                } else {
                    const QString suffix = idleText.startsWith(QStringLiteral("Idle "))
                        ? idleText.mid(5) : idleText;
                    m_statusUserLabel->setText(QStringLiteral("● Not active: %1").arg(suffix));
                    m_statusUserLabel->show();
                }
            });
    connect(&m_connection, &ViewerConnection::historyRunningApplicationsReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryAppUsage> &applications) {
                Q_UNUSED(day)
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_lastPrograms = applications;
                rebuildUsageSection(m_programsLayout, applications,
                                   QStringLiteral("Fara date inca astazi."), 8);
                refreshTriLine();
            });
    connect(&m_connection, &ViewerConnection::historyWebPagesReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryAppUsage> &pages) {
                Q_UNUSED(day)
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_lastWebPages = pages;
                rebuildUsageSection(m_webPagesLayout, pages,
                                   QStringLiteral("Fara navigare inca astazi."), 8);
                refreshTriLine();
            });
    connect(&m_connection, &ViewerConnection::historyCategoriesReceived, this,
            [this](quint32 streamId, const QHash<QString, QString> &categories) {
                if (streamId != m_primaryStreamId) {
                    return;
                }
                m_categories = categories;
                refreshTriLine();
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

void DeviceDetailView::switchSubTab(QWidget *page, QPushButton *activeButton)
{
    m_leftStack->setCurrentWidget(page);
    // m_keylogTicker doesn't exist yet the first time this runs (called
    // from the constructor, before the content/stats-panel section below
    // creates it) -- guard instead of reordering construction.
    if (m_keylogTicker) {
        m_keylogTicker->setVisible(page != m_keyloggerPage);
    }
    // A hidden QStackedWidget page's children don't necessarily get a real
    // layout pass (container width can stay stale/zero) until the page is
    // actually made current -- same reasoning as the deferred call in
    // showDevice(), but that one only runs once, before the user has ever
    // switched to "Programs", so its container was never really sized.
    QTimer::singleShot(0, this, &DeviceDetailView::resizeMonitorsToFit);
    for (QPushButton *button : {m_programsTabButton, m_monitorsTabButton, m_keyloggerTabButton}) {
        if (!button) {
            continue;
        }
        button->setObjectName(button == activeButton ? QStringLiteral("navActive")
                                                      : QStringLiteral("navButton"));
        style()->unpolish(button);
        style()->polish(button);
    }
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

    m_currentWindowPreviews = windowPreviews;
    layoutPreviewWidgets(m_windowsLayout, m_windowsContainer, windowPreviews);
    m_programsPlaceholder->setVisible(windowPreviews.isEmpty());
    if (windowPreviews.isEmpty()) {
        // layoutPreviewWidgets() cleared the layout, including the
        // placeholder -- put it back since there's nothing else to show.
        m_windowsLayout->addWidget(m_programsPlaceholder);
    }

    // m_monitorContainer's/m_windowsContainer's width isn't reliable yet
    // here -- this page may not have been shown/laid out for real by
    // QStackedWidget/QScrollArea the first time a device is opened, so
    // width() can still be whatever default/zero size it had at
    // construction. Defer one event-loop turn, by which point MainWindow
    // has already switched to this page and it's been through a real
    // layout pass.
    QTimer::singleShot(0, this, &DeviceDetailView::resizeMonitorsToFit);

    m_statusUserLabel->hide();
    m_keystrokeEntries.clear();
    m_keystrokesLoaded = false;
    rebuildKeyloggerTable();
    m_keylogTicker->setText(QStringLiteral("Se incarca..."));
    m_lastPrograms.clear();
    m_lastWebPages.clear();
    m_triLine->setFractions(0, 0, 0, 0);
    switchSubTab(m_leftStack->widget(0), m_monitorsTabButton);
    refreshStats();
}

void DeviceDetailView::setDisplayName(const QString &displayName)
{
    m_nameLabel->setText(displayName);
}

void DeviceDetailView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
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

void DeviceDetailView::rebuildUsageSection(QVBoxLayout *sectionLayout,
                                           const QList<HistoryAppUsage> &entries,
                                           const QString &emptyText, int maxRows)
{
    QLayoutItem *item = nullptr;
    while ((item = sectionLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    if (entries.isEmpty()) {
        auto *empty = new QLabel(emptyText, sectionLayout->parentWidget());
        empty->setStyleSheet(QStringLiteral("color: #6f747d;"));
        sectionLayout->addWidget(empty);
        return;
    }
    qint64 totalMs = 0;
    for (const HistoryAppUsage &entry : entries) {
        totalMs += entry.totalMs;
    }
    const quint32 streamId = m_primaryStreamId;
    int shown = 0;
    for (const HistoryAppUsage &entry : entries) {
        if (shown++ >= maxRows) {
            break;
        }
        const double fraction = totalMs > 0 ? static_cast<double>(entry.totalMs) / totalMs : 0.0;

        auto *rowContainer = new QWidget(sectionLayout->parentWidget());
        auto *rowLayout = new QHBoxLayout(rowContainer);
        rowLayout->setContentsMargins(0, 0, 0, 0);
        rowLayout->setSpacing(4);

        auto *row = new UsagePercentRow(entry.application, formatDuration(entry.totalMs), fraction,
                                        rowContainer);
        rowLayout->addWidget(row, 1);

        const QString initialCategory = m_categories.value(
            entry.application, entry.category.isEmpty() ? QStringLiteral("none") : entry.category);
        auto *categoryButton = new EfficiencyCategoryButton(initialCategory, rowContainer);
        const QString application = entry.application;
        connect(categoryButton, &EfficiencyCategoryButton::categoryChanged, this,
                [this, streamId, application](const QString &newCategory) {
                    m_categories[application] = newCategory;
                    m_connection.setAppCategory(streamId, application, newCategory);
                });
        rowLayout->addWidget(categoryButton, 0, Qt::AlignTop);

        sectionLayout->addWidget(rowContainer);
    }
}

void DeviceDetailView::refreshTriLine()
{
    qint64 productiveMs = 0;
    qint64 neutralMs = 0;
    qint64 unproductiveMs = 0;
    qint64 noneMs = 0;
    qint64 totalMs = 0;
    for (const QList<HistoryAppUsage> *list : {&m_lastPrograms, &m_lastWebPages}) {
        for (const HistoryAppUsage &entry : *list) {
            const QString category = m_categories.value(
                entry.application, entry.category.isEmpty() ? QStringLiteral("none") : entry.category);
            totalMs += entry.totalMs;
            if (category == QStringLiteral("productive")) {
                productiveMs += entry.totalMs;
            } else if (category == QStringLiteral("unproductive")) {
                unproductiveMs += entry.totalMs;
            } else if (category == QStringLiteral("neutral")) {
                neutralMs += entry.totalMs;
            } else {
                noneMs += entry.totalMs;
            }
        }
    }
    if (totalMs <= 0) {
        m_triLine->setFractions(0, 0, 0, 0);
        return;
    }
    m_triLine->setFractions(static_cast<double>(productiveMs) / totalMs,
                           static_cast<double>(neutralMs) / totalMs,
                           static_cast<double>(unproductiveMs) / totalMs,
                           static_cast<double>(noneMs) / totalMs);
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
