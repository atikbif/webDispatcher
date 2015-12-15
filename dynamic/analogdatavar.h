#ifndef ANALOGDATAVAR_H
#define ANALOGDATAVAR_H

#include <QString>

class AnalogDataVar
{
public:
    enum BACKGROUND {GREEN,YELLOW,RED,GRAY};
private:
    float value;
    QString id;
    BACKGROUND col;
public:
    AnalogDataVar();
    AnalogDataVar(const QString &id);
    float getValue() const {return value;}
    QString getID() const {return id;}
    BACKGROUND getColour() const {return col;}
    void setValue(float v) {value = v;}
    void setID(const QString &newId) {id = newId;}
    void setColour(BACKGROUND c) {col = c;}
};

#endif // ANALOGDATAVAR_H
