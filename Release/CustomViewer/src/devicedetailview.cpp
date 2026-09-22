#include "devicedetailview.h"

#include "monitorwidget.h"
#include "viewerconnection.h"

#include <QDate>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QSizePolicy>
#include <QStackedWidget>
#include <QStyle>
#include <QTextEdit>
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
    headerLayout->addWidget(backButton);
    headerLayout->addWidget(m_nameLabel);
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

    auto *keyloggerPage = new QWidget(m_leftStack);
    auto *keyloggerLayout = new QVBoxLayout(keyloggerPage);
    keyloggerLayout->setContentsMargins(10, 10, 10, 10);
    m_keyloggerLog = new QTextEdit(keyloggerPage);
    m_keyloggerLog->setReadOnly(true);
    keyloggerLayout->addWidget(m_keyloggerLog);
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
    contentLayout->addWidget(m_leftStack, 1);

    auto *statsPanel = new QWidget(content);
    statsPanel->setFixedWidth(300);
    statsPanel->setObjectName(QStringLiteral("statsPanel"));
    auto *statsLayout = new QVBoxLayout(statsPanel);
    statsLayout->setContentsMargins(12, 12, 12, 12);
    statsLayout->setSpacing(14);

    m_idleBanner = new QLabel(statsPanel);
    m_idleBanner->setStyleSheet(QStringLiteral(
        "background: #4a2020; color: #f2a4a4; padding: 8px; border-radius: 4px; font-weight: 600;"));
    m_idleBanner->hide();
    statsLayout->addWidget(m_idleBanner);

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
                    m_idleBanner->hide();
                } else {
                    const QString suffix = idleText.startsWith(QStringLiteral("Idle "))
                        ? idleText.mid(5) : idleText;
                    m_idleBanner->setText(QStringLiteral("Not active: %1").arg(suffix));
                    m_idleBanner->show();
                }
            });
    connect(&m_connection, &ViewerConnection::historyRunningApplicationsReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryAppUsage> &applications) {
                Q_UNUSED(day)
                if (streamId != m_primaryStreamId) {
                    return;
                }
                QList<QPair<QString, qint64>> entries;
                for (const HistoryAppUsage &usage : applications) {
                    entries.append({usage.application, usage.totalMs});
                }
                rebuildUsageSection(m_programsLayout, entries,
                                   QStringLiteral("Fara date inca astazi."), 8);
            });
    connect(&m_connection, &ViewerConnection::historyWebPagesReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryAppUsage> &pages) {
                Q_UNUSED(day)
                if (streamId != m_primaryStreamId) {
                    return;
                }
                QList<QPair<QString, qint64>> entries;
                for (const HistoryAppUsage &usage : pages) {
                    entries.append({usage.application, usage.totalMs});
                }
                rebuildUsageSection(m_webPagesLayout, entries,
                                   QStringLiteral("Fara navigare inca astazi."), 8);
            });
    connect(&m_connection, &ViewerConnection::historyKeystrokesReceived, this,
            [this](quint32 streamId, const QString &day, const QList<HistoryKeystrokeEntry> &entries) {
                Q_UNUSED(day)
                if (streamId != m_primaryStreamId) {
                    return;
                }
                QString html;
                for (const HistoryKeystrokeEntry &entry : entries) {
                    html += QStringLiteral("<div><span style='color:#888'>%1</span> "
                                           "<span style='color:#5b8def'>[%2]</span> %3</div>")
                                .arg(QDateTime::fromMSecsSinceEpoch(entry.timestampMs)
                                         .toString(QStringLiteral("HH:mm:ss")),
                                     entry.windowTitle.toHtmlEscaped(), entry.text.toHtmlEscaped());
                }
                m_keyloggerLog->setHtml(html);
            });

    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(30000);
    connect(m_refreshTimer, &QTimer::timeout, this, &DeviceDetailView::refreshStats);
}

void DeviceDetailView::switchSubTab(QWidget *page, QPushButton *activeButton)
{
    m_leftStack->setCurrentWidget(page);
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

    m_idleBanner->hide();
    switchSubTab(m_leftStack->widget(0), m_monitorsTabButton);
    refreshStats();
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
}

void DeviceDetailView::rebuildUsageSection(QVBoxLayout *sectionLayout,
                                           const QList<QPair<QString, qint64>> &entries,
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
    for (const auto &entry : entries) {
        totalMs += entry.second;
    }
    int shown = 0;
    for (const auto &entry : entries) {
        if (shown++ >= maxRows) {
            break;
        }
        const double fraction = totalMs > 0 ? static_cast<double>(entry.second) / totalMs : 0.0;
        auto *row = new UsagePercentRow(entry.first, formatDuration(entry.second), fraction,
                                        sectionLayout->parentWidget());
        sectionLayout->addWidget(row);
    }
}
