#ifndef CONN_H
#define CONN_H

#include <QObject>
#include <QDebug>
#include <QUdpSocket>
#include <QThread>
#include <QHostInfo>
#include <QHostAddress>
#include <QTimer>
#include <QNetworkDatagram>
#include "stunclient.h"
#include <byteswap.h>


/*
 *
 * Now lets talk about the Torrent Tracker
 *
 * I have use the Tracker to be a way to share the
 * public endpoint.
 *
 */

/*
 * As the same of StunClient
 * all packeges if coded using Strucs and unions
 */


union IPv4 {
    quint8 arry[4];
    quint32 num;
};

/**
 * @brief The Connection union
 * At the begin of any comunication with the tracker 128 bit
 * must be send or recive and there have the same mean just
 * the order that change because that i have used a union.
 *
 * @brief connection_id
 * Is a id send by the Tracker server and must be a constant
 * while use the tracker
 * @brief action
 * Signals to the tracker or client whats about is the packege
 * @brief transaction_id
 * A random number create by the client used to verify the consistence
 * of the packege
 */
union __attribute__((__packed__)) Connection {
    struct __attribute__((__packed__)) {
        quint64 connection_id;
        quint32 action;
        quint32 transaction_id;
    } request ;
    struct __attribute__((__packed__)) {
        quint32 action;
        quint32 transaction_id;
        quint64 connection_id;
    } responce ;
};

/**
 * @brief The Announce struct
 * Is with this packege that the client sends to the tracker he intentions
 *
 * @brief info_hash
 * We can think this may be a room number, the tracker uses this number
 * to determinate the swarm peers
 * @brief peer_id
 * Is kind a useless attribute by the view point of the client tracker
 * this information is not send back by the tracker.
 * @brief downloaded, left, uploaded
 * There are the amount of bytes downloaded, remaining and uploaded in the current section
 * again this is a useless attribute.
 * @brief event
 * Tell to the tracker your download status
 * 0: none; 1: completed; 2: started; 3: stopped
 * @brief IPv4
 * As the names susges is the ip of the client this attribute is normally
 * ignore by the tracker, and the tracker will use the ip that he sees
 * @brief key
 * This is used by the tracker if the client can operate with ipv4 and ipv6
 * @brief num_want
 * There is no documentation about this but i guess is maximun amount of endpoints
 * send back from the tracker
 * @brief port
 * As IPv4 this is the udp port that the client is listend
 */
struct __attribute__((__packed__)) Announce  {
    Connection conn;
    quint8 info_hash[20];
    quint8 peer_id[20];
    quint64 downloaded;
    quint64 left;
    quint64 uploaded;
    quint32 event;
    union IPv4 IPv4;
    quint32 key;
    quint32 num_want;
    quint16 port;
};

/**
 * @brief The AnnounceResponce struct
 * After done the Annouce the tracker will send back a Reponce
 *
 * This packege don't have a connection_id because that there
 * do not start with Connection union
 *
 * @brief action, transaction_id
 * Do the same as the action end transaction_id from Connection union
 * @brief interval
 * The interval in seconds for the client send a Annouce again
 * @brief leechers, seeders
 * Is the number of leechers and seeders inside of the "room"
 */
struct AnnounceResponce {
    quint32 action;
    quint32 transaction_id;
    quint32 interval;
    quint32 leechers;
    quint32 seeders;
};

/**
 * @brief The Conn class
 * This class has the goal to maneger the Tracker List and keep updated
 */

class Conn : public QObject
{
    Q_OBJECT
public:
    explicit Conn(QHostAddress host, quint16 port, QObject *parent = nullptr);

    explicit Conn(const QString &url, QObject *parent = nullptr);

    /**
     * @brief startConn
     * Starts a connection with the server
     */
    void startConn();

    QUdpSocket *getSock() const;

signals:
    void update();

public slots:
    /**
     * @brief announce
     * Makes a annouce to the tracker server and updates the enpoint list
     */
    void announce();

private:
    StunClient *sock;
    quint16 port;
    QHostAddress host;
    QList<EndPoint> endpoints;
    QTimer announce_timer;
    quint64 connection_id = 0;
};

#endif // CONN_H
