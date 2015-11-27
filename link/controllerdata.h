#ifndef CONTROLLERDATA_H
#define CONTROLLERDATA_H

#include <QString>
#include "variables.h"

class ControllerData
{
    QString ipAddr;
    int portNum;
    VarStorage vars;
    QString comment;
public:
    ControllerData(const QString &m_comment="");
    VarStorage& getVars(void) {return vars;}
    QString getComment(void) {return comment;}
    void updComment(const QString &newComment) {comment=newComment;}
};



#endif // CONTROLLERDATA_H
