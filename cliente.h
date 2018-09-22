#ifndef CLIENTE_H
#define CLIENTE_H

#include <QObject>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDataStream>
#include <QNetworkInterface>
#include <QThread>

class Cliente : public QObject
{
    Q_OBJECT
public:
    explicit Cliente(QString ip, int port, QObject *parent = nullptr);

    void registerIntention(QNetworkDatagram *datagram);

    void reciveEndpoint(QNetworkDatagram *datagram);

signals:

public slots:
    void readPendingDatagrams();
private:
    QUdpSocket *in_sok;
    QHostAddress addr;
    quint16 porta;
};

#endif // CLIENTE_H
