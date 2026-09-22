#include "historyrecorder.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QSqlError>
#include <QSqlQuery>

#include <algorithm>

namespace {
constexpr auto kConnectionName = "personalhost_history";
}

HistoryRecorder::HistoryRecorder(QString historyDir, QObject *parent)
    : QObject(parent)
    , m_historyDir(std::move(historyDir))
{
}

HistoryRecorder::~HistoryRecorder()
{
    if (m_database.isOpen()) {
        m_database.close();
    }
    m_database = QSqlDatabase();
    QSqlDatabase::removeDatabase(QString::fromLatin1(kConnectionName));
}

bool HistoryRecorder::start(QString *error)
{
    if (!QDir().mkpath(m_historyDir)) {
        if (error) {
            *error = QStringLiteral("Nu pot crea directorul de istoric: %1").arg(m_historyDir);
        }
        return false;
    }

    m_database = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QString::fromLatin1(kConnectionName));
    m_database.setDatabaseName(QDir(m_historyDir).filePath(QStringLiteral("history.sqlite")));
    if (!m_database.open()) {
        if (error) {
            *error = QStringLiteral("Nu pot deschide history.sqlite: %1")
                         .arg(m_database.lastError().text());
        }
        return false;
    }

    QSqlQuery query(m_database);
    const bool created = query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS frames ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  session_username TEXT NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"
        "  monitor_name TEXT NOT NULL,"
        "  timestamp_ms INTEGER NOT NULL,"
        "  file_path TEXT NOT NULL,"
        "  width INTEGER NOT NULL,"
        "  height INTEGER NOT NULL"
        ")"));
    if (!created) {
        if (error) {
            *error = QStringLiteral("Nu pot crea tabela frames: %1").arg(query.lastError().text());
        }
        return false;
    }
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_frames_monitor_time ON frames(monitor_stream_id, timestamp_ms)"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS activity ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"
        "  timestamp_ms INTEGER NOT NULL,"
        "  input_count INTEGER NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_activity_monitor_time ON activity(monitor_stream_id, timestamp_ms)"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS app_segments ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"
        "  application TEXT NOT NULL,"
        "  start_ms INTEGER NOT NULL,"
        "  end_ms INTEGER NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_app_segments_monitor_time "
        "ON app_segments(monitor_stream_id, start_ms)"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS web_visits ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  monitor_stream_id INTEGER NOT NULL,"
        "  url TEXT NOT NULL,"
        "  start_ms INTEGER NOT NULL,"
        "  end_ms INTEGER NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_web_visits_monitor_time "
        "ON web_visits(monitor_stream_id, start_ms)"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS keystrokes ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  session_id INTEGER NOT NULL,"
        "  timestamp_ms INTEGER NOT NULL,"
        "  window_title TEXT NOT NULL,"
        "  text TEXT NOT NULL"
        ")"));
    query.exec(QStringLiteral(
        "CREATE INDEX IF NOT EXISTS idx_keystrokes_session_time ON keystrokes(session_id, timestamp_ms)"));

    query.exec(QStringLiteral(
        "CREATE TABLE IF NOT EXISTS app_categories ("
        "  application TEXT PRIMARY KEY,"
        "  category TEXT NOT NULL"
        ")"));

    emit logMessage(QStringLiteral("Istoric activat, rata 1 cadru/%1s per monitor, in %2")
                        .arg(m_minIntervalMs / 1000)
                        .arg(m_historyDir));
    return true;
}

