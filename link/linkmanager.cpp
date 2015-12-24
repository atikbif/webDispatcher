#include "linkmanager.h"
#include <QStringList>
#include <QSharedPointer>

LinkManager::LinkManager(ObjList *obj_list, QObject *parent):QObject(parent),objects(obj_list)
{
    QStringList grNames = objects->getGroupNames();
    foreach (QString grName, grNames) {
        QStringList obNames = objects->getObjNamesInGroup(grName);
        foreach (QString obName, obNames) {
            QSharedPointer<ObjectData> ob = objects->getObject(grName, obName);
            ObjectManager* obMan = new ObjectManager(ob);
            connect(obMan,SIGNAL(correctAnswer(QString)),this,SIGNAL(correctAnswer(QString)));
            connect(obMan,SIGNAL(noAnswer(QString)),this,SIGNAL(noAnswer(QString)));
            children.append(obMan);
        }
    }
}

LinkManager::~LinkManager()
{
    foreach(ObjectManager* ob, children) {
        delete ob;
    }
}
