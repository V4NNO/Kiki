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
    // The tab became visible / hidden: the Selector only lives while shown.
    void setActive(bool active);

signals:
    // Chart -> GoToHistoryDialog ("Jump to History") -> windowContent.jmp
    // ("history", {employeeId, type: "custom", customBegin, customEnd,
    // timeStep}).
    void goToHistory(quint32 deviceKey, const QDateTime &begin, const QDateTime &end, qint64 timeStepMs);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    // Selector: (re)load [intervalBegin, intervalEnd).
    void load();
    void onEvents(quint32 streamId, const QString &tag, const QList<KeyEvent> &events, bool complete);
    void onError(quint32 streamId, const QString &tag, const QString &message);
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
    QString m_requestTag;
    quint64 m_generation = 0;
    qint64 m_intervalBegin = 0;
    qint64 m_intervalEnd = 0;
    QList<KeyEvent> m_events;
    QList<KeyloggerRow> m_rows; // the current Result.content
    QString m_errorString;

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
    QLabel *m_errorLabel = nullptr;
    QTimer *m_autoupdateTimer = nullptr;
    QPointer<QWidget> m_dialog;
};
