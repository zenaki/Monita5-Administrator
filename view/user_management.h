#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include <QDialog>
#include <QtGui>
#include <QMessageBox>
#include <QStandardItemModel>

#include "configure/mysql.h"

namespace Ui {
class user_management;
}

class user_management : public QDialog
{
    Q_OBJECT

public:
    explicit user_management(
            QWidget *parent = 0,
            int id = 0,
            int stack = 0
    );
    ~user_management();

    int user_id;

    mysql db_mysql;
    QSqlDatabase db;

    void set_user_role();

    int editor_id;
    void fill_form_edit();

private slots:
    void on_pb_ok_create_user_clicked();
    void on_pb_cancel_create_user_clicked();
    //    void handleRoleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
//    void SlotItemChanged(QStandardItem *poItem);
//    void SlotListItemActivated(const QModelIndex &oIndex);

    void on_pb_ok_edit_user_clicked();

    void on_pb_cancel_edit_user_clicked();

private:
    Ui::user_management *ui;

    QStandardItemModel *role_model;
    QStringList role_list;

};

#endif // USER_MANAGEMENT_H
