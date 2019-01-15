#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, int id) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    login_window = parent;
    user_id = id;

    this->setWindowTitle("Monita5 - Administrator");

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

    ui->treeWidget->setColumnCount(4);
    ui->treeWidget->setColumnHidden(1,true);
    QStringList header_name_list;
    header_name_list.append("Name");
    header_name_list.append("ID");
    header_name_list.append("Type");
    header_name_list.append("Asset Type");
    ui->treeWidget->setHeaderLabels(header_name_list);
//    ui->treeWidget->setHeaderHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::refresh()
{
    ui->treeWidget->clear();
//    assetModel = new QStandardItemModel;
    QStringList result = db_mysql.read(db, "call get_root_company("+QString::number(user_id)+")", QStringList());
    root_company_id = result.at(0).toInt();
    root_company_name = result.at(1);

    for (int i = 0; i < result.length(); i+=4) {
        addTreeRoot(result.at(i+1),result.at(i),result.at(i+2),result.at(i+3));
    }
    for (int i = 0; i < result.length(); i+=4) {
        bool ok = true;
        for (int j = 0; j < ui->treeWidget->topLevelItemCount(); j++) {
            if (result.at(i) == ui->treeWidget->topLevelItem(j)->text(1) &&
                    result.at(i+1) == ui->treeWidget->topLevelItem(j)->text(0) &&
                    result.at(i+2) == ui->treeWidget->topLevelItem(j)->text(2) &&
                    result.at(i+3) == ui->treeWidget->topLevelItem(j)->text(3)) {
                ok = false;
                break;
            }

            if (result.at(i) == ui->treeWidget->topLevelItem(j)->text(1) &&
                    result.at(i+2) == ui->treeWidget->topLevelItem(j)->text(2)) {
                ui->treeWidget->topLevelItem(j)->setText(0,result.at(i+1));
                ui->treeWidget->topLevelItem(j)->setText(3,result.at(i+3));
                ok = false;
                break;
            }
        }
        if (ok) {
            addTreeRoot(result.at(i+1), result.at(i), result.at(i+2), result.at(i+3));
        }
    }
//    addTreeRoot("Root-1","1","Root 1","R1");
//    addTreeRoot("Root-2","2","Root 2","R2");
//    addTreeRoot("Root-3","3","Root 3","R3");
}

void MainWindow::addTreeRoot(QString name, QString id, QString type, QString asset_type)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->treeWidget);

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, id);
    treeItem->setText(2, type);
    treeItem->setText(3, asset_type);
}

void MainWindow::addTreeChild(QTreeWidgetItem *parent,
                  QString name, QString id, QString type, QString asset_type)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, id);
    treeItem->setText(2, type);
    treeItem->setText(3, asset_type);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
}


void MainWindow::onCustomContextMenu(QPoint p)
{
//    Q_UNUSED(p);
    QTreeWidgetItem *treeItem = ui->treeWidget->itemAt(p);
    if (treeItem) {
        current_name = treeItem->text(0);
        current_id = treeItem->text(1).toInt();
        current_type = treeItem->text(2);
        current_asset_type = treeItem->text(3);
    } else {
        current_id = 0;
        current_type = "none";

    }
    current_item = treeItem;

    QMenu *menu = new QMenu(this);
    if (current_type == "company") {
        menu->addAction(new QAction("Add Company",this));
        connect(menu->actions().at(0), SIGNAL(triggered(bool)), SLOT(on_actionCreate_Company_triggered()));
        menu->addAction(new QAction("Edit Company",this));
        connect(menu->actions().at(1), SIGNAL(triggered(bool)), SLOT(on_actionEdit_Company_triggered()));
        menu->addAction(new QAction("Delete Company",this));
        connect(menu->actions().at(2), SIGNAL(triggered(bool)), SLOT(on_actionDelete_Company_triggered()));
        menu->addAction(new QAction("Add Asset",this));
        connect(menu->actions().at(3), SIGNAL(triggered(bool)), SLOT(on_actionCreate_Asset_triggered()));
        menu->addSeparator();
        menu->addAction(new QAction("Manage Module",this));
        connect(menu->actions().at(5), SIGNAL(triggered(bool)), SLOT(on_actionManage_Module_triggered()));
    } else if (current_type == "asset") {
        menu->addAction(new QAction("Add Asset",this));
        connect(menu->actions().at(0), SIGNAL(triggered(bool)), SLOT(on_actionCreate_Asset_triggered()));
        menu->addAction(new QAction("Edit Asset",this));
        connect(menu->actions().at(1), SIGNAL(triggered(bool)), SLOT(on_actionEdit_Asset_triggered()));
        menu->addAction(new QAction("Delete Asset",this));
        connect(menu->actions().at(2), SIGNAL(triggered(bool)), SLOT(on_actionDelete_Asset_triggered()));
        QTreeWidgetItem *parentItem = treeItem->parent();

        while (1) {
            if (parentItem->text(2) == "company") {
                current_company_id = parentItem->text(1).toInt();
                break;
            }
            parentItem = parentItem->parent();
        }

    } else if (current_type == "none") {
        menu->addAction(new QAction("Add Company",this));
        connect(menu->actions().at(0), SIGNAL(triggered(bool)), SLOT(on_actionCreate_Company_triggered()));
    }
    menu->popup(ui->treeWidget->viewport()->mapToGlobal(p));
}

