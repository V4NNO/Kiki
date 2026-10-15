#pragma once

#include "historysegmentstore.h"
#include "historyvideoexport.h"
#include "viewerconnection.h"

#include <QIcon>
#include <QPropertyAnimation>
#include <QFrame>
#include <QPointer>
#include "viewertabbar.h"
#include <QDate>
#include <QDateTime>
#include <QDialog>
#include <QElapsedTimer>
#include <QHash>
#include <QColor>
#include <QList>
#include <QPair>
#include <QSet>
#include <QWidget>

class QComboBox;
class QVariantAnimation;
class QDateTimeEdit;
class QDialog;
class QEvent;
class QHBoxLayout;
class QLabel;
class QListWidget;
class QMouseEvent;
class QPushButton;
class QResizeEvent;
class QScrollArea;
class QScrollBar;
class QTabBar;
class QTableWidget;
class QTimer;
class QWheelEvent;

// history/video/Header.qml: 24px bar (#3f4047, double lines top/bottom),
// the employee name centered and "( moment )" 10px to its right.
class HistoryVideoHeader final : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryVideoHeader(QWidget *parent = nullptr);
    void setName(const QString &name);
    void setMoment(const QString &moment);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_name;
    QString m_moment;
};

// history/Video.qml's frame row: every screen of the employee side by side
// (spacing 30), full height, online frames as wide as their aspect ratio
// needs, loading/offline ones width/count; centered when narrower than the
// view, horizontally scrollable otherwise (it lives in a QScrollArea).
class HistoryVideoStrip final : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryVideoStrip(QWidget *parent = nullptr);
    void setStreams(const QList<quint32> &streamIds);
    void setAllLoading();
    void setFrame(quint32 streamId, const QImage &image);
    void setOffline(quint32 streamId);
    // Video.qml: a frame still on its way -- the Spinner over whatever the
    // screen showed before (its Image keeps the old url meanwhile).
    void setLoading(quint32 streamId);
    // The scroll area's usable size -- frames are its height minus 5, and
    // width/count / centering are relative to its width.
    void setViewSize(const QSize &size);
    // Video.qml's ScrollView flickable: press-and-drag scrolls the row.
    void setScrollBar(QScrollBar *scrollBar) { m_scrollBar = scrollBar; }

signals:
    void clicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    enum class Status { Loading, Online, Offline };
    struct Screen {
        quint32 streamId = 0;
        QImage image;
        Status status = Status::Loading;
    };
    QList<QRect> screenRects() const;
    void relayout();

    QList<Screen> m_screens;
    QSize m_viewSize;
    QTimer *m_spinnerTimer = nullptr;
    int m_spinnerAngle = 0;
    QScrollBar *m_scrollBar = nullptr;
    QPoint m_pressGlobal;
    int m_pressScroll = 0;
    bool m_dragging = false;
    bool m_pressed = false;
};

// History.qml's Running applications `panel` content:
// utils/sessionInfo/WebPagesAndPrograms.qml -- a "WebPages" and a
// "Programs" section, one TriLine.qml row each (title, url/executable in its
// category color, share of the current moment, active marker, and the
// categorization button on hover).
class HistoryInfoPanel final : public QWidget
{
    Q_OBJECT

public:
    struct Item {
        QString resource; // url or executable (TriLine text1)
        QString title;    // TriLine text3
        double percent = 0.0;
        QString category;
        bool active = false;
        bool webPage = false; // a WebPages row (its text1 is a url)
    };
    explicit HistoryInfoPanel(QWidget *parent = nullptr);
    void setItems(const QList<Item> &webPages, const QList<Item> &programs);
    // Space around the rows: 10 in History's 420px panel (Info.qml);
    // 0 in the active cell's SessionInfo, whose rows are infoWidth wide.
    void setInset(int inset);

signals:
    void categorizationRequested(const QString &resource, bool webPage);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    struct Row {
        QRect rect;
        Item item;
    };
    struct Title {
        QRect rect;
        QString text;
    };
    void relayout();
    QRect categorizationRect(const Row &row) const;
    // TriLine.qml's two TextWithToolTips: the title (text3, a click opens
    // the categorization) and the url / executable under it (text1: a web
    // page's opens in the browser, a program's opens the categorization).
    // Each rect is as wide as its text actually is.
    struct TextRects {
        QRect title;
        QRect resource;
        QRect titleLine;    // the whole line, for drawing
        QRect resourceLine;
    };
    TextRects textRects(const Row &row) const;
    enum class Part { None, Title, Resource, Categorize };
    Part partAt(const QPoint &pos, int *row) const;

    QList<Item> m_webPages;
    QList<Item> m_programs;
    QList<Row> m_rows;
    QList<Title> m_titles;
    QList<int> m_separatorsY;
    int m_hoveredRow = -1;
    Part m_hoveredPart = Part::None;
    Part m_pressedPart = Part::None;
    int m_pressedRow = -1;
    int m_inset = 10;
};

