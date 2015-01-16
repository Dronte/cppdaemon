#include "project.h"
#include<QDebug>

Project::Project(QObject *parent) : QObject(parent),
    snapshotUpdater(&threadPool,[&](const Snapshot & s){qDebug()<<"Callback!";this->updateSnapshot(s);}),
    snapshotSemaphore(maxRunningTasks)
{
    qRegisterMetaType<QList<Usage> >("QList<Usage>");
}

Project::~Project()
{

}

void Project::setSource(int taskId,const QString & fileName,const QByteArray & source){
    qDebug()<<"settingSource "<<fileName;
    snapshotUpdater.setSource(fileName,source);
}

void findUsagesFuture(Project * project,int taskId,const QString & fileName,int line,int row){
    qDebug()<<"findUsagesFuture"<<project->currentSnapshot.size();
    project->snapshotSemaphore.acquire();
    if(!project->currentSnapshot.contains(fileName)){
       //ERROR
        qDebug()<<"file"<<fileName<<"Not in snapshot";
        return;
    }
    const Document::Ptr & document = project->currentSnapshot.document(fileName);
    qDebug()<<"is tokenized"<<document->isTokenized();
    FindUsages findUsages(document->utf8Source(),document,project->currentSnapshot);
    findUsages(document->lastVisibleSymbolAt(line,row));
    QList<Usage> usages=findUsages.usages();
    project->snapshotSemaphore.release();
    emit project->findUsagesFinished(taskId,usages);
}

void Project::findUsages(int taskId,const QString & fileName,int row,int line){
    qDebug()<<"Finding Usages!";
    QtConcurrent::run(&threadPool,findUsagesFuture,this,taskId,fileName,row,line);
    // It is possible not to use friend functions using lambdas but code turnes out to be ugly
}

void Project::updateSnapshot(const Snapshot & snapshot){
    snapshotSemaphore.acquire(maxRunningTasks);
    currentSnapshot=snapshot;
    snapshotSemaphore.release(maxRunningTasks);
}
