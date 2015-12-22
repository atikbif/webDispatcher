#include "pagecreator.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QSharedPointer>
#include <QRegExp>

void PageCreator::createIndexFile(const QString &fName)
{
    const QString pObjName = "objName";
    const QString pLinkName = "link.html";
    const QString pGrName = "grName";
    QFile pFile(":/webPatterns/emptyPage.html"); // pattern
    if(pFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&pFile);
        in.setCodec("UTF-8");
        QString pattern = in.readAll();
        pFile.close();
        // заголовок страницы
        pattern = pattern.replace("page title","АРМ Диспетчера");
        QRegExp cont("<div class=\"container\">\n");
        int pos = cont.indexIn(pattern);
        if(pos!=-1) {
            pos+=cont.matchedLength();
            // формирование содержимого страницы
            QString body;
            const int tabCnt=2;
            addTabs(tabCnt,body);body += "<h2>Список объектов:</h2>\n";
            // чтение стандартных шаблонов
            // группа объектов
            QFile grObFile(":/webPatterns/groupOfObjects.html");
            QString grObPattern;
            if(grObFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&grObFile);
                in.setCodec("UTF-8");
                grObPattern = in.readAll();
                grObFile.close();
            }
            // сохранение и удаление образца объекта в списке
            QString elemText;
            QRegExp element("[ \\t]*<li.*/li>\\n");
            int elemPos = element.indexIn(grObPattern);
            if(elemPos!=-1) {
                elemText = element.cap();
            }
            grObPattern.remove(element);
            QFile oneObFile(":/webPatterns/singleObject.html");
            QString oneObPattern;
            if(oneObFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&oneObFile);
                in.setCodec("UTF-8");
                oneObPattern = in.readAll();
                oneObFile.close();
            }
            // перебор объектов
            QStringList grNames = objects.getGroupNames();
            foreach (QString grName, grNames) {
                QStringList objNames = objects.getObjNamesInGroup(grName);
                if(objNames.count()==1) {
                    QString objStr = oneObPattern;
                    QSharedPointer<ObjectData> ob = objects.getObject(grName,objNames.at(0));
                    QString webFileName = ob->getHTMLPageName();
                    QString obName = ob->getName();
                    if(!grName.isEmpty()) obName = grName+":"+obName;
                    objStr = objStr.replace(pLinkName,webFileName);
                    objStr = objStr.replace(pObjName,obName);
                    body += objStr;
                    createObjFile(ob);
                }else if(objNames.count()>1) {
                    QString grPattern = grObPattern;
                    int pos = elemPos;
                    foreach (QString obName, objNames) {
                        if(pos!=-1) {
                            QString objStr = elemText;
                            QSharedPointer<ObjectData> ob = objects.getObject(grName,obName);
                            QString webFileName = ob->getHTMLPageName();
                            objStr = objStr.replace(pLinkName,webFileName);
                            objStr = objStr.replace(pObjName,obName);
                            grPattern.insert(pos,objStr);
                            pos += objStr.length();
                            createObjFile(ob);
                        }
                    }
                    grPattern.replace(pGrName,grName);
                    body += grPattern;
                }
            }
            pattern.insert(pos,body);
            // запись результата в файл
            QFile file(fName);
            if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out.setCodec("UTF-8");
                out << pattern.toUtf8();
                file.close();
            }
        }
    }
}

