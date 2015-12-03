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
    QFile file(fName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << "<!DOCTYPE html>\n";
        out << "<html>\n";
        out << "<head>\n";
        out << "<meta charset=\"utf-8\">\n";
        out << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
        out << "    <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">\n";
        out << "    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\n";
        out << "    <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js\"></script>\n";
        out << "</head>\n";
        out << "<body>\n";

        out << "<div class=\"container\">\n";
        out << QString("    <h2>АРМ Диспетчера</h2>\n").toUtf8();
        out << QString("    <p>Список объектов:</p>\n").toUtf8();

        QStringList grNames = objects.getGroupNames();
        foreach (QString grName, grNames) {
            QStringList objNames = objects.getObjNamesInGroup(grName);
            if(objNames.count()==1) {
                QSharedPointer<ObjectData> ob = objects.getObject(grName,objNames.at(0));
                QString webFileName = ob->getHTMLPageName();
                QString obName = ob->getName();
                if(!grName.isEmpty()) obName = grName+":"+obName;
                QString obStr = "<a href=\"" + webFileName +
                        "\" class=\"btn btn-info\" role=\"button\">" +
                        obName+"</a>\n";
                out << "    <p>\n";
                out << "        "+obStr.toUtf8();
                out << "    </p>\n";
                createObjFile(ob);
            }else if(objNames.count()>1) {
                out << "    <p>\n";
                out << "        <div class=\"dropdown\">\n";
                out << "            <button class=\"btn btn-primary dropdown-toggle\" type=\"button\" data-toggle=\"dropdown\">";
                out << QString(grName).toUtf8() << "\n";
                out << "                <span class=\"caret\"></span>\n";
                out << "            </button>\n";
                out << "            <ul class=\"dropdown-menu\">\n";
                foreach (QString obName, objNames) {
                    QSharedPointer<ObjectData> ob = objects.getObject(grName,obName);
                    QString webFileName = ob->getHTMLPageName();
                    out << "                <li class=\"list-group-item\"><a href=\"" +
                           webFileName + "\">" + obName.toUtf8() + "</a></li>\n";
                    createObjFile(ob);
                }
                out << "            </ul>\n";
                out << "        </div>\n";
                out << "    </p>\n";
            }
        }

        out << "</div>\n";

        out << "</body>\n";
        out << "</html>\n";
    }
}

