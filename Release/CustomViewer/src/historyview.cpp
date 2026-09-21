#include "historyview.h"

#include <QComboBox>
#include <QDateTime>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPainter>
#include <QPushButton>
#include <QScrollBar>
#include <QSlider>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

namespace {

QString categoryDisplayName(const QString &category)
{
    if (category == QStringLiteral("productive")) {
        return QStringLiteral("Productiv");
    }
    if (category == QStringLiteral("unproductive")) {
        return QStringLiteral("Neproductiv");
    }
    return QStringLiteral("Neutru");
}

QColor categoryColor(const QString &category)
{
    if (category == QStringLiteral("productive")) {
        return QColor(0x2e, 0xcc, 0x71);
    }
    if (category == QStringLiteral("unproductive")) {
        return QColor(0xe7, 0x4c, 0x3c);
    }
    return QColor(0xf1, 0xc4, 0x0f);
}

QString formatDuration(qint64 ms)
{
    const qint64 totalMinutes = ms / 60000;
    return QStringLiteral("%1h %2m").arg(totalMinutes / 60).arg(totalMinutes % 60);
}

}

ActivityBarWidget::ActivityBarWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(48);
}

void ActivityBarWidget::setSamples(const QList<HistoryActivitySample> &samples, qint64 rangeStartMs,
                                   qint64 rangeEndMs)
{
    m_samples = samples;
    m_rangeStart = rangeStartMs;
    m_rangeEnd = rangeEndMs;
    update();
}

void ActivityBarWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x24, 0x27, 0x2e));
    if (m_samples.isEmpty() || m_rangeEnd <= m_rangeStart) {
        return;
    }
    int maxEvents = 1;
    for (const HistoryActivitySample &sample : m_samples) {
        maxEvents = qMax(maxEvents, sample.inputEvents);
    }
    const double span = static_cast<double>(m_rangeEnd - m_rangeStart);
    const int barWidth = qMax(2, width() / qMax(1, m_samples.size()) - 1);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0x5b, 0x8d, 0xef));
    for (const HistoryActivitySample &sample : m_samples) {
        const double fraction = (sample.timestampMs - m_rangeStart) / span;
        const int x = static_cast<int>(fraction * width());
        const int barHeight =
            static_cast<int>((static_cast<double>(sample.inputEvents) / maxEvents) * (height() - 4));
        painter.drawRect(x, height() - barHeight, barWidth, barHeight);
    }
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

void EfficiencyBarWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x24, 0x27, 0x2e));
    if (m_segments.isEmpty() || m_rangeEnd <= m_rangeStart) {
        return;
    }
    const double span = static_cast<double>(m_rangeEnd - m_rangeStart);
    painter.setPen(Qt::NoPen);
    for (const HistoryAppSegment &segment : m_segments) {
        const double startFraction = (segment.startMs - m_rangeStart) / span;
        const double endFraction = (segment.endMs - m_rangeStart) / span;
        const int x = static_cast<int>(qBound(0.0, startFraction, 1.0) * width());
        const int right = static_cast<int>(qBound(0.0, endFraction, 1.0) * width());
        painter.setBrush(categoryColor(m_categories.value(segment.application,
                                                          QStringLiteral("neutral"))));
        painter.drawRect(x, 0, qMax(1, right - x), height());
    }
}

