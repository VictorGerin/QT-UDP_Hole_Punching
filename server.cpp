#include "server.h"
#include <QDebug>

Server::Server(int port, QObject *parent) : QObject(parent)
{
    qDebug() << "Init Server";
    in_sok = new QUdpSocket(this);
    in_sok->bind(QHostAddress::Any, port);

    connect(in_sok, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void Server::reciveAIntention(QNetworkDatagram *datagram)
{
    quint8 temp;
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::ReadWrite);
    QByteArray _bytes = datagram->data();
    QDataStream in(&_bytes, QIODevice::ReadOnly);
    QString nome;

    in >> temp; //Ignora o primeiro byte
    in >> temp;

    switch (temp) {
    case 0:
        //Send Ark
        out << (quint8) 10 << (quint8) 1;
        in_sok->writeDatagram(bytes, datagram->senderAddress(), datagram->senderPort());
        break;
    case 2:
        quint32 _ip;
        in >> _ip;
        quint16 porta;
        in >> porta;
        in >> nome;

        qDebug() << QHostAddress(_ip) << porta << nome;

        if(!this->first) {
            this->first = (Client *) malloc(sizeof(Client));
            this->first->privado.addr = QHostAddress(_ip);
            this->first->privado.porta = porta;
            this->first->publico.addr = datagram->senderAddress();
            this->first->publico.porta = datagram->senderPort();
        } else {
            this->second = (Client *) malloc(sizeof(Client));
            this->second->privado.addr = QHostAddress(_ip);
            this->second->privado.porta = porta;
            this->second->publico.addr = datagram->senderAddress();
            this->second->publico.porta = datagram->senderPort();
            rendezvous();
        }

        break;
    default:
        break;
    }

}

void Server::rendezvous()
{
    QByteArray bytes2;
    QDataStream out2(&bytes2, QIODevice::ReadWrite);
    out2 << (quint8) 20 << (quint8) 0;
    bytes2.append(serializeEndpoins(second));
    in_sok->writeDatagram(bytes2, first->publico.addr, first->publico.porta);


    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::ReadWrite);
    out << (quint8) 20 << (quint8) 0;
    bytes.append(serializeEndpoins(first));
    in_sok->writeDatagram(bytes, second->publico.addr, second->publico.porta);

}

QByteArray Server::serializeEndpoins(Client *end)
{
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::ReadWrite);
    out << end->publico.addr.toIPv4Address();
    out << end->publico.porta;
    return bytes;
}

void Server::readPendingDatagrams()
{
    while (in_sok->hasPendingDatagrams()) {
        QNetworkDatagram datagram = in_sok->receiveDatagram();
        QByteArray bytes = datagram.data();
        QDataStream in(&bytes, QIODevice::ReadOnly);
        quint8 t;
        in >> t;

        if(t == 10)
            reciveAIntention(&datagram);

    }
}
