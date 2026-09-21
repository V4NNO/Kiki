#pragma once

#include <QByteArray>
#include <QHash>
#include <QImage>
#include <QList>
#include <QObject>
#include <QPair>
#include <QSqlDatabase>
#include <QString>
#include <QStringList>

struct AppSegment {
    QString application;
    qint64 startMs = 0;
    qint64 endMs = 0;
};

struct AppUsage {
    QString application;
    qint64 totalMs = 0;
    QString category; // "productive" | "neutral" | "unproductive"
};

struct ActivitySample {
    qint64 timestampMs = 0;
    int inputEvents = 0;
};

struct KeystrokeEntry {
    qint64 timestampMs = 0;
    QString windowTitle;
    QString text;
};

// Writes a low-rate, independent snapshot timeline to a local SQLite index
// (frames.sqlite) plus JPEG files on disk, separate from the live PSV1
// stream sent to viewers. Deliberately much lower frequency than the live
// feed -- see polished-singing-seahorse.md Phase 2 for the disk-usage
// numbers behind the default 10s-per-monitor rate -- so this is a
// "what was on screen around time X" timeline, not frame-accurate video.
//
// Also stores the data behind the History view's Activity/Efficiency bars,
// Running Applications panel and (per explicit, informed user request) a
// keylogger text timeline -- see polished-singing-seahorse.md for the
// policy discussion that preceded adding that last one.
class HistoryRecorder final : public QObject
{
    Q_OBJECT

public:
    explicit HistoryRecorder(QString historyDir, QObject *parent = nullptr);
    ~HistoryRecorder() override;

    bool start(QString *error);
    bool isValid() const { return m_database.isOpen(); }

    // Rate-limited internally per monitorStreamId; safe to call on every
    // captured frame, most calls will just be dropped.
    void recordFrame(quint32 sessionId, const QString &sessionUsername, quint32 monitorStreamId,
                     const QString &monitorName, const QImage &image);

    // Timestamped as "now" each time; call on whatever cadence the caller
    // samples activity at (PersonalSubService samples every 10s).
    void recordActivity(quint32 sessionId, quint32 monitorStreamId, int inputEvents);

    // Extends the currently open segment for (sessionId, monitorStreamId)
    // if application is unchanged, otherwise closes it and opens a new one.
    // Safe to call as often as metadata arrives.
    void noteApplication(quint32 sessionId, quint32 monitorStreamId, const QString &application);

    void recordKeystroke(quint32 sessionId, const QString &windowTitle, const QString &text);

    void setCategory(const QString &application, const QString &category);

    // Read side, used to answer viewer HistoryQuery requests. All return
    // empty on failure or when nothing matches; never throw/assert on a
    // client-supplied streamId/day/timestamp that turns out not to exist.
    QStringList listDays(quint32 monitorStreamId) const;
    QList<qint64> listFrameTimestamps(quint32 monitorStreamId, const QString &day) const;
    QByteArray readFrame(quint32 monitorStreamId, qint64 timestampMs) const;
    QList<ActivitySample> listActivity(quint32 monitorStreamId, const QString &day) const;
    QList<AppSegment> listAppSegments(quint32 monitorStreamId, const QString &day) const;
    QList<AppUsage> listRunningApplications(quint32 monitorStreamId, const QString &day) const;
    QList<QPair<QString, QString>> listCategories() const;
    QList<KeystrokeEntry> listKeystrokes(quint32 sessionId, const QString &day) const;

signals:
    void logMessage(const QString &message);

private:
    struct OpenSegment {
        QString application;
        qint64 rowId = -1;
    };

    QString category(const QString &application) const;

    QString m_historyDir;
    QSqlDatabase m_database;
    QHash<quint32, qint64> m_lastRecordedMs;
    int m_minIntervalMs = 10000;
    QHash<quint64, OpenSegment> m_openSegments; // key: (sessionId << 32) | monitorStreamId
};
