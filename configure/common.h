#ifndef COMMON_H
#define COMMON_H

#include <QDebug>

#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDir>
#include <QFile>
#include <QSettings>
#include <QDateTime>

#include <QStandardItemModel>

#define PATH_MONITA             ".Mon5AdminConfig/"
#define PATH_CONFIGURATION_JSON ".Mon5AdminConfig/monita5-administrator-cfg.json"

struct monitaTree {
    int id;
    QStandardItem *item_name;
    QString type;
    QStringList child;
};

#endif // COMMON_H
