#include "controllerdata.h"

ControllerData::ControllerData(const QString &fName, QObject *parent):
    QObject(parent),ipAddr(""),portNum(0),vars(new VarStorage()), comment("")
{
    Q_UNUSED(fName)
}


void ControllerData::updData(int memAddr, const QByteArray &data)
{
    Q_UNUSED(memAddr)
    Q_UNUSED(data)
}

