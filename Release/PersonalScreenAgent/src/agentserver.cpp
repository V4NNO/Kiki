#include "agentserver.h"

#include <QFile>
#include <QHostAddress>

AgentServer::AgentServer(QObject *parent)
    : QTcpServer(parent)
{
}

bool AgentServer::startListening(quint16 port, const TlsConfig &tls, const AgentSettings &settings,
                                 QString *error)
{
    stopListening();
    m_tls = tls;
    m_settings = settings;

    if (m_settings.token.isEmpty()) {
        if (error) {
            *error = QStringLiteral("Este obligatoriu un token de autentificare.");
        }
        return false;
    }

    if (m_tls.enabled) {
        QFile certFile(m_tls.certificatePath);
        if (!certFile.open(QIODevice::ReadOnly)) {
            if (error) {
                *error = QStringLiteral("Nu pot citi certificatul TLS de la %1.")
                             .arg(m_tls.certificatePath);
            }
            return false;
        }
        m_certificate = QSslCertificate(certFile.readAll(), QSsl::Pem);
        certFile.close();
        if (m_certificate.isNull()) {
            if (error) {
                *error = QStringLiteral("Certificatul TLS este invalid.");
            }
            return false;
        }

        QFile keyFile(m_tls.keyPath);
        if (!keyFile.open(QIODevice::ReadOnly)) {
            if (error) {
                *error = QStringLiteral("Nu pot citi cheia privata TLS de la %1.")
                             .arg(m_tls.keyPath);
            }
            return false;
        }
        m_privateKey = QSslKey(keyFile.readAll(), QSsl::Rsa, QSsl::Pem);
        keyFile.close();
        if (m_privateKey.isNull()) {
            if (error) {
                *error = QStringLiteral("Cheia privata TLS este invalida.");
            }
            return false;
        }
    }

    if (!listen(QHostAddress::Any, port)) {
        if (error) {
            *error = errorString();
        }
        return false;
    }
    return true;
}

void AgentServer::stopListening()
{
    close();
    for (AgentConnection *connection : std::as_const(m_connections)) {
        connection->sendGoodbyeAndClose();
        connection->deleteLater();
    }
    m_connections.clear();
    emit connectionCountChanged(0);
}

void AgentServer::incomingConnection(qintptr socketDescriptor)
{
    auto *socket = new QSslSocket(this);
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        socket->deleteLater();
        return;
    }

    if (m_tls.enabled) {
        socket->setLocalCertificate(m_certificate);
        socket->setPrivateKey(m_privateKey);
        socket->setProtocol(QSsl::TlsV1_2OrLater);
        connect(socket, &QSslSocket::encrypted, this, [this, socket] {
            adoptConnection(socket);
        });
        connect(socket, &QSslSocket::sslErrors, this, [this, socket](const QList<QSslError> &errors) {
            Q_UNUSED(errors)
            emit logMessage(QStringLiteral("Eroare TLS pentru %1:%2, conexiune inchisa.")
                                .arg(socket->peerAddress().toString())
                                .arg(socket->peerPort()));
            socket->abort();
        });
        socket->startServerEncryption();
        return;
    }

    const bool isLoopback = socket->peerAddress().isLoopback();
    if (!isLoopback) {
        emit logMessage(QStringLiteral("Conexiune necriptata refuzata de la %1 (doar localhost).")
                            .arg(socket->peerAddress().toString()));
        socket->abort();
        socket->deleteLater();
        return;
    }
    adoptConnection(socket);
}

void AgentServer::adoptConnection(QSslSocket *socket)
{
    auto *connection = new AgentConnection(socket, m_settings, this);
    connect(connection, &AgentConnection::logMessage, this, &AgentServer::logMessage);
    connect(connection, &AgentConnection::authenticated, this, [this](AgentConnection *self) {
        self->setMonitors(m_monitors);
    });
    connect(connection, &AgentConnection::closed, this, [this](AgentConnection *self) {
        m_connections.removeOne(self);
        self->deleteLater();
        emit connectionCountChanged(m_connections.size());
    });
    m_connections.append(connection);
    emit connectionCountChanged(m_connections.size());
}

void AgentServer::broadcastFrame(quint32 streamId, const QImage &image)
{
    for (AgentConnection *connection : std::as_const(m_connections)) {
        if (connection->isAuthenticated()) {
            connection->pushFrame(streamId, image);
        }
    }
}

void AgentServer::broadcastMetadata(quint32 streamId, const QString &application,
                                    const QString &idleText)
{
    for (AgentConnection *connection : std::as_const(m_connections)) {
        if (connection->isAuthenticated()) {
            connection->pushMetadata(streamId, application, idleText);
        }
    }
}

void AgentServer::broadcastMonitorList(const QList<MonitorInfo> &monitors)
{
    m_monitors = monitors;
    for (AgentConnection *connection : std::as_const(m_connections)) {
        if (connection->isAuthenticated()) {
            connection->setMonitors(monitors);
        }
    }
}
