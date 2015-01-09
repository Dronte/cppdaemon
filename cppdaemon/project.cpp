#include "project.h"
#include<QDebug>

Project::Project(QObject *parent) : QObject(parent)
{

}

Project::~Project()
{

}

void Project::setSource(int taskId,const QString filename,const QByteArray & source){
    qDebug()<<"settingSource";
    /*
    for(auto task:tasks){
        task->cancel();
    }
    */
}

void Project::findUsages(int taskId,const QString & document,int row,int line){
    qDebug()<<"Finding Usages!";
}
