#include "dynreqmanager.h"
#include <QDateTime>
#include "datastorage.h"


const QString DynReqManager::discrFileName = "din.txt";
const QString DynReqManager::analogFileName = "ain.txt";
const QString DynReqManager::messageFileName = "message.txt";
const QString DynReqManager::obStatFileName = "status.txt";

QString DynReqManager::getDiscreteData(const QString &reqObName)
{
    int obCount = DataStorage::Instance().getObCount();
    for(int i=0;i<obCount;i++) {
        ObjectVars vars = DataStorage::Instance().getObject(i);
        QString obName = vars.getID();
        obName.remove(".html");
        if(obName==reqObName) {
            QString res;
            for(int j=0;j<vars.getDiscrVarCount();j++) {
                DiscreteDataVar var = vars.getDiscreteVar(j);
                int col = 0;
                if(var.getColour()==DiscreteDataVar::GRAY) col=0;
                else if(var.getColour()==DiscreteDataVar::WHITE) col=1;
                else if(var.getColour()==DiscreteDataVar::BLACK) col=2;
                res+="di_"+var.getID()+"=";
                res+=QString::number(col) + ";";
            }
            return res;
        }
    }
    return QString();
}

QString DynReqManager::getAnalogData(const QString &reqObName)
{
    int obCount = DataStorage::Instance().getObCount();
    for(int i=0;i<obCount;i++) {
        ObjectVars vars = DataStorage::Instance().getObject(i);
        QString obName = vars.getID();
        obName.remove(".html");
        if(obName==reqObName) {
            QString res;
            for(int j=0;j<vars.getAnVarCount();j++) {
                AnalogDataVar var = vars.getAnalogVar(j);
                res+="ain_"+ var.getID()+"=";
                float value = var.getValue();
                res+=QString::number(value);
                int col = 0;
                if(var.getColour()==AnalogDataVar::GRAY) col=0;
                else if(var.getColour()==AnalogDataVar::GREEN) col=1;
                else if(var.getColour()==AnalogDataVar::YELLOW) col=2;
                else if(var.getColour()==AnalogDataVar::RED) col=3;
                res+="_"+QString::number(col)+";";
            }
            return res;
        }
    }
    return QString();
}

QString DynReqManager::getMessageData(const QString &reqObName)
{
    int obCount = DataStorage::Instance().getObCount();
    for(int i=0;i<obCount;i++) {
        ObjectVars vars = DataStorage::Instance().getObject(i);
        QString obName = vars.getID();
        obName.remove(".html");
        if(obName==reqObName) {
            QString res;
            for(int j=0;j<vars.getMessageVarCount();j++) {
                MessageDataVar var = vars.getMessageVar(j);
                res+=var.getMessage()+"=";
                int col=1;
                if(var.getColour()==MessageDataVar::GREEN) col=1;
                else if(var.getColour()==MessageDataVar::YELLOW) col=2;
                else if(var.getColour()==MessageDataVar::RED) col=3;
                res+=QString::number(col)+";";
            }
            return res;
        }
    }
    return QString();
}

QString DynReqManager::geObtStatusData(const QString &reqObName)
{
    int obCount = DataStorage::Instance().getObCount();
    for(int i=0;i<obCount;i++) {
        ObjectVars vars = DataStorage::Instance().getObject(i);
        QString obName = vars.getID();
        obName.remove(".html");
        if(obName==reqObName) {
            QString res;
            res = vars.getTime().toString();
            int col = 0;
            if(vars.getColour()==ObjectVars::GRAY) col=0;
            else if(vars.getColour()==ObjectVars::GREEN) col=1;
            else if(vars.getColour()==ObjectVars::YELLOW) col=2;
            else if(vars.getColour()==ObjectVars::RED) col=3;
            res += "=" + QString::number(col);
            return res;
        }
    }
    return QString();
}

DynReqManager::DynReqManager()
{
    QTime time = QTime::currentTime();
    qsrand((unsigned int)time.msec());
}

QString DynReqManager::getData(const QString& reqFileName, const QString &reqObName)
{
    QString res;
    if(reqFileName==discrFileName) {
        res = getDiscreteData(reqObName);
    }else if(reqFileName==analogFileName) {
        res = getAnalogData(reqObName);
    }else if(reqFileName==messageFileName) {
        res = getMessageData(reqObName);
    }else if(reqFileName==obStatFileName) {
        res = geObtStatusData(reqObName);
    }
    return res;
}

