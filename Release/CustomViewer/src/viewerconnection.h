#pragma once

#include "frameprotocol.h"

#include <QHash>
#include <QImage>
#include <QJsonObject>
#include <QObject>
#include <QSslError>
#include <QSslSocket>
#include <QTimer>

struct HistoryActivitySample {
    qint64 timestampMs = 0;
    int inputEvents = 0;
};

struct HistoryAppSegment {
    QString application;
    qint64 startMs = 0;
    qint64 endMs = 0;
};

struct HistoryAppUsage {
    QString application;
    qint64 totalMs = 0;
    QString category;
};

struct HistoryKeystrokeEntry {
    qint64 timestampMs = 0;
    QString windowTitle;
    QString text;
};

class ViewerConnection final : public QObject
{
    Q_OBJECT

public:
    explicit ViewerConnection(QObject *parent = nullptr);

    void connectToAgent(const QString &host, quint16 port, const QString &token,
                        bool useTls, const QString &certificateSha256);
    void disconnectFromAgent();
    void startDemo();
    void stopDemo();
    bool isConnected() const;

    // History (Faza 2): served by PersonalHost over this same connection,
    // so it works whether or not viewer and host are on the same machine.
    // No-ops (silently) against an agent that never announces support --
    // the request just gets an {"error":...} response, surfaced via
    // historyError.
    void requestHistoryDays(quint32 streamId);
    void requestHistoryFrames(quint32 streamId, const QString &day);
    void requestHistoryFrame(quint32 streamId, qint64 timestampMs);
    void requestHistoryActivity(quint32 streamId, const QString &day);
    void requestHistoryAppSegments(quint32 streamId, const QString &day);
    void requestRunningApplications(quint32 streamId, const QString &day);
    void requestWebPages(quint32 streamId, const QString &day);
    void requestCategories(quint32 streamId);
    void setAppCategory(quint32 streamId, const QString &application, const QString &category);
    void requestKeystrokes(quint32 streamId, const QString &day);

signals:
    void statusChanged(const QString &status, bool connected);
    void agentIdentified(const QString &agentName, const QString &sessionName);
    // sessionUsername/sessionState are empty/"" when the agent has no
    // notion of Windows sessions (single-process PersonalScreenAgent, demo
    // mode); PersonalHost fills them in so the viewer can group monitors
    // by the session/user they belong to.
    // isWindow: true for a PersonalSubService's ActiveWindowCapture live
    // preview stream, not a real monitor -- see
    // SubServiceHost::kActiveWindowStreamId.
    void monitorDiscovered(quint32 streamId, const QString &name, const QSize &size,
                           quint32 sessionId, const QString &sessionUsername,
                           const QString &sessionState, bool isWindow);
    void frameReady(quint32 streamId, const QImage &image, quint64 sequence,
                    qint64 latencyMs);
    void metadataChanged(quint32 streamId, const QString &application,
                         const QString &idleText);
    void protocolError(const QString &message);

    void historyDaysReceived(quint32 streamId, const QStringList &days);
    void historyFramesReceived(quint32 streamId, const QString &day, const QList<qint64> &timestamps);
    void historyFrameReceived(quint32 streamId, qint64 timestampMs, const QImage &image);
    void historyError(quint32 streamId, const QString &message);
    void historyActivityReceived(quint32 streamId, const QString &day,
                                 const QList<HistoryActivitySample> &samples);
    void historyAppSegmentsReceived(quint32 streamId, const QString &day,
                                    const QList<HistoryAppSegment> &segments);
    void historyRunningApplicationsReceived(quint32 streamId, const QString &day,
                                            const QList<HistoryAppUsage> &applications);
    // Reuses HistoryAppUsage -- application field holds the url, category
    // is unused (empty) here since web pages aren't categorized.
    void historyWebPagesReceived(quint32 streamId, const QString &day,
                                 const QList<HistoryAppUsage> &pages);
    void historyCategoriesReceived(quint32 streamId, const QHash<QString, QString> &categories);
    void historyKeystrokesReceived(quint32 streamId, const QString &day,
                                   const QList<HistoryKeystrokeEntry> &entries);

private slots:
    void onSocketConnected();
    void onSocketEncrypted();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);
    void onSslErrors(const QList<QSslError> &errors);
    void renderDemoFrame();

private:
    struct StreamState {
        QString name;
        QImage image;
        quint64 sequence = 0;
    };

    void sendClientHello();
    void parseAvailableMessages();
    void processMessage(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processJsonMessage(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processFullFrame(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processDeltaFrame(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processHistoryQuery(const ViewerProtocol::Header &header, const QByteArray &payload);
    void processHistoryFrame(const ViewerProtocol::Header &header, const QByteArray &payload);
    void sendHistoryQuery(quint32 streamId, const QJsonObject &object);
    void failProtocol(const QString &message);
    bool isSafePlainTextTarget() const;

    QSslSocket m_socket;
    ViewerProtocol::PsvFrameReader m_frameReader;
    QHash<quint32, StreamState> m_streams;
    QTimer m_demoTimer;
    QString m_host;
    QString m_token;
    QString m_certificateSha256;
    bool m_useTls = true;
    bool m_demoMode = false;
    quint64 m_demoSequence = 0;
};

