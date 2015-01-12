#ifndef SNAPSHOTUPDATER_H
#define SNAPSHOTUPDATER_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include<CppDocument.h>
#include "task.h"

using namespace CPlusPlus;

class SnapshotUpdater:QObject
{
    Q_OBJECT;
private:
    class SourceParser{
    public:
        void operator()(const QString&fileName);
    };
    Snapshot snapshot;
    QThreadPool *threadPool;
    QMutex pendingSourceMutex;
    QMap<QString/*filename*/,QByteArray/*source*/> pendingSource;
    QFutureWatcher<void> processing;
    SourceParser sourceParser;
    friend void addSourceFuture(SnapshotUpdater * snapshotUpdater,const QString & fileName,const QByteArray & source);
public:
    void addSource(const QString & fileName,const QByteArray & source);
    void runProcessing();
    SnapshotUpdater(QThreadPool * threadPool);
    ~SnapshotUpdater();
};
void addSourceFuture(SnapshotUpdater * snapshotUpdater,const QString & fileName,const QByteArray & source);

#endif // SNAPSHOTUPDATER_H