void PageCreator::createObjFile(QSharedPointer<ObjectData> ob)
{
    QString fName = QApplication::applicationDirPath() + "/html/" + ob->getHTMLPageName();
    createObjStyle(ob);
    createObjScript(ob);
    createAnalogValuesScript(ob);
    createDiscreteValuesScript(ob);
    createMessageScript(ob);
    QFile pFile(":/webPatterns/objPage.html"); // pattern
    if(pFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&pFile);
        in.setCodec("UTF-8");
        QString pattern = in.readAll();
        pFile.close();
        QString tmp = fName;
        tmp.replace('\\','/');
        QRegExp onlyName(".*/([^/]*)$");
        if(onlyName.indexIn(tmp)!=-1) {
            tmp = onlyName.cap(1);
        }
        tmp.remove(QRegExp(".html$"));

        QString styleFileName = tmp + ".css";
        QString scriptFileName = tmp + ".js";
        QString anScriptFileName = tmp + "_an.js";
        QString dScriptFileName = tmp + "_discr.js";
        QString mScriptFileName = tmp + "_message.js";
        pattern.replace("objPage.css",styleFileName);
        pattern.replace("objPage.js",scriptFileName);
        pattern.replace("objPage_an.js",anScriptFileName);
        pattern.replace("objPage_discr.js",dScriptFileName);
        pattern.replace("objPage_message.js",mScriptFileName);

        pattern.replace("obj name",ob->getName());
        pattern.replace("obj comment",ob->getComment());

        QVector<AnalogValue*> anHighPrior;
        QVector<AnalogValue*> anStPrior;
        QVector<AnalogValue*> anLowPrior;

        int contrCount = ob->getContrCount();
        for(int i=0;i<contrCount;i++) {
            QSharedPointer<ControllerData> c = ob->getController(i);
            QSharedPointer<VarStorage> vars = c->getVars();
            int anVarCnt = vars->getAnVarCount();
            for(int j=0;j<anVarCnt;j++) {
                AnalogValue* av = new AnalogValue(vars->getAnalogVar(j));
                AnalogValue::PRIORITY pr = av->getDisplayPriority();
                if(pr==AnalogValue::HIGH_PR) anHighPrior.append(av);
                else if(pr==AnalogValue::NORM_PR) anStPrior.append(av);
                else if(pr==AnalogValue::LOW_PR) anLowPrior.append(av);
            }
        }

        QRegExp anVar("[ \\t]*<canvas id=\"ain1\".*</canvas>\\n");
        int pos = anVar.indexIn(pattern);
        if(pos!=-1) {
            QString anP = anVar.cap();
            pattern.remove(anP);
            foreach (AnalogValue* var, anHighPrior) {
                QString curVar = anP;
                curVar.replace("ain1","ain_"+var->getName());
                pattern.insert(pos,curVar);
                pos+=curVar.length();
                delete var;
            }
            foreach (AnalogValue* var, anStPrior) {
                QString curVar = anP;
                curVar.replace("ain1","ain_"+var->getName());
                pattern.insert(pos,curVar);
                pos+=curVar.length();
                delete var;
            }
            foreach (AnalogValue* var, anLowPrior) {
                QString curVar = anP;
                curVar.replace("ain1","ain_"+var->getName());
                pattern.insert(pos,curVar);
                pos+=curVar.length();
                delete var;
            }
        }

        QVector<DiscretValue*> dHighPrior;
        QVector<DiscretValue*> dStPrior;
        QVector<DiscretValue*> dLowPrior;

        for(int i=0;i<contrCount;i++) {
            QSharedPointer<ControllerData> c = ob->getController(i);
            QSharedPointer<VarStorage> vars = c->getVars();
            int dVarCnt = vars->getDiscrVarCount();
            for(int j=0;j<dVarCnt;j++) {
                DiscretValue* dv = new DiscretValue(vars->getDiscreteVar(j));
                DiscretValue::PRIORITY pr = dv->getDisplayPriority();
                if(pr==DiscretValue::HIGH_PR) dHighPrior.append(dv);
                else if(pr==DiscretValue::NORM_PR) dStPrior.append(dv);
                else if(pr==DiscretValue::LOW_PR) dLowPrior.append(dv);
            }
        }

        QRegExp dVar("[ \\t]*<div class=\"bitData\"[^\\n]*</div>\\n");
        pos = dVar.indexIn(pattern);
        if(pos!=-1) {
            QString dP = dVar.cap();
            pattern.remove(dP);
            foreach (DiscretValue* var, dHighPrior) {
                QString curVar = dP;
                curVar.replace("dinName",var->getComment());
                curVar.replace("di1","di_"+var->getName());
                pattern.insert(pos,curVar);
                pos+=curVar.length();
                delete var;
            }
            foreach (DiscretValue* var, dStPrior) {
                QString curVar = dP;
                curVar.replace("dinName",var->getComment());
                curVar.replace("di1","di_"+var->getName());
                pattern.insert(pos,curVar);
                pos+=curVar.length();
                delete var;
            }
            foreach (DiscretValue* var, dLowPrior) {
                QString curVar = dP;
                curVar.replace("dinName",var->getComment());
                curVar.replace("di1","di_"+var->getName());
                pattern.insert(pos,curVar);
                pos+=curVar.length();
                delete var;
            }
        }

        QFile file(fName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << pattern;
        }
    }
}

void PageCreator::createObjStyle(QSharedPointer<ObjectData> ob)
{
    QString fName = QApplication::applicationDirPath() + "/html/" + ob->getHTMLPageName();
    QString styleFileName = fName.remove(QRegExp(".html$"))+".css";
    QFile pFile(":/webPatterns/objPage.css"); // pattern
    if(pFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&pFile);
        in.setCodec("UTF-8");
        QString pattern = in.readAll();
        pFile.close();
        QFile file(styleFileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << pattern;
        }
    }
}

