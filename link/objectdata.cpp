#include "objectdata.h"

ObjectData::ObjectData(const QString &fName):name(""),comment("")
{
    Q_UNUSED(fName)
}

QSharedPointer<ControllerData> ObjectData::getController(int num)
{
    if((num>=0)&&(num<controllers.count())) {
        return  controllers[num];
    }
    return QSharedPointer<ControllerData>(new ControllerData(""));
}

ObjectData::~ObjectData()
{
    foreach (QSharedPointer<ControllerData> ptr, controllers) {
        ptr.clear();
    }
}



