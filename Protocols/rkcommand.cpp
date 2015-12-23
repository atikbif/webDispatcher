#include "rkcommand.h"
#include <QThread>
#include "checksum.h"
#include <QByteArray>

RkCommand::RkCommand()
{

}

RkCommand::~RkCommand()
{

}


bool RkCommand::waitAnAnswer(Request &req, QIODevice &io)
{
    if(io.isOpen()){
        QByteArray answer;
        int wait = 100;
        if(req.getWaitTime()!=0) wait = req.getWaitTime();
        if(io.waitForReadyRead(wait)){
            int cnt=0;
            answer+=io.readAll();
            while(io.waitForReadyRead(10)) {
                answer+=io.readAll();
                cnt++;if(cnt>=maxCnt) break;
            }
        }
        req.updateRdData(answer);
        if(answer.count()) {return true;}
    }
    return false;
}

bool RkCommand::checkAnAnswer(Request &req)
{
    QByteArray answer = req.getRdData();
    if(CheckSum::getCRC16(answer)==0) return true;
    return false;
}

bool RkCommand::getAnAnswer(Request &req)
{
    Q_UNUSED(req)
    return true;
}

