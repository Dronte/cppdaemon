#include "snapshotupdater.h"
#include<QMutexLocker>

SnapshotUpdater::SnapshotUpdater(QThreadPool * threadPool,std::function<void (const Snapshot &)> callback):
    callback(callback),threadPool(threadPool),sourceParser(this)
{
    //connect(&processing,SIGNAL(finished()),this,SLOT(snapshotUpdated()));
}

SnapshotUpdater::~SnapshotUpdater()
{ }

SnapshotUpdater::SourceParser::SourceParser(SnapshotUpdater* snapshotUpdater):snapshotUpdater(snapshotUpdater){};

void SnapshotUpdater::setSource(const QString & fileName,const QByteArray & source){
    QFuture<void> future = QtConcurrent::run(threadPool,setSourceFuture,this,fileName,source);
}

void setSourceFuture(SnapshotUpdater * snapshotUpdater,const QString & fileName,const QByteArray & source){
    snapshotUpdater->processing.cancel();
    snapshotUpdater->processing.waitForFinished();
    QMutexLocker locker(&snapshotUpdater->pendingSourceMutex);
    snapshotUpdater->pendingSource[fileName]=source;
    auto fileNameList = snapshotUpdater->pendingSource.keys();
    snapshotUpdater->processing.setFuture(QtConcurrent::map(fileNameList,
                                                    snapshotUpdater->sourceParser));
}

void SnapshotUpdater::SourceParser::operator()(const QString & fileName){
    qDebug()<<"SnapshotUpdater::SourceParser operator()"<<fileName;
    QMutexLocker locker(&snapshotUpdater->pendingSourceMutex);
    auto document = snapshotUpdater->snapshot.preprocessedDocument(
                    snapshotUpdater->pendingSource[fileName],fileName);
    document->tokenize();
    document->parse();
    document->check();
    snapshotUpdater->snapshot.insert(document);
    snapshotUpdater->pendingSource.remove(fileName);
    if(snapshotUpdater->pendingSource.isEmpty()){
        snapshotUpdater->callback(snapshotUpdater->snapshot);
    }
    /*for(auto includedFile:snapshotUpdater->snapshot.allIncludesForDocument(document)){
        if(snapshotUpdater->pendingSource.find(includedFile)!=snapshotUpdater->pendingSource.end()){
            for(auto includePath:includePaths){
                if
            }
        }
    }*/
}

