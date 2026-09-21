#pragma once

#include "frameprotocol.h"

#include <QHash>
#include <QImage>
#include <QObject>
#include <QSslError>
#include <QSslSocket>
#include <QTimer>

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

signals:
    void statusChanged(const QString &status, bool connected);
    void agentIdentified(const QString &agentName, const QString &sessionName);
    void monitorDiscovered(quint32 streamId, const QString &name, const QSize &size);
    void frameReady(quint32 streamId, const QImage &image, quint64 sequence,
                    qint64 latencyMs);
    void metadataChanged(quint32 streamId, const QString &application,
                         const QString &idleText);
    void protocolError(const QString &message);

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
    void failProtocol(const QString &message);
    bool isSafePlainTextTarget() const;

    QSslSocket m_socket;
    QByteArray m_receiveBuffer;
    QHash<quint32, StreamState> m_streams;
    QTimer m_demoTimer;
    QString m_host;
    QString m_token;
    QString m_certificateSha256;
    bool m_useTls = true;
    bool m_demoMode = false;
    quint64 m_demoSequence = 0;
};