HistoryView::HistoryView(ViewerConnection &connection, QWidget *parent)
    : QWidget(parent)
    , m_connection(connection)
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(6);

    auto *topBar = new QHBoxLayout;
    m_monitorCombo = new QComboBox(this);
    m_dayCombo = new QComboBox(this);
    topBar->addWidget(m_monitorCombo, 2);
    topBar->addWidget(m_dayCombo, 1);
    topBar->addStretch();
    m_timeLabel = new QLabel(this);
    m_timeLabel->setObjectName(QStringLiteral("historyTime"));
    topBar->addWidget(m_timeLabel);
    m_toggleAppsButton = new QPushButton(QStringLiteral("◀ Running applications"), this);
    m_toggleAppsButton->setObjectName(QStringLiteral("flatButton"));
    connect(m_toggleAppsButton, &QPushButton::clicked, this, &HistoryView::onToggleRunningApps);
    topBar->addWidget(m_toggleAppsButton);
    root->addLayout(topBar);

    auto *body = new QHBoxLayout;
    m_imageLabel = new QLabel(this);
    m_imageLabel->setObjectName(QStringLiteral("historyPreview"));
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setMinimumSize(400, 250);
    body->addWidget(m_imageLabel, 1);

    m_textLog = new QTextEdit(this);
    m_textLog->setReadOnly(true);
    m_textLog->hide();
    body->addWidget(m_textLog, 1);

    m_runningAppsList = new QListWidget(this);
    m_runningAppsList->setFixedWidth(240);
    body->addWidget(m_runningAppsList);
    root->addLayout(body, 1);

    auto *transport = new QHBoxLayout;
    m_changeSettingsButton = new QPushButton(QStringLiteral("Change settings"), this);
    m_changeSettingsButton->setObjectName(QStringLiteral("flatButton"));
    connect(m_changeSettingsButton, &QPushButton::clicked, this, &HistoryView::onChangeSettingsClicked);
    m_videoTabButton = new QPushButton(QStringLiteral("→ Video"), this);
    m_videoTabButton->setObjectName(QStringLiteral("flatButton"));
    connect(m_videoTabButton, &QPushButton::clicked, this, &HistoryView::onVideoTabClicked);
    m_textTabButton = new QPushButton(QStringLiteral("→ Text"), this);
    m_textTabButton->setObjectName(QStringLiteral("flatButton"));
    connect(m_textTabButton, &QPushButton::clicked, this, &HistoryView::onTextTabClicked);
    m_playButton = new QPushButton(QStringLiteral("▶"), this);
    m_playButton->setObjectName(QStringLiteral("flatButton"));
    m_playButton->setFixedWidth(36);
    connect(m_playButton, &QPushButton::clicked, this, &HistoryView::onPlayClicked);
    m_speedCombo = new QComboBox(this);
    m_speedCombo->addItems({QStringLiteral("1x"), QStringLiteral("2x"), QStringLiteral("4x")});
    connect(m_speedCombo, &QComboBox::currentIndexChanged, this, &HistoryView::onSpeedChanged);
    auto *muteButton = new QPushButton(QStringLiteral("🔇"), this);
    muteButton->setObjectName(QStringLiteral("flatButton"));
    muteButton->setEnabled(false);
    muteButton->setToolTip(QStringLiteral("Fara audio in acest sistem."));
    m_timelineSlider = new QSlider(Qt::Horizontal, this);
    m_timelineSlider->setMinimum(0);
    m_timelineSlider->setMaximum(0);
    connect(m_timelineSlider, &QSlider::valueChanged, this, &HistoryView::onTimelineMoved);

    transport->addWidget(m_changeSettingsButton);
    transport->addWidget(m_videoTabButton);
    transport->addWidget(m_textTabButton);
    transport->addWidget(m_playButton);
    transport->addWidget(m_speedCombo);
    transport->addWidget(muteButton);
    transport->addWidget(m_timelineSlider, 1);
    root->addLayout(transport);

    m_activityBar = new ActivityBarWidget(this);
    root->addWidget(m_activityBar);
    m_efficiencyBar = new EfficiencyBarWidget(this);
    root->addWidget(m_efficiencyBar);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName(QStringLiteral("privacy"));
    m_statusLabel->setWordWrap(true);
    root->addWidget(m_statusLabel);

    m_playbackTimer = new QTimer(this);
    m_playbackTimer->setInterval(800);
    connect(m_playbackTimer, &QTimer::timeout, this, &HistoryView::onPlaybackTick);

    connect(m_monitorCombo, &QComboBox::currentIndexChanged, this, &HistoryView::onMonitorChanged);
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

void HistoryView::setMonitors(const QHash<quint32, QString> &monitorNames)
{
    m_monitorNames = monitorNames;
    const quint32 previous = currentStreamId();
    m_monitorCombo->blockSignals(true);
    m_monitorCombo->clear();
    for (auto it = monitorNames.cbegin(); it != monitorNames.cend(); ++it) {
        m_monitorCombo->addItem(it.value(), it.key());
    }
    int indexToSelect = 0;
    if (previous != 0) {
        const int found = m_monitorCombo->findData(previous);
        if (found >= 0) {
            indexToSelect = found;
        }
    }
    m_monitorCombo->setCurrentIndex(m_monitorCombo->count() > 0 ? indexToSelect : -1);
    m_monitorCombo->blockSignals(false);
}

void HistoryView::activate()
{
    if (m_activated || m_monitorCombo->count() == 0) {
        return;
    }
    m_activated = true;
    onMonitorChanged(m_monitorCombo->currentIndex());
}