// utils/sessionInfo/CategorizationPanel.qml (a GenericBox): "Efficiency
// <mask>" and one categorizationPanel/Row.qml per scope, root first -- the
// organization, every department down to the employee's own (indented,
// multiuser icon), then the employee (user icon) -- each with its own
// Productive / Neutral / Unproductive / Uncategorized, Cancel / OK. Every
// row is that scope's own rule; Uncategorized = no rule there, the scope
// above decides. The mask is the site for a web page, the executable for a
// program.
class CategorizationDialog final : public QDialog
{
    Q_OBJECT

public:
    struct Row {
        QString name;
        bool employee = false;  // user_on.png; a department/organization: multiuser_on.png
        bool readOnly = false;  // Row.qml: a scope the viewer may not edit, in gray
        QString category;       // "none" when the scope has no rule
    };
    CategorizationDialog(const QString &mask, const QList<Row> &rows, QWidget *parent = nullptr);
    QString category(int row) const { return row >= 0 && row < m_rows.size() ? m_rows.at(row).category : QString(); }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QRect ratingRect(int row, int option) const;
    QRect cancelRect() const;
    QRect okRect() const;
    QRect closeRect() const;
    // The dialog grows by a row pitch for every scope past the two of the
    // measured dialog.
    int extraHeight() const;

    QString m_title;
    QList<Row> m_rows;
    QPoint m_hover;
};

// Opens the CategorizationPanel for a resource of an employee and pushes
// what changed (applicationsIndex.pushActions): one rule per edited scope.
// The scopes are updated in place; true when anything changed.
bool runCategorizationDialog(QWidget *parent, ViewerConnection &connection, quint32 streamId,
                             const QString &resource, bool webPage, const QString &employeeName,
                             CategoryScopes &scopes);

// HistoryTab.qml's historyPanelState period: "recent" (recentType d/w/m/q +
// recentMultiplier, with recentBegin/recentEnd fixed when it was chosen) or
// "custom" (customBegin/customEnd). History shows [begin(), end()).
struct HistoryPeriod {
    QString type = QStringLiteral("recent");
    QString recentType = QStringLiteral("d");
    int recentMultiplier = 1;
    QDateTime recentBegin;
    QDateTime recentEnd;
    QDateTime customBegin;
    QDateTime customEnd;

    // HistoryTab.getInterval().
    QDateTime begin() const { return type == QStringLiteral("custom") ? customBegin : recentBegin; }
    QDateTime end() const { return type == QStringLiteral("custom") ? customEnd : recentEnd; }
    // DateTimeUtils.js getPeriod(kind, multiplier): the multiplier-th most
    // recent calendar day/week/month/quarter, the current one up to now.
    static QPair<QDateTime, QDateTime> getPeriod(const QString &kind, int multiplier);
    // DateTimeUtils.js getMultiplier(kind, start, end).
    static int getMultiplier(const QString &kind, const QDateTime &start);
    // A recent period, its range computed now.
    static HistoryPeriod recent(const QString &kind, int multiplier);
    // DateTimeUtils.js formatPeriod(period, useDate, false).
    static QString formatPeriod(const QDateTime &start, const QDateTime &end);
};

// utils/HistoryChoicePanel.qml (a GenericBox) in its "add" ("Add history
// watching") and "change" ("Change range and employee") modes: Employee,
// Period (TimeRangeReport: Day/Week/Month/Quarter with a ◀▶ period field,
// or "Arbitrary period" with Begin — End date-time inputs) and Time step
// (TimeStepComboBox for that range, with the "no audio" info icon),
// Cancel / OK.
class HistoryChoiceDialog final : public QDialog
{
    Q_OBJECT

public:
    enum class Mode { Add, Change };
    HistoryChoiceDialog(Mode mode, const QList<QPair<quint32, QString>> &employees, quint32 employee,
                        const HistoryPeriod &period, qint64 timeStepMs, QWidget *parent = nullptr);
    quint32 employee() const { return m_employee; }
    HistoryPeriod period() const { return m_period; }
    qint64 timeStepMs() const { return m_timeStepMs; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;

private:
    QRect fieldRect(int row) const;
    QRect cancelRect() const;
    QRect okRect() const;
    QRect closeRect() const;
    QRect infoRect() const;
    QRect previousRect() const;
    QRect nextRect() const;
    QString employeeName() const;
    void showMenu(int row);
    bool isCustom() const { return m_kind == 4; }
    QString recentType() const;
    // TimeStepComboBox.targetRange / updateModel(): the steps that fit the
    // chosen range, the current one moved to the closest of them.
    qint64 targetRangeMs() const;
    QList<qint64> stepsForRange() const;
    void updateTimeSteps();
    void updateCustomFields();

    QString m_title;
    QList<QPair<quint32, QString>> m_employees;
    quint32 m_employee = 0;
    HistoryPeriod m_period;
    // TimeRangeReport: combo index (Day, Week, Month, Quarter, Arbitrary
    // period) and periodMultiplierPerRange.
    int m_kind = 0;
    int m_multipliers[5] = {1, 1, 1, 1, 1};
    QDateTimeEdit *m_customBeginEdit = nullptr;
    QDateTimeEdit *m_customEndEdit = nullptr;
    qint64 m_timeStepMs = 0;
    QPoint m_hover;
};

// History.qml's keylogger bar: 30px of keylogger_bg.png with the period's
// typed text in one line -- what was typed up to the current moment in
// white, the rest in gray -- centered while it fits.
class KeystreamBar final : public QWidget
{
    Q_OBJECT

public:
    explicit KeystreamBar(QWidget *parent = nullptr);
    void setText(const QString &past, const QString &future);

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    // The bar is a ScrollView: a horizontal scrollbar along its bottom
    // whenever the line is wider than the view.
    void updateScrollRange();
    int rowWidth() const;

