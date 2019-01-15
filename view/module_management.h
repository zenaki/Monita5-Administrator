#ifndef MODULE_MANAGEMENT_H
#define MODULE_MANAGEMENT_H

#include <QWidget>
#include <QMessageBox>

#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QSignalMapper>

#include "configure/mysql.h"

namespace Ui {
class module_management;
}

class module_management : public QWidget
{
    Q_OBJECT

public:
    explicit module_management(QWidget *parent = 0, int id = 0, int c_id = 0, QString c_name = "");
    ~module_management();

    int user_id;
    int company_id;
    QString company_name;
    QString current_module_id;
    QString current_module_name;
    QString current_module_sn;
    QString current_module_type_id;
    QString current_protocol_id;

    QStandardItemModel *model_protocol;

    QStringList list_module;
    QStringList list_module_type;
    QStringList list_company;
    QStringList list_protocol;
    QStringList list_parsing_ref;

    QString current_sn;

    QStringList list_tag_point;
    QStringList list_point_type;

    int deleted_row;

    mysql db_mysql;
    QSqlDatabase db;

    void refresh_module();
    void refresh_tag_point();
    void refresh_protocol();
    void refresh_parsing_ref();

private slots:
    void on_pb_add_module_clicked();
    void on_pb_delete_module_clicked();
    void saveButton_module(int index);
    void on_pb_refresh_module_clicked();
    void manageButton_module(int index);
    void saveButton_tag_point(int index);
    void on_pb_add_tag_point_clicked();
    void on_pb_delete_tag_point_clicked();
    void on_pb_refresh_tag_point_clicked();

    void on_tbv_protocol_clicked(const QModelIndex &index);
    void on_pb_add_parsing_ref_clicked();
    void on_pb_delete_parsing_ref_clicked();
    void on_pb_refresh_parsing_ref_clicked();
    void saveButton_parsing_ref(int index);

private:
    Ui::module_management *ui;

    int max_module = 20;
    /** Module Management **/
    QCheckBox       *check_module[20];
    QSignalMapper   *SigMapCheck_module;
    QLineEdit       *le_module_id[20];
    QPushButton     *pb_save_module[20];
    QSignalMapper   *SigMapButton_save_module;
    QLineEdit       *le_module_name[20];
    QLineEdit       *le_module_sn[20];
    QComboBox       *cb_module_type[20];
    QComboBox       *cb_module_company[20];
    QPushButton     *pb_manage_module[20];
    QSignalMapper   *SigMapButton_manage_module;

    int max_tag_point = 100;
    /** Tag Point Management **/
    QCheckBox       *check_tag_point[100];
    QSignalMapper   *SigMapCheck_tag_module;
    QLineEdit       *le_tag_point_id[100];
    QPushButton     *pb_save_tag_point[100];
    QSignalMapper   *SigMapButton_tag_point;
    QLineEdit       *le_tag_point_name[100];
    QLineEdit       *le_tag_point_unit[100];
    QComboBox       *cb_tag_point_type[100];
    QLabel          *lb_tag_point_name[100];

    int max_parsing_ref = 100;
    /** Parsing Reference **/
    QCheckBox       *check_parsing_ref[100];
    QPushButton     *pb_save_parsing_ref[100];
    QSignalMapper   *SigMapButton_parsing_ref;
    QSpinBox        *sb_order_number[100];
    QComboBox       *cb_tag_point[100];

};

#endif // MODULE_MANAGEMENT_H
