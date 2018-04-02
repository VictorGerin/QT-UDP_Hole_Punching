#include <QCoreApplication>
#include "server.h"
#include "cliente.h"

#include <QDataStream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server *s = new Server();
//    Cliente *c = new Cliente();

    return a.exec();
}
