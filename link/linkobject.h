#ifndef LINKOBJECT_H
#define LINKOBJECT_H

#include <QObject>
#include "objectdata.h"
#include <QSharedPointer>
#include <QMutex>
#include <QVector>
#include "Protocols/request.h"
#include <QHash>
#include "dynamic/objectvars.h"
#include <QUdpSocket>

class LinkObject: public QObject
{
    Q_OBJECT
    QSharedPointer<ObjectData> obPtr;
    QMutex mutex;
    bool stopCmd;
    QVector<Request*> getRequestLine(QSharedPointer<ControllerData> plc);
    QHash<int, unsigned int> getAnswers(QVector<Request*> reqs, QUdpSocket &udp);
    void answerAnalyse(QSharedPointer<ControllerData> plc, QHash<int,unsigned int> answerData, ObjectVars &obVars);
    void discreteAnalyse(QSharedPointer<ControllerData> plc, ObjectVars &obVars);
    void messageAnalyse(QSharedPointer<ControllerData> plc, ObjectVars &obVars);
    void colourAnalyse(ObjectVars &obVars);
public:
    explicit LinkObject(QSharedPointer<ObjectData> ptr, QObject *parent = 0);
    ~LinkObject();
    void stopScanning(void);
public slots:
    void startScanning(void);
};

#endif // LINKOBJECT_H
