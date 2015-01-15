#include<QString>
#include<QObject>
#include<QLocalSocket>
#include<string>
#include "socketlistener.h"

Server::Server(const QString  & bindTo)
    :bindTo(bindTo)
{
    localServer.listen(bindTo);
    connect(&localServer,SIGNAL(newConnection()),this,SLOT(connected()));
    qDebug()<<"localServer state:"<<localServer.isListening();
    connect(this,SIGNAL(requested(qintptr,QByteArray)),&api,SLOT(processRequest(qintptr,QByteArray)));
    connect(&api,SIGNAL(requestFinished(qintptr,QByteArray)),this,SLOT(write(qintptr,QByteArray)));

}

void Server::connected(){
    qDebug()<<"NewConnection!";
    QLocalSocket * connection = localServer.nextPendingConnection();
    connections[connection->socketDescriptor()] = connection;
    connect(connection,SIGNAL(readyRead()),&readSlotsMapper,SLOT(map()));
    readSlotsMapper.setMapping(connection,QString::number(connection->socketDescriptor()));
    connect(&readSlotsMapper,SIGNAL(mapped(const QString &)),
            this,SLOT(read(const QString &)));
    //TODO connect(connection,SIGNAL(error()),this,SLOT(error));
    readSlotsMapper.setMapping(connection,QString::number(connection->socketDescriptor()));
    connect(&readSlotsMapper,SIGNAL(mapped(const QString &)),this,SLOT(disconnected(const QString &)));
}

void Server::write(qintptr socketDescriptor,QByteArray answer){
   if(connections.find(socketDescriptor)==connections.end()){
       return;
   }
   QLocalSocket*socket = connections[socketDescriptor];
   socket->write(answer);
}

void Server::read(const QString & socketDescriptorString){
   qDebug()<<"read sender:"<<sender();
   qintptr socketDescriptor = stoll(socketDescriptorString.toStdString());
   qDebug()<<"readingFrom "<<socketDescriptor;
   if(connections.find(socketDescriptor)==connections.end()){
       return;
   }
   QLocalSocket*socket = connections[socketDescriptor];
   QDataStream in(socket);
   QByteArray message;
   if(bytesPending.find(socketDescriptor)==bytesPending.end()){
       if(socket->bytesAvailable() >= 8){
           quint64 size;
           in  >> size;
           bytesPending[socketDescriptor] = size;
       }else{
           return;
       }
   }
   if(socket->bytesAvailable() >= bytesPending[socketDescriptor]){
       char *buffer = new char[bytesPending[socketDescriptor]+1];
       buffer[bytesPending[socketDescriptor]] = 0;
       in.readRawData(buffer,bytesPending[socketDescriptor]);
       bytesPending.remove(socketDescriptor);
       message = QByteArray(buffer);
       delete[] buffer;
   }else{
      return;
   }
   emit requested(socketDescriptor,message);
}

void Server::disconnected(const QString & socketDescriptorString){
    qintptr socketDescriptor = stoll(socketDescriptorString.toStdString());
    qDebug()<<socketDescriptor<<" disconnected!";
    QLocalSocket* sender = connections[socketDescriptor];
    qDebug()<<"disconnected"<<sender<<" "<<QObject::sender();
    qDebug()<<"mapping "<<readSlotsMapper.mapping(socketDescriptorString);
    readSlotsMapper.removeMappings(sender);
    readSlotsMapper.removeMappings(NULL);
    //disconnectSlotsMapper.removeMappings(sender);
    connections.remove(socketDescriptor);
    // FIXME A bug here
}
Server::~Server()
{
    localServer.close();
    localServer.removeServer(bindTo);
}

