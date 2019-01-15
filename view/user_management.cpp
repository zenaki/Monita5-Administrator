#include "user_management.h"
#include "ui_user_management.h"

user_management::user_management(QWidget *parent, int id, int stack) :
    QDialog(parent),
    ui(new Ui::user_management)
{
    ui->setupUi(this);

    user_id = id;
    ui->stackedWidget->setCurrentIndex(stack);
//    set_user_role();
}

user_management::~user_management()
{
    delete ui;
}

void user_management::set_user_role()
{

    QStringList result = db_mysql.read(db, "call get_role_list()", QStringList());
    if (result.length() > 0) {
        role_list = result;
        result = db_mysql.read(db, "call get_role_user("+QString::number(user_id)+")", QStringList());
        role_model = new QStandardItemModel(role_list.length()/3,1);
        for (int i = 0; i < role_list.length(); i+=3) {
            QStandardItem* role_type = new QStandardItem(role_list.at(i+1));
            role_type->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            bool ok = false;
            for (int j = 0; j < result.length(); j++) {
                if (result.at(j) == role_list.at(i)) {
                    ok = true;
                    break;
                }
            }
            if (ok) {
                role_type->setData(Qt::Checked, Qt::CheckStateRole);
            } else {
                role_type->setData(Qt::Unchecked, Qt::CheckStateRole);
            }

            role_model->setItem(i/3, 0, role_type);
        }
        ui->lv_role_create_user->setModel(role_model);
        ui->lv_role_edit_user->setModel(role_model);
    } else {
        QMessageBox::critical(this,"Error","Gagal mendapatkan list user role");
    }
}

void user_management::on_pb_ok_create_user_clicked()
{
    QStringList user_role;
    for (int i = 0; i < ui->lv_role_create_user->model()->rowCount(); i++) {
        if (ui->lv_role_create_user->model()->index(i,0).data(Qt::CheckStateRole).toBool()) {
            user_role.append(role_list.at(i*3));
        }
    }
    if (!ui->le_username_create_user->text().isEmpty() &&
            !ui->le_password_create_user->text().isEmpty() &&
            !ui->le_name_create_user->text().isEmpty() &&
            !ui->le_email_create_user->text().isEmpty()) {
        QStringList parameter;
        parameter.append(QString::number(user_id));
        parameter.append(ui->le_username_create_user->text());
        parameter.append(ui->le_password_create_user->text());
        parameter.append(ui->le_name_create_user->text());
        parameter.append(ui->le_email_create_user->text());

        QStringList result = db_mysql.read(db, "call create_user(:p1,:p2,:p3,:p4,:p5)",parameter);
        if (result.length() > 0) {
            if (result.at(0) == "SUCCESS") {
                QMessageBox::information(this,result.at(0),result.at(1));

                if (result.length() > 2) {
                    QString last_id = result.at(2);
                    QStringList result_list;
                    bool ok = true;
                    for (int i = 0; i < user_role.length(); i++) {
                        parameter.clear();
                        parameter.append(last_id);
                        parameter.append(user_role.at(i));
                        result = db_mysql.read(db, "call set_user_role(:p1,:p2)",parameter);

                        if (result.length()>0) {
                            if (result.at(0) == "SUCCESS") {
//                                QMessageBox::information(this,result.at(0),result.at(1));
                            } else {
                                ok = false;
//                                QMessageBox::critical(this,result.at(0),result.at(1));
                            }
                            result_list.append(result.at(0));
                            result_list.append(result.at(1));
                        } else {
                            QMessageBox::critical(this,"Error",result.join("\n"));
                            result_list.append("Error");
                            result_list.append(result.join("\n"));
                        }
                    }
                    if (ok) {
                        QMessageBox::information(this,"SUCCESS",result_list.join("\n"));
                        this->close();
                    } else {
                        QMessageBox::critical(this,"ERROR",result_list.join("\n"));
                    }
                }
            } else {
                QMessageBox::critical(this,result.at(0),result.at(1));
            }
        } else {
            QMessageBox::critical(this,"Error",result.join("\n"));
        }
    } else {
        QMessageBox::critical(this,"Error","Mohon formulir pendaftaran user dilengkapi");
    }
}

void user_management::on_pb_cancel_create_user_clicked()
{
    this->close();
}

void user_management::fill_form_edit()
{
    QStringList result = db_mysql.read(db, "call get_user("+QString::number(user_id)+")",QStringList());
    if (result.length() == 3) {
        ui->le_username_edit_user->setText(result.at(0));
        ui->le_name_edit_user->setText(result.at(1));
        ui->le_email_edit_user->setText(result.at(2));
    } else {
        QMessageBox::critical(this,"Error","User tidak ditemukan");
    }

}

void user_management::on_pb_ok_edit_user_clicked()
{
    QStringList user_role;
    for (int i = 0; i < ui->lv_role_edit_user->model()->rowCount(); i++) {
        if (ui->lv_role_create_user->model()->index(i,0).data(Qt::CheckStateRole).toBool()) {
            user_role.append(role_list.at(i*3));
        }
    }
    if (!ui->le_username_edit_user->text().isEmpty() &&
            !ui->le_password_edit_user->text().isEmpty() &&
            !ui->le_name_edit_user->text().isEmpty() &&
            !ui->le_email_edit_user->text().isEmpty()) {
        QStringList parameter;
        parameter.append(QString::number(editor_id));
        parameter.append(QString::number(user_id));
        parameter.append(ui->le_password_edit_user->text());
        parameter.append(ui->le_name_edit_user->text());
        parameter.append(ui->le_email_edit_user->text());

        QStringList result = db_mysql.read(db, "call update_user(:p1,:p2,:p3,:p4,:p5)",parameter);
        if (result.length() > 0) {
            if (result.at(0) == "SUCCESS") {
                QMessageBox::information(this,result.at(0),result.at(1));

                parameter.clear();
                parameter.append(QString::number(user_id));
                result = db_mysql.read(db, "call delete_user_role(:p1)",parameter);

                QStringList result_list;
                bool ok = true;
                for (int i = 0; i < user_role.length(); i++) {
                    parameter.clear();
                    parameter.append(QString::number(user_id));
                    parameter.append(user_role.at(i));
                    result = db_mysql.read(db, "call set_user_role(:p1,:p2)",parameter);

                    if (result.length()>0) {
                        if (result.at(0) == "SUCCESS") {
//                            QMessageBox::information(this,result.at(0),result.at(1));
                        } else {
                            ok = false;
//                            QMessageBox::critical(this,result.at(0),result.at(1));
                        }
                        result_list.append(result.at(0));
                        result_list.append(result.at(1));
                    } else {
                        QMessageBox::critical(this,"Error",result.join("\n"));
                        result_list.append("Error");
                        result_list.append(result.join("\n"));
                    }
                }
                if (ok) {
                    QMessageBox::information(this,"SUCCESS",result_list.join("\n"));
                    this->close();
                } else {
                    QMessageBox::critical(this,"ERROR",result_list.join("\n"));
                }
            } else {
                QMessageBox::critical(this,result.at(0),result.at(1));
            }
        } else {
            QMessageBox::critical(this,"Error",result.join("\n"));
        }
    } else {
        QMessageBox::critical(this,"Error","Mohon formulir pendaftaran user dilengkapi");
    }
}

void user_management::on_pb_cancel_edit_user_clicked()
{
    this->close();
}
