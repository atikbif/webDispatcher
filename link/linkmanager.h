#ifndef LINKMANAGER_H
#define LINKMANAGER_H

#include "objlist.h"
#include "objectmanager.h"
#include <QVector>

class LinkManager
{
    ObjList* objects;
    QVector<ObjectManager*> children;
public:
    LinkManager(ObjList* obj_list);
    ~LinkManager();
};

#endif // LINKMANAGER_H
