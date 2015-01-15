#ifndef SNAPSHOTUPDATER_H
#define SNAPSHOTUPDATER_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include<CppDocument.h>
#include<functional>
#include "task.h"

using namespace CPlusPlus;

class SnapshotUpdater;

class SnapshotUpdater: public QObject
{
    Q_OBJECT;
private:
    class SourceParser{
    private:
        SnapshotUpdater *snapshotUpdater;
    public:
        explicit SourceParser(SnapshotUpdater*sourceParser);
        void operator()(const QString&fileName);
    };
    std::function<void (const Snapshot &)> callback;
    QMutex snapshotMutex;
    Snapshot snapshot;
    QThreadPool *threadPool;
    QMutex pendingSourceMutex;
    QMap<QString/*filename*/,QByteArray/*source*/> pendingSource;
    QSet<QString> includePaths;
    QFutureWatcher<void> processing;
    SourceParser sourceParser;
    friend void setSourceFuture(SnapshotUpdater * snapshotUpdater,const QString & fileName,const QByteArray & source);
public:
    void setSource(const QString & fileName,const QByteArray & source);
    void runProcessing();
    SnapshotUpdater(QThreadPool * threadPool,std::function<void (const Snapshot &)> callback);
    ~SnapshotUpdater();
//public slots:
    //void snapshotUpdated();
signals:

};
void setSourceFuture(SnapshotUpdater * snapshotUpdater,const QString & fileName,const QByteArray & source);

#endif // SNAPSHOTUPDATER_H
