#include <QCoreApplication>
#include <QString>
#include <QList>
#include <QDebug>
#include <iostream>
#include "socketlistener.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Server listener("/home/dronte/cppdaemon/socket");
    return app.exec();
}
