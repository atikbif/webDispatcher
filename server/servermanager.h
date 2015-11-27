#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include "server.h"
#include <QThread>

class ServerManager : public QObject
{
    Q_OBJECT

    QThread servThread;
    Server *server;
public:
    explicit ServerManager(QObject *parent = 0);
    ~ServerManager();

signals:
    void newClient(const QString &ipAddr);
public slots:
};

#endif // SERVERMANAGER_H
