#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server/servermanager.h"
#include <QDateTime>
#include "webPages/pagecreator.h"
#include <QBrush>


const QString MainWindow::mainXMLFileName="config.xml";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),obTree(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ServerManager* manager = new ServerManager(this);
    connect(manager,SIGNAL(newClient(QString)),this,SLOT(newWebClient(QString)));
    obTree = new ObjList(mainXMLFileName);
    updTree();
    PageCreator pCr(*obTree);
    pCr.createPages();
    link = new LinkManager(obTree);
    connect(link,SIGNAL(correctAnswer(QString)),this,SLOT(correctAnswer(QString)));
    connect(link,SIGNAL(noAnswer(QString)),this,SLOT(noAnswer(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete obTree;
    delete link;
}

void MainWindow::updTree()
{
    plcItems.clear();
    ui->treeWidget->clear();
    QStringList groups = obTree->getGroupNames();
    foreach(QString grName, groups) {
        QTreeWidgetItem* grItem = new QTreeWidgetItem(ui->treeWidget,QStringList()<<grName);
        QStringList obNames = obTree->getObjNamesInGroup(grName);
        foreach (QString obName, obNames) {
            QTreeWidgetItem* obItem = new QTreeWidgetItem(grItem,QStringList()<<obName);
            QSharedPointer<ObjectData> objPtr = obTree->getObject(grName,obName);
            int contrCount = objPtr->getContrCount();
            for(int k=0;k<contrCount;k++) {
                QSharedPointer<ControllerData> plc = objPtr->getController(k);
                QString plcDef = plc->getComment() + " (" + plc->getIP() + ":" + QString::number(plc->getPortNum()) + ")";
                QTreeWidgetItem* plcItem = new QTreeWidgetItem(obItem,QStringList()<<plcDef);
                plcItems.insert(plc->getIP(),plcItem);
                QSharedPointer<VarStorage> vars = plc->getVars();
                int anVarsCount = vars->getAnVarCount();
                if(anVarsCount) {
                    QTreeWidgetItem* anRoot = new QTreeWidgetItem(plcItem,QStringList()<<"аналоговые переменные");
                    for(int i=0;i<anVarsCount;i++) {
                        QStringList varParts = vars->getAnalogVar(i).getName().split(':');
                        QString anVarName = varParts.last() + " (" +
                                vars->getAnalogVar(i).getComment()+") адрес: " + QString::number(vars->getAnalogVar(i).getAddr());
                        QTreeWidgetItem* anVar = new QTreeWidgetItem(anRoot,QStringList()<<anVarName);
                        Q_UNUSED(anVar)
                    }
                }
                int discrVarsCount = vars->getDiscrVarCount();
                if(discrVarsCount) {
                    QTreeWidgetItem* dRoot = new QTreeWidgetItem(plcItem,QStringList()<<"дискретные переменные");
                    for(int i=0;i<discrVarsCount;i++) {
                        QString dVarName = "(" +
                                vars->getDiscreteVar(i).getComment() + ") бит " +
                                QString::number(vars->getDiscreteVar(i).getBitNum()) + " " +
                                vars->getDiscreteVar(i).getParentName();
                        QTreeWidgetItem* dItem = new QTreeWidgetItem(dRoot,QStringList()<<dVarName);
                        Q_UNUSED(dItem)
                    }
                }
                int messageVarsCount = vars->getMessageVarCount();
                if(messageVarsCount) {
                    QTreeWidgetItem* mRoot = new QTreeWidgetItem(plcItem,QStringList()<<"сообщения");
                    for(int i=0;i<messageVarsCount;i++) {
                        QString mVarName = "(" +
                                vars->getMessageVar(i).getMessage() + ") бит " +
                                QString::number(vars->getMessageVar(i).getBitNum()) + " " +
                                vars->getMessageVar(i).getParentName() + ", тип: ";
                        if(vars->getMessageVar(i).getType()==MessageValue::INFO) mVarName += "информация";
                        else if(vars->getMessageVar(i).getType()==MessageValue::WARNING) mVarName += "предупреждение";
                        else if(vars->getMessageVar(i).getType()==MessageValue::ALARM) mVarName += "авария";
                        QTreeWidgetItem* mItem = new QTreeWidgetItem(mRoot,QStringList()<<mVarName);
                        Q_UNUSED(mItem)
                    }
                }
            }
        }
    }
    ui->treeWidget->repaint();
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
void MainWindow::on_pushButtonUpdObjects_clicked()
{
    if(obTree!=nullptr) {
        obTree->updateList(mainXMLFileName);
        updTree();
        PageCreator pCr(*obTree);
        pCr.createPages();
    }
}

void MainWindow::correctAnswer(QString ip)
{
    if(plcItems.keys().contains(ip)) {
        QTreeWidgetItem* item = plcItems.value(ip);
        item->setForeground(0,QBrush(Qt::darkGreen));
        ui->treeWidget->repaint();
    }
}

void MainWindow::noAnswer(QString ip)
{
    if(plcItems.keys().contains(ip)) {
        QTreeWidgetItem* item = plcItems.value(ip);
        item->setForeground(0,QBrush(Qt::darkRed));
        ui->treeWidget->repaint();
    }
}
