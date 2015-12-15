#ifndef OBJECTVARS_H
#define OBJECTVARS_H

#include "analogdatavar.h"
#include "discretedatavar.h"
#include <QVector>

class ObjectVars
{
    QVector<AnalogDataVar*> anVars;
    QVector<DiscreteDataVar*> dVars;
public:
    ObjectVars();
    ObjectVars(const ObjectVars&);
    ObjectVars& operator=(const ObjectVars&);
    ~ObjectVars();
    void addAnalogVar(const AnalogDataVar &v);
    void addDiscreteVar(const DiscreteDataVar &v);
};

#endif // OBJECTVARS_H