quint32 HistoryView::currentStreamId() const
{
    return m_monitorCombo->currentData().toUInt();
}

void HistoryView::onMonitorChanged(int index)
{
    m_playbackTimer->stop();
    m_playButton->setText(QStringLiteral("▶"));
    m_dayCombo->clear();
    m_timestamps.clear();
    m_timelineSlider->setMaximum(0);
    m_imageLabel->clear();
    m_timeLabel->clear();
    m_runningApps.clear();
    m_appSegments.clear();
    m_keystrokeEntries.clear();
    m_runningAppsList->clear();
    m_textLog->clear();
    if (index < 0) {
        return;
    }
    const quint32 streamId = currentStreamId();
    m_statusLabel->setText(QStringLiteral("Se incarca zilele disponibile..."));
    m_connection.requestHistoryDays(streamId);
    m_connection.requestCategories(streamId);
}

void HistoryView::onDayChanged(int index)
{
    m_playbackTimer->stop();
    m_playButton->setText(QStringLiteral("▶"));
    m_timestamps.clear();
    m_timelineSlider->setMaximum(0);
    m_imageLabel->clear();
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
    const quint32 streamId = currentStreamId();
    const qint64 timestampMs = m_timestamps.at(index);
    m_timeLabel->setText(
        QDateTime::fromMSecsSinceEpoch(timestampMs).toString(QStringLiteral("dd.MM.yyyy  HH:mm:ss")));
    m_connection.requestHistoryFrame(streamId, timestampMs);
    updateTextLogHighlight(timestampMs);
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
    if (m_timelineSlider->value() >= m_timelineSlider->maximum()) {
        m_timelineSlider->setValue(0);
    }
    m_playButton->setText(QStringLiteral("⏸"));
    m_playbackTimer->start();
}

void HistoryView::onPlaybackTick()
{
    const int next = m_timelineSlider->value() + 1;
    if (next > m_timelineSlider->maximum()) {
        m_playbackTimer->stop();
        m_playButton->setText(QStringLiteral("▶"));
        return;
    }
    m_timelineSlider->setValue(next);
}

void HistoryView::onSpeedChanged(int index)
{
    static const int intervals[] = {800, 400, 200};
    m_playbackTimer->setInterval(intervals[qBound(0, index, 2)]);
}

void HistoryView::onVideoTabClicked()
{
    m_textLog->hide();
    m_imageLabel->show();
}

void HistoryView::onTextTabClicked()
{
    m_imageLabel->hide();
    m_textLog->show();
}

void HistoryView::onToggleRunningApps()
{
    const bool visible = !m_runningAppsList->isVisible();
    m_runningAppsList->setVisible(visible);
    m_toggleAppsButton->setText(visible ? QStringLiteral("▶ Running applications")
                                        : QStringLiteral("◀ Running applications"));
}

void HistoryView::onChangeSettingsClicked()
{
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle(QStringLiteral("Categorii aplicatii"));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setMinimumWidth(420);
    auto *layout = new QVBoxLayout(dialog);
    auto *form = new QFormLayout;

    const quint32 streamId = currentStreamId();
    for (const HistoryAppUsage &usage : std::as_const(m_runningApps)) {
        auto *combo = new QComboBox(dialog);
        combo->addItem(QStringLiteral("Productiv"), QStringLiteral("productive"));
        combo->addItem(QStringLiteral("Neutru"), QStringLiteral("neutral"));
        combo->addItem(QStringLiteral("Neproductiv"), QStringLiteral("unproductive"));
        const QString current = m_categories.value(usage.application, QStringLiteral("neutral"));
        combo->setCurrentIndex(combo->findData(current));
        const QString application = usage.application;
        connect(combo, &QComboBox::currentIndexChanged, this,
                [this, streamId, application, combo](int) {
                    const QString category = combo->currentData().toString();
                    m_categories[application] = category;
                    m_connection.setAppCategory(streamId, application, category);
                    refreshEfficiencyBar();
                    rebuildRunningAppsList();
                });
        form->addRow(usage.application, combo);
    }
    if (m_runningApps.isEmpty()) {
        layout->addWidget(new QLabel(QStringLiteral("Nicio aplicatie inregistrata inca pentru ziua "
                                                     "selectata."),
                                     dialog));
    }
    layout->addLayout(form);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Close, dialog);
    connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::close);
    layout->addWidget(buttons);
    dialog->show();
}