void MainWindow::on_treeWidget_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    QTreeWidgetItem *treeItem = ui->treeWidget->currentItem();
//    qDebug() << treeItem->text(0) << treeItem->text(1) << treeItem->text(2);
    if (treeItem) {
        current_id = treeItem->text(1).toInt();
        current_type = treeItem->text(2);
    } else {
        current_id = 0;
        current_type = "none";
    }
    QStringList result;
    if (current_type == "company") {
        result = db_mysql.read(db, "call get_asset('"+current_type+"',"+
                               QString::number(current_id)+",0)",
                               QStringList());
    } else if (treeItem->text(2) == "asset") {
        QTreeWidgetItem *parentItem = treeItem->parent();
        while (1) {
            if (parentItem->text(2) == "company") {
                current_company_id = parentItem->text(1).toInt();
                break;
            }
            parentItem = parentItem->parent();
        }
        result = db_mysql.read(db, "call get_asset('"+current_type+"',"+
                               QString::number(current_company_id)+","+
                               QString::number(current_id)+")",
                               QStringList()
                );
    }

//    qDebug() << "hasil klik 2x";
//    qDebug() << result;

    for (int i = 0; i < result.length(); i+=4) {
        bool ok = true;
        for (int j = 0; j < treeItem->childCount(); j++) {
            if (result.at(i) == treeItem->child(j)->text(1) &&
                    result.at(i+1) == treeItem->child(j)->text(0) &&
                    result.at(i+2) == treeItem->child(j)->text(2) &&
                    result.at(i+3) == treeItem->child(j)->text(3)) {
                ok = false;
                break;
            }

            if (result.at(i) == treeItem->child(j)->text(1) &&
                    result.at(i+2) == treeItem->child(j)->text(2)) {
                treeItem->child(j)->setText(0,result.at(i+1));
                treeItem->child(j)->setText(3,result.at(i+3));
                ok = false;
                break;
            }
        }
        if (ok) {
            addTreeChild(treeItem,result.at(i+1), result.at(i), result.at(i+2), result.at(i+3));
        }
    }
}


void MainWindow::on_actionCreate_User_triggered()
{
    um = new user_management(this,user_id,CREATE);
    um->db = QSqlDatabase::database(this->db.connectionName());
    um->set_user_role();
    um->exec();
}

void MainWindow::on_actionCreate_Company_triggered()
{
    cm = new company_management(this,user_id,current_id,0,CREATE);
    cm->db = QSqlDatabase::database(this->db.connectionName());
    cm->exec();
    if (current_type == "none") this->refresh();
}

void MainWindow::on_actionEdit_Company_triggered()
{
    cm = new company_management(this,user_id,0,current_id,UPDATE);
    cm->db = QSqlDatabase::database(this->db.connectionName());
    cm->fill_form_edit();
    cm->exec();
    if (current_type == "none") this->refresh();
}

void MainWindow::on_actionDelete_Company_triggered()
{
//    cm = new company_management(this,user_id,0,current_id);
//    cm->db = QSqlDatabase::database(this->db.connectionName());
//    cm->delete_company();

    QStringList parameter;
    parameter.append(QString::number(user_id));
    parameter.append(QString::number(current_id));

    QStringList result = db_mysql.read(db, "call delete_company(:p1,:p2)", parameter);
    if (result.length() > 0) {
        if (result.at(0) == "SUCCESS") {
            QMessageBox::information(this,result.at(0),result.at(1));
        } else {
            QMessageBox::critical(this,result.at(0),result.at(1));
        }
    }
//    this->refresh();
    current_item->parent()->removeChild(current_item);
}

void MainWindow::on_actionCreate_Asset_triggered()
{
    if (current_type == "company") {
        am = new asset_management(this,user_id,0, current_id,0,CREATE);
    } else if (current_type == "asset") {
        am = new asset_management(this,user_id,current_id,current_company_id,0,CREATE);
    }
    am->db = QSqlDatabase::database(this->db.connectionName());
    am->set_asset_type();
    am->exec();
}

void MainWindow::on_actionEdit_Asset_triggered()
{
    am = new asset_management(this,user_id,0,current_company_id,current_id,UPDATE);
    am->db = QSqlDatabase::database(this->db.connectionName());
    am->set_asset_type();
    am->fill_form_edit(current_name,current_asset_type);
    am->exec();
}

void MainWindow::on_actionDelete_Asset_triggered()
{
    QStringList parameter;
    parameter.append(QString::number(user_id));
    parameter.append(QString::number(current_id));

    QStringList result = db_mysql.read(db, "call delete_asset(:p1,:p2)", parameter);
    if (result.length() > 0) {
        if (result.at(0) == "SUCCESS") {
            QMessageBox::information(this,result.at(0),result.at(1));
        } else {
            QMessageBox::critical(this,result.at(0),result.at(1));
        }
    }
//    this->refresh();
    current_item->parent()->removeChild(current_item);
}

void MainWindow::on_actionLogout_triggered()
{
    this->close();
    login_window->show();
}

void MainWindow::on_actionEdit_User_triggered()
{
    um = new user_management(this,user_id,1);
    um->editor_id = user_id;
    um->db = QSqlDatabase::database(this->db.connectionName());
    um->set_user_role();
    um->fill_form_edit();
    um->exec();
}

void MainWindow::on_actionManage_Module_triggered()
{
    module_management *module_m = new module_management(this, user_id, current_id, current_name);
    module_m->db = QSqlDatabase::database(this->db.connectionName());
    module_m->refresh_module();
    ChildWindow *childWindow = new ChildWindow(ui->mdiArea, module_m);
    childWindow->setAttribute(Qt::WA_DeleteOnClose);
    childWindow->show();
}
