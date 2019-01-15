#include "company_management.h"
#include "ui_company_management.h"

company_management::company_management(QWidget *parent,
                                       int id,
                                       int parent_id,
                                       int current_id,
                                       int stack) :
    QDialog(parent),
    ui(new Ui::company_management)
{
    ui->setupUi(this);

    user_id = id;
    company_parent_id = parent_id;
    company_id = current_id;
    ui->stackedWidget->setCurrentIndex(stack);
}

company_management::~company_management()
{
    delete ui;
}

void company_management::on_pb_ok_create_company_clicked()
{
    if (!ui->le_name_create_company->text().isEmpty() &&
            !ui->te_address_create_company->toPlainText().isEmpty()) {
        QStringList parameter;
        parameter.append(QString::number(user_id));
        parameter.append(QString::number(company_parent_id));
        parameter.append(ui->le_name_create_company->text());
        parameter.append(ui->te_address_create_company->toPlainText());

        QStringList result = db_mysql.read(db, "call create_company(:p1,:p2,:p3,:p4)",parameter);
        if (result.length() > 0) {
            if (result.at(0) == "SUCCESS") {
                QMessageBox::information(this,result.at(0),result.at(1));
                if (result.length() > 2) {
                    QString last_id = result.at(2);
                    parameter.clear();
                    parameter.append(QString::number(user_id));
                    parameter.append(last_id);

                    result = db_mysql.read(db, "call set_user_company(:p1,:p2)",parameter);
                    if (result.at(0) == "SUCCESS") {
                        QMessageBox::information(this,result.at(0),result.at(1));
                        this->close();
                    } else {
                        QMessageBox::critical(this,result.at(0),result.at(1));
                    }

                    parameter.clear();
                    parameter.append("1");
                    parameter.append(last_id);

                    result = db_mysql.read(db, "call set_user_company(:p1,:p2)",parameter);
                }
            } else {
                QMessageBox::critical(this,result.at(0),result.at(1));
            }
        } else {
            QMessageBox::critical(this,"Error",result.join(','));
        }
    } else {
        QMessageBox::critical(this,"Error","Mohon formulir pendaftaran perusahaan dilengkapi");
    }
}

void company_management::on_pb_cancel_create_company_clicked()
{
    this->close();
}

void company_management::fill_form_edit()
{
    QStringList result = db_mysql.read(db, "call get_company_detail('"+QString::number(company_id)+"')", QStringList());
    if (result.length() > 0) {
        ui->le_name_edit_company->setText(result.at(0));
        ui->te_address_edit_company->setText(result.at(1));
    } else {
        QMessageBox::critical(this,"Error","Data perusahaan tidak ditemukan");
    }
}
void company_management::on_pb_ok_edit_company_clicked()
{
    if (!ui->le_name_edit_company->text().isEmpty() &&
            !ui->te_address_edit_company->toPlainText().isEmpty()) {
        QStringList parameter;
        parameter.append(QString::number(user_id));
        parameter.append(QString::number(company_id));
        parameter.append(ui->le_name_edit_company->text());
        parameter.append(ui->te_address_edit_company->toPlainText());

        QStringList result = db_mysql.read(db, "call update_company(:p1,:p2,:p3,:p4)",parameter);
        if (result.length() > 0) {
            if (result.at(0) == "SUCCESS") {
                QMessageBox::information(this,result.at(0),result.at(1));
                this->close();
            } else {
                QMessageBox::critical(this,result.at(0),result.at(1));
            }
        } else {
            QMessageBox::critical(this,"Error",result.join(','));
        }
    } else {
        QMessageBox::critical(this,"Error","Mohon formulir pengeditan perusahaan dilengkapi");
    }
}

void company_management::on_pb_cancel_edit_company_clicked()
{
    this->close();
}

void company_management::delete_company()
{
    QStringList parameter;
    parameter.append(QString::number(user_id));
    parameter.append(QString::number(company_id));

    QStringList result = db_mysql.read(db, "call delete_company(:p1,:p2)", parameter);
    if (result.length() > 0) {
        if (result.at(0) == "SUCCESS") {
            QMessageBox::information(this,result.at(0),result.at(1));
        } else {
            QMessageBox::critical(this,result.at(0),result.at(1));
        }
    }
    this->close();
}
