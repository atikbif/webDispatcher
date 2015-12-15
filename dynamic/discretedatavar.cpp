#include "discretedatavar.h"

DiscreteDataVar::DiscreteDataVar():id(""),col(GRAY),value(false)
{

}

DiscreteDataVar::DiscreteDataVar(const QString &id):col(GRAY),value(false)
{
    this->id = id;
}
