#ifndef COMPANY_MANAGEMENT_H
#define COMPANY_MANAGEMENT_H

#include <QDialog>
#include <QMessageBox>

#include "configure/mysql.h"

namespace Ui {
class company_management;
}

class company_management : public QDialog
{
    Q_OBJECT

public:
    explicit company_management(
            QWidget *parent = 0,
            int id = 0,
            int parent_id = 0,
            int current_id = 0,
            int stack = 0
    );
    ~company_management();

    int user_id;
    int company_parent_id;
    int company_id;

    mysql db_mysql;
    QSqlDatabase db;

    void fill_form_edit();
    void delete_company();

private slots:
    void on_pb_ok_create_company_clicked();
    void on_pb_cancel_create_company_clicked();
    
    void on_pb_ok_edit_company_clicked();
    void on_pb_cancel_edit_company_clicked();

private:
    Ui::company_management *ui;

};

#endif // COMPANY_MANAGEMENT_H
