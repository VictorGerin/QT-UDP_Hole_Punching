#include <QCoreApplication>

#include "stunclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    StunClient s("udp://iphone-stun.strato-iphone.de:3478");
//    StunClient s("udp://stun.12connect.com:3478");

    return a.exec();
}
