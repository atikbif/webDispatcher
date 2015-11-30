#ifndef OBJLIST_H
#define OBJLIST_H

/* перечень всех объектов с разбиением на группы    */

#include <QString>
#include <QVector>
#include <QHash>
#include <QStringList>
#include <QSharedPointer>
#include "objectdata.h"

class ObjList
{
    QHash<QString, QVector<QSharedPointer<ObjectData> > > objGroups;
    void createWebFiles(void);  // сгенерировать шаблоны для веб страниц
public:
    ObjList(const QString &fName);
    QStringList getGroupNames(void) const {return objGroups.keys();}
    QStringList getObjNamesInGroup(const QString &grName);
    QSharedPointer<ObjectData> getObject(const QString &grName, const QString &objName);
    void updateList(const QString &fName);
};

#endif // OBJLIST_H