void HistoryRecorder::recordFrame(quint32 sessionId, const QString &sessionUsername,
                                  quint32 monitorStreamId, const QString &monitorName,
                                  const QImage &image)
{
    if (image.isNull()) {
        return;
    }
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    qint64 &lastRecorded = m_lastRecordedMs[monitorStreamId];
    if (lastRecorded != 0 && now - lastRecorded < m_minIntervalMs) {
        return;
    }
    lastRecorded = now;

    const QDateTime timestamp = QDateTime::fromMSecsSinceEpoch(now);
    const QString relativeDir = QStringLiteral("frames/%1/%2/%3")
                                     .arg(sessionId)
                                     .arg(monitorStreamId)
                                     .arg(timestamp.toString(QStringLiteral("yyyyMMdd")));
    const QString absoluteDir = QDir(m_historyDir).filePath(relativeDir);
    if (!QDir().mkpath(absoluteDir)) {
        emit logMessage(QStringLiteral("Nu pot crea directorul de istoric %1").arg(absoluteDir));
        return;
    }
    const QString fileName = timestamp.toString(QStringLiteral("HHmmss_zzz")) + QStringLiteral(".jpg");
    const QString relativePath = relativeDir + QLatin1Char('/') + fileName;
    const QString absolutePath = QDir(absoluteDir).filePath(fileName);

    if (!image.save(absolutePath, "JPEG", 75)) {
        emit logMessage(QStringLiteral("Nu am putut salva captura de istoric la %1").arg(absolutePath));
        return;
    }

    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO frames (session_id, session_username, monitor_stream_id, monitor_name, "
        "timestamp_ms, file_path, width, height) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"));
    query.addBindValue(sessionId);
    query.addBindValue(sessionUsername);
    query.addBindValue(monitorStreamId);
    query.addBindValue(monitorName);
    query.addBindValue(now);
    query.addBindValue(relativePath);
    query.addBindValue(image.width());
    query.addBindValue(image.height());
    if (!query.exec()) {
        emit logMessage(QStringLiteral("Nu am putut scrie randul de istoric: %1")
                            .arg(query.lastError().text()));
    }
}

QStringList HistoryRecorder::listDays(quint32 monitorStreamId) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    // 'localtime' matches how the day folders themselves were named:
    // recordFrame() derives them from QDateTime::fromMSecsSinceEpoch(),
    // which is local time by default.
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT DISTINCT strftime('%Y%m%d', timestamp_ms / 1000, 'unixepoch', 'localtime') AS day "
        "FROM frames WHERE monitor_stream_id = ? ORDER BY day DESC"));
    query.addBindValue(monitorStreamId);
    QStringList days;
    if (query.exec()) {
        while (query.next()) {
            days.append(query.value(0).toString());
        }
    }
    return days;
}

QList<qint64> HistoryRecorder::listFrameTimestamps(quint32 monitorStreamId, const QString &day) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT timestamp_ms FROM frames WHERE monitor_stream_id = ? "
        "AND strftime('%Y%m%d', timestamp_ms / 1000, 'unixepoch', 'localtime') = ? "
        "ORDER BY timestamp_ms ASC"));
    query.addBindValue(monitorStreamId);
    query.addBindValue(day);
    QList<qint64> timestamps;
    if (query.exec()) {
        while (query.next()) {
            timestamps.append(query.value(0).toLongLong());
        }
    }
    return timestamps;
}

QByteArray HistoryRecorder::readFrame(quint32 monitorStreamId, qint64 timestampMs) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT file_path FROM frames WHERE monitor_stream_id = ? AND timestamp_ms = ? LIMIT 1"));
    query.addBindValue(monitorStreamId);
    query.addBindValue(timestampMs);
    if (!query.exec() || !query.next()) {
        return {};
    }
    QFile file(QDir(m_historyDir).filePath(query.value(0).toString()));
    if (!file.open(QIODevice::ReadOnly)) {
        return {};
    }
    return file.readAll();
}

void HistoryRecorder::recordActivity(quint32 sessionId, quint32 monitorStreamId, int inputEvents)
{
    if (!m_database.isOpen()) {
        return;
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO activity (session_id, monitor_stream_id, timestamp_ms, input_count) "
        "VALUES (?, ?, ?, ?)"));
    query.addBindValue(sessionId);
    query.addBindValue(monitorStreamId);
    query.addBindValue(QDateTime::currentMSecsSinceEpoch());
    query.addBindValue(inputEvents);
    if (!query.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut scrie randul de activitate: %1").arg(query.lastError().text()));
    }
}

