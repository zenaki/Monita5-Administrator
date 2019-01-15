#include "module_management.h"
#include "ui_module_management.h"

module_management::module_management(QWidget *parent, int id, int c_id, QString c_name) :
    QWidget(parent),
    ui(new Ui::module_management)
{
    ui->setupUi(this);

    user_id = id;
    company_id = c_id;
    company_name = c_name;

    this->setWindowTitle("Module Management ("+company_name+")");
//    refresh_module();

//    SigMapCheck_module = new QSignalMapper(this);
    SigMapButton_save_module = new QSignalMapper(this);
    connect(SigMapButton_save_module, SIGNAL(mapped(int)), this, SLOT(saveButton_module(int)));
    SigMapButton_manage_module = new QSignalMapper(this);
    connect(SigMapButton_manage_module, SIGNAL(mapped(int)), this, SLOT(manageButton_module(int)));

//    SigMapCheck_tag_point = new QSignalMapper(this);
    SigMapButton_tag_point = new QSignalMapper(this);
    connect(SigMapButton_tag_point, SIGNAL(mapped(int)), this, SLOT(saveButton_tag_point(int)));
    SigMapButton_parsing_ref = new QSignalMapper(this);
    connect(SigMapButton_parsing_ref, SIGNAL(mapped(int)), this, SLOT(saveButton_parsing_ref(int)));
}

module_management::~module_management()
{
    delete ui;
}

