#ifndef OBJECTVARS_H
#define OBJECTVARS_H

#include "analogdatavar.h"
#include "discretedatavar.h"
#include "messagedatavar.h"
#include <QVector>
#include <QDateTime>

class ObjectVars
{
public:
    enum BACKGROUND{GREEN,YELLOW,RED,GRAY};
private:
    QVector<AnalogDataVar*> anVars;
    QVector<DiscreteDataVar*> dVars;
    QVector<MessageDataVar*> mVars;
    BACKGROUND col;
    QDateTime varsTime;
    QString id;
    void clear(void);
public:
    ObjectVars();
    ObjectVars(const ObjectVars &ob);
    ObjectVars& operator=(const ObjectVars &ob);
    ~ObjectVars();
    void addAnalogVar(const AnalogDataVar &v);
    void addDiscreteVar(const DiscreteDataVar &v);
    void addMessageVar(const MessageDataVar &v);
    void setColour(BACKGROUND c) {col = c;}
    void setTime(const QDateTime &dt) {varsTime = dt;}
    QDateTime getTime(void) const {return varsTime;}
    BACKGROUND getColour(void) const {return col;}
    int getAnVarCount(void) const {return anVars.count();}
    int getDiscrVarCount(void) const {return dVars.count();}
    int getMessageVarCount(void) const {return mVars.count();}
    AnalogDataVar getAnalogVar(int num) const;
    DiscreteDataVar getDiscreteVar(int num) const;
    MessageDataVar getMessageVar(int num) const;
    QString getID(void) const {return id;}
    void setID(const QString &idValue) {id = idValue;}
};

#endif // OBJECTVARS_H
