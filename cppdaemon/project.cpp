#include "project.h"
#include<QDebug>

Project::Project(QObject *parent) : QObject(parent),
    snapshotUpdater(&threadPool,[&](const Snapshot & s){this->updateSnapshot(s);}),
    snapshotSemaphore(maxRunningTasks)
{
}

Project::~Project()
{

}

void Project::setSource(int taskId,const QString fileName,const QByteArray & source){
    qDebug()<<"settingSource";
    snapshotUpdater.setSource(fileName,source);
    /*
    for(auto task:tasks){
        task->cancel();
    }
    */
}

/*void Project::findUsages(int taskId,const QString & document,int row,int line){
    qDebug()<<"Finding Usages!";
}*/

void Project::updateSnapshot(const Snapshot & snapshot){
    snapshotSemaphore.acquire(maxRunningTasks);
    currentSnapshot=snapshot;
    snapshotSemaphore.release(maxRunningTasks);
}
