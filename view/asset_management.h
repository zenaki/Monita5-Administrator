#ifndef ASSET_MANAGEMENT_H
#define ASSET_MANAGEMENT_H

#include <QDialog>
#include <QMessageBox>

#include "configure/mysql.h"

namespace Ui {
class asset_management;
}

class asset_management : public QDialog
{
    Q_OBJECT

public:
    explicit asset_management(
            QWidget *parent = 0,
            int id = 0,
            int parent_id = 0,
            int company_id = 0,
            int current_id = 0,
            int stack = 0
    );
    ~asset_management();

    mysql db_mysql;
    QSqlDatabase db;

    void set_asset_type();
    void fill_form_edit(QString name, QString type);

private slots:
    void on_pb_ok_create_asset_clicked();
    void on_pb_cancel_create_asset_clicked();

    void on_pb_ok_edit_asset_clicked();

    void on_pb_cancel_edit_asset_clicked();

private:
    Ui::asset_management *ui;

    int user_id;
    int parent_id;
    int company_id;
    int asset_id;

    QStringList type_list;
};

#endif // ASSET_MANAGEMENT_H
