#include "linkobject.h"
#include "Protocols/rk.h"
#include "Protocols/udpdecorator.h"
#include <QThread>
#include "dynamic/datastorage.h"

using namespace RkProtocol;

QVector<Request *> LinkObject::getRequestLine(QSharedPointer<ControllerData> plc)
{
    Q_UNUSED(plc)
    QVector<Request*> reqs;
    for(int i=0;i<10;i++) {
        Request* req = new Request();
        req->setNetAddress(0);
        req->setMemAddress(0);
        req->setDataNumber(7);
        reqs.append(req);
    }
    return reqs;
}

QHash<int, unsigned int> LinkObject::getAnswers(QVector<Request *> reqs, QUdpSocket &udp)
{
    QHash<int, unsigned int> answerData;
    foreach (Request* req, reqs) {
        // request
        CommandInterface* cmd = new ReadTime();

        cmd = new UdpDecorator(cmd);
        if(cmd->execute(*req,udp)) {
            // save data
            for(int i=0;i<req->getAnswerData().count();i++) {
                answerData.insert(req->getMemAddress()+i,req->getAnswerData().at(i));
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
            int value = answerData.value(addr) * 256;
            value += answerData.value(addr+1);
            float res = (float)value * anVar.getCoeff();
            // test limits
            anData.setValue(res);
            anData.setColour(AnalogDataVar::GREEN);
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
    Q_UNUSED(plc)
    Q_UNUSED(obVars)
}

void LinkObject::messageAnalyse(QSharedPointer<ControllerData> plc, ObjectVars &obVars)
{
    Q_UNUSED(plc)
    Q_UNUSED(obVars)
}

void LinkObject::colourAnalyse(ObjectVars &obVars)
{
    Q_UNUSED(obVars)
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
            QThread::msleep(1);
        }
        DataStorage::Instance().updateObject(obVars);
    }
}
