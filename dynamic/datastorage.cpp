#include "datastorage.h"

void DataStorage::clear()
{
    QMutexLocker locker(&mutex);
    foreach (ObjectVars* ob, objects) {
        delete ob;
    }
}

ObjectVars DataStorage::getObject(int num) const
{
    QMutexLocker locker(&mutex);
    ObjectVars ob;
    if((num>=0)&&(num<objects.count())) {
        ob = *objects.at(num);
    }
    return ob;
}

void DataStorage::updateObject(const ObjectVars &ob)
{
    QMutexLocker locker(&mutex);
    if(ob.getID().isEmpty()) return;
    bool existFlag = false;
    foreach (ObjectVars* ob_i, objects) {
        if(ob.getID()==ob_i->getID()) {
            *ob_i = ob;
            existFlag = true;
        }
    }
    if(!existFlag) {
        ObjectVars* newOb = new ObjectVars(ob);
        objects.append(newOb);
    }
}

DataStorage::DataStorage()
{

}

DataStorage::~DataStorage()
{
    clear();
}
