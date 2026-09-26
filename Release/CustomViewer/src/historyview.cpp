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
#include <QVBoxLayout>

#include <algorithm>
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

void drawPositionLine(QPainter &painter, qint64 positionMs, qint64 rangeStartMs, qint64 rangeEndMs,
                     int widgetWidth, int widgetHeight)
{
    if (positionMs <= 0 || rangeEndMs <= rangeStartMs) {
        return;
    }
    const int x = xForTimeOfDay(positionMs, rangeStartMs, rangeEndMs, widgetWidth);
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
    const double bucketWidth = static_cast<double>(width()) / static_cast<double>(bucketCount);
    painter.setPen(Qt::NoPen);
    // #7aa1e2: exact color from the real viewer's HistoLine.qml canvas draw.
    painter.setBrush(QColor(0x7a, 0xa1, 0xe2));
    for (auto it = eventsByBucket.cbegin(); it != eventsByBucket.cend(); ++it) {
        const double fraction = qBound(0.0, static_cast<double>(it.value()) / maxBucketEvents, 1.0);
        const int barHeight = qMax(1, static_cast<int>(fraction * (height() - 4)));
        const int x = static_cast<int>(it.key() * bucketWidth);
        const int barWidth = qMax(1, static_cast<int>(bucketWidth) - 1);
        painter.drawRect(x, height() - barHeight, barWidth, barHeight);
    }

    drawPositionLine(painter, m_currentPositionMs, m_rangeStart, m_rangeEnd, width(), height());
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
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x24, 0x27, 0x2e));
    if (!m_segments.isEmpty() && m_rangeEnd > m_rangeStart) {
        const double span = static_cast<double>(m_rangeEnd - m_rangeStart);
        painter.setPen(Qt::NoPen);
        for (const HistoryAppSegment &segment : m_segments) {
            const double startFraction = (segment.startMs - m_rangeStart) / span;
            const double endFraction = (segment.endMs - m_rangeStart) / span;
            const int x = static_cast<int>(qBound(0.0, startFraction, 1.0) * width());
            const int right = static_cast<int>(qBound(0.0, endFraction, 1.0) * width());
            painter.setBrush(EfficiencyCategoryButton::color(
                m_categories.value(segment.application, QStringLiteral("none"))));
            painter.drawRect(x, 0, qMax(1, right - x), height());
        }
    }
    drawPositionLine(painter, m_currentPositionMs, m_rangeStart, m_rangeEnd, width(), height());
}

namespace {
constexpr int kViolationRowHeight = 20;
constexpr int kViolationLabelWidth = 140;
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

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0x1b, 0x1e, 0x24));
        painter.drawRect(stripX, y, stripWidth, kViolationRowHeight - 2);

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
    // Matches the real SliderBar.qml: a thin, always-visible scrub bar with
    // a drag handle -- NOT where hour labels live (that's TimeAxisWidget,
    // part of the violation panel, matching Chart.qml's separate TimeLine).
    setFixedHeight(14);
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
    return QSize(400, 14);
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
    // Thin scrub track + drag circle only -- no hour labels here, see
    // TimeAxisWidget (matches the real SliderBar.qml's own minimal look).
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x1b, 0x1e, 0x24));
    if (m_timestamps.isEmpty()) {
        return;
    }
    const qint64 rangeStart = m_timestamps.first();
    const qint64 rangeEnd = m_timestamps.last();
    const int midY = height() / 2;

    painter.setPen(QPen(QColor(0x3a, 0x3e, 0x47), 3));
    painter.drawLine(0, midY, width(), midY);

    if (m_currentIndex >= 0 && m_currentIndex < m_timestamps.size()) {
        const int x = xForTimeOfDay(m_timestamps.at(m_currentIndex), rangeStart, rangeEnd, width());
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0xff, 0xd7, 0x00));
        painter.drawEllipse(QPoint(x, midY), 5, 5);
    }
}

