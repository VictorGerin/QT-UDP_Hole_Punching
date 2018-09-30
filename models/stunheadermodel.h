#ifndef STUNHEADERMODEL_H
#define STUNHEADERMODEL_H

#include <QObject>

/**
 * @brief STUN_header
 * Must appear in all mensagens sends between the client and server STUN
 */
class __attribute__ ((__packed__)) StunHeaderModel
{
public:
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
     * As can be saw the mensage type has this format and the upper 2 bits, not show here, must
     * be equals to zero. This field is used to describe one method and 4 possibles state
     * called classes, the methods are describe by the M bits and the class
     * by the C bits.
     *
     * The Default STUN server only has one method called Binding (method=0b000000000001),
     * and this method don't have any atribute to srecive from the client
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
     * is only used to make easy read a part of this number
     */
    union __attribute__ ((__packed__)) {
        quint8 bytes[4];
        quint16 shorts[2];
        quint32 num = __bswap_32(0x2112A442);
    } Magic_Cookie;

    /**
     * @brief The Transaction_ID union
     *
     * This is a unique id create by the client that the server
     * must always reply the same
     */
    union __attribute__ ((__packed__)) {
        quint8 bytes[12];
        quint16 shorts[6];
        quint32 ints[3];
    } Transaction_ID;
};

#endif // STUNHEADERMODEL_H
