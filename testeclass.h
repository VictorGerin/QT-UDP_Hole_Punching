#ifndef TESTECLASS_H
#define TESTECLASS_H

#include <QObject>

#include "conn.h"

class TesteClass : public QObject
{
    Q_OBJECT
public:
    explicit TesteClass(QObject *parent = nullptr);

signals:

public slots:

    void sendToAllPeopleInRoom();
    void reciveData();

private:
    Conn *connection;
    void timerEvent(QTimerEvent *);
};

#endif // TESTECLASS_H
