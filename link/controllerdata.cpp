#include "controllerdata.h"
#include <QFile>

void ControllerData::readAnalogVars(QDomDocument &doc)
{
    QDomNodeList anVars = doc.elementsByTagName("analog");
    for(int i=0;i<anVars.count();i++) {
        QDomNode n = anVars.item(i);
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            QString name;
            QString comment;
            QString priority;
            QString addr;
            QDomNodeList chNodes = e.childNodes();
            for(int j=0;j<chNodes.count();j++) {
                QDomNode n = chNodes.item(j);
                QDomElement e = n.toElement();
                if(!e.isNull()) {
                    if(e.tagName()=="name") name = e.text();
                    else if(e.tagName()=="comment") comment = e.text();
                    else if(e.tagName()=="priority") priority = e.text();
                    else if(e.tagName()=="address") addr = e.text();
                }
            }
            AnalogValue* v = new AnalogValue(ipAddr+":"+QString::number(portNum)+":"+name,comment);
            AnalogValue::PRIORITY pr = AnalogValue::NORM_PR;
            if(priority=="high") pr = AnalogValue::HIGH_PR;
            else if(priority=="normal") pr = AnalogValue::NORM_PR;
            else if(priority=="low") pr = AnalogValue::LOW_PR;
            else if(priority=="none") pr = AnalogValue::NONE_PR;
            v->setDisplayPriority(pr);
            v->setAddr(addr.toInt());
            vars->addAnalogVar(v);
        }
    }
}

void ControllerData::readDiscreteVars(QDomDocument &doc)
{
    QDomNodeList discrVars = doc.elementsByTagName("discrete");
    for(int i=0;i<discrVars.count();i++) {
        QDomNode n = discrVars.item(i);
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            QString name;
            QString comment;
            QString priority;
            QString bitNum;
            QString parent;
            QDomNodeList chNodes = e.childNodes();
            for(int j=0;j<chNodes.count();j++) {
                QDomNode n = chNodes.item(j);
                QDomElement e = n.toElement();
                if(!e.isNull()) {
                    if(e.tagName()=="name") name = e.text();
                    else if(e.tagName()=="comment") comment = e.text();
                    else if(e.tagName()=="priority") priority = e.text();
                    else if(e.tagName()=="bit") bitNum = e.text();
                    else if(e.tagName()=="parent") parent = e.text();
                }
            }
            DiscretValue* v = new DiscretValue(ipAddr+":"+QString::number(portNum)+":"+name,comment);
            AnalogValue::PRIORITY pr = AnalogValue::NORM_PR;
            if(priority=="high") pr = AnalogValue::HIGH_PR;
            else if(priority=="normal") pr = AnalogValue::NORM_PR;
            else if(priority=="low") pr = AnalogValue::LOW_PR;
            else if(priority=="none") pr = AnalogValue::NONE_PR;
            v->setDisplayPriority(pr);
            v->setParentName(parent);
            v->setBitNum(bitNum.toInt());
            vars->addDiscreteVar(v);
        }
    }
}

void ControllerData::readMessageVars(QDomDocument &doc)
{
    QDomNodeList messageVars = doc.elementsByTagName("message");
    for(int i=0;i<messageVars.count();i++) {
        QDomNode n = messageVars.item(i);
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            QString name;
            QString text;
            QString type;
            QString bitNum;
            QString parent;
            QDomNodeList chNodes = e.childNodes();
            for(int j=0;j<chNodes.count();j++) {
                QDomNode n = chNodes.item(j);
                QDomElement e = n.toElement();
                if(!e.isNull()) {
                    if(e.tagName()=="name") name = e.text();
                    else if(e.tagName()=="text") text = e.text();
                    else if(e.tagName()=="type") type = e.text();
                    else if(e.tagName()=="bit") bitNum = e.text();
                    else if(e.tagName()=="parent") parent = e.text();
                }
            }
            MessageValue* v = new MessageValue(ipAddr+":"+QString::number(portNum)+":"+name);
            MessageValue::MESSAGETYPE mt = MessageValue::INFO;
            if(type=="info") mt = MessageValue::INFO;
            else if(type=="warning") mt = MessageValue::WARNING;
            else if(type=="alarm") mt = MessageValue::ALARM;
            v->setParentName(parent);
            v->setBitNum(bitNum.toInt());
            v->setMessage(text);
            v->setType(mt);
            vars->addMessageVar(v);
        }
    }
}

ControllerData::ControllerData(const QString &fName, QObject *parent):
    QObject(parent),ipAddr(""),portNum(0),vars(new VarStorage()), comment("")
{
    QDomDocument doc("contrConfig");
    QFile file(fName);
    if (!file.open(QIODevice::ReadOnly)) return;
    if (!doc.setContent(&file)) {
        return;
    }
    if(doc.elementsByTagName("contrConf").count()!=1) return;
    QDomNodeList defs = doc.elementsByTagName("definition");
    if(defs.count()==1) {
        QDomElement e = defs.item(0).toElement();
        if(!e.isNull()) comment = e.text();
    }
    QDomNodeList ips = doc.elementsByTagName("ip");
    if(ips.count()==1) {
        QDomElement e = ips.item(0).toElement();
        if(!e.isNull()) ipAddr = e.text();
    }
    QDomNodeList ports = doc.elementsByTagName("port");
    if(ports.count()==1) {
        QDomElement e = ports.item(0).toElement();
        if(!e.isNull()) portNum = e.text().toInt();
    }
    readAnalogVars(doc);
    readDiscreteVars(doc);
    readMessageVars(doc);
}

void ControllerData::updData(int memAddr, const QByteArray &data)
{
    Q_UNUSED(memAddr)
    Q_UNUSED(data)
}



