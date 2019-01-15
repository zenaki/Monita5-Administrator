#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
#include <QTreeWidget>

#include "configure/mysql.h"
#include "view/user_management.h"
#include "view/company_management.h"
#include "view/asset_management.h"

#include "view/childwindow.h"
#include "view/module_management.h"

#define CREATE 0
#define UPDATE 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, int id = 0);
    ~MainWindow();

    int user_id;

    mysql db_mysql;
    QSqlDatabase db;

    void refresh();

private:
    Ui::MainWindow *ui;

    QWidget *login_window;
    user_management *um;
    company_management *cm;
    asset_management *am;

    QStandardItemModel *assetModel;
    QMenu *assetMenu;

    QString current_name;
    int current_id;
    QString current_type;
    QString current_asset_type;
    QTreeWidgetItem *current_item;
    int current_company_id;
    int root_company_id;
    QString root_company_name;

    void addTreeRoot(QString name, QString id, QString type, QString asset_type);
    void addTreeChild(QTreeWidgetItem *parent,
                      QString name, QString id, QString type, QString asset_type);

private slots:
    void onCustomContextMenu(QPoint p);
    void on_actionCreate_User_triggered();
    void on_actionCreate_Company_triggered();
    void on_actionEdit_Company_triggered();
    void on_actionDelete_Company_triggered();
    void on_actionCreate_Asset_triggered();
    void on_actionEdit_Asset_triggered();
    void on_actionDelete_Asset_triggered();
    void on_actionLogout_triggered();
    void on_treeWidget_doubleClicked(const QModelIndex &index);
    void on_actionEdit_User_triggered();
    void on_actionManage_Module_triggered();
};

#endif // MAINWINDOW_H