void module_management::refresh_module()
{
    QStringList result;
    QStringList list_company_id;

    result = db_mysql.read(db, "call get_module_type()",QStringList());
    list_module_type = result;

    result = db_mysql.read(db, "call get_all_company_id("+QString::number(company_id)+")",QStringList());
    list_company = result;
    list_company.prepend(company_name);
    list_company.prepend(QString::number(company_id));

    for (int i = 0; i < list_company.length(); i+=2) list_company_id.append(list_company.at(i));
    result = db_mysql.read(db, "call get_module('"+list_company_id.join(",")+"')",QStringList());
    list_module = result;

//    ui->tbw_module->clear();
    ui->tbw_module->verticalHeader()->setHidden(true);
    ui->tbw_module->setColumnCount(9);

    QTableWidgetItem *header_name[9];

    header_name[0] = new QTableWidgetItem();
    header_name[0]->setText("");
    ui->tbw_module->setHorizontalHeaderItem(0,header_name[0]);
    ui->tbw_module->setColumnWidth(0, 25);

    header_name[1] = new QTableWidgetItem();
    header_name[1]->setText("No");
    ui->tbw_module->setHorizontalHeaderItem(1,header_name[1]);
    ui->tbw_module->setColumnWidth(1, 25);

    header_name[2] = new QTableWidgetItem();
    header_name[2]->setText("ID");
    ui->tbw_module->setHorizontalHeaderItem(2,header_name[2]);
    ui->tbw_module->setColumnWidth(2, 25);
    ui->tbw_module->setColumnHidden(2,true);

    header_name[3] = new QTableWidgetItem();
    header_name[3]->setText("");
    ui->tbw_module->setHorizontalHeaderItem(3,header_name[3]);
    ui->tbw_module->setColumnWidth(3, 50);

    header_name[4] = new QTableWidgetItem();
    header_name[4]->setText("Module Name");
    ui->tbw_module->setHorizontalHeaderItem(4,header_name[4]);
    ui->tbw_module->setColumnWidth(4, 200);

    header_name[5] = new QTableWidgetItem();
    header_name[5]->setText("Serial Number");
    ui->tbw_module->setHorizontalHeaderItem(5,header_name[5]);
    ui->tbw_module->setColumnWidth(5, 200);

    header_name[6] = new QTableWidgetItem();
    header_name[6]->setText("Module Type");
    ui->tbw_module->setHorizontalHeaderItem(6,header_name[6]);
    ui->tbw_module->setColumnWidth(6, 200);

    header_name[7] = new QTableWidgetItem();
    header_name[7]->setText("Company Name");
    ui->tbw_module->setHorizontalHeaderItem(7,header_name[7]);
    ui->tbw_module->setColumnWidth(7, 200);

    header_name[8] = new QTableWidgetItem();
    header_name[8]->setText("");
    ui->tbw_module->setHorizontalHeaderItem(8,header_name[8]);
    ui->tbw_module->setColumnWidth(8, 70);

    this->ui->tbw_module->setRowCount(0);
    for (int i = 0; i < list_module.length(); i+=5) {
        ui->tbw_module->insertRow(ui->tbw_module->rowCount());

        check_module[ui->tbw_module->rowCount()-1] = new QCheckBox(this);
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 0, check_module[ui->tbw_module->rowCount()-1]);

        ui->tbw_module->setItem(ui->tbw_module->rowCount()-1, 1, new QTableWidgetItem(QString::number(ui->tbw_module->rowCount())));

        le_module_id[ui->tbw_module->rowCount()-1] = new QLineEdit(this);
        le_module_id[ui->tbw_module->rowCount()-1]->setText(list_module.at(i));
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 2, le_module_id[ui->tbw_module->rowCount()-1]);

        pb_save_module[ui->tbw_module->rowCount()-1] = new QPushButton(this);
        pb_save_module[ui->tbw_module->rowCount()-1]->setText("Save");
        SigMapButton_save_module->setMapping(pb_save_module[ui->tbw_module->rowCount()-1], ui->tbw_module->rowCount()-1);
        connect(pb_save_module[ui->tbw_module->rowCount()-1], SIGNAL(clicked(bool)),SigMapButton_save_module, SLOT(map()));
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 3, pb_save_module[ui->tbw_module->rowCount()-1]);

        le_module_name[ui->tbw_module->rowCount()-1] = new QLineEdit(this);
        le_module_name[ui->tbw_module->rowCount()-1]->setText(list_module.at(i+1));
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 4, le_module_name[ui->tbw_module->rowCount()-1]);

        le_module_sn[ui->tbw_module->rowCount()-1] = new QLineEdit(this);
        le_module_sn[ui->tbw_module->rowCount()-1]->setText(list_module.at(i+2));
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 5, le_module_sn[ui->tbw_module->rowCount()-1]);

        cb_module_type[ui->tbw_module->rowCount()-1] = new QComboBox(this);
        for (int j = 0; j < list_module_type.length(); j+=2) {
            cb_module_type[ui->tbw_module->rowCount()-1]->addItem(list_module_type.at(j+1));
            if (list_module.at(i+3) == list_module_type.at(j))
                cb_module_type[ui->tbw_module->rowCount()-1]->setCurrentIndex(j);
        }
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 6, cb_module_type[ui->tbw_module->rowCount()-1]);

        cb_module_company[ui->tbw_module->rowCount()-1] = new QComboBox(this);
        for (int j = 0; j < list_company.length(); j+=2) {
            cb_module_company[ui->tbw_module->rowCount()-1]->addItem(list_company.at(j+1));
            if (list_module.at(i+4) == list_company.at(j))
                cb_module_company[ui->tbw_module->rowCount()-1]->setCurrentIndex(j);
        }
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 7, cb_module_company[ui->tbw_module->rowCount()-1]);

        pb_manage_module[ui->tbw_module->rowCount()-1] = new QPushButton(this);
        pb_manage_module[ui->tbw_module->rowCount()-1]->setText("Manage");
        SigMapButton_manage_module->setMapping(pb_manage_module[ui->tbw_module->rowCount()-1], ui->tbw_module->rowCount()-1);
        connect(pb_manage_module[ui->tbw_module->rowCount()-1], SIGNAL(clicked(bool)),SigMapButton_manage_module, SLOT(map()));
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 8, pb_manage_module[ui->tbw_module->rowCount()-1]);
    }

    ui->tab_module->setHidden(true);
}

