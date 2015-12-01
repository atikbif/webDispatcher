#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server/servermanager.h"
#include <QDateTime>
#include "link/objlist.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    const QString mainXMLFileName="config.xml";
    ui->setupUi(this);
    ServerManager* manager = new ServerManager(this);
    connect(manager,SIGNAL(newClient(QString)),this,SLOT(newWebClient(QString)));
    ObjList obTree(mainXMLFileName);
    Q_UNUSED(obTree)
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newWebClient(const QString &ipAddr)
{
    for(int i=0;i<ui->tableWidgetIP->rowCount();i++) {
        QTableWidgetItem* ipItem = ui->tableWidgetIP->item(i,0);
        QStringList itList = ipItem->text().split(' ');
        if(itList.count()) {
            if(itList.at(0)==ipAddr) {
                ui->tableWidgetIP->removeRow(i);
                break;
            }
        }
    }
    ui->tableWidgetIP->insertRow(0);
    ui->tableWidgetIP->setItem(0,0,new QTableWidgetItem(ipAddr));
    ui->tableWidgetIP->setItem(0,1,new QTableWidgetItem(QDateTime::currentDateTime().toString()));
    if(ui->tableWidgetIP->rowCount()>10) ui->tableWidgetIP->removeRow(ui->tableWidgetIP->rowCount()-1);
}