void PageCreator::createObjScript(QSharedPointer<ObjectData> ob)
{
    QString fName = QApplication::applicationDirPath() + "/html/" + ob->getHTMLPageName();
    QString scriptFileName = fName.remove(QRegExp(".html$"))+".js";

    QFile pFile(":/webPatterns/objPage.js"); // pattern
    if(pFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&pFile);
        in.setCodec("UTF-8");
        QString pattern = in.readAll();

        int contrCount = ob->getContrCount();

        QRegExp anExp("[ \\t]*anVar[^;]*myProgress[^;]*;\\n");
        int pos = anExp.indexIn(pattern);
        int visVarCnt = 0;
        if(pos!=-1) {
            QString ap = anExp.cap();
            pattern.remove(ap);

            for(int i=0;i<contrCount;i++) {
                QSharedPointer<ControllerData> c = ob->getController(i);
                QSharedPointer<VarStorage> vars = c->getVars();
                int anVarCnt = vars->getAnVarCount();
                for(int j=0;j<anVarCnt;j++) {
                    AnalogValue av = vars->getAnalogVar(j);
                    if(av.getDisplayPriority()!=AnalogValue::NONE_PR) {
                        QString curPattern = ap;
                        curPattern.replace("ain1","ain_"+av.getName());
                        curPattern.replace("ainName",av.getComment());
                        curPattern.replace("anVar[0]","anVar["+QString::number(visVarCnt)+"]");
                        pattern.insert(pos,curPattern.toUtf8());
                        QString settings="";
                        addTabs(2,settings);
                        settings += "anVar[" + QString::number(visVarCnt) + "].min=" + QString::number(av.getMin()) + ";\n";
                        addTabs(2,settings);
                        settings += "anVar[" + QString::number(visVarCnt) + "].max=" + QString::number(av.getMax()) + ";\n";
                        addTabs(2,settings);
                        settings += "anVar[" + QString::number(visVarCnt) + "].value=" + QString::number(av.getMax()) + ";\n";
                        pos+=curPattern.length();
                        pattern.insert(pos,settings);pos+=settings.length();
                        visVarCnt++;
                    }
                }
            }

        }
        QRegExp avCnt("anVarCount=(\\d+)");
        pattern.replace(avCnt,"anVarCount="+QString::number(visVarCnt));

        pFile.close();
        QFile file(scriptFileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << pattern;
        }
    }
}

void PageCreator::addTabs(int tabCount, QString &str)
{
    for(int i=0;i<tabCount;i++) str+="\t";
}

void PageCreator::createAnalogValuesScript(QSharedPointer<ObjectData> ob)
{
    QString fName = QApplication::applicationDirPath() + "/html/" + ob->getHTMLPageName();
    QString scriptFileName = fName.remove(QRegExp(".html$"))+"_an.js";

    QFile pFile(":/webPatterns/anValue.js"); // pattern
    if(pFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&pFile);
        in.setCodec("UTF-8");
        QString pattern = in.readAll();
        pattern.replace("obPageName",ob->getHTMLPageName().remove(".html"));
        pFile.close();
        QFile file(scriptFileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << pattern;
        }
    }
}

void PageCreator::createDiscreteValuesScript(QSharedPointer<ObjectData> ob)
{
    QString fName = QApplication::applicationDirPath() + "/html/" + ob->getHTMLPageName();
    QString scriptFileName = fName.remove(QRegExp(".html$"))+"_discr.js";

    QFile pFile(":/webPatterns/dValue.js"); // pattern
    if(pFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&pFile);
        in.setCodec("UTF-8");
        QString pattern = in.readAll();
        pattern.replace("obPageName",ob->getHTMLPageName().remove(".html"));
        pFile.close();
        QFile file(scriptFileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << pattern;
        }
    }
}

void PageCreator::createMessageScript(QSharedPointer<ObjectData> ob)
{
    QString fName = QApplication::applicationDirPath() + "/html/" + ob->getHTMLPageName();
    QString scriptFileName = fName.remove(QRegExp(".html$"))+"_message.js";

    QFile pFile(":/webPatterns/objPage_message.js"); // pattern
    if(pFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&pFile);
        in.setCodec("UTF-8");
        QString pattern = in.readAll();
        pattern.replace("obPageName",ob->getHTMLPageName().remove(".html"));
        pFile.close();
        QFile file(scriptFileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out.setCodec("UTF-8");
            out << pattern;
        }
    }
}

PageCreator::PageCreator(ObjList &m_objects): objects(m_objects)
{

}

void PageCreator::createPages()
{
    QDir dir(QApplication::applicationDirPath() + "/html");
    if(!dir.exists()) {
        dir.mkdir(".");
    }
    createIndexFile(dir.absolutePath()+"/index.html");
}

