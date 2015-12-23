#include "linkobject.h"
#include "Protocols/rk.h"
#include "Protocols/udpdecorator.h"
#include <QThread>
#include "dynamic/datastorage.h"

using namespace RkProtocol;

QVector<Request *> LinkObject::getRequestLine(QSharedPointer<ControllerData> plc)
{
    QVector<Request*> reqs;
    const int maxLength = 64;
    const int maxSpace = 32;
    int varCnt = plc->getVars()->getAnVarCount();
    // take all analog vars
    QVector<AnalogValue*> vars;
    for(int i=0;i<varCnt;i++) {
        AnalogValue* var = new AnalogValue(plc->getVars()->getAnalogVar(i));
        vars.append(var);
    }
    if(vars.count()) {
        // search min and max address
        const int varSize = 2;
        int minAddr = vars.at(0)->getAddr();
        int maxAddr = minAddr + varSize - 1;
        for(int i=0;i<vars.count();i++) {
            int addr = vars.at(i)->getAddr();
            if(addr+varSize-1>maxAddr) maxAddr = addr+varSize-1;
            if(addr<minAddr) minAddr = addr;
        }
        // make memory map
        QVector<bool> map;
        map.resize(maxAddr-minAddr+1);
        map.fill(false);
        foreach (AnalogValue *var, vars) {
            for(int i=0;i<varSize;i++) map.replace(var->getAddr()-minAddr+i,true);
        }
        bool newRequest = true;
        int space = 0;
        int reqLength = 0;
        // scan map and create requests
        Request* req = nullptr;
        for(int i=0;i<map.count();i++) {
            if(newRequest) {
                if(map.at(i)) {
                    newRequest = false;
                    space = 0;
                    reqLength = 1;
                    req = new Request;
                    req->setNetAddress(0);
                    req->setMemAddress(i+minAddr);
                    req->setDataNumber(reqLength);
                    if(reqLength>=maxLength) {
                        reqs.append(req);
                        newRequest = true;
                        req = nullptr;
                    }
                }
            }else {
                if(map.at(i)) {
                    reqLength++;
                    if(space) {reqLength+=space;space=0;}
                    if(req!=nullptr) req->setDataNumber(reqLength);
                    if(reqLength>=maxLength) {
                        reqs.append(req);
                        newRequest = true;
                        req = nullptr;
                    }
                }else {
                    space++;
                    if(space>maxSpace) {
                        reqs.append(req);
                        newRequest = true;
                        req = nullptr;
                    }
                    if(space+reqLength>=maxLength) {
                        reqs.append(req);
                        newRequest = true;
                        req = nullptr;
                    }
                }
            }
        }
        if(req!=nullptr) {
            if(obPtr->getWaitTime()) req->setWaitTime(obPtr->getWaitTime());
            reqs.append(req);
        }
    }
    return reqs;
}

QHash<int, unsigned int> LinkObject::getAnswers(QVector<Request *> reqs, QUdpSocket &udp)
{
    QHash<int, unsigned int> answerData;
    foreach (Request* req, reqs) {
        // request
        CommandInterface* cmd = new ReadDispRam();

        cmd = new UdpDecorator(cmd);
        if(cmd->execute(*req,udp)) {
            // save data
            for(int i=0;i<req->getRdData().count();i++) {
                answerData.insert(req->getMemAddress()+i,(quint8)(req->getRdData().at(i)));
            }
        }
        delete cmd;
        QThread::msleep(10);
    }
    return answerData;
}

void LinkObject::answerAnalyse(QSharedPointer<ControllerData> plc, QHash<int, unsigned int> answerData, ObjectVars &obVars)
{
    bool updTimeFlag = false;
    for(int i=0;i<plc->getVars()->getAnVarCount();i++) {
        AnalogValue anVar = plc->getVars()->getAnalogVar(i);
        AnalogDataVar anData(anVar.getName());
        anData.setColour(AnalogDataVar::GRAY);
        anData.setValue(0);
        int addr = anVar.getAddr();
        if((answerData.keys().contains(addr))&&(answerData.keys().contains(addr+1))) {
            updTimeFlag = true;
            int value = (quint8)answerData.value(addr+1) * 256;
            value += (quint8)answerData.value(addr);
            float res = (float)value * anVar.getCoeff();
            // test limits
            anData.setValue(res);
            if((res<anVar.getMinLimit())||(res>anVar.getMaxLimit())) anData.setColour(AnalogDataVar::RED);
            else anData.setColour(AnalogDataVar::GREEN);
        }
        obVars.addAnalogVar(anData);
    }

    // discrete data analyse
    discreteAnalyse(plc,obVars);
    // message data analyse
    messageAnalyse(plc,obVars);
    // object colour analyse
    colourAnalyse(obVars);
    // time update
    if(updTimeFlag) obVars.setTime(QDateTime::currentDateTime());
}

