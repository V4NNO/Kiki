#pragma once

#include "agentconnection.h"

#include <QList>
#include <QSslCertificate>
#include <QSslKey>
#include <QTcpServer>

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

    bool startListening(quint16 port, const TlsConfig &tls, const AgentSettings &settings,
                        QString *error);
    void stopListening();

    void broadcastFrame(quint32 streamId, const QImage &image);
    void broadcastMetadata(quint32 streamId, const QString &application, const QString &idleText);
    void broadcastMonitorList(const QList<MonitorInfo> &monitors);

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
};
