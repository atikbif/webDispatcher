#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <QObject>
#include <QThread>
#include "linkobject.h"
#include <QSharedPointer>

class ObjectManager: public QObject
{
    Q_OBJECT
    QThread obThread;
    LinkObject *ob;
public:
    explicit ObjectManager(QSharedPointer<ObjectData> obData, QObject *parent = 0);
    ~ObjectManager();
signals:
    void startScanning(void);
    void correctAnswer(QString ip);
    void noAnswer(QString ip);
};

#endif // OBJECTMANAGER_H
