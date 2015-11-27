#ifndef VARIABLES_H
#define VARIABLES_H

#include <QVector>
#include <QDateTime>

class ObjVariable
{
public:
    enum PRIORITY{HIGH_PR,NORM_PR,LOW_PR};
private:
    const QString name;
    const QString comment;
    PRIORITY displPriority;
public:

    ObjVariable(const QString &m_name, const QString &m_comment = "", PRIORITY prior = NORM_PR):
        name(m_name), comment(m_comment), displPriority(prior)
    {

    }
    QString getName() {return name;}
    QString getComment() {return comment;}
    PRIORITY getDisplayPriority() {return displPriority;}
    void setDisplayPriority(PRIORITY prior) {displPriority=prior;}
};



class AnalogValue: public ObjVariable
{
    int value;
    QDateTime varUpdTime;
    bool quality;
public:
    AnalogValue(const QString &m_name, const QString &m_comment="", PRIORITY prior = NORM_PR):
        ObjVariable(m_name,m_comment,prior),value(0),varUpdTime(QDateTime()),quality(false)
    {

    }
    int getValue() {return value;}
    void setValue(int m_value) {
        value = m_value;
        varUpdTime = QDateTime::currentDateTime();
        quality = true;
    }
    void setBadQuality(void) {quality=false;}
    bool getQuality(void) {return quality;}
};

class DiscretValue: public ObjVariable
{
    QString analogParentName;
    int bitNum;
    bool checkBitNum(int value) {
        const int theFirstBitNum = 0;
        const int theLastBitNum = 15;
        if((value<theFirstBitNum)||(value>theLastBitNum)) return false;
        return true;
    }

public:
    DiscretValue(const QString &m_name, const QString &m_comment="", PRIORITY prior = NORM_PR) :
        ObjVariable(m_name,m_comment,prior),analogParentName(""), bitNum(0)
    {

    }
    int getBitNum(void) {return bitNum;}
    bool setBitNum(int m_bitNum) {
        if(checkBitNum(m_bitNum)) {
            bitNum = m_bitNum;
            return true;
        }
        return false;
    }
    QString getParentName(void) {return analogParentName;}
    bool getValue(int anVarValue) {
            if(checkBitNum(bitNum)) {
                return anVarValue & (1<<bitNum);
            }
        return false;
    }
};

class MessageValue: public DiscretValue
{
public:
    enum MESSAGETYPE{INFO,WARNING,ALARM};
private:
    QString message;
    MESSAGETYPE type;
public:
    MessageValue(const QString &m_name, const QString &m_comment="", MESSAGETYPE m_type=INFO):
        DiscretValue(m_name,m_comment),message(""),type(m_type)
    {

    }
    void setMessage(const QString &m_message);
    QString getMessage(void) {return message;}
    MESSAGETYPE getType(void) {return type;}
};

class VarStorage
{
    QVector<AnalogValue*> anVars;
    QVector<DiscretValue*> discrVars;
    QVector<MessageValue*> messageVars;
public:
    VarStorage() {}
    int getAnVarCount() {return anVars.count();}
    int getDiscrVarCount() {return discrVars.count();}
    int getMessageVarCount() {return messageVars.count();}
    void addAnalogVar(AnalogValue *ptr) {anVars.append(ptr);}
    void addDiscreteVar(DiscretValue *ptr) {discrVars.append(ptr);}
    void addMessageVar(MessageValue *ptr) {messageVars.append(ptr);}
    AnalogValue getAnalogVar(const QString &vName,bool *res) {
        foreach (AnalogValue *var, anVars) {
           if(var->getName()==vName) {*res=true;return *var;}
        }
        *res = false;
        return  AnalogValue("");
    }
    AnalogValue getAnalogVar(int num) {
        if((num>=0)&&(num<getAnVarCount())) {
            return *anVars.at(num);
        }
        return AnalogValue("");
    }
    DiscretValue getDiscreteVar(int num) {
        if((num>=0)&&(num<getDiscrVarCount())) {
            return *discrVars.at(num);
        }
        return DiscretValue("");
    }
    MessageValue getMessageVar(int num) {
        if((num>=0)&&(num<getMessageVarCount())) {
            return *messageVars.at(num);
        }
        return MessageValue("");
    }

    void clearAll(void) {
        foreach (AnalogValue *var, anVars) {
            delete var;
        }
        anVars.clear();
        foreach(DiscretValue *var, discrVars) {
            delete var;
        }
        discrVars.clear();
        foreach(MessageValue *var, messageVars) {
            delete var;
        }
        messageVars.clear();
    }
};

#endif // VARIABLES_H

