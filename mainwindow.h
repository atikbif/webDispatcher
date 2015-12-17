#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "link/objlist.h"
#include "link/linkmanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    ObjList* obTree;
    LinkManager* link;
    static const QString mainXMLFileName;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void updTree(void);
private slots:
    void newWebClient(const QString &ipAddr);
    void on_pushButtonUpdObjects_clicked();
};

#endif // MAINWINDOW_H
