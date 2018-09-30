#include "testeclass.h"

TesteClass::TesteClass(QObject *parent) : QObject(parent)
{
    //Set a constant seed for the room_id be the same
    qsrand(4);

    connection = new Conn("udp://public.popcorn-tracker.org:6969");

    connect(connection, &Conn::update, this, &TesteClass::sendToAllPeopleInRoom);
    connect(connection->getSock(), &QUdpSocket::readyRead, this, &TesteClass::reciveData);

    connection->startConn();
    connection->announce();

    using namespace std::chrono;
    startTimer(5s);
}

void TesteClass::timerEvent(QTimerEvent */*event*/)
{
    connection->announce();
}

void TesteClass::sendToAllPeopleInRoom()
{
    StunClient *sock = connection->getSock();
    foreach (EndPointModel end, connection->getEndpoints()) {

        //Ignore the current endpoint
        if(end.port == sock->getCurrent_addr().port && end.address.ipv4.num == sock->getCurrent_addr().address.ipv4.num)
            continue;

        sock->writeDatagram(QByteArray("Sending mensage"), QHostAddress(end.address.ipv4.num), end.port);
    }
}

void TesteClass::reciveData()
{
    StunClient *sock = connection->getSock();
    QNetworkDatagram datagram = sock->receiveDatagram();
    qDebug() << "Mensage recive from :" << datagram.senderAddress() << datagram.senderPort();
    qDebug() << "mensage : " << datagram.data();
}
