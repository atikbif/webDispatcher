#include "dynreqmanager.h"

DynReqManager::DynReqManager(ObjList &m_objects):objects(m_objects)
{

}

QString DynReqManager::getData(const QString &reqName)
{
    Q_UNUSED(reqName)
    return QString();
}

