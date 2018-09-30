#ifndef STUN_CLIENT_H
#define STUN_CLIENT_H

#include <QObject>
#include <QList>
#include <QHostAddress>
#include <QUrl>
#include <QHostInfo>
#include <QTimer>
#include <QTimerEvent>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QEventLoop>
#include <byteswap.h>

/*
 *
 * So you want know more about the Stun here implemtaded
 * let-me guide you
 *
 * All information about the protocol can be found in [RFC 5389]
 * https://tools.ietf.org/html/rfc5389
 *
 */

/*
 * Each struct is used to facilitate the write and read operation
 * under the UDP port.
 *
 * So each this is a packet format of STUN protocol
 */

/**
 * @brief must appear in all mensagens sends between the client and server STUN
 */
struct __attribute__ ((__packed__)) STUN_header {
    /**
     * @brief Message_Type
     *                  0                 1
     *                  2  3  4 5 6 7 8 9 0 1 2 3 4 5
     *
     *                 +--+--+-+-+-+-+-+-+-+-+-+-+-+-+
     *                 |M |M |M|M|M|C|M|M|M|C|M|M|M|M|
     *                 |11|10|9|8|7|1|6|5|4|0|3|2|1|0|
     *                 +--+--+-+-+-+-+-+-+-+-+-+-+-+-+
     *
     *          Figure 3: Format of STUN Message Type Field
     *
     * As can be saw the mensage type has this format and the upper 2 bits must
     * be equals to zero this is used to describe one method and 4 possibles state
     * called classes the methods is describe by the M bits and the class
     * by the C bits.
     *
     * The Default STUN server only has one method called Binding (method=0b000000000001),
     * and this method don't have any atribute to send to the server
     *
     */
    quint16 Message_Type;

    /**
     * @brief Message_Len
     * The message length MUST contain the size, in bytes, of the message
     * not including the 20-byte STUN header.
     */
    quint16 Message_Len = 0;

    /**
     * @brief The Magic_Cookie union
     *
     * There must be a constant number equals to 0x2112A442, the union here
     * is only used to make easy read onde part of this number
     */
    union __attribute__ ((__packed__)) {
        quint8 bytes[4];
        quint16 shorts[2];
        quint32 num = __bswap_32(0x2112A442);
    } Magic_Cookie;

    /**
     * @brief The Transaction_ID union
     *
     * This is a unique id create by the client that the server must reply
     */
    union __attribute__ ((__packed__)) {
        quint8 bytes[12];
        quint16 shorts[6];
        quint32 ints[3];
    } Transaction_ID;
};

/**
 * @brief The StunAtribute struct
 * This struct to me may be call as Atribute header
 * Any comunication by the server or the client, may use this to send information
 * Any atribute is located right after the Stun Header.
 */
struct __attribute__ ((__packed__)) StunAtribute {
    /**
     * @brief type
     * Tells the type of the atribute
     */
    quint16 type;
    /**
     * @brief len
     * The size of the atribute not include the "Atribute header"
     */
    quint16 len;
};

/**
 * @brief The Address union
 * This union holds a ipv4 or ipv6 addres
 */
union __attribute__ ((__packed__)) Address {
    union __attribute__ ((__packed__)) {
        quint8 bytes[4];
        quint32 num;
    } ipv4;

    union __attribute__ ((__packed__)) {
        quint8 bytes[16];
        quint64 num[2];
    } ipv6;
};

/**
 * @brief The EndPoint struct
 * End point by defenition is any pair of Andress and port
 * used to comunicate
 */
struct __attribute__ ((__packed__)) EndPoint {
    quint16 port;
    Address address;
};

/**
 * @brief The MappedAddress struct
 *
 * The most import and simple Atribute is the MappedAddress
 * is here that the client can know he public end point (ip + udp port)
 *
 */
struct __attribute__ ((__packed__)) MappedAddress : StunAtribute {
    /**
    * @brief family
    * if 0x0001 the ip is ipv4
    * if 0x0002 the ip is ipv6
    */
    quint16 family;
    /**
     * @brief point
     * This is the public endpoint sended back from the server.
     */
    EndPoint point;
};

/*
 * Now that i have finish describe the protocol pacages here supported
 * let's see the class
 */

/**
 * @brief The StunClient class
 *
 * This is a UdpSocket because the same socket used to comunicate with
 * the Stun server must be used to comunicate through the UDP hole
 *
 */
class StunClient : public QUdpSocket
{
    Q_OBJECT
public:
    /**
     * @brief StunClient
     * @param host Host address of Stun server
     * @param port Port of the Stun server
     * @param parent Qt parent object
     */
    explicit StunClient(QHostAddress host, quint16 port = 3478, QObject *parent = nullptr);
    /**
     * @brief StunClient
     * @param url A URL with the host and port of STUN server
     * @param parent Qt parent object
     */
    explicit StunClient(const QString &url, QObject *parent = nullptr);

    /**
     * @brief getCurrent_addr
     * @return the current public endpoint
     */
    EndPoint getCurrent_addr() const;

signals:
    /**
     * @brief updated
     * signal when a update with the stun server occurs
     */
    void updated();

public slots:

    /**
     * @brief binding
     * Execute the only one oprate of STUN
     */
    void binding();

private:
    /**
     * @brief genHeader
     * @return A new Stun header without the Type
     */
    STUN_header genHeader();

    /**
     * @brief finishStart
     * Used in the end of contructor function
     */
    void finishStart();

    /**
     * @brief decodeRecive
     * Decode the StunAtributes recive from the server end update the
     * currentAtributes list
     * @param data
     */
    void decodeRecive(QByteArray data);

    void timerEvent(QTimerEvent *) override;

private:
    /**
     * @brief host
     * Stun Server Address
     */
    QHostAddress host;
    /**
     * @brief port
     * Stun Server port
     */
    quint16 port;

    /**
     * @brief currentAtributes
     * The most update atributes get from the server
     */
    QList<StunAtribute *> currentAtributes;

    /**
     * @brief current_addr
     * The current public endpoint
     */
    EndPoint current_addr;

};

#endif // STUN_CLIENT_H
