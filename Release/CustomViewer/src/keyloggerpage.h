#pragma once

#include "keyloggermodel.h"
#include "viewerconnection.h"

#include <QPointer>
#include <QWidget>

class KeyloggerChart;
class KeyloggerTable;
class MultiSwitch;
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTimer;

// trackerQuadratorActiveCell/Keylogger.qml: the employee's Keylogger tab --
// Toolbar (subject switch, Search, Hide system keys, Autoupdateing,
// Export), the Table of records, "The intensity of keystrokes for today",
// the ChartToolbar (Range / Step) and the Chart. One Selector per session
// marker loads [today 00:00, now); the Result and the Chart are derived from
// its events with the toolbar's settings, locally.
class KeyloggerPage final : public QWidget
{
    Q_OBJECT

public:
    KeyloggerPage(ViewerConnection &connection, QWidget *parent = nullptr);

    // The employee (deviceKey), its primary stream (the anchor of the
    // queries), its name and the selected session's marker. A different
    // session starts a new Selector.
    void setSession(quint32 deviceKey, quint32 streamId, const QString &employeeName,
                    const SessionIdentity &identity);
    // The TabView the tab lives in (the employee's page, header included):
    // the "Keylogger does not capture" box is placed on it, 17px down and
    // 20px in from its right edge, as the original parents it to
    // keylogger.parent.
    void setWarningHost(QWidget *host);
    // The Keylogger tab became the current one / stopped being it. The page
    // works (loads, autoupdates) only while it is the current tab AND shown
    // -- closing the employee or leaving the Tracker hides it.
    void setActive(bool active);

signals:
    // Chart -> GoToHistoryDialog ("Jump to History") -> windowContent.jmp
    // ("history", {employeeId, type: "custom", customBegin, customEnd,
    // timeStep}).
    void goToHistory(quint32 deviceKey, const QDateTime &begin, const QDateTime &end, qint64 timeStepMs);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    // Selector: (re)load [intervalBegin, intervalEnd).
    void load();
    void onEvents(quint32 streamId, const QString &tag, const KeyEventsReply &reply);
    void onError(quint32 streamId, const QString &tag, const QString &message);
    // Selector failures that are not the host's answer: the connection
    // dropped, or no answer came.
    void failRequest(const QString &message, bool reloadOnReconnect);
    void onConnectionStatus(bool connected);
    void onSessionStarted();
    // m_active && shown: start / stop the work accordingly.
    bool isWorking() const;
    void updateWorking();
    // employee.v2oKeyloggerEnabled / dcKeyloggerEnabled.
    void updateDisabledStates();
    void positionOverlays();
    // Result / Chart from the loaded events and the toolbar.
    void rebuildResult();
    void rebuildChart();
    void updateStates();
    void exportClicked();
    void openGoToHistory(const QDateTime &left, const QDateTime &right);

    ViewerConnection &m_connection;
    quint32 m_deviceKey = 0;
    quint32 m_streamId = 0;
    QString m_employeeName;
    SessionIdentity m_identity;
    bool m_active = false;

    // Selector state (S_loading / S_loadedPartial / S_loadedComplete / error).
    enum class Status { Idle, Loading, LoadedPartial, LoadedComplete, Error };
    Status m_status = Status::Idle;
    // What the data on display came as (complete / partial).
    Status m_loadedStatus = Status::Idle;
    QString m_requestTag;
    quint64 m_generation = 0;
    qint64 m_intervalBegin = 0;
    qint64 m_intervalEnd = 0;
    QList<KeyEvent> m_events;
    QList<LegacyKeyChunk> m_legacy;
    QList<KeyloggerRow> m_rows; // the current Result.content
    QString m_errorString;
    QString m_partialReason;
    bool m_working = false;
    bool m_reloadOnReconnect = false;

    // Toolbar.qml
    MultiSwitch *m_subject = nullptr;
    QLineEdit *m_search = nullptr;
    QCheckBox *m_cutSystemKeys = nullptr;
    QCheckBox *m_autoupdate = nullptr;
    QPushButton *m_exportButton = nullptr;
    QWidget *m_toolbar = nullptr;
    KeyloggerTable *m_table = nullptr;
    QLabel *m_chartTitle = nullptr;
    // ChartToolbar.qml
    MultiSwitch *m_range = nullptr;
    MultiSwitch *m_step = nullptr;
    QWidget *m_chartToolbar = nullptr;
    KeyloggerChart *m_chart = nullptr;
    // The khaki loading box with its spinner, centered on the tab.
    QWidget *m_spinnerBox = nullptr;
    // A failed / partial load, said in words above the table.
    QLabel *m_errorLabel = nullptr;
    // "viewer keylogger disabled" (Fonts.rr_xtra_b, in the table's place).
    QLabel *m_disabledLabel = nullptr;
    // "Keylogger does not capture": warning_bg.png box at the tab's top
    // right (17px down, 20px in).
    QWidget *m_captureWarning = nullptr;
    QTimer *m_autoupdateTimer = nullptr;
    // A request with no answer is given up after this.
    QTimer *m_requestTimer = nullptr;
    QPointer<QWidget> m_dialog;
};
