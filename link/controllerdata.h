#ifndef CONTROLLERDATA_H
#define CONTROLLERDATA_H

/* свойства контроллера */

#include <QString>
#include <QByteArray>
#include <QObject>
#include <QSharedPointer>
#include "variables.h"

class ControllerData: public QObject
{
    Q_OBJECT
    QString ipAddr;
    int portNum;
    QSharedPointer<VarStorage> vars;
    QString comment;
public:
    ControllerData(const QString &fName, QObject *parent = 0);
    QSharedPointer<VarStorage> getVars(void) {return vars;}
    QString getComment(void) const {return comment;}
    QString getIP(void) const {return ipAddr;}
    int getPortNum(void) const {return portNum;}
public slots:
    void updData(int memAddr, const QByteArray &data);
};



#endif // CONTROLLERDATA_H
