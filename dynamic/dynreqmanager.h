#ifndef DYNREQMANAGER_H
#define DYNREQMANAGER_H

/*  предоставляет данные объектов по динамическим запросам  */
#include "link/objlist.h"
#include <QString>

class DynReqManager
{
    static const QString discrFileName;
    static const QString analogFileName;
    static const QString messageFileName;
    static const QString obStatFileName;
    static QString getDiscreteData(const QString &reqObName);
    static QString getAnalogData(const QString &reqObName);
    static QString getMessageData(const QString &reqObName);
    static QString geObtStatusData(const QString &reqObName);
public:
    DynReqManager();
    static QString getData(const QString& reqFileName, const QString &reqObName);
};

#endif // DYNREQMANAGER_H