void module_management::refresh_tag_point()
{
    QStringList result;

    result = db_mysql.read(db, "call get_point_type()",QStringList());
    list_point_type = result;

    result = db_mysql.read(db, "call get_tag_point('"+current_module_sn+"')",QStringList());
    list_tag_point = result;

    ui->tbw_tag_point->verticalHeader()->setHidden(true);
    ui->tbw_tag_point->setColumnCount(8);

    QTableWidgetItem *header_name[8];

    header_name[0] = new QTableWidgetItem();
    header_name[0]->setText("");
    ui->tbw_tag_point->setHorizontalHeaderItem(0,header_name[0]);
    ui->tbw_tag_point->setColumnWidth(0, 25);

    header_name[1] = new QTableWidgetItem();
    header_name[1]->setText("No");
    ui->tbw_tag_point->setHorizontalHeaderItem(1,header_name[1]);
    ui->tbw_tag_point->setColumnWidth(1, 25);

    header_name[2] = new QTableWidgetItem();
    header_name[2]->setText("ID");
    ui->tbw_tag_point->setHorizontalHeaderItem(2,header_name[2]);
    ui->tbw_tag_point->setColumnWidth(2, 25);
    ui->tbw_tag_point->setColumnHidden(2,true);

    header_name[3] = new QTableWidgetItem();
    header_name[3]->setText("");
    ui->tbw_tag_point->setHorizontalHeaderItem(3,header_name[3]);
    ui->tbw_tag_point->setColumnWidth(3, 50);

    header_name[4] = new QTableWidgetItem();
    header_name[4]->setText("Name");
    ui->tbw_tag_point->setHorizontalHeaderItem(4,header_name[4]);
    ui->tbw_tag_point->setColumnWidth(4, 200);

    header_name[5] = new QTableWidgetItem();
    header_name[5]->setText("Unit");
    ui->tbw_tag_point->setHorizontalHeaderItem(5,header_name[5]);
    ui->tbw_tag_point->setColumnWidth(5, 70);

    header_name[6] = new QTableWidgetItem();
    header_name[6]->setText("Type");
    ui->tbw_tag_point->setHorizontalHeaderItem(6,header_name[6]);
    ui->tbw_tag_point->setColumnWidth(6, 70);

    header_name[7] = new QTableWidgetItem();
    header_name[7]->setText("Tag");
    ui->tbw_tag_point->setHorizontalHeaderItem(7,header_name[7]);
    ui->tbw_tag_point->setColumnWidth(7, 150);

    this->ui->tbw_tag_point->setRowCount(0);
    for (int i = 0; i < list_tag_point.length(); i+=5) {
        ui->tbw_tag_point->insertRow(ui->tbw_tag_point->rowCount());

        check_tag_point[ui->tbw_tag_point->rowCount()-1] = new QCheckBox(this);
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 0, check_tag_point[ui->tbw_tag_point->rowCount()-1]);

        ui->tbw_tag_point->setItem(ui->tbw_tag_point->rowCount()-1, 1, new QTableWidgetItem(QString::number(ui->tbw_tag_point->rowCount())));

        le_tag_point_id[ui->tbw_tag_point->rowCount()-1] = new QLineEdit(this);
        le_tag_point_id[ui->tbw_tag_point->rowCount()-1]->setText(list_tag_point.at(i));
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 2, le_tag_point_id[ui->tbw_tag_point->rowCount()-1]);

        pb_save_tag_point[ui->tbw_tag_point->rowCount()-1] = new QPushButton(this);
        pb_save_tag_point[ui->tbw_tag_point->rowCount()-1]->setText("Save");
        SigMapButton_tag_point->setMapping(pb_save_tag_point[ui->tbw_tag_point->rowCount()-1], ui->tbw_tag_point->rowCount()-1);
        connect(pb_save_tag_point[ui->tbw_tag_point->rowCount()-1], SIGNAL(clicked(bool)),SigMapButton_tag_point, SLOT(map()));
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 3, pb_save_tag_point[ui->tbw_tag_point->rowCount()-1]);

        le_tag_point_name[ui->tbw_tag_point->rowCount()-1] = new QLineEdit(this);
        le_tag_point_name[ui->tbw_tag_point->rowCount()-1]->setText(list_tag_point.at(i+1));
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 4, le_tag_point_name[ui->tbw_tag_point->rowCount()-1]);

        le_tag_point_unit[ui->tbw_tag_point->rowCount()-1] = new QLineEdit(this);
        le_tag_point_unit[ui->tbw_tag_point->rowCount()-1]->setText(list_tag_point.at(i+2));
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 5, le_tag_point_unit[ui->tbw_tag_point->rowCount()-1]);

        cb_tag_point_type[ui->tbw_tag_point->rowCount()-1] = new QComboBox(this);
        for (int j = 0; j < list_point_type.length(); j+=2) {
            cb_tag_point_type[ui->tbw_tag_point->rowCount()-1]->addItem(list_point_type.at(j+1));
            if (list_tag_point.at(i+3) == list_point_type.at(j))
                cb_tag_point_type[ui->tbw_tag_point->rowCount()-1]->setCurrentIndex(j);
        }
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 6, cb_tag_point_type[ui->tbw_tag_point->rowCount()-1]);

        lb_tag_point_name[ui->tbw_tag_point->rowCount()-1] = new QLabel(this);
        lb_tag_point_name[ui->tbw_tag_point->rowCount()-1]->setText(list_tag_point.at(i+4));
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 7, lb_tag_point_name[ui->tbw_tag_point->rowCount()-1]);
    }
}

