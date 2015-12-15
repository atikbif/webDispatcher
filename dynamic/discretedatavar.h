#ifndef DISCRETEDATAVAR_H
#define DISCRETEDATAVAR_H

#include <QString>

class DiscreteDataVar
{
public:
    enum BACKGROUND {WHITE, BLACK, GRAY};
private:
    QString id;
    BACKGROUND col;
    bool value;
public:
    DiscreteDataVar();
    DiscreteDataVar(const QString &id);
    QString getID() const {return id;}
    BACKGROUND getColour() const {return col;}
    bool getValue() const {return value;}
    void setID(const QString &newID) {id = newID;}
    void setColour(BACKGROUND c) {col = c;}
    void setValue(bool v) {value=v;}
};

#endif // DISCRETEDATAVAR_H
