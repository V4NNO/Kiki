#include "keyloggerpage.h"

#include "historyview.h"
#include "keyloggerchart.h"
#include "keyloggertable.h"
#include "multiswitch.h"
#include "savedialog.h"

#include <QAction>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QResizeEvent>
#include <QTimer>
#include <QVBoxLayout>

namespace {
// ChartToolbar.qml: the Range choices and their Steps (seconds).
struct RangeChoice {
    const char *text;
    int length;
    int steps[3];
};
const RangeChoice kRanges[] = {
    {"1 hour", 60 * 60, {60, 5 * 60, 15 * 60}},
    {"4 hours", 4 * 60 * 60, {5 * 60, 15 * 60, 30 * 60}},
    {"8 hours", 8 * 60 * 60, {10 * 60, 15 * 60, 30 * 60}},
    {"1 day", 24 * 60 * 60, {20 * 60, 30 * 60, 60 * 60}},
};

// Fonts.rr_small_b / rr_small / rr_big_b as the running original draws them.
QFont smallFont()
{
    QFont font(QStringLiteral("Roboto"));
    font.setPointSize(8);
    return font;
}

QFont bigFont()
{
    QFont font(QStringLiteral("Roboto"));
    font.setPixelSize(14);
    return font;
}

QLabel *whiteLabel(const QString &text, const QFont &font, QWidget *parent)
{
    auto *label = new QLabel(text, parent);
    label->setFont(font);
    label->setStyleSheet(QStringLiteral("color: white; background: transparent;"));
    return label;
}

// Controls/CheckBox.qml: checkBox/(un)checked|disabled.png, Fonts.rr_small,
// white text 20px from the box.
QCheckBox *checkBox(const QString &text, bool checked, QWidget *parent)
{
    auto *box = new QCheckBox(text, parent);
    box->setChecked(checked);
    box->setFont(smallFont());
    box->setStyleSheet(QStringLiteral(
        "QCheckBox { color: white; spacing: 7px; background: transparent; }"
        "QCheckBox::indicator { width: 13px; height: 13px; }"
        "QCheckBox::indicator:unchecked { image: url(:/controls/checkBox/unchecked.png); }"
        "QCheckBox::indicator:checked { image: url(:/controls/checkBox/checked.png); }"
        "QCheckBox::indicator:disabled { image: url(:/controls/checkBox/disabled.png); }"));
    return box;
}

// The khaki loading box (Keylogger.qml: 100x100, opacity .5, radius 5)
// with ViewUtils.Spinner turning in it.
class SpinnerBox final : public QWidget
{
public:
    explicit SpinnerBox(QWidget *parent)
        : QWidget(parent)
    {
        setFixedSize(100, 100);
        setAttribute(Qt::WA_TransparentForMouseEvents);
        m_timer = new QTimer(this);
        m_timer->setInterval(50);
        connect(m_timer, &QTimer::timeout, this, [this] {
            m_angle = (m_angle + 15) % 360;
            update();
        });
    }

protected:
    void showEvent(QShowEvent *) override { m_timer->start(); }
    void hideEvent(QHideEvent *) override { m_timer->stop(); }
    void paintEvent(QPaintEvent *) override
    {
        static const QPixmap spinner(QStringLiteral(":/history/spinner/spinner.png"));
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.setPen(Qt::NoPen);
        QColor khaki(0xf0, 0xe6, 0x8c);
        khaki.setAlphaF(0.5);
        painter.setBrush(khaki);
        painter.drawRoundedRect(rect(), 5, 5);
        painter.translate(rect().center());
        painter.rotate(m_angle);
        painter.drawPixmap(-spinner.width() / 2, -spinner.height() / 2, spinner);
    }

private:
    QTimer *m_timer = nullptr;
    int m_angle = 0;
};
} // namespace

