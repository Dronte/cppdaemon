#ifndef API_H
#define API_H

#include <QObject>
#include <QSignalMapper>
class Task;

class Api : public QObject
{
    Q_OBJECT
private:
    void addSource(const QJsonValue & paramters)const;
    QSignalMapper taskReadyMapper;
    QSignalMapper taskFinishedMapper;
    void handleNewTask(Task * task,qintptr socketDescriptor);
    //void getSenderTask(const QString & socketDescriptor);
public:
    explicit Api(QObject *parent = 0);
    ~Api();

signals:

public slots:
    void taskReady(QString socketDescriptorString);
    void taskFinished(QString socketDescriptorString);
    void processRequest(qintptr,QByteArray request);
};

#endif // API_H
