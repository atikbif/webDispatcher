#include "server.h"

#include "qhttpserver-master/src/qhttpserver.h"
#include "qhttpserver-master/src/qhttprequest.h"
#include "qhttpserver-master/src/qhttpresponse.h"
#include "responder.h"

Server::Server(QObject *parent) : QObject(parent)
{
    QHttpServer *server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    server->listen(QHostAddress::Any, 8080);
}

void Server::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    new Responder(req, resp);
    emit newClient(req->remoteAddress());
}

