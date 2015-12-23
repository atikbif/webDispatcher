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
    QString htmlPageName;
    void addController(QSharedPointer<ControllerData> contr) {controllers.append(contr);}
    int waitTime;
    int period;
public:
    ObjectData(const QString &fName);
    int getContrCount(void) {return controllers.count();}
    QSharedPointer<ControllerData> getController(int num);
    QString getName(void) {return name;}
    QString getComment(void) {return comment;}
    QString getHTMLPageName(void) {return htmlPageName;}
    int getWaitTime() const {return waitTime;}
    int getPeriod() const {return period;}
    ~ObjectData();
};

#endif // OBJECTDATA_H
