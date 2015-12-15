#include "datastorage.h"

void DataStorage::updateObject(const ObjectVars &ob)
{
    Q_UNUSED(ob)
    QMutexLocker locker(&mutex);
}

DataStorage::DataStorage()
{

}
