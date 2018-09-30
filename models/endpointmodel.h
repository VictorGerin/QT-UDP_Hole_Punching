#ifndef ENDPOINTMODEL_H
#define ENDPOINTMODEL_H

#include <QObject>
#include "addressmodel.h"

/**
 * @brief The EndPoint struct
 * End point by defenition is any pair of Andress and port
 * used to comunicate
 */
class __attribute__ ((__packed__)) EndPointModel
{
public:
    quint16 port;
    AddressModel address;
};

#endif // ENDPOINTMODEL_H