void module_management::on_pb_add_module_clicked()
{
    if (ui->tbw_module->rowCount() < max_module) {
        ui->tbw_module->insertRow(ui->tbw_module->rowCount());
    
        check_module[ui->tbw_module->rowCount()-1] = new QCheckBox(this);
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 0, check_module[ui->tbw_module->rowCount()-1]);
    
        ui->tbw_module->setItem(ui->tbw_module->rowCount()-1, 1, new QTableWidgetItem(QString::number(ui->tbw_module->rowCount())));
    
        le_module_id[ui->tbw_module->rowCount()-1] = new QLineEdit(this);
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 2, le_module_id[ui->tbw_module->rowCount()-1]);
    
        pb_save_module[ui->tbw_module->rowCount()-1] = new QPushButton(this);
        pb_save_module[ui->tbw_module->rowCount()-1]->setText("Save");
        SigMapButton_save_module->setMapping(pb_save_module[ui->tbw_module->rowCount()-1], ui->tbw_module->rowCount()-1);
        connect(pb_save_module[ui->tbw_module->rowCount()-1], SIGNAL(clicked(bool)),SigMapButton_save_module, SLOT(map()));
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 3, pb_save_module[ui->tbw_module->rowCount()-1]);
    
        le_module_name[ui->tbw_module->rowCount()-1] = new QLineEdit(this);
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 4, le_module_name[ui->tbw_module->rowCount()-1]);
    
        le_module_sn[ui->tbw_module->rowCount()-1] = new QLineEdit(this);
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 5, le_module_sn[ui->tbw_module->rowCount()-1]);
    
        cb_module_type[ui->tbw_module->rowCount()-1] = new QComboBox(this);
        for (int i = 0; i < list_module_type.length(); i+=2)
            cb_module_type[ui->tbw_module->rowCount()-1]->addItem(list_module_type.at(i+1));
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 6, cb_module_type[ui->tbw_module->rowCount()-1]);
    
        cb_module_company[ui->tbw_module->rowCount()-1] = new QComboBox(this);
        for (int i = 0; i < list_company.length(); i+=2)
            cb_module_company[ui->tbw_module->rowCount()-1]->addItem(list_company.at(i+1));
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 7, cb_module_company[ui->tbw_module->rowCount()-1]);
    
        pb_manage_module[ui->tbw_module->rowCount()-1] = new QPushButton(this);
        pb_manage_module[ui->tbw_module->rowCount()-1]->setText("Manage");
        SigMapButton_manage_module->setMapping(pb_manage_module[ui->tbw_module->rowCount()-1], ui->tbw_module->rowCount()-1);
        connect(pb_manage_module[ui->tbw_module->rowCount()-1], SIGNAL(clicked(bool)),SigMapButton_manage_module, SLOT(map()));
        ui->tbw_module->setCellWidget(ui->tbw_module->rowCount()-1, 8, pb_manage_module[ui->tbw_module->rowCount()-1]);
        pb_manage_module[ui->tbw_module->rowCount()-1]->setEnabled(false);
    }
}

void module_management::on_pb_delete_module_clicked()
{
    int delete_count = 0;
    QStringList list_module_id;
    QString msg = "Apakah anda yakin ingin menonaktifkan module dibawah ini ??\n\n";
    for (int i = 0; i < ui->tbw_module->rowCount(); i++) {
        if (check_module[i]->isChecked()) {
            if (!le_module_id[i]->text().isEmpty()) {
                delete_count++;
                list_module_id.append(le_module_id[i]->text());
                msg += QString::number(delete_count) + ". " + le_module_name[i]->text() +
                        " - " + le_module_sn[i]->text();
            }
        }
    }
    if (list_module_id.length() > 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Module", msg, QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QStringList result = db_mysql.read(db, "call delete_module("+QString::number(user_id)+",'"+list_module_id.join(",")+"')",QStringList());
            if (result.length() > 0) {
                if (result.at(0) == "SUCCESS") {
                    QMessageBox::information(this,result.at(0),result.at(1));
                    this->refresh_module();
                } else {
                    QMessageBox::critical(this,result.at(0),result.at(1));
                }
            } else {
                QMessageBox::critical(this,"Error",result.join(','));
            }
        }
    }
}

