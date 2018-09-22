#include <QCoreApplication>
#include <QCommandLineParser>
#include "server.h"
#include "cliente.h"

#include <QDataStream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("");
    parser.addHelpOption();
    parser.addVersionOption();


    QCommandLineOption cliOption(QStringList() << "c" << "cli" << "client", "Set to be the client");
    parser.addOption(cliOption);

    QCommandLineOption serOption(QStringList() << "s" << "ser" << "server", "Set to be the server");
    parser.addOption(serOption);

    QCommandLineOption serverOption("ip", "Set the rendezvous server", "Server IP");
    serverOption.setDefaultValue("localhost");
    parser.addOption(serverOption);

    QCommandLineOption portOption("port", "Set the work UDP port", "Port");
    portOption.setDefaultValue("7755");
    parser.addOption(portOption);

    // Process the actual command line arguments given by the user
    parser.process(a);

    if(parser.isSet("client")) {
        Cliente *c = new Cliente(parser.value("ip"), parser.value("port").toInt());
    } else if(parser.isSet("server")) {
        Server *s = new Server(parser.value("port").toInt());
    }

    return a.exec();
}
