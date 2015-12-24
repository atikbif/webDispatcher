#ifndef LINKMANAGER_H
#define LINKMANAGER_H

#include "objlist.h"
#include "objectmanager.h"
#include <QVector>
#include <QObject>

class LinkManager: public QObject
{
    Q_OBJECT
    ObjList* objects;
    QVector<ObjectManager*> children;
public:
    LinkManager(ObjList* obj_list, QObject *parent = 0);
    ~LinkManager();
signals:
    void correctAnswer(QString ip);
    void noAnswer(QString ip);
};

#endif // LINKMANAGER_H