void HistoryRecorder::noteApplication(quint32 sessionId, quint32 monitorStreamId,
                                      const QString &application)
{
    if (!m_database.isOpen() || application.isEmpty()) {
        return;
    }
    const quint64 key = (static_cast<quint64>(sessionId) << 32) | monitorStreamId;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    auto it = m_openSegments.find(key);

    if (it != m_openSegments.end() && it->application == application) {
        QSqlQuery update(m_database);
        update.prepare(QStringLiteral("UPDATE app_segments SET end_ms = ? WHERE id = ?"));
        update.addBindValue(now);
        update.addBindValue(it->rowId);
        update.exec();
        return;
    }

    QSqlQuery insert(m_database);
    insert.prepare(QStringLiteral(
        "INSERT INTO app_segments (session_id, monitor_stream_id, application, start_ms, end_ms) "
        "VALUES (?, ?, ?, ?, ?)"));
    insert.addBindValue(sessionId);
    insert.addBindValue(monitorStreamId);
    insert.addBindValue(application);
    insert.addBindValue(now);
    insert.addBindValue(now);
    if (!insert.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut scrie segmentul de aplicatie: %1").arg(insert.lastError().text()));
        return;
    }
    m_openSegments[key] = OpenSegment{application, insert.lastInsertId().toLongLong()};
}

void HistoryRecorder::noteUrl(quint32 sessionId, quint32 monitorStreamId, const QString &url)
{
    if (!m_database.isOpen()) {
        return;
    }
    const quint64 key = (static_cast<quint64>(sessionId) << 32) | monitorStreamId;
    const qint64 now = QDateTime::currentMSecsSinceEpoch();
    auto it = m_openWebSegments.find(key);

    if (url.isEmpty()) {
        // Not on a (recognized) browser right now -- just close whatever
        // was open, if anything. No blank row for "not browsing".
        if (it != m_openWebSegments.end()) {
            QSqlQuery update(m_database);
            update.prepare(QStringLiteral("UPDATE web_visits SET end_ms = ? WHERE id = ?"));
            update.addBindValue(now);
            update.addBindValue(it->rowId);
            update.exec();
            m_openWebSegments.erase(it);
        }
        return;
    }

    if (it != m_openWebSegments.end() && it->url == url) {
        QSqlQuery update(m_database);
        update.prepare(QStringLiteral("UPDATE web_visits SET end_ms = ? WHERE id = ?"));
        update.addBindValue(now);
        update.addBindValue(it->rowId);
        update.exec();
        return;
    }

    QSqlQuery insert(m_database);
    insert.prepare(QStringLiteral(
        "INSERT INTO web_visits (session_id, monitor_stream_id, url, start_ms, end_ms) "
        "VALUES (?, ?, ?, ?, ?)"));
    insert.addBindValue(sessionId);
    insert.addBindValue(monitorStreamId);
    insert.addBindValue(url);
    insert.addBindValue(now);
    insert.addBindValue(now);
    if (!insert.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut scrie vizita web: %1").arg(insert.lastError().text()));
        return;
    }
    m_openWebSegments[key] = OpenWebSegment{url, insert.lastInsertId().toLongLong()};
}

void HistoryRecorder::recordKeystroke(quint32 sessionId, const QString &windowTitle,
                                      const QString &text)
{
    if (!m_database.isOpen() || text.isEmpty()) {
        return;
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO keystrokes (session_id, timestamp_ms, window_title, text) VALUES (?, ?, ?, ?)"));
    query.addBindValue(sessionId);
    query.addBindValue(QDateTime::currentMSecsSinceEpoch());
    query.addBindValue(windowTitle);
    query.addBindValue(text);
    if (!query.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut scrie textul tastat: %1").arg(query.lastError().text()));
    }
}

void HistoryRecorder::setCategory(const QString &application, const QString &category)
{
    if (!m_database.isOpen() || application.isEmpty()) {
        return;
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "INSERT INTO app_categories (application, category) VALUES (?, ?) "
        "ON CONFLICT(application) DO UPDATE SET category = excluded.category"));
    query.addBindValue(application);
    query.addBindValue(category);
    if (!query.exec()) {
        emit logMessage(
            QStringLiteral("Nu am putut salva categoria: %1").arg(query.lastError().text()));
    }
}

QString HistoryRecorder::category(const QString &application) const
{
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral("SELECT category FROM app_categories WHERE application = ?"));
    query.addBindValue(application);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return QStringLiteral("neutral");
}

QList<ActivitySample> HistoryRecorder::listActivity(quint32 monitorStreamId, const QString &day) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT timestamp_ms, input_count FROM activity WHERE monitor_stream_id = ? "
        "AND strftime('%Y%m%d', timestamp_ms / 1000, 'unixepoch', 'localtime') = ? "
        "ORDER BY timestamp_ms ASC"));
    query.addBindValue(monitorStreamId);
    query.addBindValue(day);
    QList<ActivitySample> samples;
    if (query.exec()) {
        while (query.next()) {
            samples.append(ActivitySample{query.value(0).toLongLong(), query.value(1).toInt()});
        }
    }
    return samples;
}

