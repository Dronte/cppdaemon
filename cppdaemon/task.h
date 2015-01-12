#ifndef TASK_H
#define TASK_H

#include <QObject>

class Task : public QObject
{
    Q_OBJECT
protected:
    int taskId;
public:
    explicit Task(QObject *parent = 0);
    ~Task();

signals:
    void finished();
public slots:
};

class addSource: public Task
{
    Q_OBJECT
};

#endif // TASK_H
