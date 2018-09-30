#ifndef MAPPEDADDRESSMODEL_H
#define MAPPEDADDRESSMODEL_H

#include <QObject>
#include "stunattributemodel.h"
#include "endpointmodel.h"


/**
 * @brief The MappedAddress struct
 *
 * The most import and simple Atribute is the MappedAddress
 * is here that the client can know he public end point (ip + udp port)
 *
 */
class __attribute__ ((__packed__)) MappedAddressModel : public StunAttributeModel
{
public:
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
        EndPointModel point;
};

#endif // MAPPEDADDRESSMODEL_H
