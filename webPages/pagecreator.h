#ifndef PAGECREATOR_H
#define PAGECREATOR_H

#include "link/objlist.h"


class PageCreator
{
    ObjList &objects;
    void createIndexFile(const QString &fName);
    void createObjFile(QSharedPointer<ObjectData> ob);
    void createObjStyle(const QString &fName);
public:
    PageCreator(ObjList &m_objects);
    void createPages(void);
};

#endif // PAGECREATOR_H
