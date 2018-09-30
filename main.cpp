#include <QCoreApplication>

#include "testeclass.h"


/*
 * This software é divided in 2 major parts
 * The StunClient and The Torrent Tracker
 *
 * Stun is a protocol used to let know the client he current
 * state is a way to discovery the NAT end orther thinks
 *
 * Go to stunclient.h to know more
 *
 *
 * The Torrent Tracker class with name "conn.h" is a simple implementation
 * of a udp client torrent tracker, the tracker here is used to dicovery
 * orther people and to annouce the current public endpoint (ip + udp port)
 *
 * Go to conn.h to know more
 *
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    new TesteClass();

//    Conn c("udp://public.popcorn-tracker.org:6969");
//    c.startConn();

    return a.exec();
}


