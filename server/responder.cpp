#include "responder.h"
#include "qhttpserver-master/src/qhttprequest.h"
#include "qhttpserver-master/src/qhttpresponse.h"
#include <QRegExp>
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QUrlQuery>
#include "dynamic/dynreqmanager.h"

Responder::Responder(QHttpRequest *req, QHttpResponse *resp, QObject *parent) : QObject(parent)
{
    m_req = req;
    connect(req, SIGNAL(end()), resp, SLOT(end()));
    connect(resp, SIGNAL(done()), this, SLOT(deleteLater()));


    bool flag = false;
    QString ext;
    QByteArray blob;
    QString fName = req->path();
    if(fName.isEmpty()||(fName=="/")) fName = "index.html";
    fName = QApplication::applicationDirPath() +"/html/" + fName;
    QString reqBody = QUrlQuery(req->url()).queryItemValue("ob");
    if(QFile::exists(fName)) {

        QFile file(fName);
        QFileInfo fInfo(file);
        QRegExp extExpr("^.*\\.([^\\.]+)$");
        if(extExpr.indexIn(fInfo.fileName())!=-1) {
            ext = extExpr.cap(1);
        }
        if(file.open(QIODevice::ReadOnly)) {
            blob = file.readAll();
            flag = true;
            file.close();
        }
    }else if(!reqBody.isEmpty()){
        fName = req->url().fileName();
        if(fName=="din.txt") {
            blob = DynReqManager::getData(fName,reqBody).toUtf8();
            flag = true;
        }else if(fName=="ain.txt") {
            blob = DynReqManager::getData(fName,reqBody).toUtf8();
            flag = true;
        }else if(fName=="message.txt") {
            blob = DynReqManager::getData(fName,reqBody).toUtf8();
            flag = true;
        }else if(fName=="status.txt") {
            blob = DynReqManager::getData(fName,reqBody).toUtf8();
            flag = true;
        }
    }

    if(flag) {
        if(ext=="css") resp->setHeader("Content-Type", "text/css");
        else if(ext=="js") resp->setHeader("Content-Type", "text/javascript");
        else if(ext=="png") resp->setHeader("Content-Type", "image/png");
        else resp->setHeader("Content-Type", "text/html");
        resp->writeHead(200);
        resp->write(blob);
    }else {
        resp->writeHead(403);
        resp->write(QByteArray("File not found!"));
        return;
    }
}

Responder::~Responder()
{
    delete m_req;
    m_req = 0;
}

