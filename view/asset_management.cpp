#include "asset_management.h"
#include "ui_asset_management.h"

asset_management::asset_management(QWidget *parent,
                                   int id,
                                   int parent_id,
                                   int company_id,
                                   int current_id,
                                   int stack) :
    QDialog(parent),
    ui(new Ui::asset_management)
{
    ui->setupUi(this);

    this->user_id = id;
    this->parent_id = parent_id;
    this->company_id = company_id;
    this->asset_id = current_id;
    this->ui->stackedWidget->setCurrentIndex(stack);
}

asset_management::~asset_management()
{
    delete ui;
}

void asset_management::set_asset_type()
{
    QStringList result = db_mysql.read(db, "call get_asset_type()", QStringList());
    if (result.length() > 0) {
        type_list = result;
        for (int i = 0; i < type_list.length(); i+=2) {
            ui->cb_asset_type_create_asset->addItem(type_list.at(i+1));
            ui->cb_asset_type_edit_asset->addItem(type_list.at(i+1));
        }
    } else {
        QMessageBox::critical(this,"Error","Gagal mendapatkan list user role");
    }
}

void asset_management::fill_form_edit(QString name, QString type)
{
    ui->le_asset_name_edit_asset->setText(name);
    if (type == "e") {
        type = "equipment";
    } else if (type == "eg") {
        type = "equipment group";
    }
    ui->cb_asset_type_edit_asset->setCurrentText(type);
}

void asset_management::on_pb_ok_create_asset_clicked()
{
    if (!ui->le_asset_name_create_asset->text().isEmpty() &&
            !ui->cb_asset_type_create_asset->currentText().isEmpty()) {
        QStringList parameter;
        parameter.append(QString::number(user_id));
        parameter.append(ui->le_asset_name_create_asset->text());
        parameter.append(type_list.at(ui->cb_asset_type_create_asset->currentIndex()*2));
        parameter.append(QString::number(parent_id));
        parameter.append(QString::number(company_id));

        QStringList result = db_mysql.read(db, "call create_asset(:p1,:p2,:p3,:p4,:p5)",parameter);
        if (result.length() > 0) {
            if (result.at(0) == "SUCCESS") {
                QMessageBox::information(this,result.at(0),result.at(1));
                this->close();
            } else {
                QMessageBox::critical(this,result.at(0),result.at(1));
            }
        }
    } else {
        QMessageBox::critical(this,"Error","Mohon formulir pendaftaran asset dilengkapi");
    }
}

void asset_management::on_pb_cancel_create_asset_clicked()
{
    this->close();
}

void asset_management::on_pb_ok_edit_asset_clicked()
{
    if (!ui->le_asset_name_edit_asset->text().isEmpty() &&
            !ui->cb_asset_type_edit_asset->currentText().isEmpty()) {
        QStringList parameter;
        parameter.append(QString::number(user_id));
        parameter.append(QString::number(asset_id));
        parameter.append(ui->le_asset_name_edit_asset->text());
        parameter.append(type_list.at(ui->cb_asset_type_edit_asset->currentIndex()*2));

        QStringList result = db_mysql.read(db, "call update_asset(:p1,:p2,:p3,:p4)",parameter);
        if (result.length() > 0) {
            if (result.at(0) == "SUCCESS") {
                QMessageBox::information(this,result.at(0),result.at(1));
                this->close();
            } else {
                QMessageBox::critical(this,result.at(0),result.at(1));
            }
        }
    } else {
        QMessageBox::critical(this,"Error","Mohon formulir pengeditan asset dilengkapi");
    }
}

void asset_management::on_pb_cancel_edit_asset_clicked()
{
    this->close();
}
