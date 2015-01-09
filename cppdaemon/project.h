#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include<QAtomicPointer>
#include<QThreadPool>
#include<QList>

#include<FindUsages.h>
using namespace CPlusPlus;

class Project : public QObject
{
    Q_OBJECT
private:
    QAtomicPointer<Snapshot*> currentSnapshot;
    QAtomicPointer<Snapshot*> nextSnapshot;
    QThreadPool threadPoll;

    void findUsagesFuture(const QString & document,int row,int line)const;
public:
    explicit Project(QObject *parent = 0);
    ~Project();
    void complete(const QString & document,int row,int line);
    void findUsages(int taskId,const QString & document,int row,int line);
    void setSource(int taskId,const QString filename,const QByteArray & source);
signals:
    void findUsagesFinished(int,QList<Usage> );
    //void setSourceFinished();
};

#endif // PROJECT_H
