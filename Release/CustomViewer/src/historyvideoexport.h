#pragma once

#include "webmwriter.h"

#include <QHash>
#include <QList>
#include <QObject>
#include <QPointer>
#include <QString>

#include <atomic>
#include <memory>

class QThread;

// What PlayerVideoSaver exports, decided on the UI thread from the player's
// own data: the period's markers that have a picture on at least one screen
// (markers with none are dropped and take no time), and for each of them
// where in the downloaded video every screen's picture is.
struct HistoryVideoJob {
    QString cacheFile;   // the period's vhdb.<n>.sqlite (HistorySegmentStore)
    QString destination; // the .webm
    double mps = 1.0;    // SpeedButton's speed: each marker lasts 1/mps s
    QString trackName;   // "History of <session>"
    WebmWriter::Quality quality = WebmWriter::Quality::Good;
    QList<quint32> streams; // screen order
    struct Marker {
        QHash<quint32, qint64> lookupMs; // screen -> moment of its picture
    };
    QList<Marker> markers;
};

// PlayerVideoSaver (videoSaver.cpp) with its VideoComposer worker: the
// states, the progress and the error strings of the original. start()
// runs the job on its own thread, reading the downloaded video through a
// connection of its own to the period's cache file -- so the export goes on
// whatever the player does meanwhile.
class HistoryVideoSaver final : public QObject
{
    Q_OBJECT

public:
    // PlayerVideoSaver.S_*.
    enum class State { Ready = 1, Waiting, Working, Error, Done, Canceled };

    explicit HistoryVideoSaver(QObject *parent = nullptr);
    ~HistoryVideoSaver() override;

    State state() const { return m_state; }
    bool busy() const { return m_state == State::Waiting || m_state == State::Working; }
    double progress() const { return m_progress; }
    QString errorString() const { return m_error; }

    // S_waiting: the period is still downloading; the job starts once
    // startJob() is called.
    void wait();
    void startJob(const HistoryVideoJob &job);
    void cancel();
    void fail(const QString &message);

signals:
    void stateChanged();
    void progressChanged(double progress);

private:
    void setState(State state);
    void onWorkerProgress(double progress);
    void onWorkerFinished(bool ok, const QString &error);

    State m_state = State::Ready;
    double m_progress = 0.0;
    QString m_error;
    QThread *m_thread = nullptr;
    std::shared_ptr<std::atomic<bool>> m_cancelFlag;
};
