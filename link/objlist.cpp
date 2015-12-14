#include "objlist.h"
#include <QDomDocument>
#include <QFile>
#include <QApplication>

void ObjList::createWebFiles()
{

}

void ObjList::clearAll()
{
    /*QStringList groups = getGroupNames();
    foreach (QString grName, groups) {
        QStringList objects = getObjNamesInGroup(grName);
        foreach(QString objName, objects) {
            QSharedPointer<ObjectData> ptr = getObject(grName,objName);
            ptr.clear();
        }
    }*/
    objGroups.clear();
}

ObjList::ObjList(const QString &fName)
{
    updateList(fName);
    QString test = "hello" + fName;
    Q_UNUSED(test)
}

QStringList ObjList::getObjNamesInGroup(const QString &grName)
{
    QStringList result;
    QVector< QSharedPointer<ObjectData> > objects = objGroups.value(grName);
    foreach(QSharedPointer<ObjectData> ptr, objects) {
        if(!ptr.isNull()) {
            result += ptr->getName();
        }
    }
    return result;
}

QSharedPointer<ObjectData> ObjList::getObject(const QString &grName, const QString &objName)
{
    QSharedPointer<ObjectData> res;
    QVector< QSharedPointer<ObjectData> > objects = objGroups.value(grName);
    foreach(QSharedPointer<ObjectData> ptr, objects) {
        if(!ptr.isNull()) {
            if(ptr->getName()==objName) {
                res=ptr;
                break;
            }
        }
    }
    return res;
}

void ObjList::updateList(const QString &fName)
{
    QString fullName = QApplication::applicationDirPath()+"/config/" + fName;
    QDomDocument doc("config");
    QFile file(fullName);
    if (!file.open(QIODevice::ReadOnly)) return;
    if (!doc.setContent(&file)) {
        return;
    }
    if(doc.elementsByTagName("webConf").count()!=1) return;

    clearAll();

    QDomNodeList groups = doc.elementsByTagName("group");
    for(int i=0;i<groups.count();i++) {
        QDomNode n = groups.item(i);
        QDomElement e = n.toElement();
        if(!e.isNull()) {
            QString grName = e.attribute("name");
            QDomNodeList childs = e.childNodes();
            for(int j=0;j<childs.count();j++) {
                QDomNode n = childs.item(j);
                QDomElement e = n.toElement();
                if(e.tagName()=="object") {
                    QString xmlFileName = QApplication::applicationDirPath()+"/config/" + e.attribute("xmlFile");
                    QSharedPointer<ObjectData> obj = QSharedPointer<ObjectData>(new ObjectData(xmlFileName));
                    QVector<QSharedPointer<ObjectData> > objects = objGroups.value(grName);
                    objects.append(obj);
                    objGroups.insert(grName,objects);
                }
            }
        }
    }
}
