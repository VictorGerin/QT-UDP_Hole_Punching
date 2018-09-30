#ifndef ADDRESSMODEL_H
#define ADDRESSMODEL_H

#include <QObject>


/**
 * @brief The Address union
 * This union holds a ipv4 or ipv6 addres
 */
class __attribute__ ((__packed__)) AddressModel
{
public:
    union __attribute__ ((__packed__)) {
        quint8 bytes[4];
        quint32 num;
    } ipv4;

    union __attribute__ ((__packed__)) {
        quint8 bytes[16];
        quint64 num[2];
    } ipv6;
};

#endif // ADDRESSMODEL_H
