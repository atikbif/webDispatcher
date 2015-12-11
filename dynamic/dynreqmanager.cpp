#include "dynreqmanager.h"
#include <QDateTime>

const QString DynReqManager::discrFileName = "din.txt";
const QString DynReqManager::analogFileName = "ain.txt";
const QString DynReqManager::messageFileName = "message.txt";
const QString DynReqManager::obStatFileName = "status.txt";

QString DynReqManager::getDiscreteData(const QString &reqObName)
{
    Q_UNUSED(reqObName)
    QString res;
    int r = qrand() % 2;
    res+="di_192.168.100.35:12144:DI1="+QString::number(r)+";";
    r = qrand() % 2;
    res+="di_192.168.100.36:12144:BIT1="+QString::number(r)+";";
    r = qrand() % 2;
    res+="di_192.168.100.37:12144:BIT1="+QString::number(r)+";";
    return res;
}

QString DynReqManager::getAnalogData(const QString &reqObName)
{
    Q_UNUSED(reqObName)
    QString res;
    float d;
    int r = qrand() % 1000;d = (double)r*1.1;
    int col = (qrand() % 3) + 1;
    res+="ain_192.168.100.35:12144:ADC1="+QString::number(d)+"_"+ QString::number(col) +";";
    r = qrand() % 1000;d = (double)r*1.1;
    col = (qrand() % 3) + 1;
    res+="ain_192.168.100.36:12144:ADC1="+QString::number(d)+"_"+ QString::number(col) +";";
    r = qrand() % 300;d = (double)r*1.1;
    col = (qrand() % 3) + 1;
    res+="ain_192.168.100.37:12144:ADC1="+QString::number(d)+"_"+ QString::number(col) +";";
    return res;
}

QString DynReqManager::getMessageData(const QString &reqObName)
{
    Q_UNUSED(reqObName)
    QString res;
    int r = (qrand() % 10) + 1;
    for(int i=0;i<(int)r;i++) {
        int t = (qrand() % 3) + 1;
        QString m = "сообщение "  + QTime::currentTime().toString();
        res+= m + "=" + QString::number(t) + ";";
    }
    return res;
}

QString DynReqManager::geObtStatusData(const QString &reqObName)
{
    Q_UNUSED(reqObName)
    QString res;
    res = QDateTime::currentDateTime().toString();
    res += "=" + QString::number((qrand() % 4));
    return res;
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

