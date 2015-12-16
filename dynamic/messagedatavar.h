#ifndef MESSAGEDATAVAR_H
#define MESSAGEDATAVAR_H

#include <QString>

class MessageDataVar
{
public:
    enum BACKGROUND {GREEN,YELLOW,RED};
private:
    QString value;
    BACKGROUND col;
public:
    MessageDataVar();
    QString getMessage(void) const {return value;}
    void setMessage(const QString &m) {value = m;}
    BACKGROUND getColour(void) const {return col;}
    void setColour(BACKGROUND c) {col = c;}
};

#endif // MESSAGEDATAVAR_H
