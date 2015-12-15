#include "objectvars.h"

ObjectVars::ObjectVars()
{

}

ObjectVars::ObjectVars(const ObjectVars &)
{

}

ObjectVars &ObjectVars::operator=(const ObjectVars &)
{
    ObjectVars* v = new ObjectVars();
    return *v;
}

ObjectVars::~ObjectVars()
{
    foreach(AnalogDataVar* v, anVars) {
        delete v;
    }
    foreach (DiscreteDataVar* v, dVars) {
        delete v;
    }
}

void ObjectVars::addAnalogVar(const AnalogDataVar &v)
{
    AnalogDataVar* var = new AnalogDataVar(v);
    anVars.append(var);
}

void ObjectVars::addDiscreteVar(const DiscreteDataVar &v)
{
    DiscreteDataVar* var = new DiscreteDataVar(v);
    dVars.append(var);
}