void LinkObject::discreteAnalyse(QSharedPointer<ControllerData> plc, ObjectVars &obVars)
{
    int dvarCnt = plc->getVars()->getDiscrVarCount();
    for(int i=0;i<dvarCnt;i++) {
        DiscretValue dVar = plc->getVars()->getDiscreteVar(i);
        DiscreteDataVar dData(dVar.getName());
        dData.setColour(DiscreteDataVar::GRAY);
        dData.setValue(false);
        for(int j=0;j<obVars.getAnVarCount();j++) {
            AnalogDataVar anData = obVars.getAnalogVar(j);
            QRegExp namePattern(".*"+dVar.getParentName()+"$");
            if(namePattern.indexIn(anData.getID())!=-1) {
                int value = anData.getValue();
                if(value & (1 << dVar.getBitNum())) {
                    dData.setValue(true);
                    dData.setColour(DiscreteDataVar::BLACK);
                }else {
                    dData.setValue(false);
                    dData.setColour(DiscreteDataVar::WHITE);
                }
                if(anData.getColour()==AnalogDataVar::GRAY)
                    dData.setColour(DiscreteDataVar::GRAY);
                break;
            }
        }
        obVars.addDiscreteVar(dData);
    }
}

void LinkObject::messageAnalyse(QSharedPointer<ControllerData> plc, ObjectVars &obVars)
{
    int mvarCnt = plc->getVars()->getMessageVarCount();
    for(int i=0;i<mvarCnt;i++) {
        MessageValue mVar = plc->getVars()->getMessageVar(i);

        for(int j=0;j<obVars.getAnVarCount();j++) {
            AnalogDataVar anData = obVars.getAnalogVar(j);
            QRegExp namePattern(".*"+mVar.getParentName()+"$");
            if(namePattern.indexIn(anData.getID())!=-1) {
                if(anData.getColour()!=AnalogDataVar::GRAY) {
                    int value = anData.getValue();
                    if(value & (1 << mVar.getBitNum())) {
                        MessageDataVar mData;
                        mData.setMessage(mVar.getMessage());
                        if(mVar.getType()==MessageValue::ALARM) mData.setColour(MessageDataVar::RED);
                        else if(mVar.getType()==MessageValue::WARNING) mData.setColour(MessageDataVar::YELLOW);
                        else if(mVar.getType()==MessageValue::INFO) mData.setColour(MessageDataVar::GREEN);
                        obVars.addMessageVar(mData);
                    }
                }
            }
        }
    }
}

void LinkObject::colourAnalyse(ObjectVars &obVars)
{
    enum {GRAY,RED,YELLOW,GREEN} col;
    col = GREEN;
    for(int i=0;i<obVars.getMessageVarCount();i++) {
        MessageDataVar var = obVars.getMessageVar(i);
        if(var.getColour()==MessageDataVar::RED) {col = RED;break;}
        if(var.getColour()==MessageDataVar::YELLOW) {
            if(col==GREEN) col = YELLOW;
        }
    }
    if(col!=RED) {
        for(int i=0;i<obVars.getAnVarCount();i++) {
            AnalogDataVar var = obVars.getAnalogVar(i);
            if(var.getColour()==AnalogDataVar::GRAY) {
                col = GRAY;
                break;
            }
        }
    }
    if(col==GRAY) obVars.setColour(ObjectVars::GRAY);
    else if(col==RED) obVars.setColour(ObjectVars::RED);
    else if(col==GREEN) obVars.setColour(ObjectVars::GREEN);
    else if(col==YELLOW) obVars.setColour(ObjectVars::YELLOW);
}

LinkObject::LinkObject(QSharedPointer<ObjectData> ptr, QObject *parent):QObject(parent),
    obPtr(ptr), stopCmd(false)
{

}

LinkObject::~LinkObject()
{

}

void LinkObject::stopScanning()
{
    QMutexLocker locker(&mutex);
    stopCmd = true;
}

void LinkObject::startScanning()
{
    QUdpSocket udp;
    int plcQuantity = obPtr->getContrCount();

    forever {
        mutex.lock();
        if(stopCmd) {mutex.unlock();break;}
        mutex.unlock();

        ObjectVars obVars;
        obVars.setID(obPtr->getHTMLPageName());

        for(int i=0;i<plcQuantity;i++) {
            QSharedPointer<ControllerData> plc = obPtr->getController(i);
            udp.connectToHost(plc->getIP(),plc->getPortNum());
            udp.waitForConnected();
            udp.open(QIODevice::ReadWrite);

            // create request line
            QVector<Request*> reqs = getRequestLine(plc);

            // read data
            QHash<int,unsigned int> answerData = getAnswers(reqs,udp);

            // free requests' memory
            foreach(Request* req, reqs) {
                delete req;
            }

            udp.disconnectFromHost();
            udp.close();

            // data analyse
            answerAnalyse(plc,answerData,obVars);

            mutex.lock();
            if(stopCmd) {mutex.unlock();break;}
            mutex.unlock();
            QThread::msleep(10);
        }
        DataStorage::Instance().updateObject(obVars);
        for(int i=0;i<obPtr->getPeriod();i++) {
            mutex.lock();
            if(stopCmd) {mutex.unlock();break;}
            mutex.unlock();
            QThread::sleep(1);
        }

    }
}