KeyloggerPage::KeyloggerPage(ViewerConnection &connection, QWidget *parent)
    : QWidget(parent)
    , m_connection(connection)
{
    // ColumnLayout { spacing: 10; anchors.margins: 27 }
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(27, 27, 27, 27);
    root->setSpacing(10);

    // Toolbar.qml: RowLayout spacing 25.
    m_toolbar = new QWidget(this);
    auto *toolbar = new QHBoxLayout(m_toolbar);
    toolbar->setContentsMargins(0, 0, 0, 0);
    toolbar->setSpacing(25);
    m_subject = new MultiSwitch(m_toolbar);
    m_subject->addTab(QStringLiteral("Web pages"), QStringLiteral(":/keylogger/toolbar/sites_active.png"),
                      QStringLiteral(":/keylogger/toolbar/sites_normal.png"));
    m_subject->addTab(QStringLiteral("Programs"), QStringLiteral(":/keylogger/toolbar/programs_active.png"),
                      QStringLiteral(":/keylogger/toolbar/programs_normal.png"));
    m_subject->addTab(QStringLiteral("Web pages and programs"), QStringLiteral(":/keylogger/toolbar/total_active.png"),
                      QStringLiteral(":/keylogger/toolbar/total_normal.png"));
    m_subject->setCurrentIndex(2); // Component.onCompleted: the last one
    toolbar->addWidget(m_subject);
    toolbar->addWidget(whiteLabel(QStringLiteral("Search:"), smallFont(), m_toolbar));
    // SearchTextField: searchTextField/bg.png (5px borders), lupa.png 6px
    // from the right, Fonts.rr_medium, #7d7e83 text (white while focused),
    // placeholder #8D8D91.
    m_search = new QLineEdit(m_toolbar);
    m_search->setPlaceholderText(QStringLiteral("Keywords"));
    QFont searchFont(QStringLiteral("Roboto"));
    searchFont.setPixelSize(12);
    m_search->setFont(searchFont);
    m_search->setStyleSheet(QStringLiteral(
        "QLineEdit { border-width: 5px; border-image: url(:/controls/searchTextField/bg.png) 5 5 5 5 stretch stretch;"
        " color: #7d7e83; background: transparent; padding: -3px 0; }"
        "QLineEdit:focus { color: white; }"));
    {
        QPalette palette = m_search->palette();
        palette.setColor(QPalette::PlaceholderText, QColor(0x8d, 0x8d, 0x91));
        m_search->setPalette(palette);
    }
    m_search->addAction(QIcon(QStringLiteral(":/controls/searchTextField/lupa.png")), QLineEdit::TrailingPosition);
    toolbar->addWidget(m_search, 1);
    m_cutSystemKeys = checkBox(QStringLiteral("Hide system keys"), true, m_toolbar);
    toolbar->addWidget(m_cutSystemKeys);
    m_autoupdate = checkBox(QStringLiteral("Autoupdateing"), false, m_toolbar);
    toolbar->addWidget(m_autoupdate);
    // Controls/Button.qml: button/*.png BorderImage, rr_medium_b.
    m_exportButton = new QPushButton(QStringLiteral("Export"), m_toolbar);
    m_exportButton->setFont(smallFont());
    m_exportButton->setFixedHeight(25);
    m_exportButton->setStyleSheet(QStringLiteral(
        "QPushButton { color: white; border-width: 10px; padding: -10px 2px;"
        " border-image: url(:/activeCell/button/normal.png) 10 10 10 10 stretch stretch; }"
        "QPushButton:hover { border-image: url(:/activeCell/button/hover.png) 10 10 10 10 stretch stretch; }"
        "QPushButton:pressed { border-image: url(:/activeCell/button/press.png) 10 10 10 10 stretch stretch; }"
        "QPushButton:disabled { color: gray; }"));
    toolbar->addWidget(m_exportButton);
    root->addWidget(m_toolbar);

    m_table = new KeyloggerTable(this);
    root->addWidget(m_table, 1);

    m_chartTitle = whiteLabel(QStringLiteral("The intensity of keystrokes for today"), bigFont(), this);
    root->addWidget(m_chartTitle);

    // ChartToolbar.qml: Range: [1 hour | 4 hours | 8 hours | 1 day]  Step: [...]
    m_chartToolbar = new QWidget(this);
    auto *chartToolbar = new QHBoxLayout(m_chartToolbar);
    chartToolbar->setContentsMargins(0, 0, 0, 0);
    chartToolbar->setSpacing(25);
    chartToolbar->addWidget(whiteLabel(QStringLiteral("Range:"), smallFont(), m_chartToolbar));
    m_range = new MultiSwitch(m_chartToolbar);
    for (const RangeChoice &range : kRanges) {
        m_range->addTab(QString::fromLatin1(range.text));
    }
    m_range->setCurrentIndex(0);
    chartToolbar->addWidget(m_range, 1);
    chartToolbar->addWidget(whiteLabel(QStringLiteral("Step:"), smallFont(), m_chartToolbar));
    m_step = new MultiSwitch(m_chartToolbar);
    chartToolbar->addWidget(m_step, 1);
    root->addWidget(m_chartToolbar);
    const auto fillSteps = [this] {
        // The step model follows the range: "<mins> min" / "<mins> mins".
        m_step->clear();
        const RangeChoice &range = kRanges[qBound(0, m_range->currentIndex(), 3)];
        for (int step : range.steps) {
            const int minutes = step / 60;
            m_step->addTab(QStringLiteral("%1 %2").arg(minutes).arg(minutes > 1 ? QStringLiteral("mins")
                                                                                  : QStringLiteral("min")));
        }
        m_step->setCurrentIndex(0);
    };
    fillSteps();
    connect(m_range, &MultiSwitch::currentChanged, this, [this, fillSteps] {
        fillSteps();
        rebuildChart();
    });
    connect(m_step, &MultiSwitch::currentChanged, this, &KeyloggerPage::rebuildChart);

    m_chart = new KeyloggerChart(this);
    root->addWidget(m_chart);
    connect(m_chart, &KeyloggerChart::wantGoToHistory, this, &KeyloggerPage::openGoToHistory);

    m_spinnerBox = new SpinnerBox(this);
    m_spinnerBox->hide();

    // The Result follows the toolbar at once.
    connect(m_subject, &MultiSwitch::currentChanged, this, &KeyloggerPage::rebuildResult);
    connect(m_search, &QLineEdit::textChanged, this, &KeyloggerPage::rebuildResult);
    connect(m_cutSystemKeys, &QCheckBox::toggled, this, &KeyloggerPage::rebuildResult);
    connect(m_exportButton, &QPushButton::clicked, this, &KeyloggerPage::exportClicked);

    // The Selector's Timer: every minute while autoupdating and not loading,
    // intervalEnd moves to now.
    m_autoupdateTimer = new QTimer(this);
    m_autoupdateTimer->setInterval(60 * 1000);
    connect(m_autoupdateTimer, &QTimer::timeout, this, [this] {
        if (m_status != Status::Loading) {
            load();
        }
    });
    connect(m_autoupdate, &QCheckBox::toggled, this, &KeyloggerPage::updateStates);

    connect(&m_connection, &ViewerConnection::keyEventsReceived, this, &KeyloggerPage::onEvents);
    connect(&m_connection, &ViewerConnection::keyEventsFailed, this, &KeyloggerPage::onError);
    updateStates();
}