QList<AppSegment> HistoryRecorder::listAppSegments(quint32 monitorStreamId, const QString &day) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT application, start_ms, end_ms FROM app_segments WHERE monitor_stream_id = ? "
        "AND strftime('%Y%m%d', start_ms / 1000, 'unixepoch', 'localtime') = ? "
        "ORDER BY start_ms ASC"));
    query.addBindValue(monitorStreamId);
    query.addBindValue(day);
    QList<AppSegment> segments;
    if (query.exec()) {
        while (query.next()) {
            segments.append(AppSegment{query.value(0).toString(), query.value(1).toLongLong(),
                                       query.value(2).toLongLong()});
        }
    }
    return segments;
}

QList<AppUsage> HistoryRecorder::listRunningApplications(quint32 monitorStreamId,
                                                          const QString &day) const
{
    QHash<QString, qint64> totals;
    for (const AppSegment &segment : listAppSegments(monitorStreamId, day)) {
        totals[segment.application] += qMax<qint64>(0, segment.endMs - segment.startMs);
    }
    QList<AppUsage> usage;
    for (auto it = totals.cbegin(); it != totals.cend(); ++it) {
        usage.append(AppUsage{it.key(), it.value(), category(it.key())});
    }
    std::sort(usage.begin(), usage.end(),
             [](const AppUsage &a, const AppUsage &b) { return a.totalMs > b.totalMs; });
    return usage;
}

QList<WebVisit> HistoryRecorder::listWebVisits(quint32 monitorStreamId, const QString &day) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT url, start_ms, end_ms FROM web_visits WHERE monitor_stream_id = ? "
        "AND strftime('%Y%m%d', start_ms / 1000, 'unixepoch', 'localtime') = ? "
        "ORDER BY start_ms ASC"));
    query.addBindValue(monitorStreamId);
    query.addBindValue(day);
    QList<WebVisit> visits;
    if (query.exec()) {
        while (query.next()) {
            visits.append(WebVisit{query.value(0).toString(), query.value(1).toLongLong(),
                                   query.value(2).toLongLong()});
        }
    }
    return visits;
}

QList<WebUsage> HistoryRecorder::listWebPages(quint32 monitorStreamId, const QString &day) const
{
    QHash<QString, qint64> totals;
    for (const WebVisit &visit : listWebVisits(monitorStreamId, day)) {
        totals[visit.url] += qMax<qint64>(0, visit.endMs - visit.startMs);
    }
    QList<WebUsage> usage;
    for (auto it = totals.cbegin(); it != totals.cend(); ++it) {
        usage.append(WebUsage{it.key(), it.value()});
    }
    std::sort(usage.begin(), usage.end(),
             [](const WebUsage &a, const WebUsage &b) { return a.totalMs > b.totalMs; });
    return usage;
}

QList<QPair<QString, QString>> HistoryRecorder::listCategories() const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    QList<QPair<QString, QString>> categories;
    if (query.exec(QStringLiteral("SELECT application, category FROM app_categories"))) {
        while (query.next()) {
            categories.append({query.value(0).toString(), query.value(1).toString()});
        }
    }
    return categories;
}

QList<KeystrokeEntry> HistoryRecorder::listKeystrokes(quint32 sessionId, const QString &day) const
{
    if (!m_database.isOpen()) {
        return {};
    }
    QSqlQuery query(m_database);
    query.prepare(QStringLiteral(
        "SELECT timestamp_ms, window_title, text FROM keystrokes WHERE session_id = ? "
        "AND strftime('%Y%m%d', timestamp_ms / 1000, 'unixepoch', 'localtime') = ? "
        "ORDER BY timestamp_ms ASC"));
    query.addBindValue(sessionId);
    query.addBindValue(day);
    QList<KeystrokeEntry> entries;
    if (query.exec()) {
        while (query.next()) {
            entries.append(KeystrokeEntry{query.value(0).toLongLong(), query.value(1).toString(),
                                          query.value(2).toString()});
        }
    }
    return entries;
}
