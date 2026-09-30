#pragma once

#include "viewerconnection.h"

#include <QDate>
#include <QDialog>
#include <QHash>
#include <QColor>
#include <QWidget>

class QComboBox;
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
    };
    explicit HistoryInfoPanel(QWidget *parent = nullptr);
    void setItems(const QList<Item> &webPages, const QList<Item> &programs);

signals:
    void categorizationRequested(const QString &resource);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

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

    QList<Item> m_webPages;
    QList<Item> m_programs;
    QList<Row> m_rows;
    QList<Title> m_titles;
    QList<int> m_separatorsY;
    int m_hoveredRow = -1;
};

// utils/sessionInfo/CategorizationPanel.qml (a GenericBox): "Efficiency
// <resource>" and one categorizationPanel/Row.qml per level -- the
// organization, then the employee -- each with Productive / Neutral /
// Unproductive / Uncategorized, Cancel / OK. The organization row is the
// global rule, the employee row that employee's own override
// (Uncategorized = no override, inherit the global one).
class CategorizationDialog final : public QDialog
{
    Q_OBJECT

public:
    CategorizationDialog(const QString &resource, const QString &globalCategory,
                         const QString &employeeCategory, const QString &employeeName,
                         QWidget *parent = nullptr);
    QString category() const { return m_category; }
    QString employeeCategory() const { return m_employeeCategory; }

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QRect ratingRect(int row, int option) const;
    QRect cancelRect() const;
    QRect okRect() const;
    QRect closeRect() const;

    QString m_title;
    QString m_employeeName;
    QString m_category;
    QString m_employeeCategory;
    QPoint m_hover;
};

// utils/HistoryChoicePanel.qml (a GenericBox) in its "add" ("Add history
// watching") and "change" ("Change range and employee") modes: Employee,
// Period (TimeRangeReport: kind + a ◀▶ period field) and Time step (with
// the "no audio" info icon), Cancel / OK. History here works one day at a
// time, so Day is the only period kind that can be chosen.
class HistoryChoiceDialog final : public QDialog
{
    Q_OBJECT

public:
    enum class Mode { Add, Change };
    HistoryChoiceDialog(Mode mode, const QList<QPair<quint32, QString>> &employees, quint32 employee,
                        const QDate &day, qint64 timeStepMs, QWidget *parent = nullptr);
    quint32 employee() const { return m_employee; }
    QDate day() const { return m_day; }
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