void module_management::saveButton_module(int index)
{
    QString c_id = list_company.at(cb_module_company[index]->currentIndex()*2);
    QString m_type_id = list_module_type.at(cb_module_type[index]->currentIndex()*2);
    if (!le_module_id[index]->text().isEmpty()) {
        QString m_id = le_module_id[index]->text();
        if (!le_module_name[index]->text().isEmpty() &&
                !le_module_sn[index]->text().isEmpty()) {
            QStringList parameter;
            parameter.append(QString::number(user_id));
            parameter.append(m_id);
            parameter.append(m_type_id);
            parameter.append(le_module_name[index]->text());
            parameter.append(le_module_sn[index]->text());

            QStringList result = db_mysql.read(db, "call update_module(:p1,:p2,:p3,:p4,:p5)",parameter);
            if (result.length() > 0) {
                if (result.at(0) == "SUCCESS") {
                    QMessageBox::information(this,result.at(0),result.at(1));
                } else {
                    QMessageBox::critical(this,result.at(0),result.at(1));
                }
            } else {
                QMessageBox::critical(this,"Error",result.join(','));
            }
        } else {
            QMessageBox::critical(this,"Error","Mohon formulir module dilengkapi");
        }
    } else {
        if (!le_module_name[index]->text().isEmpty() &&
                !le_module_sn[index]->text().isEmpty()) {
            QStringList parameter;
            parameter.append(QString::number(user_id));
            parameter.append(c_id);
            parameter.append(m_type_id);
            parameter.append(le_module_name[index]->text());
            parameter.append(le_module_sn[index]->text());

            QStringList result = db_mysql.read(db, "call create_module(:p1,:p2,:p3,:p4,:p5)",parameter);
            if (result.length() > 0) {
                if (result.at(0) == "SUCCESS") {
                    QMessageBox::information(this,result.at(0),result.at(1));
                    le_module_id[index]->setText(result.at(2));
                    pb_manage_module[index]->setEnabled(true);
                } else {
                    QMessageBox::critical(this,result.at(0),result.at(1));
                }
            } else {
                QMessageBox::critical(this,"Error",result.join(','));
            }
        } else {
            QMessageBox::critical(this,"Error","Mohon formulir module dilengkapi");
        }
    }
}

void module_management::manageButton_module(int index)
{
    this->refresh_module();
    current_module_id = le_module_id[index]->text();
    current_module_name = le_module_name[index]->text();
    current_module_sn = le_module_sn[index]->text();
    current_module_type_id = list_module_type.at(cb_module_type[index]->currentIndex()*2);

    ui->lb_module_name->setText(current_module_name);
    ui->lb_serial_number->setText(current_module_sn);
    ui->lb_module_name_2->setText(current_module_name);
    ui->lb_serial_number_2->setText(current_module_sn);

    ui->tab_module->setHidden(false);
    ui->tab_module->setCurrentIndex(0);
    ui->tBox_protocol->setCurrentIndex(0);
    this->refresh_tag_point();
    this->refresh_protocol();
}

void module_management::on_pb_refresh_module_clicked()
{
    this->refresh_module();
}

void module_management::saveButton_tag_point(int index)
{
    QString tp_type_id = list_point_type.at(cb_tag_point_type[index]->currentIndex()*2);
    if (!le_tag_point_id[index]->text().isEmpty()) {
        QString tp_id = le_tag_point_id[index]->text();
        if (!le_tag_point_name[index]->text().isEmpty()) {
            QStringList parameter;
            parameter.append(QString::number(user_id));
            parameter.append(tp_id);
            parameter.append(le_tag_point_name[index]->text());
            parameter.append(le_tag_point_unit[index]->text());
            parameter.append(tp_type_id);
            parameter.append(current_module_sn);

            QStringList result = db_mysql.read(db, "call update_tag_point(:p1,:p2,:p3,:p4,:p5, :p6)",parameter);
            if (result.length() > 0) {
                if (result.at(0) == "SUCCESS") {
                    QMessageBox::information(this,result.at(0),result.at(1));
                } else {
                    QMessageBox::critical(this,result.at(0),result.at(1));
                }
            } else {
                QMessageBox::critical(this,"Error",result.join(','));
            }
        } else {
            QMessageBox::critical(this,"Error","Mohon formulir tag point dilengkapi");
        }
    } else {
        if (!le_tag_point_name[index]->text().isEmpty()) {
            QStringList parameter;
            parameter.append(QString::number(user_id));
            parameter.append(le_tag_point_name[index]->text());
            parameter.append(le_tag_point_unit[index]->text());
            parameter.append(tp_type_id);
            parameter.append(current_module_sn);

            QStringList result = db_mysql.read(db, "call create_tag_point(:p1,:p2,:p3,:p4,:p5)",parameter);
            if (result.length() > 0) {
                if (result.at(0) == "SUCCESS") {
                    QMessageBox::information(this,result.at(0),result.at(1));
                    le_tag_point_id[index]->setText(result.at(2));
                    lb_tag_point_name[index]->setText(current_module_sn+"//"+le_tag_point_name[index]->text());
                } else {
                    QMessageBox::critical(this,result.at(0),result.at(1));
                }
            } else {
                QMessageBox::critical(this,"Error",result.join(','));
            }
        } else {
            QMessageBox::critical(this,"Error","Mohon formulir tag point dilengkapi");
        }
    }
}

