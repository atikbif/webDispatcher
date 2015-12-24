#include "tcpdecorator.h"
#include <QThread>
#include <QTcpSocket>

TcpDecorator::TcpDecorator(CommandInterface *cmd):CommandDecorator(cmd)
{

}

bool TcpDecorator::waitAnAnswer(Request &req, QIODevice &io)
{
    return CommandDecorator::waitAnAnswer(req,io);
}

bool TcpDecorator::checkAnAnswer(Request &req)
{
    return CommandDecorator::checkAnAnswer(req);
}

bool TcpDecorator::getAnAnswer(Request &req)
{
    return CommandDecorator::getAnAnswer(req);
}

TcpDecorator::~TcpDecorator()
{

}

bool TcpDecorator::execute(Request &req, QIODevice &io)
{
    form(req);
    QTcpSocket* s = dynamic_cast<QTcpSocket*>(&io);
    if(s->state()==QTcpSocket::ConnectedState) {
        s->write(req.getBody());
        s->waitForBytesWritten();


        QByteArray rxData;

        int wait = 15;
        if(req.getWaitTime()!=0) wait = req.getWaitTime();
        int length=0;
        for(int i=0;i<wait;i++) {
            if(s->waitForReadyRead(1)) {
                rxData+=s->readAll();
                if(rxData.count()) {
                    if(length==rxData.count()) break;
                    length = rxData.count();
                }
            }
        }
        if(rxData.count()) {
            req.updateRdData(rxData);
            req.setAnswerData(rxData);
            if(checkAnAnswer(req)) {
                getAnAnswer(req);
                return true;
            }
        }
    }
    req.setAnswerData(QByteArray());
    return false;
}
