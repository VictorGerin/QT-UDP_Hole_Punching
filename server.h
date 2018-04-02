#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QEventLoop>
#include <QDataStream>

typedef struct {
    QHostAddress addr;
    quint16 porta;
} Endpoint;

typedef struct {
    Endpoint publico;
    Endpoint privado;
} Client;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

    void reciveAIntention(QNetworkDatagram *datagram);

    void rendezvous();

    QByteArray serializeEndpoins(Client *end);

signals:

public slots:
    void readPendingDatagrams();
private:
    QUdpSocket *in_sok;
    Client *first = 0;
    Client *second = 0;
};

#endif // SERVER_H
