#include "linkmanager.h"
#include <QStringList>
#include <QSharedPointer>

LinkManager::LinkManager(ObjList *obj_list):objects(obj_list)
{
    QStringList grNames = objects->getGroupNames();
    foreach (QString grName, grNames) {
        QStringList obNames = objects->getObjNamesInGroup(grName);
        foreach (QString obName, obNames) {
            QSharedPointer<ObjectData> ob = objects->getObject(grName, obName);
            ObjectManager* obMan = new ObjectManager(ob);
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
