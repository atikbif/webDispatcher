#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include "qhttpserver-master/src/qhttpserverfwd.h"

class Server : public QObject
{
    Q_OBJECT

    QHttpServer *server;
public:
    explicit Server(QObject *parent = 0);
    ~Server();
signals:
    void newClient(const QString &ipAddr);
public slots:

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);
};

#endif // SERVER_H
