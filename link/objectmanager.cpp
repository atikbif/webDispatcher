#include "objectmanager.h"

ObjectManager::ObjectManager(QSharedPointer<ObjectData> obData, QObject *parent): QObject(parent)
{
    ob = new LinkObject(obData);
    ob->moveToThread(&obThread);
    connect(&obThread,SIGNAL(finished()),ob,SLOT(deleteLater()));
    connect(this,SIGNAL(startScanning()),ob,SLOT(startScanning()));
    connect(ob,SIGNAL(correctAnswer(QString)),this,SIGNAL(correctAnswer(QString)));
    connect(ob,SIGNAL(noAnswer(QString)),this,SIGNAL(noAnswer(QString)));
    obThread.start();
    emit startScanning();
}

ObjectManager::~ObjectManager()
{
    ob->stopScanning();
    obThread.quit();
    obThread.wait();
}
