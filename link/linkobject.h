#ifndef LINKOBJECT_H
#define LINKOBJECT_H

#include <QObject>
#include "objectdata.h"
#include <QSharedPointer>
#include <QMutex>

class LinkObject: public QObject
{
    Q_OBJECT
    QSharedPointer<ObjectData> obPtr;
    QMutex mutex;
    bool stopCmd;
public:
    explicit LinkObject(QSharedPointer<ObjectData> ptr, QObject *parent = 0);
    ~LinkObject();
    void stopScanning(void);
public slots:
    void startScanning(void);
};

#endif // LINKOBJECT_H
