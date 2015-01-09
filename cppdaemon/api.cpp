#include "api.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "task.h"

Api::Api(QObject *parent) : QObject(parent)
{
    connect(&taskReadyMapper,SIGNAL(mapped(QString)),this,SLOT(taskReady(QString)));
    connect(&taskFinishedMapper,SIGNAL(mapped(QString)),this,SLOT(taskFinished(QString)));
}

Api::~Api()
{

}

void Api::taskReady(QString socketDescriptorString){
    
}

void Api::taskFinished(QString socketDescriptorString){
    
}

void Api::processRequest(qintptr socketDescriptor,QByteArray request){
    QJsonDocument document = QJsonDocument::fromJson(request);
    QJsonParseError error;
    if(document.isObject()){
        QJsonObject object = document.object();
        QJsonValue methodValue = object["method"];
        QJsonValue arguments = object["arguments"];
        if(!methodValue.isString()){
            //ERROR
        }
        QString method = methodValue.toString();
        if(method==QString("setSource")){
            setSource(arguments);
        }else if(method==QString("complete")){
            //complete(socketDescriptor,request);
        }
    }else{
        //TODO handle an error;
    }
}

void Api::setSource(const QJsonValue & arguments)const{
    QJsonValue fileName = arguments.object()["fileName"];
    QJsonValue text = arguments.object()["source"];
    project.setSource(fileName.toString(),text.toString().toUtf8());
}

/*
void Api::complete(qintptr socketDescriptor,QJsonObject parameters){
    Task * task = project.complete
    connect(task,SIGNAL(ready()),&taskReadyMapper,SLOT(map));
    taskReadyMapper.setMapping(task,QString::number(socketDescriptor));
}
*/

void Api::handleNewTask(Task * task,qintptr socketDescriptor){
    connect(task,SIGNAL(ready()),&taskReadyMapper,SLOT(map()));
    taskReadyMapper.setMapping(task,QString::number(socketDescriptor));

    connect(task,SIGNAL(ready()),&taskFinishedMapper,SLOT(map()));
    taskFinishedMapper.setMapping(task,QString::number(socketDescriptor));
}

