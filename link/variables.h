#ifndef VARIABLES_H
#define VARIABLES_H

/* базовые типы данных для объектов */

#include <QVector>
#include <QDateTime>

class ObjVariable
{
public:
    enum PRIORITY{HIGH_PR,NORM_PR,LOW_PR,NONE_PR};
private:
    QString name;
    QString comment;
    PRIORITY displPriority;
public:
    ObjVariable(const QString &m_name, const QString &m_comment = "", PRIORITY prior = NORM_PR):
        name(m_name), comment(m_comment), displPriority(prior)
    {
    }
    QString getName() const {return name;}
    QString getComment() const {return comment;}
    PRIORITY getDisplayPriority() const {return displPriority;}
    void setDisplayPriority(PRIORITY prior) {displPriority=prior;}
};



class AnalogValue: public ObjVariable
{
    int addr;
    float min,max,coeff;
public:
    AnalogValue(const QString &m_name, const QString &m_comment="", PRIORITY prior = NORM_PR):
        ObjVariable(m_name,m_comment,prior),addr(0),
        min(0),max(65535),coeff(1)
    {

    }
    float getMin() const {return min;}
    float getMax() const {return max;}
    float getCoeff() const {return coeff;}
    void setMin(float value) {min = value;}
    void setMax(float value) {max = value;}
    void setCoeff(float value) {coeff = value;}
    int getAddr() const {return addr;}
    void setAddr(int newAddr) {addr = newAddr;}
};

class DiscretValue: public ObjVariable
{
    QString analogParentName;
    int bitNum;
    bool checkBitNum(int value) const{
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
    void setParentName(const QString &parName) {analogParentName=parName;}
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
    void setMessage(const QString &m_message) {message = m_message;}
    QString getMessage(void) const {return message;}
    MESSAGETYPE getType(void) const {return type;}
    void setType(MESSAGETYPE mt) {type = mt;}
};

class VarStorage
{
    QVector<AnalogValue*> anVars;
    QVector<DiscretValue*> discrVars;
    QVector<MessageValue*> messageVars;
    VarStorage(const VarStorage&);
    VarStorage& operator=(const VarStorage&);
public:
    VarStorage() {}
    int getAnVarCount() const {return anVars.count();}
    int getDiscrVarCount() const {return discrVars.count();}
    int getMessageVarCount() const {return messageVars.count();}
    void addAnalogVar(AnalogValue *ptr) {anVars.append(ptr);}
    void addDiscreteVar(DiscretValue *ptr) {discrVars.append(ptr);}
    void addMessageVar(MessageValue *ptr) {messageVars.append(ptr);}
    AnalogValue getAnalogVar(const QString &vName,bool *res) const{
        foreach (AnalogValue *var, anVars) {
           if(var->getName()==vName) {*res=true;return *var;}
        }
        *res = false;
        return  AnalogValue("");
    }
    AnalogValue getAnalogVar(int num) const{
        if((num>=0)&&(num<getAnVarCount())) {
            return *anVars.at(num);
        }
        return AnalogValue("");
    }
    bool updAnalogVar(const AnalogValue &var) {
        foreach (AnalogValue * v, anVars) {
            if(v->getName()==var.getName()) {
                *v = var;
                return true;
            }
        }
        return false;
    }

    DiscretValue getDiscreteVar(int num) const{
        if((num>=0)&&(num<getDiscrVarCount())) {
            return *discrVars.at(num);
        }
        return DiscretValue("");
    }

    bool updDiscreteVar(const DiscretValue &var) {
        foreach (DiscretValue *v, discrVars) {
            if(v->getName()==var.getName()) {
                *v = var;
                return true;
            }
        }
        return false;
    }

    MessageValue getMessageVar(int num) const{
        if((num>=0)&&(num<getMessageVarCount())) {
            return *messageVars.at(num);
        }
        return MessageValue("");
    }

    bool updMessageVar(const MessageValue &var) {
        foreach(MessageValue *v, messageVars) {
            if(v->getName()==var.getName()) {
                *v = var;
                return true;
            }
        }

        return false;
    }

    void clearAll(void) {
        foreach (AnalogValue *var, anVars) {
            delete var;
            var = nullptr;
        }
        anVars.clear();
        foreach(DiscretValue *var, discrVars) {
            delete var;
            var = nullptr;
        }
        discrVars.clear();
        foreach(MessageValue *var, messageVars) {
            delete var;
            var = nullptr;
        }
        messageVars.clear();
    }

    ~VarStorage() {
        clearAll();
    }
};

#endif // VARIABLES_H

