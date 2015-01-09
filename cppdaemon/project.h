#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include<QAtomicPointer>
#include<QThreadPool>
using namespace CPlusPlus;
class Task;
class project : public QObject
{
    Q_OBJECT
private:
    QAtomicPointer<Snapshot*> currentSnapshot;
    QAtomicPointer<Snapshot*> nextSnapshot;
    QThreadPool threadPoll;
public:
    explicit project(QObject *parent = 0);
    ~project();
    Task* complete(const QString & document,int row,int line);
    Task* setSource(const QString filename,const QByteArray & source);
signals:

public slots:
};

#endif // PROJECT_H