void module_management::on_pb_add_tag_point_clicked()
{
    if (ui->tbw_tag_point->rowCount() < max_tag_point) {
        ui->tbw_tag_point->insertRow(ui->tbw_tag_point->rowCount());
    
        check_tag_point[ui->tbw_tag_point->rowCount()-1] = new QCheckBox(this);
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 0, check_tag_point[ui->tbw_tag_point->rowCount()-1]);
    
        ui->tbw_tag_point->setItem(ui->tbw_tag_point->rowCount()-1, 1, new QTableWidgetItem(QString::number(ui->tbw_tag_point->rowCount())));
    
        le_tag_point_id[ui->tbw_tag_point->rowCount()-1] = new QLineEdit(this);
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 2, le_tag_point_id[ui->tbw_tag_point->rowCount()-1]);
    
        pb_save_tag_point[ui->tbw_tag_point->rowCount()-1] = new QPushButton(this);
        pb_save_tag_point[ui->tbw_tag_point->rowCount()-1]->setText("Save");
        SigMapButton_tag_point->setMapping(pb_save_tag_point[ui->tbw_tag_point->rowCount()-1], ui->tbw_tag_point->rowCount()-1);
        connect(pb_save_tag_point[ui->tbw_tag_point->rowCount()-1], SIGNAL(clicked(bool)),SigMapButton_tag_point, SLOT(map()));
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 3, pb_save_tag_point[ui->tbw_tag_point->rowCount()-1]);
    
        le_tag_point_name[ui->tbw_tag_point->rowCount()-1] = new QLineEdit(this);
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 4, le_tag_point_name[ui->tbw_tag_point->rowCount()-1]);
    
        le_tag_point_unit[ui->tbw_tag_point->rowCount()-1] = new QLineEdit(this);
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 5, le_tag_point_unit[ui->tbw_tag_point->rowCount()-1]);
    
        cb_tag_point_type[ui->tbw_tag_point->rowCount()-1] = new QComboBox(this);
        for (int j = 0; j < list_point_type.length(); j+=2)
            cb_tag_point_type[ui->tbw_tag_point->rowCount()-1]->addItem(list_point_type.at(j+1));
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 6, cb_tag_point_type[ui->tbw_tag_point->rowCount()-1]);
    
        lb_tag_point_name[ui->tbw_tag_point->rowCount()-1] = new QLabel(this);
        ui->tbw_tag_point->setCellWidget(ui->tbw_tag_point->rowCount()-1, 7, lb_tag_point_name[ui->tbw_tag_point->rowCount()-1]);
    }
}

void module_management::on_pb_delete_tag_point_clicked()
{
    int delete_count = 0;
    QStringList list_tag_point_id;
    QString msg = "Apakah anda yakin ingin menghapus tag point dibawah ini ??\n\n";
    for (int i = 0; i < ui->tbw_tag_point->rowCount(); i++) {
        if (check_tag_point[i]->isChecked()) {
            if (!le_tag_point_id[i]->text().isEmpty()) {
                delete_count++;
                list_tag_point_id.append(le_tag_point_id[i]->text());
                msg += QString::number(delete_count) + ". Name : " + le_tag_point_name[i]->text() +
                        " | Unit : " + le_tag_point_unit[i]->text();
            }
        }
    }
    if (list_tag_point_id.length() > 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Tag Point", msg, QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QStringList result = db_mysql.read(db, "call delete_tag_point('"+list_tag_point_id.join(",")+"')",QStringList());
            if (result.length() > 0) {
                if (result.at(0) == "SUCCESS") {
                    QMessageBox::information(this,result.at(0),result.at(1));
                    this->refresh_tag_point();
                } else {
                    QMessageBox::critical(this,result.at(0),result.at(1));
                }
            } else {
                QMessageBox::critical(this,"Error",result.join(','));
            }
        }
    }
}

void module_management::on_pb_refresh_tag_point_clicked()
{
    this->refresh_tag_point();
}

