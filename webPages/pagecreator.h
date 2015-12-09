#ifndef PAGECREATOR_H
#define PAGECREATOR_H

#include "link/objlist.h"


class PageCreator
{
    ObjList &objects;
    void createIndexFile(const QString &fName);
    void createObjFile(QSharedPointer<ObjectData> ob);
    void createObjStyle(QSharedPointer<ObjectData> ob);
    void createObjScript(QSharedPointer<ObjectData> ob);

    void addTabs(int tabCount, QString &str);

    void createAnalogValuesScript(QSharedPointer<ObjectData> ob);
public:
    PageCreator(ObjList &m_objects);
    void createPages(void);
};

#endif // PAGECREATOR_H
