#pragma once

#include "viewerconnection.h"

#include <QHash>
#include <QWidget>

class QComboBox;
class QLabel;
class QListWidget;
class QPushButton;
class QSlider;
class QTextEdit;
class QTimer;

// Vertical-bar chart of input-activity intensity, mapped across
// [rangeStartMs, rangeEndMs]. Height is normalized to the loudest sample in
// the current set, not to some fixed scale, so a quiet day still shows
// visible variation instead of a flat line near zero.
class ActivityBarWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit ActivityBarWidget(QWidget *parent = nullptr);
    void setSamples(const QList<HistoryActivitySample> &samples, qint64 rangeStartMs,
                    qint64 rangeEndMs);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<HistoryActivitySample> m_samples;
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
};

// Horizontal colored segments (green/yellow/red = productive/neutral/
// unproductive) showing which category of application was active across
// [rangeStartMs, rangeEndMs].
class EfficiencyBarWidget final : public QWidget
{
    Q_OBJECT

public:
    explicit EfficiencyBarWidget(QWidget *parent = nullptr);
    void setSegments(const QList<HistoryAppSegment> &segments,
                     const QHash<QString, QString> &categories, qint64 rangeStartMs,
                     qint64 rangeEndMs);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<HistoryAppSegment> m_segments;
    QHash<QString, QString> m_categories;
    qint64 m_rangeStart = 0;
    qint64 m_rangeEnd = 0;
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

    void setMonitors(const QHash<quint32, QString> &monitorNames);
    // Called whenever this page becomes the visible one; (re)kicks off the
    // monitor->days->frame request chain if nothing is loaded yet.
    void activate();

private slots:
    void onDaysReceived(quint32 streamId, const QStringList &days);
    void onFramesReceived(quint32 streamId, const QString &day, const QList<qint64> &timestamps);
    void onFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image);
    void onActivityReceived(quint32 streamId, const QString &day,
                            const QList<HistoryActivitySample> &samples);
    void onAppSegmentsReceived(quint32 streamId, const QString &day,
                               const QList<HistoryAppSegment> &segments);
    void onRunningAppsReceived(quint32 streamId, const QString &day,
                               const QList<HistoryAppUsage> &applications);
    void onCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories);
    void onKeystrokesReceived(quint32 streamId, const QString &day,
                              const QList<HistoryKeystrokeEntry> &entries);
    void onHistoryError(quint32 streamId, const QString &message);

    void onMonitorChanged(int index);
    void onDayChanged(int index);
    void onTimelineMoved(int index);
    void onPlayClicked();
    void onPlaybackTick();
    void onVideoTabClicked();
    void onTextTabClicked();
    void onToggleRunningApps();
    void onChangeSettingsClicked();
    void onSpeedChanged(int index);

private:
    quint32 currentStreamId() const;
    void requestFrameAt(int index);
    void refreshDayDependentData();
    void updateTextLogHighlight(qint64 timestampMs);
    void rebuildRunningAppsList();

    ViewerConnection &m_connection;

    QComboBox *m_monitorCombo = nullptr;
    QComboBox *m_dayCombo = nullptr;
    QLabel *m_timeLabel = nullptr;

    QPushButton *m_toggleAppsButton = nullptr;
    QListWidget *m_runningAppsList = nullptr;

    QLabel *m_imageLabel = nullptr;
    QTextEdit *m_textLog = nullptr;

    QPushButton *m_videoTabButton = nullptr;
    QPushButton *m_textTabButton = nullptr;
    QPushButton *m_changeSettingsButton = nullptr;
    QPushButton *m_playButton = nullptr;
    QComboBox *m_speedCombo = nullptr;
    QSlider *m_timelineSlider = nullptr;
    QLabel *m_statusLabel = nullptr;

    ActivityBarWidget *m_activityBar = nullptr;
    EfficiencyBarWidget *m_efficiencyBar = nullptr;

    QTimer *m_playbackTimer = nullptr;

    void refreshEfficiencyBar();

    QHash<quint32, QString> m_monitorNames;
    QList<qint64> m_timestamps;
    QHash<QString, QString> m_categories;
    QList<HistoryAppUsage> m_runningApps;
    QList<HistoryAppSegment> m_appSegments;
    QList<HistoryKeystrokeEntry> m_keystrokeEntries;
    bool m_activated = false;
};
