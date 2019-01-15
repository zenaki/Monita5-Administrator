#include "configure/mysql.h"

mysql::mysql()
{

}

QSqlDatabase mysql::connect_db(QString name)
{
    QStringList temp = cfg.read("MYSQL");
    host = temp.at(0);
    port = temp.at(1).toInt();
    username = temp.at(2);
    password = temp.at(3);
    db_name = temp.at(4);

//    dbase = new QSqlDatabase();
    dbase = QSqlDatabase::addDatabase("QMYSQL", name);
    dbase.setHostName(host);
    dbase.setPort(port);
    dbase.setUserName(username);
    dbase.setPassword(password);
    dbase.setDatabaseName(db_name);

    return (QSqlDatabase ) dbase;
//    if (dbase->isValid()) {
//        return dbase;
//    } else {
//        qDebug() << "";
//    }
}

void mysql::close(QSqlDatabase db)
{
    QString connection;
    connection = db.connectionName();
    db.close();
//    db = QSqlDatabase();
    db.removeDatabase(connection);
}

QStringList mysql::read(QSqlDatabase db, QString query, QStringList parameter)
{
    QStringList result;
    if (db.isValid()) {
        if (!db.isOpen()) {
            db.open();
        }
        QSqlQuery q(QSqlDatabase::database(db.connectionName()));
//        q.prepare(query);
        for (int i = 0; i < parameter.length(); i++) {
            if (query.indexOf(":p"+QString::number(i+1)) > 0) {
                query.replace(":p"+QString::number(i+1), "'"+parameter.at(i)+"'");
            }
        }
//        qDebug() << query;
        if (q.exec(query)) {
            while(q.next()) {
                for (int i = 0; i < q.record().count(); i++) {
                    result.append(q.value(i).toString());
                }
            }
        } else {
            qDebug() << "Cannot read from database with query: \n" << query;
//            qDebug() << "Cannot read from database";
        }
        db.close();
    }
    return result;
}

void mysql:: write(QSqlDatabase db, QString query, QStringList parameter)
{
    if (db.isValid()) {
        if (!db.isOpen()) {
            db.open();
        }
        QSqlQuery q(QSqlDatabase::database(db.connectionName()));
        for (int i = 0; i < parameter.length(); i++) {
            if (query.indexOf(":p"+QString::number(i+1)) > 0) {
                query.replace(":p"+QString::number(i+1), "'"+parameter.at(i)+"'");
            }
        }
//        qDebug() << query;
//        q.prepare(query);
        if (q.exec(query)) {
            qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") + "::Success write to database";
//            qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") + "::Success write to database with query: \n" << query;
//            while(q.next()) {
//                 result.append(q.value(0).toString());
//            }
        } else {
            qDebug() << QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss:zzz") + "::Cannot write to database with query: \n" << query;
//            qDebug() << "Cannot write to database";
        }
        db.close();
    }
}

void mysql::tf_conn(QSqlDatabase *db1, QSqlDatabase *db2)
{
    db2->setHostName(db1->hostName());
    db2->setPort(db1->port());
    db2->setUserName(db1->userName());
    db2->setPassword(db1->password());
    db2->setDatabaseName(db1->databaseName());
}