void KeyloggerPage::setSession(quint32 deviceKey, quint32 streamId, const QString &employeeName,
                               const SessionIdentity &identity)
{
    const bool changed = deviceKey != m_deviceKey || streamId != m_streamId || !(identity == m_identity);
    m_deviceKey = deviceKey;
    m_streamId = streamId;
    m_employeeName = employeeName;
    m_identity = identity;
    if (!changed) {
        return;
    }
    // A new session marker: a new Selector (nothing carried over).
    m_events.clear();
    m_rows.clear();
    m_table->setRows({});
    m_chart->setData({}, 0, 0);
    m_status = Status::Idle;
    if (m_active) {
        load();
    }
    updateStates();
}

void KeyloggerPage::setActive(bool active)
{
    if (m_active == active) {
        return;
    }
    m_active = active;
    if (active && m_status != Status::Loading) {
        load();
    }
    updateStates();
}

void KeyloggerPage::load()
{
    if (m_streamId == 0) {
        return;
    }
    // intervalEnd: now; intervalBegin: its local day's 00:00.
    const QDateTime end = QDateTime::currentDateTime();
    m_intervalEnd = end.toMSecsSinceEpoch();
    m_intervalBegin = QDateTime(end.date(), QTime(0, 0)).toMSecsSinceEpoch();
    // The cookie: an answer carrying any other is someone else's.
    m_requestTag = QStringLiteral("tracker-keylogger-%1-%2").arg(m_deviceKey).arg(++m_generation);
    m_status = Status::Loading;
    m_connection.requestKeyEvents(m_streamId, m_intervalBegin, m_intervalEnd, m_requestTag);
    updateStates();
}

void KeyloggerPage::onEvents(quint32 streamId, const QString &tag, const QList<KeyEvent> &events, bool complete)
{
    if (streamId != m_streamId || tag != m_requestTag) {
        return;
    }
    m_events = events;
    m_status = complete ? Status::LoadedComplete : Status::LoadedPartial;
    rebuildResult();
    rebuildChart();
    updateStates();
}

