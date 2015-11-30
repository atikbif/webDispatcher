#ifndef DYNREQMANAGER_H
#define DYNREQMANAGER_H

/*  предоставляет данные объектов по динамическим запросам  */
#include "link/objlist.h"

class DynReqManager
{
    ObjList& objects;
public:
    DynReqManager(ObjList &m_objects);
    QString getData(const QString& reqName);
};

#endif // DYNREQMANAGER_H
