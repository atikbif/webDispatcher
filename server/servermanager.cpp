#include "servermanager.h"

ServerManager::ServerManager(QObject *parent) : QObject(parent)
{
    server = new Server();
    server->moveToThread(&servThread);
    connect(&servThread,SIGNAL(finished()),server,SLOT(deleteLater()));
    connect(server,SIGNAL(newClient(QString)),this,SIGNAL(newClient(QString)));
    servThread.start();
}

ServerManager::~ServerManager()
{
    servThread.quit();
    servThread.wait();
}

