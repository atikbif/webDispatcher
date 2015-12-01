#include "objectdata.h"
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QDir>

ObjectData::ObjectData(const QString &fName):
    name(""),comment(""),htmlPageName("")
{
    QDomDocument doc("objConfig");
    QFile file(fName);
    if (!file.open(QIODevice::ReadOnly)) return;
    if (!doc.setContent(&file)) {
        return;
    }
    if(doc.elementsByTagName("objConf").count()!=1) return;
    QDomNodeList names = doc.elementsByTagName("name");
    if(names.count()==1) {
        QDomElement e = names.item(0).toElement();
        if(!e.isNull()) name = e.text();
    }
    QDomNodeList comments = doc.elementsByTagName("comment");
    if(comments.count()==1) {
        QDomElement e = comments.item(0).toElement();
        if(!e.isNull()) comment = e.text();
    }
    QDomNodeList pages = doc.elementsByTagName("pageName");
    if(pages.count()==1) {
        QDomElement e = pages.item(0).toElement();
        if(!e.isNull()) htmlPageName = e.text();
    }
    QDomNodeList plcs = doc.elementsByTagName("controller");
    for(int i=0;i<plcs.count();i++) {
        QDomNode n = plcs.item(i);
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            QFileInfo fInfo(file);
            QString plcFileName = fInfo.absoluteDir().absolutePath() + "/" + e.attribute("file");
            QSharedPointer<ControllerData> ptr = QSharedPointer<ControllerData>(new ControllerData(plcFileName));
            controllers.append(ptr);
        }
    }
}

QSharedPointer<ControllerData> ObjectData::getController(int num)
{
    if((num>=0)&&(num<controllers.count())) {
        return  controllers[num];
    }
    return QSharedPointer<ControllerData>(new ControllerData(""));
}

ObjectData::~ObjectData()
{
    foreach (QSharedPointer<ControllerData> ptr, controllers) {
        ptr.clear();
    }
}



