#include "api.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QList>
Api::Api(QObject *parent) : QObject(parent)
{
    connect(&project,SIGNAL(findUsagesFinished(int,QList<Usage>)),
            this,SLOT(findUsagesFinished(int,QList<Usage>)) );
}

Api::~Api()
{

}

void Api::processRequest(qintptr socketDescriptor,QByteArray request){
    QJsonDocument document = QJsonDocument::fromJson(request);
    //QJsonParseError error;
    if(document.isObject()){
        QJsonObject object = document.object();
        QJsonValue methodValue = object["method"];
        QJsonValue taskIdValue = object["taskId"];
        QJsonValue argumentsValue = object["arguments"];
        if(!methodValue.isString()){
            //ERROR
        }
        if(!argumentsValue.isObject()){
            //ERROR
        }
        QString method = methodValue.toString();
        qDebug()<<"method="<<method;
        QJsonObject arguments = argumentsValue.toObject();
        int taskId = taskIdValue.toInt();
        if(method==QString("setSource")){
            setSource(taskId,arguments);
        }else if(method==QString("complete")){
            //complete(socketDescriptor,request);
        }else if(method==QString("findUsages")){
            findUsages(taskId,socketDescriptor,arguments);
        }
    }else{
        //TODO handle an error;
    }
}

void Api::setSource(int taskId,const QJsonObject & arguments){
    QJsonValue fileName = arguments["fileName"];
    QJsonValue text = arguments["source"];
    project.setSource(taskId,fileName.toString(),text.toString().toUtf8());
}

void Api::findUsages(int taskId,qintptr socketDescriptor,const QJsonObject & arguments){
    QJsonValue fileNameValue = arguments["fileName"];
    QJsonValue lineValue = arguments["line"];
    QJsonValue rowValue = arguments["row"];
    QString fileName = fileNameValue.toString();
    int line = lineValue.toInt();
    int row = rowValue.toInt();
    //TODO error handling
    taskSocketMap[taskId]=socketDescriptor;
    project.findUsages(taskId,fileName,line,row);
}

void Api::findUsagesFinished(int taskId,QList<Usage> usages){
    qintptr socketDescriptor = taskSocketMap[taskId];
    taskSocketMap.remove(taskId);
    QJsonObject usagesObject;
    QMap<QString,QJsonArray> usagesMap;
    for(auto usage:usages){
        qDebug()<<usage.lineText;
        QJsonObject usageObject;
        usageObject["col"] = usage.col;
        usageObject["line"] = usage.line;
        usageObject["len"] = usage.len;
        usageObject["lineText"] = usage.lineText;
        if(usagesMap.find(usage.path) == usagesMap.end()){
            usagesMap[usage.path] = QJsonArray();
        }
        usagesMap[usage.path].append(usageObject);
    }
    for(auto usagePath:usagesMap.keys()){
        usagesObject[usagePath] = usagesMap[usagePath];
    }
    QJsonObject answer;
    answer["usages"] = usagesObject;
    qDebug()<<"result:"<<QJsonDocument(answer).toJson();
    emit requestFinished(socketDescriptor,QJsonDocument(answer).toJson());
}



