#include "stunclient.h"

/*
 * Both contructors is simple and only do is set the Stun endpoint server
 */

StunClient::StunClient(QHostAddress host, quint16 port, QObject *parent)
    : QUdpSocket (parent) {
    this->host = host;
    this->port = port;
    finishStart();
}

StunClient::StunClient(const QString &url, QObject *parent) : StunClient(
        QHostInfo::fromName(QUrl(url).host()).addresses()[0],
        static_cast<quint16>(QUrl(url).port()),
        parent
        ){

}

void StunClient::binding()
{
    //Create a header
    STUN_header header = genHeader();
    //Set binding method
    header.Message_Type = __bswap_16(0x0001);

    //send to the Stun Server
    writeDatagram(reinterpret_cast<char*>(&header), sizeof (header), host, port);

    //Create a loop that will exit if ...
    QEventLoop blocking;
    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(3 * 1000);

    //have recive something
    connect(this, &QUdpSocket::readyRead, &blocking, &QEventLoop::quit);
    //or the timeout ocurrs
    connect(&timer, &QTimer::timeout, &blocking, &QEventLoop::quit);

    //Start timer and wait responce
    timer.start();
    blocking.exec();

    //Read data end populate the Stun recive header
    QNetworkDatagram datagram = receiveDatagram();
    QByteArray data = datagram.data();
    STUN_header header_recived;
    memcpy(reinterpret_cast<void *>(&header_recived), static_cast<void *>(data.data()), sizeof (header_recived));
    header_recived.Message_Type = __bswap_16(header_recived.Message_Type);
    header_recived.Message_Len = __bswap_16(header_recived.Message_Len);


    //Verify if the transaction id is the same
    if(header.Transaction_ID.ints[0] != header_recived.Transaction_ID.ints[0] &&
            header.Transaction_ID.ints[1] != header_recived.Transaction_ID.ints[1] &&
            header.Transaction_ID.ints[2] != header_recived.Transaction_ID.ints[2]) {
        qDebug() << "Deu merda STUN";
        return;
    }

    //if the responce type is a error type
    if(header_recived.Message_Type == 0x0111) {
        qDebug() << "Deu merda2 STUN";
        return;
    }

    //decode the atributes recived
    decodeRecive(data);

    //show the update public endpoint
    MappedAddress *attr = reinterpret_cast<MappedAddress *>(currentAtributes[0]);
    qDebug() << QHostAddress(attr->point.address.ipv4.num) << attr->point.port;

    //emit the updated signal
    emit updated();

}

STUN_header StunClient::genHeader() {
    STUN_header temp;
    /*
     * Create a header with a random trasaction ID
     */
    for(quint8 i = 0 ; i < 3 ; i++) {
        temp.Transaction_ID.ints[i] = static_cast<quint32>(qrand());
    }
    return temp;
}

void StunClient::finishStart()
{
    binding();
}

void StunClient::decodeRecive(QByteArray data)
{
    //Clear the currentAtributes if was some
    for(int i = 0 ; i < currentAtributes.size() ; i++) {
        delete currentAtributes[i];
    }
    currentAtributes.clear();

    //Calculate the start addres of the Stun atributes
    char *initData = static_cast<char*>(data.data());
    initData = initData + sizeof(STUN_header);

    //Re create the Stun header recived
    STUN_header header_recived;
    memcpy(reinterpret_cast<void *>(&header_recived), static_cast<void *>(data.data()), sizeof (header_recived));
    header_recived.Message_Type = __bswap_16(header_recived.Message_Type);
    header_recived.Message_Len = __bswap_16(header_recived.Message_Len);

    //Each loop attr must point to the next atribute to be readed
    StunAtribute *attr = reinterpret_cast<StunAtribute *>(initData);

    //Stop when the ammount of bytes readed is more or equals if the Message len
    while(reinterpret_cast<char*>(attr) - initData < header_recived.Message_Len) {
        //First step is fix the byte order
        attr->len = __bswap_16(attr->len);
        attr->type = __bswap_16(attr->type);

        //Type 0x0001 is MappedAddress
        if(attr->type == 0x0001) {
            //Create a new attribute to be iserted in the list
            MappedAddress *map = new MappedAddress;
            //Copy the content from the recive array
            memcpy(map, attr, sizeof (MappedAddress));

            //Fix the byte order
            map->point.port = __bswap_16(map->point.port);
            map->family = __bswap_16(map->family);

            //update the current public port
            current_addr.port = map->point.port;

            //Verify the address type
            if(map->family == 0x01) { //0x01 is ipv4
                //fix the byte order
                map->point.address.ipv4.num = __bswap_32(map->point.address.ipv4.num);

                //update the current public ip andress
                current_addr.address.ipv4.num = map->point.address.ipv4.num;
            } else { //0x02 is ipv6
                quint8 temp = map->point.address.ipv6.bytes[0];
                for(quint8 i = 1 ; i < sizeof (map->point.address.ipv6.bytes) ; i++) {
                    map->point.address.ipv6.bytes[i - 1] = map->point.address.ipv6.bytes[i];
                }
                map->point.address.ipv6.bytes[sizeof (map->point.address.ipv6.bytes) - 1] = temp;
            }
            //Add the created attribute to the list
            currentAtributes << map;

        //   I have found one server case that treats 0x8020 and 0x0020
        // as XOR-MAPPED-ADDRESS attribute, but in original documentation
        // only 0x0020 is right
        } else if(attr->type == 0x0020 || attr->type == 0x8020) {
            //As XOR-MAPPED-ADDRESS is the same of MappedAddress
            //so to facilitate the things i have use the same class
            MappedAddress *map = new MappedAddress;

            //Copy the content from the recive array
            memcpy(map, attr, sizeof (MappedAddress));

            //Fix the byte order
            map->family = __bswap_16(map->family);
            //Fix the byteorder end decode the port doing a XOR with de MagicCookie
            map->point.port = __bswap_16(map->point.port ^ header_recived.Magic_Cookie.shorts[0]);

            //update the current public port
            current_addr.port = map->point.port;

            //Verify the address type
            if(map->family == 0x01) { //0x01 is ipv4
                //fix the byte order end decode the port doing a XOR with de MagicCookie
                map->point.address.ipv4.num = __bswap_32(map->point.address.ipv4.num ^ header_recived.Magic_Cookie.num);

                //update the current public ip andress
                current_addr.address.ipv4.num = map->point.address.ipv4.num;
            } else { //0x02 is ipv6
                quint8 temp = map->point.address.ipv6.bytes[0] ^ header_recived.Magic_Cookie.bytes[0];
                for(quint8 i = 1 ; i < sizeof (map->point.address.ipv6.bytes) ; i++) {
                    map->point.address.ipv6.bytes[i - 1] = map->point.address.ipv6.bytes[i] ^ header_recived.Magic_Cookie.bytes[i % 4];
                }
                map->point.address.ipv6.bytes[sizeof (map->point.address.ipv6.bytes) - 1] = temp;
            }

            //Add the created attribute to the list
            currentAtributes << map;
        }

        attr = reinterpret_cast<StunAtribute *>(reinterpret_cast<char *>(attr) + attr->len + sizeof (StunAtribute));
    }

}

void StunClient::timerEvent(QTimerEvent */*event*/)
{
    binding();
}

EndPoint StunClient::getCurrent_addr() const
{
    return current_addr;
}