    QString m_past;
    QString m_future;
    QScrollBar *m_scrollBar = nullptr;
};

// history/SliderBar.qml: the 4px scrub bar drawn from the original
// sliderBar/*.png assets, with the same three layers and the same two
// independent states the original keeps -- bg_none where nothing was
// recorded, bg_online_cropped over `effectiveRanges` (a recording exists
// here, but its frames have not been fetched), bg_loaded_cropped over
// `loadedRanges` (frames actually downloaded and ready), plus the 19x19
// pick handle.
//
// Everything is addressed in markers, exactly like the original: marker i is
// the Time step long slice [rangeStart + i*step, rangeStart + (i+1)*step) of
// the period, ranges are inclusive marker spans, and the handle sits on
// marker boundaries -- not on raw captured-frame indices, which are
// irregular and made the handle jump.
class TimelineWidget final : public QWidget
{
    Q_OBJECT

public:
    // One drawn stretch of the bar, in whole markers (inclusive) -- what
    // SliderBar.qml's two Repeaters iterate over.
    struct MarkerRange {
        int start = 0;
        int stop = 0;
    };

    explicit TimelineWidget(QWidget *parent = nullptr);

    void setRange(qint64 rangeStartMs, qint64 rangeEndMs);
    // Marker granularity (History's "Time step").
    void setStepMs(qint64 stepMs);
    // The period's recorded frame timestamps. Sorts once and precomputes the
    // effective (gray) ranges; the loaded overlay is reset, so a freshly
    // opened period shows every recorded stretch gray until frames arrive.
    void setTimestamps(const QList<qint64> &timestamps);

    // The loaded (blue) overlay, grown one marker at a time as frames are
    // received and shown -- never the whole period at once.
    void markLoaded(int marker);
    // A whole downloaded stretch at once -- this is what actually fills the
    // bar, a single seek's one marker being far too narrow to see.
    void markLoadedRange(qint64 fromMs, qint64 toMs);
    void clearLoaded();

    void setCurrentMarker(int marker); // does not emit markerSelected
    int currentMarker() const { return m_currentMarker; }
    int markersAmount() const { return m_markersAmount; }
    bool hasData() const { return !m_timestamps.isEmpty(); }

    // HistoryTab.qml's isAllowedMarker(): is anything recorded at marker?
    bool isAllowedMarker(int marker) const;
    // setOnlineMarkerForRewind(): the recorded marker to land on when the
    // target itself has nothing -- the next recorded range to the right
    // (preferRight) or the previous one to the left, clamped at the ends.
    int nearestAllowedMarker(int marker, bool preferRight) const;
    // HistoryTab.setOnlineMarkerForRewind: target itself if recorded, else
    // the neighbouring range of the one holding `current` (its start going
    // right, its stop going left), or the current range's own end.
    int rewindTarget(int current, int target, bool right) const;
    // The captured frame that belongs to a marker (-1 if none at all):
    // inside the marker's own slice when there is one, else the nearest.
    qint64 timestampForMarker(int marker) const;
    int markerForTimestamp(qint64 timestampMs) const;
    // The last marker that has anything recorded (-1 when the period is
    // empty) -- where a freshly opened period starts.
    int lastAllowedMarker() const;
    // HistoryTab's effectiveRanges are the session's online ranges: a
    // moment the session was online (noData included) is reachable even
    // with no video -- History shows its status icon there.
    void setSessionRanges(const QList<QPair<qint64, qint64>> &ranges);
    // SliderBar's knob tooltip: the session's bullet (alive / dead) and its
    // name.
    void setSessionLabel(const QString &label, bool alive);

signals:
    void markerSelected(int marker);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    // SliderBar.qml's __sliderBar: the widget's width (widthSlider) - 10.
    int barWidth() const;
    double markWidth() const;
    int markerForX(int x) const;
    void seekToX(int x);
    QRect pickRect() const;
    double pickWidth() const;
    QRect barRect() const;
    void rebuildRanges();
    // Adds one marker to a sorted, non-overlapping range list, merging it
    // with the neighbours it touches.
    static void addMarker(QList<MarkerRange> &ranges, int marker);
    static bool contains(const QList<MarkerRange> &ranges, int marker);
    void drawRanges(QPainter &painter, const QList<MarkerRange> &ranges, const QPixmap &image);
    void applySessionRanges();
    // TooltipController: shown 500 ms after the mouse settles on the knob
    // (MouseAreaWithTooltip, the session) or on a stretch with no data
    // (MouseAreaWithTextualTooltip, "No data for a given period"), at the
    // mouse + 10 px, following it, hidden again after 8 s.
    enum class Tip { None, Session, NoData };
    Tip tipAt(const QPoint &pos) const;
    void updateTip(const QPoint &pos);
    void hideTip();
    QList<QPair<qint64, qint64>> m_sessionRanges;
    QString m_sessionLabel;
    bool m_sessionAlive = false;
    Tip m_tip = Tip::None;
    QTimer *m_tipDelay = nullptr;
    QTimer *m_tipHide = nullptr;
    QPointer<QWidget> m_tipWidget;

