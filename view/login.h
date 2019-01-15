#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QMessageBox>

#include "view/mainwindow.h"
#include "configure/mysql.h"

namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private slots:
    void on_pb_login_clicked();

private:
    Ui::login *ui;
    MainWindow *mw;

    mysql db_mysql;
    QSqlDatabase db;

};

#endif // LOGIN_H
