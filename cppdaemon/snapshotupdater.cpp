#include "snapshotupdater.h"
#include<QMutexLocker>

SnapshotUpdater::SnapshotUpdater(QThreadPool * threadPool):threadPool(threadPool)
{

}

SnapshotUpdater::~SnapshotUpdater()
{

}

void SnapshotUpdater::addSource(const QString & fileName,const QByteArray & source){
    QFuture<void> future = QtConcurrent::run(threadPool,addSourceFuture,this,fileName,source);
}

void addSourceFuture(SnapshotUpdater * snapshotUpdater,const QString & fileName,const QByteArray & source){
    snapshotUpdater->processing.cancel();
    snapshotUpdater->processing.waitForFinished();
    QMutexLocker locker(&snapshotUpdater->pendingSourceMutex);
    snapshotUpdater->pendingSource[fileName]=source;
    snapshotUpdater->processing.setFuture(QtConcurrent::map(snapshotUpdater->pendingSource,
                                                    snapshotUpdater->sourceParser));
}

void SnapshotUpdater::SourceParser::operator()(const QString & fileName){
    qDebug()<<"SnapshotUpdater::SourceParser operator()"<<fileName;
    //snapshot.insert(snapshot.documentFromSource)
}

