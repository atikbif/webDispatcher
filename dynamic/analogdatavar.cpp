#include "analogdatavar.h"

AnalogDataVar::AnalogDataVar():value(0),id(""),col(GRAY)
{

}

AnalogDataVar::AnalogDataVar(const QString &id):value(0),col(GRAY)
{
    this->id = id;
}
