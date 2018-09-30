#include "conn.h"

Conn::Conn(QHostAddress host, quint16 port, QObject *parent) : QObject(parent)
{
    //Setup stun server / port
    sock = new StunClient("udp://iphone-stun.strato-iphone.de:3478");

    this->host = host;
    this->port = port;

    qDebug() << host << port;

    //set a random room_id
    for(uint i = 0 ; i < sizeof (currentRoomId) ;i++) {
        currentRoomId[i] = static_cast<quint8>(qrand());
    }
}

Conn::Conn(const QString &url, QObject *parent) : Conn(
        QHostInfo::fromName(QUrl(url).host()).addresses()[0],
        static_cast<quint16>(QUrl(url).port()),
        parent
        ) {
}

void Conn::startConn()
{
    //Create a require and a response packege
    Connection response, require;

    //Set action to connect
    require.request.action = 0;
    //Set random number
    require.request.transaction_id = static_cast<quint32>(qrand());
    //Magic number must be this constat
    require.request.connection_id = __bswap_64(0x41727101980);

    //Send to the server
    sock->writeDatagram(QByteArray(reinterpret_cast<char*>(&require), sizeof(require)), host, port);

    //Wait the server response
    while(! sock->hasPendingDatagrams());

    //Decode the response into the struct
    sock->readDatagram(reinterpret_cast<char*>(&response), sizeof(Connection));
    //Fix the byte order
    response.responce.action = __bswap_32(response.responce.action);
    //Obs: there is no motive to fix the outer two attributes because
    //there only goal if for comparation

    //Verify any erros
    if(response.responce.transaction_id != require.request.transaction_id) {
        qDebug() << "ERROO !!!";
    }

    //configure the connection_id recive
    connection_id = response.responce.connection_id;
}


void Conn::announce()
{
    return announce(currentRoomId);
}

void Conn::announce(quint8 room_id[])
{
    //Create a annouce packege
    struct Announce annou;


    //Configure the packege

    //Configure as annouce
    annou.conn.request.action = __bswap_32(1);
    //Set the connection id recived by the server
    annou.conn.request.connection_id = connection_id;
    //Create a random number
    annou.conn.request.transaction_id = __bswap_32(1212);
    //Set a easy configure room id
    memcpy(annou.info_hash, room_id, 20);
    memcpy(currentRoomId, annou.info_hash, 20);
//    memset(annou.info_hash, 20, 20);
    //Set a random peer_id
    memset(annou.peer_id, 0, 20);
    //useless configuration
    annou.downloaded = annou.uploaded = annou.left = 0;
    //configure as download completed
    annou.event = __bswap_32(1);
    //This field isint used by the tracker, so the default value is 0
    //but con be configured as the ip founded by the STUN server as too
    annou.IPv4.num = 0;
    //Set the default value as -1
    annou.num_want = __bswap_32(static_cast<uint>(-1));
    //Set the port finded by the Stun server
    annou.port = __bswap_16(sock->getCurrent_addr().port);


    //send the packeger to the tracker
    sock->writeDatagram(QByteArray(reinterpret_cast<char*>(&annou), sizeof(annou)), host, port);

    //Wait a responce
    while(! sock->hasPendingDatagrams());

    //Create a response packege
    AnnounceResponce resp;
    //Read the responce data
    QByteArray data = sock->receiveDatagram().data();
    //Move the initial data into the response packege
    memcpy(&resp, data.data(), sizeof (AnnounceResponce));

    //Fix the byte order of the recived data
    resp.action = __bswap_32(resp.action);
    resp.interval = __bswap_32(resp.interval);
    resp.leechers = __bswap_32(resp.leechers);
    resp.seeders = __bswap_32(resp.seeders);

    //Verify if the recive transaction id is the same
    if(resp.transaction_id != annou.conn.request.transaction_id) {
        qDebug() << "Error !!!!!!!!!";
        qDebug() << data;
        return;
    }

    //Configure the update timer with the time recive by the server
    timer_interval = resp.interval * 1000; //store in millis seconds

    //Clear the endpoint list
    endpoints.clear();

    //Set the size, in bytes, of one endpoint IPv4 + udp port
    const quint8 endpoint_size = 6;

    /**
     * @brief endpointsize
     * The ammount of endpoint recived by the server is:
     * (Total bytes ammount - readed bytes) / size of each endpoint
     *
     * Total bytes ammount = data.size()
     * readed bytes = sizeof (AnnounceResponce)
     * size of each endpoint = 6
     */
    qint32 endpointsize = (data.size() - static_cast<int>(sizeof (AnnounceResponce))) / endpoint_size;
    //Evaluate the init address of the endpoins
    char *baseAddr = data.data() + sizeof (AnnounceResponce);
    for(int i = 0 ; i < endpointsize ;i++) {
        //Four first bytes is the ip
        IPv4 *ip = reinterpret_cast<IPv4*>(baseAddr + (i * endpoint_size));
        //The next 2 bytes is the udp port
        quint16 *porta = reinterpret_cast<quint16*>(baseAddr + (i * endpoint_size) + sizeof(IPv4));

        EndPointModel end;
        //Fix the byte order and store the result
        end.address.ipv4.num = __bswap_32(ip->num);
        end.port = __bswap_16(*porta);
        //Add into the endpoint list
        endpoints << end;
    }

    emit update();
}

quint32 Conn::getTimer_interval() const
{
    return timer_interval;
}

QList<EndPointModel> Conn::getEndpoints() const
{
    return endpoints;
}

StunClient *Conn::getSock() const
{
    return sock;
}
