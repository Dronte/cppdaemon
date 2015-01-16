#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include<QAtomicPointer>
#include<QThreadPool>
#include<QList>
#include<QSemaphore>

#include<FindUsages.h>
#include"snapshotupdater.h"
using namespace CPlusPlus;

class Project : public QObject
{
    Q_OBJECT
private:
    static const int maxRunningTasks = 256;
    QSemaphore snapshotSemaphore;
    Snapshot currentSnapshot;
    QThreadPool threadPool;
    SnapshotUpdater snapshotUpdater;
    friend void findUsagesFuture(Project * project,int taskId,const QString & fileName,int line,int row);
public:
    void findUsages(int taskId,const QString & document,int row,int line);
    explicit Project(QObject *parent = 0);
    ~Project();
    //void complete(const QString & document,int row,int line);
    //void findUsages(int taskId,const QString & document,int row,int line);
    void setSource(int taskId,const QString & filename,const QByteArray & source);
    void updateSnapshot(const Snapshot & snapshot);
signals:
    void findUsagesFinished(int,QList<Usage> );
    //void setSourceFinished();
};

#endif // PROJECT_H
