#pragma once

#include "agentconnection.h"

#include <QJsonArray>
#include <QList>
#include <QSslCertificate>
#include <QSslKey>
#include <QTcpServer>

#include <memory>

class HistoryFrameService;

class AgentServer final : public QTcpServer
{
    Q_OBJECT

public:
    struct TlsConfig {
        bool enabled = false;
        QString certificatePath;
        QString keyPath;
    };

    explicit AgentServer(QObject *parent = nullptr);
    // Out-of-line: m_frameService holds an incomplete type here.
    ~AgentServer() override;

    bool startListening(quint16 port, const TlsConfig &tls, const AgentSettings &settings,
                        QString *error);
    void stopListening();

    void broadcastFrame(quint32 streamId, const QImage &image);
    // activeMonitorStreamId: the global monitor stream the foreground window
    // is on (0 = none/unknown), for the tile's "Show active monitor" button.
    // url: the active browser page (empty when the foreground is no
    // browser) -- the viewer's ActiveApplicationRanker prefers a categorized
    // web page over the program.
    void broadcastMetadata(quint32 streamId, const QString &application, const QString &idleText,
                           quint32 activeMonitorStreamId, bool saver, const QString &url = QString());
    void broadcastMonitorList(const QList<MonitorInfo> &monitors);
    void broadcastStreamClosed(quint32 streamId);
    // Live keyboard events of the session streamId belongs to.
    void broadcastKeyEvents(quint32 streamId, const QJsonArray &events);

    // Applied to every connection adopted from here on; existing
    // connections at the time of the call are updated too.
    // Also spins up the shared history frame reader thread (getFrame is
    // served from there, never on the event loop).
    void setHistoryRecorder(HistoryRecorder *recorder);

    int connectionCount() const { return m_connections.size(); }

signals:
    void logMessage(const QString &message);
    void connectionCountChanged(int count);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    void adoptConnection(QSslSocket *socket);

    TlsConfig m_tls;
    AgentSettings m_settings;
    QSslCertificate m_certificate;
    QSslKey m_privateKey;
    QList<AgentConnection *> m_connections;
    QList<MonitorInfo> m_monitors;
    // Every stream's newest frame and metadata, handed to a viewer the moment
    // it authenticates: a still screen sends nothing new, so without these a
    // viewer that (re)connects would show "No video" until something moved.
    struct LatestMetadata {
        QString application;
        QString idleText;
        quint32 activeMonitorStreamId = 0;
        bool saver = false;
        QString url;
    };
    QHash<quint32, QImage> m_latestFrames;
    QHash<quint32, LatestMetadata> m_latestMetadata;
    HistoryRecorder *m_historyRecorder = nullptr;
    std::unique_ptr<HistoryFrameService> m_frameService;
};
