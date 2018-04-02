#include "cliente.h"

Cliente::Cliente(QObject *parent) : QObject(parent)
{
    qDebug() << "Init Client";
    in_sok = new QUdpSocket(this);

    this->addr = QHostAddress("192.168.0.15");
    porta = 7755;

    registerIntention(0);
    connect(in_sok, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));

}

void Cliente::registerIntention(QNetworkDatagram *datagram)
{
    quint8 temp;
    QByteArray bytes;
    QDataStream out(&bytes, QIODevice::ReadWrite);

    //State 0 start register intention
    if(!datagram) {
        out << (quint8) 10 << (quint8) 0;
        in_sok->writeDatagram(bytes, addr, porta);
        return;
    }

    QByteArray _bytes = datagram->data();
    QDataStream in(&_bytes, QIODevice::ReadOnly);

    in >> temp;//Ignora o primeiro byte
    in >> temp;

    switch (temp) {
    case 1:
        //Recive Ark, send data
        out << (quint8) 10 << (quint8) 2;
        out << datagram->destinationAddress().toIPv4Address();
        out << in_sok->localPort();
        in_sok->writeDatagram(bytes, addr, porta);
        break;
    default:
        break;
    }

}

void Cliente::reciveEndpoint(QNetworkDatagram *datagram) {

    QByteArray _bytes = datagram->data();
    QDataStream in(&_bytes, QIODevice::ReadOnly);

//    qDebug() << _bytes;
//    qDebug(_bytes);
//    return;

    in.skipRawData(2);

    quint32 _ip;
    in >> _ip;
    quint16 porta;
    in >> porta;

    qDebug() << "Dados recebido do master : " <<  QHostAddress(_ip) << porta;

    QByteArray bytes;
    QTextStream out(&bytes, QIODevice::ReadWrite);

    out << "Testando que foda isso veiii :)";
    out.flush();

    in_sok->writeDatagram(bytes, QHostAddress(_ip), porta);

    QThread::sleep(5);
    in_sok->writeDatagram(bytes, QHostAddress(_ip), porta);
}

void Cliente::readPendingDatagrams()
{
    while (in_sok->hasPendingDatagrams()) {
        QNetworkDatagram datagram = in_sok->receiveDatagram();
        QByteArray bytes = datagram.data();
        QDataStream in(&bytes, QIODevice::ReadOnly);
        quint8 t;
        in >> t;

        if(t == 10)
            registerIntention(&datagram);
        else if(t == 20)
            reciveEndpoint(&datagram);
        else {
            qDebug() << "Teste : " << bytes;
        }
    }
}