void PageCreator::createObjFile(QSharedPointer<ObjectData> ob)
{
    QString fName = QApplication::applicationDirPath() + "/html/" + ob->getHTMLPageName();
    QString styleFileName = fName.remove(QRegExp(".html$"))+".css";
    createObjStyle(styleFileName);
    fName += ".html";

    QFile file(fName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << "<!DOCTYPE html>\n";
        out << "<html>\n";
        out << "<head>\n";
        out << "<title>" << ob->getName().toUtf8() << "</title>\n";
        out << "<meta charset=\"utf-8\">\n";
        out << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
        out << "    <link rel=\"stylesheet\" href=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">\n";
        out << "    <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>\n";
        out << "    <script src=\"http://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js\"></script>\n";
        out << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"" << styleFileName << "\">\n";
        out << "</head>\n";
        out << "<body>\n";

        out << "<div class=\"container\">\n";
        out << "    <div class=\"alert alert-success\">\n";
        out << "        <strong>" << QString(ob->getName()).toUtf8() << "</strong>\n";
        out << "        (" + QString(ob->getComment()).toUtf8() + ") \n";
        out << "        " + QString("время опроса: ").toUtf8() + "\n";
        out << "    </div>\n";
        out << "    <h2></h2>\n";
        out << "    <ul class=\"nav nav-tabs\">\n";
        out << "        <li class=\"active\"><a data-toggle=\"tab\" href=\"#analog\">";
        out << QString("аналог.</a></li>\n").toUtf8();
        out << "        <li><a data-toggle=\"tab\" href=\"#discr\">";
        out << QString("дискр.</a></li>\n").toUtf8();
        out << "        <li><a data-toggle=\"tab\" href=\"#message\">";
        out << QString("сообщения</a></li>\n");
        out << "    </ul>\n";

        out << "    <div class=\"tab-content\">\n";
        out << "        <div id=\"analog\" class=\"tab-pane fade in active\">\n";
        int varCnt=0;
        int contrCount = ob->getContrCount();
        for(int i=0;i<contrCount;i++) {
            QSharedPointer<ControllerData> c = ob->getController(i);
            QSharedPointer<VarStorage> vars = c->getVars();
            int anVarsCount = vars->getAnVarCount();
            for(int j=0;j<anVarsCount;j++) {
                AnalogValue v = vars->getAnalogVar(j);
                if(v.getDisplayPriority()!=AnalogValue::NONE_PR) {
                    out << "            <div class=\"ainData\">\n";
                    out << "                <div>\n";
                    out << "                " << v.getComment().toUtf8() << "</div>\n";
                    out << "                <div class=\"progress\">\n";
                    out << "                    <div id=\"anVar";
                    out << QString::number(varCnt) << "\" class=\"progress-bar progress-bar-success\"  role=\"progressbar\" style=\"width:50%\">";
                    out << "<strong>" << QString::number((v.getMin()+v.getMax())/2) << "</strong>";
                    out << "</div>\n";
                    out << "                </div>\n";
                    out << "                <div class=\"leftFloat\"><small>";
                    out << QString::number(v.getMin()) << "</small></div>\n";
                    out << "                <div class=\"rightFloat\"> <small>";
                    out << QString::number(v.getMax()) << "</small></div>\n";
                    out << "            </div>\n";
                    varCnt++;
                }
            }
        }

        out << "        </div>\n";
        out << "        <div id=\"discr\" class=\"tab-pane fade\">\n";
        out << "        </div>\n";
        out << "        <div id=\"message\" class=\"tab-pane fade\">\n";
        out << "        </div>\n";
        out << "    </div>\n";

        out << "</div>\n";

        out << "</body>\n";
        out << "</html>\n";
    }
}

void PageCreator::createObjStyle(const QString &fName)
{
    QFile file(fName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << ".ainData\n";
        out << " {\n";
        out << "    float: left;\n";
        out << "    width: 150px;\n";
        out << "    background-color: #efe;\n";
        out << "    border: 1px solid #888;\n";
        out << "    border-radius: 10px;\n";
        out << "    margin: 5px;\n";
        out << "    padding: 5px;\n";
        out << "    color:#7A5A5A;\n";
        out << "    box-shadow: 5px 5px 3px #cccccc;\n";
        out << " }\n";
        out << ".bitData\n";
        out << "{\n";
        out << "    float: left;\n";
        out << "    width: 150px;\n";
        out << "    background-color: #efe;\n";
        out << "    border: 1px solid #888;\n";
        out << "    border-radius: 10px;\n";
        out << "    margin: 5px;\n";
        out << "    padding: 5px;\n";
        out << "    color:#7A5A5A;\n";
        out << "    box-shadow: 5px 5px 3px #cccccc;\n";
        out << "}\n";
        out << ".progress\n";
        out << "{\n";
        out << "    text-align: center;\n";
        out << "    text-shadow: 0px 1px 2px #00f;\n";
        out << "    background: #ccc;\n";
        out << "    margin: 0px;\n";
        out << "    padding: 0px;\n";
        out << "    border-radius: 7px;\n";
        out << "}\n";
        out << ".bitImage\n";
        out << "{\n";
        out << "    float:right;\n";
        out << "}\n";
        out << ".leftFloat\n";
        out << "{\n";
        out << "    float:left;\n";
        out << "}\n";
        out << ".rightFloat\n";
        out << "{\n";
        out << "    float:right;\n";
        out << "}\n";
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

