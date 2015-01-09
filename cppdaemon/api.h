#ifndef API_H
#define API_H

#include <QObject>
#include <QSignalMapper>
#include <FindUsages.h>
#include "project.h"
using namespace CPlusPlus;

class Api : public QObject
{
    Q_OBJECT
private:
    Project project;
    QMap<int,qintptr> taskSocketMap;

    void setSource(int taskId,const QJsonObject & arguments);
    void findUsages(int taskId,qintptr socketDescriptor,const QJsonObject & arguments);
public:
    explicit Api(QObject *parent = 0);
    ~Api();

public slots:
    void findUsagesFinished(int taskId,QList<Usage>);

    void processRequest(qintptr,QByteArray request);
signals:
    void requestFinished(qintptr,QByteArray);
};

#endif // API_H
