#include "objectvars.h"

void ObjectVars::clear()
{
    foreach(AnalogDataVar* v, anVars) {
        delete v;
    }
    anVars.clear();
    foreach (DiscreteDataVar* v, dVars) {
        delete v;
    }
    dVars.clear();
    foreach (MessageDataVar *v, mVars) {
        delete v;
    }
    mVars.clear();
}

ObjectVars::ObjectVars():col(GRAY),varsTime(QDateTime::currentDateTime()),id("")
{

}

ObjectVars::ObjectVars(const ObjectVars &ob)
{
    clear();
    col = ob.getColour();
    varsTime = ob.getTime();
    id = ob.getID();
    for(int i=0;i<ob.getAnVarCount();i++) {
        addAnalogVar(ob.getAnalogVar(i));
    }
    for(int i=0;i<ob.getDiscrVarCount();i++) {
        addDiscreteVar(ob.getDiscreteVar(i));
    }
    for(int i=0;i<getMessageVarCount();i++) {
        addMessageVar(ob.getMessageVar(i));
    }
}

ObjectVars &ObjectVars::operator=(const ObjectVars &ob)
{
    if (this == &ob)
    {
        return *this;
    }
    clear();
    col = ob.getColour();
    varsTime = ob.getTime();
    id = ob.getID();
    for(int i=0;i<ob.getAnVarCount();i++) {
        addAnalogVar(ob.getAnalogVar(i));
    }
    for(int i=0;i<ob.getDiscrVarCount();i++) {
        addDiscreteVar(ob.getDiscreteVar(i));
    }
    for(int i=0;i<ob.getMessageVarCount();i++) {
        addMessageVar(ob.getMessageVar(i));
    }
    return *this;
}

ObjectVars::~ObjectVars()
{
    clear();
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

void ObjectVars::addMessageVar(const MessageDataVar &v)
{
    MessageDataVar* var = new MessageDataVar(v);
    mVars.append(var);
}

AnalogDataVar ObjectVars::getAnalogVar(int num) const
{
    AnalogDataVar var;
    if((num>=0)&&(num<anVars.count())) var = *anVars.at(num);
    return var;
}

DiscreteDataVar ObjectVars::getDiscreteVar(int num) const
{
    DiscreteDataVar var;
    if((num>=0)&&(num<dVars.count())) var = *dVars.at(num);
    return var;
}

MessageDataVar ObjectVars::getMessageVar(int num) const
{
    MessageDataVar var;
    if((num>=0)&&(num<mVars.count())) var = *mVars.at(num);
    return var;
}
