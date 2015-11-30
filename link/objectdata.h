#ifndef OBJECTDATA_H
#define OBJECTDATA_H

/* свойства объекта */

#include "controllerdata.h"
#include <QVector>
#include <QString>
#include <QSharedPointer>

class ObjectData
{
    QString name;
    QString comment;
    QVector<QSharedPointer<ControllerData> > controllers;
    int num;
    void addController(QSharedPointer<ControllerData> contr) {controllers.append(contr);}
public:
    ObjectData(const QString &fName);
    int getContrCount(void) {return controllers.count();}
    QSharedPointer<ControllerData> getController(int num);
    QString getName(void) {return name;}
    QString getComment(void) {return comment;}
    int getNum(void) const {return num;}
    ~ObjectData();
};

#endif // OBJECTDATA_H
