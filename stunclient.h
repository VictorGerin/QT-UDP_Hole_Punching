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

#include "models/stunattributemodel.h"
#include "models/addressmodel.h"
#include "models/endpointmodel.h"
#include "models/mappedaddressmodel.h"
#include "models/stunheadermodel.h"

/*
 *
 * So you want know more about the Stun here implemtaded
 * let-me guide you
 *
 * All information about the protocol can be found in [RFC 5389]
 * https://tools.ietf.org/html/rfc5389
 *
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
    EndPointModel getCurrent_addr() const;

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
    StunHeaderModel genHeader();

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
    QList<StunAttributeModel *> currentAtributes;

    /**
     * @brief current_addr
     * The current public endpoint
     */
    EndPointModel current_addr;

};

#endif // STUN_CLIENT_H
