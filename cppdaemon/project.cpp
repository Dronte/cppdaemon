#include "project.h"
#include<QDebug>

Project::Project(QObject *parent) : QObject(parent),
    snapshotUpdater(&threadPool)
{

}

Project::~Project()
{

}

void Project::setSource(int taskId,const QString fileName,const QByteArray & source){
    qDebug()<<"settingSource";
    snapshotUpdater.addSource(fileName,source);
    /*
    for(auto task:tasks){
        task->cancel();
    }
    */
}

/*void Project::findUsages(int taskId,const QString & document,int row,int line){
    qDebug()<<"Finding Usages!";
}*/