void KeyloggerPage::onError(quint32 streamId, const QString &tag, const QString &message)
{
    if (streamId != m_streamId || (!tag.isEmpty() && tag != m_requestTag)) {
        return;
    }
    m_errorString = message;
    m_status = Status::Error;
    m_events.clear();
    m_rows.clear();
    m_table->setRows({});
    m_chart->setData({}, 0, 0);
    updateStates();
}

void KeyloggerPage::rebuildResult()
{
    if (m_status != Status::LoadedComplete && m_status != Status::LoadedPartial) {
        return;
    }
    Keylogger::ResultOptions options;
    options.cutSystemKeys = m_cutSystemKeys->isChecked();
    options.keywordFilter = m_search->text();
    const int subject = m_subject->currentIndex(); // 0 web pages, 1 programs, 2 both
    options.showSites = subject == 0 || subject == 2;
    options.showExecutables = subject == 1 || subject == 2;
    m_rows = Keylogger::buildResult(m_events, m_intervalEnd, options);
    m_table->setRows(m_rows);
}

void KeyloggerPage::rebuildChart()
{
    if (m_status != Status::LoadedComplete && m_status != Status::LoadedPartial) {
        m_chart->setData({}, 0, 0);
        return;
    }
    const RangeChoice &range = kRanges[qBound(0, m_range->currentIndex(), 3)];
    const int granula = range.steps[qBound(0, m_step->currentIndex(), 2)];
    const Keylogger::ChartData chart = Keylogger::buildChart(m_events, m_intervalEnd, range.length, granula);
    m_chart->setData(chart.content, chart.maxV, chart.granulaMs);
}

void KeyloggerPage::updateStates()
{
    const bool loaded = m_status == Status::LoadedComplete || m_status == Status::LoadedPartial;
    // Toolbar.enabled: resultInst.active; ChartToolbar.enabled: chartInst.active.
    m_toolbar->setEnabled(loaded);
    m_chartToolbar->setEnabled(loaded);
    // The spinner while the Selector loads.
    m_spinnerBox->setVisible(m_status == Status::Loading);
    m_spinnerBox->raise();
    // The Timer: running while not loading and autoupdating.
    const bool timer = m_active && m_status != Status::Loading && m_autoupdate->isChecked();
    if (timer && !m_autoupdateTimer->isActive()) {
        m_autoupdateTimer->start();
    } else if (!timer) {
        m_autoupdateTimer->stop();
    }
}

void KeyloggerPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_spinnerBox->move((width() - m_spinnerBox->width()) / 2, (height() - m_spinnerBox->height()) / 2);
}

void KeyloggerPage::paintEvent(QPaintEvent *)
{
    // Transparent: the tab's background shows through (TabView's grayHatching).
}

void KeyloggerPage::exportClicked()
{
    // Toolbar.onExportClicked: utils/KeyloggerExport.qml for this session's
    // [intervalBegin, intervalEnd), writing the current Result.
    const QDateTime from = QDateTime::fromMSecsSinceEpoch(m_intervalBegin);
    const QDateTime to = QDateTime::fromMSecsSinceEpoch(m_intervalEnd);
    const QString stem = QStringLiteral("%1_%2_%3").arg(m_employeeName.trimmed(), dateToStr(from), dateToStr(to));
    SaveDialog dialog(QStringLiteral("Save keystrokes"), QStringLiteral("csv"), QStringLiteral("keyloggerExport"),
                      stem + QStringLiteral(".csv"), this);
    dialog.addRow(QStringLiteral("Session:"), m_identity.exportMarker());
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
    // ("without system buttons" is History's only: allowCutSystemKeys.)
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    const QString result = format->currentIndex() == 1 ? Keylogger::exportToXlsx(m_rows, dialog.path())
                                                       : Keylogger::exportToCsv(m_rows, dialog.path());
    if (result != QStringLiteral("success")) {
        QMessageBox::warning(this, QStringLiteral("Keylogger export failed"), result);
    }
}

void KeyloggerPage::openGoToHistory(const QDateTime &left, const QDateTime &right)
{
    // Filters.GoToHistoryDialog (title "Jump to History") -> windowContent
    // .jmp("history", {employeeId, type: "custom", customBegin, customEnd,
    // timeStep}).
    qint64 step = 5 * 60 * 1000;
    if (!runGoToHistoryDialog(this, QStringLiteral("Jump to History"), left.toMSecsSinceEpoch(),
                              right.toMSecsSinceEpoch(), step, &step)) {
        return;
    }
    emit goToHistory(m_deviceKey, left, right, step);
}
