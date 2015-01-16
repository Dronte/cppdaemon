#ifndef SOCKETLISTENER_H
#define SOCKETLISTENER_H

#include <QObject>
#include <QLocalServer>
#include <QSignalMapper>
#include "api.h"

class QLocalServer;
class Server:public QObject
{
    Q_OBJECT
private:
    QLocalServer localServer;
    QString bindTo;
    QMap<qintptr,QLocalSocket*> connections;
    QMap<qintptr,qint64> bytesPending;
    QSignalMapper readSlotsMapper;
    //QSignalMapper readNextSlotsMapper;
    QSignalMapper disconnectSlotsMapper;
    Api api;
public:
    Server(const QString & bindTo);
    ~Server();
private slots:
    void connected();
    void disconnected(const QString & socketDescriptor);
    void read(const QString & socketDescriptor);
    void write(qintptr socketDescriptor,QByteArray answer);
signals:
    void requested(qintptr socketDescriptor,const QByteArray &);
    void readNext(QString);
};

#endif // SOCKETLISTENER_H
