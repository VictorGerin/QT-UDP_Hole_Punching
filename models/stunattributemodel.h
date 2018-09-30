#ifndef STUNATTRIBUTEMODEL_H
#define STUNATTRIBUTEMODEL_H

#include <QObject>


/**
 * @brief The StunAtribute struct
 * This struct, to me, may be call as Attribute header
 *
 * Any comunication by the server or the client, may use this attributes
 * to send information and any atribute is located right after the Stun Header.
 */
class __attribute__ ((__packed__)) StunAttributeModel
{
public:
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

#endif // STUNATTRIBUTEMODEL_H