    QList<qint64> m_timestamps;           // sorted once, in setTimestamps
    QList<MarkerRange> m_effectiveRanges; // a recording exists (gray)
    QList<MarkerRange> m_loadedRanges;    // downloaded and shown (blue)
    int m_currentMarker = -1;
    int m_markersAmount = 0;
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
    qint64 m_stepMs = 5 * 60 * 1000;
    bool m_pickHovered = false;
    bool m_pressed = false;
    // SliderBar.qml's mouseArea on the pick: while dragging, the pick sits at
    // draggedX (its center for the natural 19px width), not on the marker.
    bool m_dragging = false;
    double m_dragX = 0.0;
    double m_dragInitX = 0.0;
    int m_dragBeginX = 0;
    bool m_barPressed = false;
};

// chart/TimeLine.qml, as History's MultiSessionsSlider uses it: one date
// ("high") label per calendar day and HH:mm ("low") labels, both drawn as a
// 1px #54545a tick with the text immediately to its right. Marks are spaced
// by the raw "Time step" (doubled while there would be over 1000 of them);
// only every indexVisible-th low label is shown, where indexVisible is how
// many marks one label's own width spans.
class TimeAxisWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit TimeAxisWidget(QWidget *parent = nullptr);
    void setRange(qint64 rangeStartMs, qint64 rangeEndMs);
    void setStepMs(qint64 stepMs);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
    qint64 m_stepMs = 5 * 60 * 1000;
};

// One filter's violations row (ChartsModel.qml currentFilterValues entry:
// Selector K_byFilter, source = filterId, RF_serieInSessionsCombinedSimple,
// granula = chartTimeStep): FiltersLine label + Content DendroidLine.
struct HistoryChartFilterRow {
    QString filterId;
    QString name;
    QColor color;
    HistoryChartResult values;
};

// utils/Chart.qml as both of its users lay it out: History's Filters.qml
// (needPlayerLine: extra area 40% / separator1 10px / filters 60% of the
// 186px chartsItem) and the Violations tab's Filters.qml (extra area
// ExtraContent.requestedHeigth, filters below it). ExtraHeaders labels on
// the left; over Grid.qml (one line per chart time step) the
// MultiSessionActivity/HistoLine Activity histogram (#7aa1e2) and the
// DendroidLine/Line.qml "colors" Efficiency row; the FiltersLine + Content
// rows per filter below; the khaki HistoryPlayerMarkerControl line in
// History. Every serie is placed with its own TimeToChartConverter
// (start/stop/granula of the reply it came in).
class HistoryChartWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryChartWidget(QWidget *parent = nullptr);
    // Chart.qml timeBegin / timeEnd / timeStep (the Grid's marks).
    void setRange(qint64 rangeStartMs, qint64 rangeEndMs);
    void setChartStepMs(qint64 stepMs);
    // History's raw "Time step": the slider's marker width
    // (HistoryPlayerMarkerControl's sliderMarkWidt).
    void setMarkerStepMs(qint64 stepMs);
    // x (in this widget) where the grid starts -- Chart.qml's
    // labelsAreaLeftMargin.
    void setGridLeft(int x);
    void setActivity(const HistoryChartResult &result);
    void setProductivity(const HistoryChartResult &result);
    void setFilterRows(const QList<HistoryChartFilterRow> &rows);
    void clearSeries();
    void setCurrentPositionMs(qint64 positionMs);
    // Violations tab (trackerQuadratorActiveCell/Filters.qml): needPlayerLine
    // false -- the extra area is ExtraContent.requestedHeigth tall and the
    // filters area takes the rest of the tab; no player marker.
    void setFillMode(bool on);
    // History.qml's Filters Instantiator: the chart's content exists only
    // once the violation panel finished opening (and until it finished
    // closing); while it slides only the background shows.
    void setContentVisible(bool visible);

    // HistoryTab.qml: chartTimeStep = alingStep(rangeSeconds, stepSeconds, 60).
    static qint64 chartStepMs(qint64 rangeMs, qint64 userStepMs);

signals:
    // HistoryPlayerMarkerControl.wantChangeMarker: the khaki line dragged
    // onto another marker (live, while dragging).
    void markerDragged(int marker);
    // TimeRangeSelector.timeRangeSelected: an interval of at least 5
    // minutes selected with the mouse.
    void intervalSelected(qint64 beginMs, qint64 endMs);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    double gridWidthPx() const;
    double markerWidthPx() const;
    double markerX() const;
    bool m_contentVisible = true;
    bool m_markerDragging = false;
    double m_dragBeginX = 0;
    double m_dragInitX = 0;
    bool m_selecting = false;
    double m_selectBeginX = 0;
    double m_zoneX = 0;     // relative to the grid
    double m_zoneWidth = 0;
    HistoryChartResult m_activity;
    HistoryChartResult m_productivity;
    QList<HistoryChartFilterRow> m_filterRows;
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
    qint64 m_chartStepMs = 5 * 60 * 1000;
    qint64 m_markerStepMs = 5 * 60 * 1000;
    qint64 m_currentPositionMs = 0;
    int m_gridLeft = 0;
    bool m_fillMode = false;
};

// ChartsModel.qml's data side: the chartExtraValues Selectors --
// K_activity at granula max(chartTimeStep / 5, 60s) and K_byProductivity at
// chartTimeStep, both over [chartTimeBegin, chartTimeEnd) -- fed into a
// HistoryChartWidget. Re-queries whenever the employee or the time
// parameters change, and recalls the productivity serie 2.5s after a
// categorization change (the Solver dirtyNotifier -> forcedRefreshShoter).
class HistoryChartsModel final : public QObject
{
    Q_OBJECT

public:
    HistoryChartsModel(ViewerConnection &connection, HistoryChartWidget *chart,
                       QObject *parent = nullptr);
    void setQuery(quint32 streamId, qint64 beginMs, qint64 endMs, qint64 chartStepMs);
    void clear();
    void applicationsChanged();

private:
    void request(const QString &kind);
    void onSeries(quint32 streamId, const HistoryChartResult &result);

