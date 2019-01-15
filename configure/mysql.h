#ifndef MYSQL_H
#define MYSQL_H

#include "configure/common.h"
#include "configure/config.h"

class mysql
{
public:
    mysql();

    config cfg;

    QString host;
    int     port;
    QString db_name;
    QString username;
    QString password;
    QSqlDatabase dbase;

    QSqlDatabase connect_db(QString name);
    void close(QSqlDatabase db);

    QStringList read(QSqlDatabase db, QString query, QStringList parameter);
    void write(QSqlDatabase db, QString query, QStringList parameter);
    void tf_conn(QSqlDatabase *db1, QSqlDatabase *db2);
};

#endif // MYSQL_H