namespace {
constexpr int kTimeAxisDateHeight = 16;
constexpr int kTimeAxisTicksHeight = 20;
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
    QPainter painter(this);
    painter.fillRect(rect(), QColor(0x1b, 0x1e, 0x24));
    if (m_rangeEnd <= m_rangeStart) {
        return;
    }

    painter.setPen(QColor(0xff, 0xff, 0xff));
    painter.setFont(QFont(QStringLiteral("Segoe UI"), 8, QFont::Bold));
    painter.drawText(QRect(4, 0, width() - 8, kTimeAxisDateHeight), Qt::AlignLeft | Qt::AlignVCenter,
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
    root->setSpacing(6);

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

    m_videoColumn = new QWidget(this);
    auto *videoColumnLayout = new QVBoxLayout(m_videoColumn);
    videoColumnLayout->setContentsMargins(0, 0, 0, 0);
    videoColumnLayout->setSpacing(2);

    // Slim strip above the video: employee name (left, unobtrusive --
    // History.qml never shows it as a prominent top bar either) and the
    // Running Applications toggle (right, matches appButton's top-right
    // placement, just docked instead of overlaid on the video).
    auto *videoHeaderRow = new QHBoxLayout;
    m_employeeLabel = new QLabel(this);
    m_employeeLabel->setStyleSheet(QStringLiteral("color: #9fa5ae; font-weight: 700;"));
    videoHeaderRow->addWidget(m_employeeLabel);
    videoHeaderRow->addStretch();
    m_timeLabel = new QLabel(this);
    m_timeLabel->setObjectName(QStringLiteral("historyTime"));
    videoHeaderRow->addWidget(m_timeLabel);
    m_toggleAppsButton = new QPushButton(QStringLiteral("Running applications ◀"), this);
    m_toggleAppsButton->setObjectName(QStringLiteral("flatButton"));
    connect(m_toggleAppsButton, &QPushButton::clicked, this, &HistoryView::onToggleRunningApps);
    videoHeaderRow->addWidget(m_toggleAppsButton);
    videoColumnLayout->addLayout(videoHeaderRow);

    auto *videoBody = new QHBoxLayout;
    videoBody->addWidget(m_monitorStripArea, 1);
    m_runningAppsList = new QListWidget(this);
    m_runningAppsList->setFixedWidth(420); // matches History.qml's `panel { width: 420 }`
    m_runningAppsList->hide(); // matches appButton's panel: closed by default
    videoBody->addWidget(m_runningAppsList);
    videoColumnLayout->addLayout(videoBody, 1);

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
    root->addWidget(m_videoColumn, 1);

    // Real Kickidler keylogger table columns (see the KeylogRow/
    // groupKeystrokeEntries comment above): Date/Period/Window/Keystrokes.
    // Lives in a popup dialog (see onKeylogTableClicked), not as a mode of
    // the main page.
    m_textLog = new QTableWidget(0, 4, this);
    m_textLog->setHorizontalHeaderLabels({QStringLiteral("Date"), QStringLiteral("Pressing period"),
                                          QStringLiteral("Title"), QStringLiteral("Keystrokes")});
    m_textLog->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_textLog->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_textLog->setSelectionMode(QAbstractItemView::SingleSelection);
    m_textLog->verticalHeader()->setVisible(false);
    m_textLog->horizontalHeader()->setStretchLastSection(true);
    m_textLog->setColumnWidth(0, 90);
    m_textLog->setColumnWidth(1, 150);
    m_textLog->setColumnWidth(2, 180);
    // Not in any layout until onKeylogTableClicked reparents it into a
    // dialog -- without this, Qt shows it anyway, unpositioned, at (0,0)
    // over everything else (this was the "garbled Date/boxes in the
    // top-left corner" bug).
    m_textLog->hide();

    auto *transport = new QHBoxLayout;
    m_changeSettingsButton = new QPushButton(QStringLiteral("Change period"), this);
    m_changeSettingsButton->setObjectName(QStringLiteral("flatButton"));
    connect(m_changeSettingsButton, &QPushButton::clicked, this, &HistoryView::onChangeSettingsClicked);
    m_dayCombo = new QComboBox(this);
    m_dayCombo->setFixedWidth(110);
    m_keylogTableButton = new QPushButton(QStringLiteral("Keylogger"), this);
    m_keylogTableButton->setObjectName(QStringLiteral("flatButton"));
    connect(m_keylogTableButton, &QPushButton::clicked, this, &HistoryView::onKeylogTableClicked);
    // "Video" export in the real viewer saves an actual AVI (VideoSaverSelector.qml);
    // we have no video encoder in this project's dependencies, so this
    // exports the day's captured frames as a numbered PNG sequence instead
    // -- same underlying data (requestHistoryFrame per timestamp), honestly
    // labeled as image export rather than pretending to produce a video file.
    m_exportVideoButton = new QPushButton(QStringLiteral("Export imagini"), this);
    m_exportVideoButton->setObjectName(QStringLiteral("flatButton"));
    connect(m_exportVideoButton, &QPushButton::clicked, this, &HistoryView::onExportVideoClicked);
    m_playButton = new QPushButton(QStringLiteral("▶"), this);
    m_playButton->setObjectName(QStringLiteral("flatButton"));
    m_playButton->setFixedWidth(36);
    connect(m_playButton, &QPushButton::clicked, this, &HistoryView::onPlayClicked);
    // Real speed options (SpeedButton.qml): 0.5x/0.75x/1x/2x/4x/8x/16x.
    m_speedCombo = new QComboBox(this);
    m_speedCombo->addItems({QStringLiteral("0.5x"), QStringLiteral("0.75x"), QStringLiteral("1x"),
                            QStringLiteral("2x"), QStringLiteral("4x"), QStringLiteral("8x"),
                            QStringLiteral("16x")});
    m_speedCombo->setCurrentIndex(2); // 1x
    connect(m_speedCombo, &QComboBox::currentIndexChanged, this, &HistoryView::onSpeedChanged);
    auto *muteButton = new QPushButton(QStringLiteral("🔇"), this);
    muteButton->setObjectName(QStringLiteral("flatButton"));
    muteButton->setEnabled(false);
    muteButton->setToolTip(QStringLiteral("Fara audio in acest sistem."));
    m_timeline = new TimelineWidget(this);
    connect(m_timeline, &TimelineWidget::indexSelected, this, &HistoryView::onTimelineMoved);

    transport->addWidget(m_changeSettingsButton);
    transport->addWidget(m_dayCombo);
    transport->addWidget(m_keylogTableButton);
    transport->addWidget(m_exportVideoButton);
    transport->addWidget(m_playButton);
    transport->addWidget(m_speedCombo);
    transport->addWidget(muteButton);
    transport->addStretch();
    root->addLayout(transport);

    // The timeline is added directly to `root` (full page width), in its
    // own row -- NOT inside `transport` alongside the buttons above. This
    // is the fix for the drag-circle/yellow-line misalignment: when the
    // timeline shared a row with those buttons, its own width (and left
    // edge) differed from the violation panel bars below it (which span
    // root's full width), so the same timestamp mapped to two different
    // x positions. Sharing this exact container guarantees both use the
    // same width and the same left offset, so they line up pixel-for-pixel.
    root->addWidget(m_timeline);

    // Violation panel toggle: centered below the transport row, matches
    // History.qml's panelViolation button (bottom-center, arrow icon,
    // "Open violation panel" / "Hide violation panel" -- panel starts
    // collapsed).
    auto *violationToggleRow = new QHBoxLayout;
    violationToggleRow->addStretch();
    m_violationToggleButton = new QPushButton(QStringLiteral("▲ Open violation panel"), this);
    m_violationToggleButton->setObjectName(QStringLiteral("flatButton"));
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
    // unit. Starts hidden.
    m_violationPanel = new QWidget(this);
    auto *violationLayout = new QVBoxLayout(m_violationPanel);
    violationLayout->setContentsMargins(0, 0, 0, 0);
    violationLayout->setSpacing(2);
    m_timeAxis = new TimeAxisWidget(m_violationPanel);
    violationLayout->addWidget(m_timeAxis);
    m_activityBar = new ActivityBarWidget(m_violationPanel);
    violationLayout->addWidget(m_activityBar);
    m_efficiencyBar = new EfficiencyBarWidget(m_violationPanel);
    violationLayout->addWidget(m_efficiencyBar);
    m_violationsFilter = new ViolationsFilterWidget(m_violationPanel);
    violationLayout->addWidget(m_violationsFilter);
    m_violationPanel->hide();
    root->addWidget(m_violationPanel);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName(QStringLiteral("privacy"));
    m_statusLabel->setWordWrap(true);
    root->addWidget(m_statusLabel);

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
    const int availableHeight = qMax(
        160, m_monitorStripArea->viewport()->height() - kCaptionAndSpacing - kMargins);
    const int aspectWidth = availableHeight * 16 / 9;
    const int evenWidth = (m_monitorStripArea->viewport()->width() - kMargins) / qMax(1, count);
    const int previewWidth = qBound(kMinPreviewWidth, evenWidth, aspectWidth);
    for (QLabel *preview : std::as_const(m_monitorPreviewLabels)) {
        preview->setMinimumSize(previewWidth, availableHeight);
    }
}

void HistoryView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    resizeMonitorStripToFit();
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
    m_timeLabel->setText(
        QDateTime::fromMSecsSinceEpoch(timestampMs).toString(QStringLiteral("dd.MM.yyyy  HH:mm:ss")));
    // Every screen of the device is shown at once (see rebuildMonitorStrip),
    // so a frame is fetched for each of its monitor streams, not just the
    // one used for the non-video queries (currentStreamId()).
    for (quint32 streamId : m_deviceMonitorStreams.value(m_currentDeviceKey)) {
        m_connection.requestHistoryFrame(streamId, timestampMs);
    }
    updateTextLogHighlight(timestampMs);
    m_timeline->setCurrentIndex(index);
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
    // we only write CSV (no XLSX library in this project's dependencies),
    // but CSV opens directly in Excel/Sheets so the practical need is covered.
    auto *exportButton = new QPushButton(QStringLiteral("Export CSV"), dialog);
    connect(exportButton, &QPushButton::clicked, this, [this, dialog]() {
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
        out << "Date,Pressing period,Title,Keystrokes\n";
        for (int row = m_textLog->rowCount() - 1; row >= 0; --row) {
            QStringList fields;
            for (int col = 0; col < 4; ++col) {
                QString field = m_textLog->item(row, col) ? m_textLog->item(row, col)->text() : QString();
                field.replace(QLatin1Char('"'), QStringLiteral("\"\""));
                fields << QStringLiteral("\"%1\"").arg(field);
            }
            out << fields.join(QLatin1Char(',')) << "\n";
        }
    });
    layout->addWidget(exportButton);

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
        return;
    }
    m_statusLabel->clear();
    applyPeriodFilter();
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
        return;
    }
    m_statusLabel->clear();
    requestFrameAt(timestamps.size() - 1);
}

void HistoryView::onFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image)
{
    if (m_videoExport.active && streamId == m_videoExport.streamId
        && !m_videoExport.pending.isEmpty() && m_videoExport.pending.first() == timestampMs) {
        m_videoExport.pending.removeFirst();
        const QString fileName = QDateTime::fromMSecsSinceEpoch(timestampMs)
                                     .toString(QStringLiteral("HH-mm-ss"));
        image.save(QStringLiteral("%1/%2.png").arg(m_videoExport.directory, fileName));
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
    nonProductive.label = QStringLiteral("Aplicatii neproductive");
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
        m_textLog->setItem(i, 2, new QTableWidgetItem(row.windowTitle));
        m_textLog->setItem(i, 3, new QTableWidgetItem(row.text));
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
}