    ViewerConnection &m_connection;
    HistoryChartWidget *m_chart = nullptr;
    QTimer *m_recallTimer = nullptr;
    quint32 m_streamId = 0;
    qint64 m_beginMs = 0;
    qint64 m_endMs = 0;
    qint64 m_chartStepMs = 0;
    QHash<QString, QString> m_pendingTags; // kind -> tag of the latest request
    quint64 m_nextTag = 0;
};

// The full embedded History page (replaces what used to be a separate
// QDialog): monitor/day pickers, a Video/Text switch, Running Applications
// side panel, transport controls, timeline scrubber, and the Activity/
// Efficiency bars underneath it -- modeled on a real Kickidler History
// screenshot the user provided, not on guesswork (see
// polished-singing-seahorse.md for the fidelity/scope decisions made with
// the user, including keylogging being explicitly requested and confirmed).
class BigStatusIcon;

class HistoryView final : public QWidget
{
    Q_OBJECT

signals:
    // History.qml panelFull: the History view goes onto the window's top
    // layer (HistoryTab.qml:508) and the top panel is held collapsed.
    void panelFullChanged(bool full);

public:
    explicit HistoryView(ViewerConnection &connection, QWidget *parent = nullptr);

    // deviceNames/deviceMonitorStreams: the same device ("employee")
    // grouping MainWindow's Tracker grid uses (deviceKey -> display name /
    // deviceKey -> its monitor streamIds), so History can show every screen
    // of one employee at once instead of a flat cross-device monitor list.
    // monitorNames still gives each individual streamId's own display name,
    // used as a caption under its thumbnail in the horizontal strip.
    void setDevices(const QHash<quint32, QString> &deviceNames,
                    const QHash<quint32, QList<quint32>> &deviceMonitorStreams,
                    const QHash<quint32, QString> &monitorNames);
    // Called whenever this page becomes the visible one; (re)kicks off the
    // device->days->frame request chain if nothing is loaded yet.
    void activate();
    // filters/GoToHistoryDialog.qml equivalent, called from
    // DeviceDetailView's "Go to History" button (via MainWindow): switches
    // to this device and shows the given day ("yyyyMMdd", defaults to
    // today) as a recent Day period.
    void openForDevice(quint32 deviceKey, const QString &day = QString());
    // A History video is being saved (gBackgroundWorkers.windowsWithSaver
    // InProgress): closing the window asks first.
    bool isVideoSaving() const { return m_videoSaver && m_videoSaver->busy(); }
    // HistoryContainer's tab list (shared by every window) and this window's
    // current tab (activity/History.qml's own Config, per window).
    void restoreTabs(const QString &currentTabKey);

private slots:
    void onFramesReceived(quint32 streamId, const QString &day, const QList<qint64> &timestamps);
    void onFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image,
                         quint64 requestId);
    void onSegmentsReceived(quint32 streamId, const QString &day,
                            const QList<HistoryVideoSegmentInfo> &segments);
    void onScreensReceived(quint32 streamId, const QString &day,
                           const QList<QPair<quint32, QString>> &screens);
    void onSegmentReceived(quint32 streamId, quint64 requestId,
                           const ViewerProtocol::HistorySegmentPayload &segment);
    void onSegmentMissing(quint32 streamId, quint64 requestId);
    void onAppSegmentsReceived(quint32 streamId, const QString &day,
                               const QList<HistoryAppSegment> &segments);
    void onWebVisitsReceived(quint32 streamId, const QString &day,
                             const QList<HistoryAppSegment> &visits);
    void onCategorizationRequested(const QString &resource, bool webPage);
    void onEmployeeCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories);
    void onCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories);
    void onKeystrokesReceived(quint32 streamId, const QString &day,
                              const QList<HistoryKeystrokeEntry> &entries);
    void onHistoryError(quint32 streamId, const QString &message);

    void onTimelineMoved(int marker);
    void onPlayClicked();
    void onPlaybackTick();
    void onKeylogTableClicked();
    void onExportVideoClicked();
    void onToggleRunningApps();
    void onChangeSettingsClicked();
    void onSpeedChanged(int index);
    void onToggleViolationPanel();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    // The streamId used for every non-video History query (days, activity,
    // app segments, running apps, categories, keystrokes) -- the device's
    // first/primary monitor. These values are session-wide already (see
    // DeviceDetailView's identical assumption); KikiHost just happens to
    // record them once per monitor, so any one of the device's streams has
    // the full picture -- no merging across monitors needed.
    quint32 currentStreamId() const;
    // The screens History shows for the current period: every screen of the
    // employee that recorded something in it (listScreens), whether or not it
    // is plugged in now -- the original's player block is per session and
    // finds its screens in the data (viewer::history::player::Block has no
    // screen property at all). Until listScreens answers, the connected ones.
    QList<quint32> historyStreams() const;
    QList<quint32> m_historyStreams;
    // The anchor the current period's queries went out on. The rangeKey alone
    // does not identify a period: two employees on the same day share it.
    quint32 m_periodAnchor = 0;
    // Each screen's recorded frame times; the slider is built from their
    // union, so a stretch recorded on any one screen is reachable.
    QHash<quint32, QList<qint64>> m_framesByStream;
    void mergeFrameLists();
    void switchDevice(quint32 deviceKey);
    void rebuildMonitorStrip();
    // Moves the player to one marker of the Time step grid: the handle, the
    // moment, the keystream, the running-apps panel and the chart marker all
    // follow immediately; only the picture is fetched (or served from the
    // cache below).
    void requestFrameAtMarker(int marker);
    // The heavy part of a move: the moment, the side panels and decoding
    // every screen. A drag fires a markerSelected per mouse move, far faster
    // than three 1920x1200 screens decode, so the moves used to queue up
    // behind each other -- the lag. Now the handle moves at once and this runs
    // once per event-loop pass, for the latest position only.
    void applyMarker(int marker);
    QTimer *m_applyTimer = nullptr;
    int m_pendingApplyMarker = -1;
    // Holding "next"/dragging faster than the host can decode would otherwise
    // pile up getFrame requests and then flush in a burst. Keep at most one
    // round in flight: the timeline moves immediately, and the video catches
    // up to the latest requested marker instead of replaying every
    // intermediate one.
    void dispatchFrameRequest();
    // (generation << 32) | counter. The generation changes whenever the
    // period or the employee does, so an answer prepared for the previous
    // interval is recognisable and dropped instead of overwriting what is on
    // screen now.
    quint64 allocateRequestId();
    bool isCurrentGeneration(quint64 requestId) const
    {
        return quint32(requestId >> 32) == m_frameGeneration;
    }

    // Frames already downloaded this view, keyed by (screen, the timestamp
    // the host served). Seeking back to a position that is in here shows it
    // immediately and never asks the host again; it is also what the blue
    // "loaded" overlay is drawn from.
    QImage cachedFrame(quint32 streamId, qint64 timestampMs) const;
    void cacheFrame(quint32 streamId, qint64 timestampMs, const QImage &image);
    void clearFrameCache();
    // Re-paints the loaded overlay from the cache -- used after the marker
    // grid changes (a new Time step re-numbers every marker).
    void refreshLoadedOverlay();
    void noteFrameShown(qint64 timestampMs);

    int m_desiredMarker = -1;
    int m_inFlightMarker = -1;
    // Monotonic seek counter. Every answer carries the ordinal of the seek
    // that asked for it, and is shown when it is at least as new as what is
    // on screen -- so frames the user scrubs *through* are displayed as they
    // arrive (not only the one they stop on), while a late answer for a
    // position already overtaken can never overwrite a newer picture.
    quint64 m_seekOrdinal = 0;
    quint64 m_inFlightOrdinal = 0;
    quint64 m_displayedOrdinal = 0;
    quint64 m_inFlightRequestId = 0;
    QSet<quint32> m_inFlightStreams;
    bool m_frameRequestInFlight = false;
    quint32 m_frameGeneration = 1;
    quint32 m_frameSequence = 0;
    // Instrumentation: cursor move -> request sent -> first answer -> shown.
    QElapsedTimer m_seekTimer;
    // A screen whose answer never arrives (a dropped connection, a host that
    // does not echo request ids) must not wedge seeking for good.
    QTimer *m_frameWatchdog = nullptr;
    void releaseFrameSlot();

    // The downloaded VP8 runs, decoded locally -- the primary source for
    // every position (see historysegmentstore.h). The JPEG cache below is
    // only the fallback for stretches not downloaded yet.
    HistorySegmentStore m_segmentStore;
    QHash<quint32, QList<HistoryVideoSegmentInfo>> m_videoSegments; // per screen
    // Stretches the host answered "nothing recorded there" for, so the
    // prefetch does not ask again in a loop.
    QHash<quint32, QList<QPair<qint64, qint64>>> m_emptyRanges;
    bool m_segmentRequestInFlight = false;
    quint64 m_segmentRequestId = 0;
    quint32 m_segmentStreamId = 0;
    qint64 m_segmentFromMs = 0;
    qint64 m_segmentToMs = 0;
    // Pulls the period down run by run, nearest to the cursor first, so what
    // the user is about to scrub through is in memory before they get there.
    void dispatchPrefetch();
    bool isEmptyRange(quint32 streamId, qint64 timestampMs) const;
    // Shows every screen at this moment from what is already held; returns
    // false when at least one screen still has to be fetched.
    // What a screen shows at a moment (videoStore/player): its downloaded
    // picture, Loading while the run holding it is still to come, Offline
    // where nothing was recorded -- and, for history recorded before the VP8
    // runs, a picture fetched on demand.
    enum class ScreenState { Ready, Loading, Fetch, Offline };
    ScreenState screenState(quint32 streamId, qint64 timestampMs, qint64 *lookupMs = nullptr) const;
    bool markerReady(int marker) const;
    // Shows every screen's state at the moment; true if some screen needs a
    // picture fetched on demand.
    bool showFromStore(qint64 timestampMs);
    // The persistent cache (vhdb.<n>.sqlite) of the current period.
    QString cacheTag(quint32 deviceKey, const HistoryPeriod &period) const;
    void evictUnusedCaches();
    QString m_cacheTag;
    QTimer *m_cacheEvictTimer = nullptr;

    QHash<QPair<quint32, qint64>, QImage> m_frameCache;
    QList<QPair<quint32, qint64>> m_frameCacheOrder; // oldest first
    qint64 m_frameCacheBytes = 0;
    // The primary stream's downloaded moments, so the overlay survives a
    // Time step change (which renumbers the markers).
    QSet<qint64> m_loadedTimestamps;
    // What the cache currently holds, so only a real change of interval or
    // employee throws the downloaded frames away -- changing the Time step
    // alone keeps them.
    QString m_cacheRangeKey;
    quint32 m_cacheDeviceKey = 0;
    // HistoryTab.updateHistoryPlayer(): everything for [start, stop) of
    // the current period -- frames, programs/sites, keystrokes and the
    // ChartsModel (chartTimeStep = alingStep(range, stepSeconds, 60)).
    void loadPeriod();
    qint64 periodStartMs() const { return m_period.begin().toMSecsSinceEpoch(); }
    qint64 periodStopMs() const { return m_period.end().toMSecsSinceEpoch(); }
    void updateTextLogHighlight(qint64 timestampMs);
    void updateKeystream(qint64 timestampMs);
    void updateInfoPanel();
    // The organization's, the departments' and this employee's rules
    // (CategoryScopes::resolve: the nearest scope with a rule decides).
    CategoryScopes m_scopes;
    void showNoData();
    // Header.qml's "( start - stop )" while there is no current moment.
    void showPeriodMoment();
    void jumpTo(quint32 deviceKey, const HistoryPeriod &period);

    // HistoryPanel.qml / ViewerControls/Tabs.qml: one tab per history
    // watching (employee + period + Time step), "+" opens "Add history
    // watching", closing asks for confirmation.
    struct HistoryTab {
        quint32 deviceKey = 0;
        HistoryPeriod period;
        qint64 timeStepMs = 5 * 60 * 1000;
        int speedIndex = 2; // each tab's History has its own speed (1x)
        // Saved (HistoryTab.qml's Config): its id, the employee, the period
        // and the Time step. A restored tab finds its employee by name.
        QString id;
        QString employeeName;
        // Each tab's own View.History keeps these while another tab is shown.
        qint64 positionMs = -1;
        bool playing = false;
        bool chartOpen = false;
        bool appsOpen = false;
        bool panelFull = false;
    };
    // atIndex < 0: at the end.
    void addHistoryTab(const HistoryTab &tab, int atIndex = -1);
    void saveTabs() const;
    QString m_currentTabKey;
    // The moment (and play state) a tab is put back at once its period is
    // loaded again.
    qint64 m_resumeMomentMs = -1;
    void applyPanelState(bool chartOpen, bool appsOpen, bool panelFull);
    void setChartOpen(bool open);
    void setAppsOpen(bool open);
    // TimeRangeSelector -> GoToHistoryDialog "Change period": a new tab for
    // the selected interval, at the front.
    void onChartInterval(qint64 beginMs, qint64 endMs);
    void showHistoryTab(int index);
    void storeCurrentTab();
    void openAddTabDialog();
    void closeHistoryTab(int index);
    void updateHistoryTabText(int index);
    QList<QPair<quint32, QString>> employeeList() const;
    QList<HistoryTab> m_historyTabs;
    QWidget *m_tabStrip = nullptr;
    ViewerTabBar *m_historyTabBar = nullptr;
    int m_currentTab = -1;
    // The current tab's period; replies are matched by its range key.
    HistoryPeriod m_period;
    QString m_rangeKey;
    void onHistoryFrameMissing(quint32 streamId, quint64 requestId);
    // History.qml's panelFull: clicking the video hides sliderAndMeta and
    // chartsItem so the video (and keylogger bar) take the whole page.
    void togglePanelFull();
    void stepMarkers(int markers);
    // Left/Right reach the player unless a text field is being edited, a
    // modal dialog or popup is up, or the controls are locked.
    bool rewindAllowed() const;
    void updateStatusVisibility();
    void positionOverlays();

    ViewerConnection &m_connection;

    QPushButton *m_toggleAppsButton = nullptr;
    QScrollArea *m_infoArea = nullptr;
    HistoryInfoPanel *m_infoPanel = nullptr;

    // history/Video.qml: header + the horizontally scrollable row of every
    // screen of the current device.
    QWidget *m_videoPanel = nullptr;
    HistoryVideoHeader *m_videoHeader = nullptr;
    QScrollArea *m_monitorStripArea = nullptr;
    HistoryVideoStrip *m_videoStrip = nullptr;
    KeystreamBar *m_keystream = nullptr;
    QWidget *m_controlBlock = nullptr;
    bool m_panelFull = false;
    bool m_chartOpen = false;
    QTableWidget *m_textLog = nullptr;

    // History.qml exportTextButton ("Text"): the KeyloggerExporter flow --
    // the period's keystrokes of the session saved as csv / xlsx.
    QPushButton *m_keylogTableButton = nullptr;
    // History.qml exportAviButton ("Video"): VideoSaverSelector ->
    // VideoSaverProgress, the period saved as a .webm.
    QPushButton *m_exportVideoButton = nullptr;
    QPushButton *m_changeSettingsButton = nullptr;
    QPushButton *m_playButton = nullptr;
    QComboBox *m_speedCombo = nullptr;
    // Audio.qml: only visible at Time step = 1s in the real app (finer
    // steps are the only ones granular enough for audio to make sense
    // alongside). Always disabled here regardless -- no audio capture
    // exists anywhere in this project (KikiHost/KikiSubService).
    QPushButton *m_muteButton = nullptr;
    TimelineWidget *m_timeline = nullptr;
    // Video.qml's centered "excuse" text over the video area
    // ("No information for selected period", ...).
    QLabel *m_statusLabel = nullptr;
    // utils/LoadingStatusDialog.qml equivalent -- a small floating popup
    // ("Downloading...") instead of just the status label text below the
    // video, shown while the initial days/frames request for a device or
    // day is in flight. m_statusLabel is kept too (it also carries
    // non-loading messages like "no history yet"), this is additive.
    QPointer<QWidget> m_loadingDialog; // LoadingOverlay on the window (owned by it)
    void showLoadingDialog(const QString &message);
    void hideLoadingDialog();

    // History.qml's panelViolation button and the chartsItem it opens.
    QPushButton *m_violationToggleButton = nullptr;
    TimeAxisWidget *m_timeAxis = nullptr;
    HistoryChartWidget *m_chart = nullptr;
    HistoryChartsModel *m_chartsModel = nullptr;
    // sliderAndMeta's button columns; its width is History.qml's
    // widthActionButtons, which also decides where the chart grid starts.
    QWidget *m_leftColumn = nullptr;
    // MultiSessionsSlider's column (bar + TimeLine), widthSlider wide.
    QWidget *m_sliderColumn = nullptr;
    void applyTimeStep();

    QTimer *m_playbackTimer = nullptr;
    QList<QIcon> m_playIcons; // normal, hovered (*_active.png)
    QPushButton *m_speedButton = nullptr;
    QWidget *m_speedPopup = nullptr; // SpeedButton.qml's toolTip
    QScrollBar *m_stripScrollBar = nullptr; // Video.qml's hscroll, under the video
    // History.qml statusIcon (typeSize big) over the video: noData / lock /
    // saver for the moment under the playhead, from the period's ranges.
    BigStatusIcon *m_historyStatusIcon = nullptr;
    QList<QPair<qint64, qint64>> m_onlineRanges;
    QList<QPair<qint64, qint64>> m_lockRanges;
    QList<QPair<qint64, qint64>> m_saverRanges;
    QList<QPair<qint64, qint64>> m_noDataRanges;
    bool m_statesLoaded = false;
    // HistoryTab.waitingForLoad: the controls stay locked ("Downloading...")
    // until the period's information is in -- the screens, every screen's
    // frame list and the session states (30 s at most, InfoSelector's
    // timeout) -- and only then does the player open on its first marker.
    bool m_infoPending = false;
    bool m_screensAnswered = false;
    bool m_infoTimedOut = false;
    QTimer *m_infoTimeout = nullptr;
    void checkInfoReady();
    void startPlayer();
    // History.qml animationOpen / animationClose on chartsItem.wantHeight.
    QVariantAnimation *m_chartAnimation = nullptr;
    bool m_chartOpening = false;
    // Video.qml's scrollVideo: narrower by the panel while Running
    // applications is open (rightPadding).
    QHBoxLayout *m_videoBody = nullptr;
    void updateHistoryStatusIcon(qint64 momentMs);
    // History.qml `panel` (Running applications) over the video, and its
    // 250 ms slide.
    QFrame *m_appsPanel = nullptr;
    QPropertyAnimation *m_appsAnimation = nullptr;
    bool m_appsOpen = false;
    void updateAppsButton();
    // A requested frame not back after this long shows the loading spinner.
    QTimer *m_loadingDelay = nullptr;

    void setPlaying(bool playing);
    void updateViolationToggleText();

    QHash<quint32, QString> m_deviceNames; // deviceKey -> display name ("Employee")
    QHash<quint32, QList<quint32>> m_deviceMonitorStreams; // deviceKey -> its monitor streamIds
    QHash<quint32, QString> m_monitorNames; // streamId -> its own display name
    quint32 m_currentDeviceKey = 0;
    // "Time step" (Change settings dialog) -- see applyTimeStep.
    qint64 m_timeStepMs = 5 * 60 * 1000;

    QList<qint64> m_timestamps;
    QList<HistoryAppSegment> m_webVisits;
    QList<HistoryAppSegment> m_appSegments;
    QList<HistoryKeystrokeEntry> m_keystrokeEntries;
    bool m_activated = false;
    bool m_activateWhenReady = false;

    // The connection to the host dropped while a period was open: nothing
    // in flight can be answered any more. On the next authenticated session
    // the period is loaded again (the host may renumber the streams) and the
    // player put back where it was, playing if it was.
    void onConnectionStatus(bool connected);
    void onSessionStarted();
    void resumeAfterReconnect();
    bool m_connectionLost = false;
    bool m_reloadPending = false;
    int m_resumeMarker = -1;
    bool m_resumePlaying = false;

    // The session marker the export dialogs show (Session:). The original
    // prints domain\user@computer; the host only reports the user.
    QString sessionMarker() const;

    // History.qml videoSaverDialogHolder: the PlayerVideoSaver of this page,
    // its VideoSaverProgress box, and the period it exports (its cache tag).
    HistoryVideoSaver *m_videoSaver = nullptr;
    QPointer<QDialog> m_videoSaverBox;
    QWidget *m_videoSaverBar = nullptr;
    QString m_videoSaverTag;
    HistoryVideoJob m_videoSaverJob;
    // Starts the job once the whole period is downloaded (S_waiting until
    // then); false while it still waits.
    bool tryStartVideoJob();
    void showVideoSaverBox();
    void onVideoSaverStateChanged();
    // dispatchPrefetch found nothing left to download in the period and
    // nothing is in flight (videoStore's "downloaded").
    bool m_downloadComplete = false;

    // KeyloggerExporter.qml: the export waits for the period's keystrokes
    // (Keylogger.Selector) and writes them (Result.exportToCsv/Xlsx).
    struct TextExportState {
        bool active = false;
        QString path;
        bool xlsx = false;
        bool cutSystemKeys = false;
        QString rangeKey;
        quint32 streamId = 0;
        QPointer<QDialog> progress;
    };
    TextExportState m_textExport;
    void finishTextExport(const QList<HistoryKeystrokeEntry> &entries);
};