    QString m_title;
    QList<QPair<quint32, QString>> m_employees;
    quint32 m_employee = 0;
    QDate m_day;
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
// sliderBar/*.png assets (bg_none where nothing was recorded,
// bg_loaded_cropped where frames exist) plus the 19x19 pick handle.
// Selection is by index into the captured-frame timestamps; on-screen
// position is time-of-day across [rangeStart, rangeEnd].
class TimelineWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    void setTimestamps(const QList<qint64> &timestamps);
    void setRange(qint64 rangeStartMs, qint64 rangeEndMs);
    // Marker granularity (History's "Time step") -- SliderBar.qml widens
    // every recorded range by one marker, and it decides how far apart two
    // frames may be while still counting as one continuous recorded range.
    void setStepMs(qint64 stepMs);
    void setCurrentIndex(int index); // does not emit indexSelected
    int currentIndex() const { return m_currentIndex; }
    int count() const { return m_timestamps.size(); }

signals:
    void indexSelected(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    int indexForX(int x) const;
    void seekToX(int x);
    int xForTime(qint64 timestampMs) const;
    QRect pickRect() const;

    QList<qint64> m_timestamps;
    int m_currentIndex = -1;
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
    qint64 m_stepMs = 5 * 60 * 1000;
    bool m_pickHovered = false;
    bool m_pressed = false;
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

// history/Filters.qml -> utils/Chart.qml with needPlayerLine=true, laid out
// the way it is inside History's 186px chartsItem: ExtraHeaders labels
// ("Activity"/"Efficiency") on the left, then over a Grid.qml backdrop the
// HistoLine activity histogram (#7aa1e2) and the Line.qml productivity row
// (equal-height stacked category bands per chart step), a double-line
// separator, the (empty here) filters grid below, and the khaki
// HistoryPlayerMarkerControl line across the whole chart.
class HistoryChartWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryChartWidget(QWidget *parent = nullptr);
    void setRange(qint64 rangeStartMs, qint64 rangeEndMs);
    // History's raw "Time step"; the chart's own step is derived from it the
    // way HistoryTab.qml does (alingStep, at most 60 grid columns).
    void setStepMs(qint64 stepMs);
    // x (in this widget) where the grid starts -- History.qml's
    // labelsAreaLeftMargin, i.e. the width of the button columns.
    void setGridLeft(int x);
    void setActivity(const QList<HistoryActivitySample> &samples);
    void setEfficiency(const QList<HistoryAppSegment> &segments,
                       const QHash<QString, QString> &categories);
    void setCurrentPositionMs(qint64 positionMs);
    // Violations tab (trackerQuadratorActiveCell): the Chart isn't the fixed
    // 186px History strip with a filters area below -- it fills the whole tab,
    // with the grid full-height and only the Activity/Efficiency rows at top.
    void setFillMode(bool on);

    static qint64 chartStepMs(qint64 rangeMs, qint64 userStepMs);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<HistoryActivitySample> m_samples;
    QList<HistoryAppSegment> m_segments;
    QHash<QString, QString> m_categories;
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
    qint64 m_stepMs = 5 * 60 * 1000;
    qint64 m_currentPositionMs = 0;
    int m_gridLeft = 0;
    bool m_fillMode = false;
};

// The full embedded History page (replaces what used to be a separate
// QDialog): monitor/day pickers, a Video/Text switch, Running Applications
// side panel, transport controls, timeline scrubber, and the Activity/
// Efficiency bars underneath it -- modeled on a real Kickidler History
// screenshot the user provided, not on guesswork (see
// polished-singing-seahorse.md for the fidelity/scope decisions made with
// the user, including keylogging being explicitly requested and confirmed).
class HistoryView final : public QWidget
{
    Q_OBJECT

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
    // to this device and jumps straight to the given day ("yyyyMMdd",
    // defaults to today) once its day list arrives.
    void openForDevice(quint32 deviceKey, const QString &day = QString());

private slots:
    void onDaysReceived(quint32 streamId, const QStringList &days);
    void onFramesReceived(quint32 streamId, const QString &day, const QList<qint64> &timestamps);
    void onFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image);
    void onActivityReceived(quint32 streamId, const QString &day,
                            const QList<HistoryActivitySample> &samples);
    void onAppSegmentsReceived(quint32 streamId, const QString &day,
                               const QList<HistoryAppSegment> &segments);
    void onWebVisitsReceived(quint32 streamId, const QString &day,
                             const QList<HistoryAppSegment> &visits);
    void onCategorizationRequested(const QString &resource);
    void onEmployeeCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories);
    void onCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories);
    void onKeystrokesReceived(quint32 streamId, const QString &day,
                              const QList<HistoryKeystrokeEntry> &entries);
    void onHistoryError(quint32 streamId, const QString &message);

    void onDayChanged(int index);
    void onTimelineMoved(int index);
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
    // DeviceDetailView's identical assumption); PersonalHost just happens to
    // record them once per monitor, so any one of the device's streams has
    // the full picture -- no merging across monitors needed.
    quint32 currentStreamId() const;
    void switchDevice(quint32 deviceKey);
    void rebuildMonitorStrip();
    void requestFrameAt(int index);
    void refreshDayDependentData();
    void updateTextLogHighlight(qint64 timestampMs);
    void updateKeystream(qint64 timestampMs);
    void updateInfoPanel();
    // Global categories with this employee's own overrides applied.
    QHash<QString, QString> effectiveCategories() const;
    // The day the current tab shows ("yyyyMMdd"), shown even when nothing
    // was recorded that day ("No information for selected period").
    void selectDay(const QString &day);
    void showNoData();
    void jumpTo(quint32 deviceKey, const QString &day);

    // HistoryPanel.qml / ViewerControls/Tabs.qml: one tab per history
    // watching (employee + day + Time step), "+" opens "Add history
    // watching", closing asks for confirmation.
    struct HistoryTab {
        quint32 deviceKey = 0;
        QString day;
        qint64 timeStepMs = 5 * 60 * 1000;
    };
    void addHistoryTab(const HistoryTab &tab);
    void showHistoryTab(int index);
    void storeCurrentTab();
    void openAddTabDialog();
    void closeHistoryTab(int index);
    void updateHistoryTabText(int index);
    QList<QPair<quint32, QString>> employeeList() const;
    QList<HistoryTab> m_historyTabs;
    QTabBar *m_historyTabBar = nullptr;
    int m_currentTab = -1;
    QString m_currentDay;
    void onHistoryFrameMissing(quint32 streamId);
    // History.qml's panelFull: clicking the video hides sliderAndMeta and
    // chartsItem so the video (and keylogger bar) take the whole page.
    void togglePanelFull();
    void stepMarkers(int markers);
    void updateStatusVisibility();
    void positionOverlays();

    ViewerConnection &m_connection;

    QComboBox *m_dayCombo = nullptr;

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

    // Opens the grouped keylog table (see m_textLog) in a popup dialog --
    // that table is the real Tracker-tile Keylogger tab's shape
    // (Table.qml), not a "mode" of History's main view.
    QPushButton *m_keylogTableButton = nullptr;
    // Exports the current day's frames as a numbered PNG sequence -- see
    // onExportVideoClicked's comment for why not an actual video file.
    QPushButton *m_exportVideoButton = nullptr;
    QPushButton *m_changeSettingsButton = nullptr;
    QPushButton *m_playButton = nullptr;
    QComboBox *m_speedCombo = nullptr;
    // Audio.qml: only visible at Time step = 1s in the real app (finer
    // steps are the only ones granular enough for audio to make sense
    // alongside). Always disabled here regardless -- no audio capture
    // exists anywhere in this project (PersonalHost/PersonalSubService).
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
    QDialog *m_loadingDialog = nullptr;
    QLabel *m_loadingLabel = nullptr;
    void showLoadingDialog(const QString &message);
    void hideLoadingDialog();

    // History.qml's panelViolation button and the chartsItem it opens.
    QPushButton *m_violationToggleButton = nullptr;
    TimeAxisWidget *m_timeAxis = nullptr;
    HistoryChartWidget *m_chart = nullptr;
    // sliderAndMeta's button columns; its width is History.qml's
    // widthActionButtons, which also decides where the chart grid starts.
    QWidget *m_leftColumn = nullptr;
    void applyTimeStep();

    QTimer *m_playbackTimer = nullptr;

    void refreshEfficiencyBar();
    void setPlaying(bool playing);
    void updateViolationToggleText();

    QHash<quint32, QString> m_deviceNames; // deviceKey -> display name ("Employee")
    QHash<quint32, QList<quint32>> m_deviceMonitorStreams; // deviceKey -> its monitor streamIds
    QHash<quint32, QString> m_monitorNames; // streamId -> its own display name
    quint32 m_currentDeviceKey = 0;
    // Days with history for the current employee (m_dayCombo's items).
    QStringList m_allDays;
    // "Time step" (Change settings dialog) -- see applyTimeStep.
    qint64 m_timeStepMs = 5 * 60 * 1000;

    QList<qint64> m_timestamps;
    QHash<QString, QString> m_categories;
    QHash<QString, QString> m_employeeCategories;
    QList<HistoryAppSegment> m_webVisits;
    QList<HistoryAppSegment> m_appSegments;
    QList<HistoryActivitySample> m_activitySamples;
    QList<HistoryKeystrokeEntry> m_keystrokeEntries;
    bool m_activated = false;

    // In-progress "export imagini" job (see onExportVideoClicked): requests
    // one frame at a time (via the same requestHistoryFrame/onFrameReceived
    // path normal playback uses) and saves it to disk, so it doesn't need
    // its own protocol message.
    struct VideoExportState {
        bool active = false;
        QString directory;
        quint32 streamId = 0;
        QList<qint64> pending;
        int total = 0;
        // VideoSaverSelector.qml's quality picker -- we still write PNGs
        // (no video encoder here, see m_exportVideoButton), but quality
        // isn't purely cosmetic: it scales the exported image resolution
        // (100/75/50%), same trade-off the real quality picker makes.
        int qualityPercent = 100;
    };
    VideoExportState m_videoExport;
    void exportNextVideoFrame();
};