void HistoryView::onDaysReceived(quint32 streamId, const QStringList &days)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_dayCombo->blockSignals(true);
    m_dayCombo->clear();
    m_dayCombo->addItems(days);
    m_dayCombo->blockSignals(false);
    if (days.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("Niciun istoric inregistrat inca pentru acest monitor."));
        return;
    }
    m_statusLabel->clear();
    onDayChanged(0);
}

void HistoryView::onFramesReceived(quint32 streamId, const QString &day,
                                   const QList<qint64> &timestamps)
{
    Q_UNUSED(day)
    if (streamId != currentStreamId()) {
        return;
    }
    m_timestamps = timestamps;
    m_timelineSlider->setMaximum(qMax(0, timestamps.size() - 1));
    if (timestamps.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("Nicio captura pentru aceasta zi."));
        return;
    }
    m_statusLabel->clear();
    const int lastIndex = timestamps.size() - 1;
    if (m_timelineSlider->value() == lastIndex) {
        requestFrameAt(lastIndex);
    } else {
        m_timelineSlider->setValue(lastIndex);
    }
}

void HistoryView::onFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image)
{
    Q_UNUSED(timestampMs)
    if (streamId != currentStreamId()) {
        return;
    }
    QPixmap pixmap = QPixmap::fromImage(image);
    if (m_imageLabel->width() > 0 && m_imageLabel->height() > 0) {
        pixmap = pixmap.scaled(m_imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    m_imageLabel->setPixmap(pixmap);
}

void HistoryView::onActivityReceived(quint32 streamId, const QString &day,
                                     const QList<HistoryActivitySample> &samples)
{
    Q_UNUSED(day)
    if (streamId != currentStreamId() || m_timestamps.isEmpty()) {
        return;
    }
    m_activityBar->setSamples(samples, m_timestamps.first(), m_timestamps.last());
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
    if (m_timestamps.isEmpty()) {
        return;
    }
    m_efficiencyBar->setSegments(m_appSegments, m_categories, m_timestamps.first(),
                                 m_timestamps.last());
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
    for (const HistoryAppUsage &usage : std::as_const(m_runningApps)) {
        const QString category = m_categories.value(usage.application, usage.category);
        auto *item = new QListWidgetItem(QStringLiteral("%1  —  %2  (%3)")
                                             .arg(usage.application, formatDuration(usage.totalMs),
                                                  categoryDisplayName(category)),
                                         m_runningAppsList);
        item->setForeground(categoryColor(category));
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
    QString html;
    for (const HistoryKeystrokeEntry &entry : entries) {
        html += QStringLiteral("<div><span style='color:#888'>%1</span> "
                               "<span style='color:#5b8def'>[%2]</span> %3</div>")
                    .arg(QDateTime::fromMSecsSinceEpoch(entry.timestampMs)
                             .toString(QStringLiteral("HH:mm:ss")),
                         entry.windowTitle.toHtmlEscaped(), entry.text.toHtmlEscaped());
    }
    m_textLog->setHtml(html);
}

void HistoryView::updateTextLogHighlight(qint64 timestampMs)
{
    if (m_keystrokeEntries.isEmpty() || !m_textLog->isVisible()) {
        return;
    }
    // Scroll the text log roughly to the entry nearest the current timeline
    // position -- a full synced highlight would need per-block anchors;
    // this keeps the two views loosely in sync without that machinery.
    int closestIndex = 0;
    qint64 closestDelta = qAbs(m_keystrokeEntries.first().timestampMs - timestampMs);
    for (int i = 1; i < m_keystrokeEntries.size(); ++i) {
        const qint64 delta = qAbs(m_keystrokeEntries.at(i).timestampMs - timestampMs);
        if (delta < closestDelta) {
            closestDelta = delta;
            closestIndex = i;
        }
    }
    const qreal fraction = m_keystrokeEntries.isEmpty()
        ? 0.0
        : static_cast<qreal>(closestIndex) / qMax(1, m_keystrokeEntries.size() - 1);
    QScrollBar *bar = m_textLog->verticalScrollBar();
    if (bar) {
        bar->setValue(static_cast<int>(fraction * bar->maximum()));
    }
}

void HistoryView::onHistoryError(quint32 streamId, const QString &message)
{
    if (streamId != currentStreamId()) {
        return;
    }
    m_statusLabel->setText(message);
}
