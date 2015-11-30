#include "objlist.h"

void ObjList::createWebFiles()
{

}

ObjList::ObjList(const QString &fName)
{
    Q_UNUSED(fName)
}

QStringList ObjList::getObjNamesInGroup(const QString &grName)
{
    QStringList result;
    QVector< QSharedPointer<ObjectData> > objects = objGroups.value(grName);
    foreach(QSharedPointer<ObjectData> ptr, objects) {
        if(!ptr.isNull()) {
            result += ptr->getName();
        }
    }
    return result;
}

QSharedPointer<ObjectData> ObjList::getObject(const QString &grName, const QString &objName)
{
    QSharedPointer<ObjectData> res;
    QVector< QSharedPointer<ObjectData> > objects = objGroups.value(grName);
    foreach(QSharedPointer<ObjectData> ptr, objects) {
        if(!ptr.isNull()) {
            if(ptr->getName()==objName) {
                res=ptr;
                break;
            }
        }
    }
    return res;
}

void ObjList::updateList(const QString &fName)
{
    Q_UNUSED(fName)
}

