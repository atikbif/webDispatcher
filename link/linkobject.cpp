#include "linkobject.h"
#include <QUdpSocket>
#include "Protocols/rk.h"
#include "Protocols/udpdecorator.h"
#include <QThread>

using namespace RkProtocol;

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
        for(int i=0;i<plcQuantity;i++) {
            QSharedPointer<ControllerData> plc = obPtr->getController(i);
            udp.connectToHost(plc->getIP(),plc->getPortNum());
            udp.waitForConnected();
            udp.open(QIODevice::ReadWrite);

            // request
            CommandInterface* cmd = new ReadTime();
            Request req;
            req.setNetAddress(0);
            req.setMemAddress(0);
            req.setDataNumber(7);
            cmd = new UdpDecorator(cmd);
            if(cmd->execute(req,udp)) {

            }
            delete cmd;

            udp.disconnectFromHost();
            udp.close();
            mutex.lock();
            if(stopCmd) {mutex.unlock();break;}
            mutex.unlock();
            QThread::msleep(100);
        }
    }
}
