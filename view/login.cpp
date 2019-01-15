#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    db = db_mysql.connect_db("Monita5");
    this->setWindowTitle("Monita5 - Login");

}

login::~login()
{
    delete ui;
}

void login::on_pb_login_clicked()
{
    if (!ui->le_username->text().isEmpty() && !ui->le_password->text().isEmpty()) {
        QStringList parameter;
        parameter.append(ui->le_username->text());
        parameter.append(ui->le_password->text());
        QStringList result = db_mysql.read(db, "call get_auth(:p1,:p2)",parameter);
        if (result.length() > 0) {
            if (result.at(0).toInt() > 0) {
                mw = new MainWindow(this, result.at(0).toInt());
                mw->db = QSqlDatabase::database(this->db.connectionName());
                mw->refresh();
                mw->showMaximized();
                this->hide();
            }
        } else {
            QMessageBox::critical(this,"Error","Username dan Password salah");
        }
    } else {
        QMessageBox::critical(this,"Error","Username dan Password masih kosong");
    }
}
