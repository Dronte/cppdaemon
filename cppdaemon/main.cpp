#include <QCoreApplication>
#include <QString>
#include <QList>
#include <QDebug>
#include <cplusplus/FindUsages.h>
#include"cpphelper.h"

using namespace CPlusPlus;

void printUsage(const Usage & u){
    qDebug()<<u.path<<" "<<u.lineText<<" "<<u.col<<" "<<u.line<<endl;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CppHelper helper;
    helper.add(QString("/home/dronte/cppdaemon/cppdaemon/cppdaemon/main_corr.cpp")); // Add an existing cpp file here
    QList<Usage>list = helper.findUsages(18,10);
    qDebug()<<"list.size()"<<list.size();
    foreach (auto u, list){
        printUsage(u);
    }

    //qDebug()<<list;
    return 0;
}