void module_management::refresh_protocol()
{
    QStringList result;

    result = db_mysql.read(db, "call get_protocol('"+current_module_type_id+"')",QStringList());
    list_protocol = result;

    model_protocol = new QStandardItemModel(list_protocol.length()/3,1,this);
    ui->tbv_protocol->setModel(model_protocol);
    model_protocol->setHorizontalHeaderLabels(QString("Communication").split(";"));
    ui->tbv_protocol->setColumnWidth(0, 100);

//    for (int i = 0; i < list_protocol.length()/3; i++) {
//        QModelIndex index_1 = model_protocol->index(i,0);
//        model_protocol->setData(index_1,i+1);
//        QModelIndex index_2 = model_protocol->index(i,1);
//        model_protocol->setData(index_2,list_protocol.at(i));
//        QModelIndex index_3 = model_protocol->index(i,2);
//        model_protocol->setData(index_3,list_protocol.at(i+1));
//    }

    for (int row = 0; row < list_protocol.length(); row+=3) {
        QStandardItem *item_2 = new QStandardItem(list_protocol.at(row+1));
        model_protocol->setItem(row/3,0,item_2);
    }
}

void module_management::on_tbv_protocol_clicked(const QModelIndex &index)
{
//    qDebug() << index.row();
    ui->lb_communication->setText(list_protocol.at((index.row()*3)+2));
    current_protocol_id = list_protocol.at((index.row()*3));
    this->refresh_parsing_ref();
}

void module_management::refresh_parsing_ref()
{
    this->refresh_tag_point();

    QStringList result;

    result = db_mysql.read(db, "call get_parsing_ref('"+current_module_sn+"','"+current_protocol_id+"')",QStringList());
    list_parsing_ref = result;

    ui->tbw_parsing_ref->verticalHeader()->setHidden(true);
    ui->tbw_parsing_ref->setColumnCount(4);

    QTableWidgetItem *header_name[4];

    header_name[0] = new QTableWidgetItem();
    header_name[0]->setText("");
    ui->tbw_parsing_ref->setHorizontalHeaderItem(0,header_name[0]);
    ui->tbw_parsing_ref->setColumnWidth(0, 25);

    header_name[1] = new QTableWidgetItem();
    header_name[1]->setText("");
    ui->tbw_parsing_ref->setHorizontalHeaderItem(1,header_name[1]);
    ui->tbw_parsing_ref->setColumnWidth(1, 50);

    header_name[2] = new QTableWidgetItem();
    header_name[2]->setText("Order Number");
    ui->tbw_parsing_ref->setHorizontalHeaderItem(2,header_name[2]);
    ui->tbw_parsing_ref->setColumnWidth(2, 100);

    header_name[3] = new QTableWidgetItem();
    header_name[3]->setText("Tag Point");
    ui->tbw_parsing_ref->setHorizontalHeaderItem(3,header_name[3]);
    ui->tbw_parsing_ref->setColumnWidth(3, 200);

    this->ui->tbw_parsing_ref->setRowCount(0);
    for (int i = 0; i < list_parsing_ref.length(); i+=3) {
        ui->tbw_parsing_ref->insertRow(ui->tbw_parsing_ref->rowCount());

        check_parsing_ref[ui->tbw_parsing_ref->rowCount()-1] = new QCheckBox(this);
        ui->tbw_parsing_ref->setCellWidget(ui->tbw_parsing_ref->rowCount()-1, 0, check_parsing_ref[ui->tbw_parsing_ref->rowCount()-1]);

        pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1] = new QPushButton(this);
        pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1]->setText("Save");
        SigMapButton_parsing_ref->setMapping(pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1], ui->tbw_parsing_ref->rowCount()-1);
        connect(pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1], SIGNAL(clicked(bool)),SigMapButton_parsing_ref, SLOT(map()));
        ui->tbw_parsing_ref->setCellWidget(ui->tbw_parsing_ref->rowCount()-1, 1, pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1]);

        sb_order_number[ui->tbw_parsing_ref->rowCount()-1] = new QSpinBox(this);
        sb_order_number[ui->tbw_parsing_ref->rowCount()-1]->setMinimum(1);
        sb_order_number[ui->tbw_parsing_ref->rowCount()-1]->setMaximum(100);
        sb_order_number[ui->tbw_parsing_ref->rowCount()-1]->setValue(list_parsing_ref.at(i).toInt());
        ui->tbw_parsing_ref->setCellWidget(ui->tbw_parsing_ref->rowCount()-1, 2, sb_order_number[ui->tbw_parsing_ref->rowCount()-1]);

        cb_tag_point[ui->tbw_parsing_ref->rowCount()-1] = new QComboBox(this);
        for (int j = 0; j < list_tag_point.length(); j+=5) {
            cb_tag_point[ui->tbw_parsing_ref->rowCount()-1]->addItem(list_tag_point.at(j+4));
            if (list_parsing_ref.at(i+1) == list_tag_point.at(j))
                cb_tag_point[ui->tbw_parsing_ref->rowCount()-1]->setCurrentIndex(j/5);
        }
        ui->tbw_parsing_ref->setCellWidget(ui->tbw_parsing_ref->rowCount()-1, 3, cb_tag_point[ui->tbw_parsing_ref->rowCount()-1]);
    }
}

void module_management::on_pb_add_parsing_ref_clicked()
{
    if (ui->tbw_parsing_ref->rowCount() < max_parsing_ref) {
        ui->tbw_parsing_ref->insertRow(ui->tbw_parsing_ref->rowCount());

        check_parsing_ref[ui->tbw_parsing_ref->rowCount()-1] = new QCheckBox(this);
        ui->tbw_parsing_ref->setCellWidget(ui->tbw_parsing_ref->rowCount()-1, 0, check_parsing_ref[ui->tbw_parsing_ref->rowCount()-1]);

        pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1] = new QPushButton(this);
        pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1]->setText("Save");
        SigMapButton_parsing_ref->setMapping(pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1], ui->tbw_parsing_ref->rowCount()-1);
        connect(pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1], SIGNAL(clicked(bool)),SigMapButton_parsing_ref, SLOT(map()));
        ui->tbw_parsing_ref->setCellWidget(ui->tbw_parsing_ref->rowCount()-1, 1, pb_save_parsing_ref[ui->tbw_parsing_ref->rowCount()-1]);

        sb_order_number[ui->tbw_parsing_ref->rowCount()-1] = new QSpinBox(this);
        sb_order_number[ui->tbw_parsing_ref->rowCount()-1]->setMinimum(1);
        sb_order_number[ui->tbw_parsing_ref->rowCount()-1]->setMaximum(100);
        ui->tbw_parsing_ref->setCellWidget(ui->tbw_parsing_ref->rowCount()-1, 2, sb_order_number[ui->tbw_parsing_ref->rowCount()-1]);

        cb_tag_point[ui->tbw_parsing_ref->rowCount()-1] = new QComboBox(this);
        for (int j = 0; j < list_tag_point.length(); j+=5) {
            cb_tag_point[ui->tbw_parsing_ref->rowCount()-1]->addItem(list_tag_point.at(j+4));
        }
        ui->tbw_parsing_ref->setCellWidget(ui->tbw_parsing_ref->rowCount()-1, 3, cb_tag_point[ui->tbw_parsing_ref->rowCount()-1]);
    }
}

void module_management::on_pb_delete_parsing_ref_clicked()
{
    int delete_count = 0;
    QStringList list_order_number;
    QStringList list_tag_point_id;
    QString msg = "Apakah anda yakin ingin menghapus parsing reference dibawah ini ??\n\n";
    for (int i = 0; i < ui->tbw_parsing_ref->rowCount(); i++) {
        if (check_parsing_ref[i]->isChecked()) {
            delete_count++;
            list_order_number.append(QString::number(sb_order_number[i]->value()));
            list_tag_point_id.append(list_tag_point.at(cb_tag_point[i]->currentIndex()*5));
            msg += QString::number(delete_count) +
                    ". Order Number : " + QString::number(sb_order_number[i]->value()) +
                    " | Tag point : " + cb_tag_point[i]->currentText();
        }
    }
    if (list_order_number.length() > 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Parsing Reference", msg, QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            QStringList result = db_mysql.read(db, "call delete_parsing_ref('"+list_order_number.join(",")+
                                               "','"+list_tag_point_id.join(",")+
                                               "','"+current_protocol_id+"')",QStringList());
            if (result.length() > 0) {
                if (result.at(0) == "SUCCESS") {
                    QMessageBox::information(this,result.at(0),result.at(1));
                    this->refresh_parsing_ref();
                } else {
                    QMessageBox::critical(this,result.at(0),result.at(1));
                }
            } else {
                QMessageBox::critical(this,"Error",result.join(','));
            }
        }
    }
}

void module_management::on_pb_refresh_parsing_ref_clicked()
{
    this->refresh_parsing_ref();
}

void module_management::saveButton_parsing_ref(int index)
{
    QStringList parameter;
    parameter.append(QString::number(sb_order_number[index]->value()));
    parameter.append(list_tag_point.at(cb_tag_point[index]->currentIndex()*5));
    parameter.append(current_protocol_id);

    QStringList result = db_mysql.read(db, "call create_parsing_ref(:p1,:p2,:p3)",parameter);
    if (result.length() > 0) {
        if (result.at(0) == "SUCCESS") {
            QMessageBox::information(this,result.at(0),result.at(1));
        } else {
            QMessageBox::critical(this,result.at(0),result.at(1));
        }
    } else {
        QMessageBox::critical(this,"Error",result.join(','));
    }
}
