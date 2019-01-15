#include "configure/config.h"

config::config()
{
}

QStringList config::read(QString obj)
{
    QStringList result;
    if (loadConfig(config::Json)) {
        JsonDoc = QJsonDocument::fromJson(data_json);
        QJsonObject object = JsonDoc.object();
        QJsonValue value = object.value(obj);
        if (obj == "MYSQL") {
            if (object.value(obj).isArray()) {
                QJsonArray array = value.toArray();
                foreach (const QJsonValue & v, array) {
                    result.append(v.toObject().value("HOST").toString());
                    result.append(QString::number(v.toObject().value("PORT").toInt()));
                    result.append(v.toObject().value("USERNAME").toString());
                    result.append(v.toObject().value("PASSWORD").toString());
                    result.append(v.toObject().value("DATABASE").toString());
                }
            } else {
                result.append(value.toObject().value("HOST").toString());
                result.append(QString::number(value.toObject().value("PORT").toInt()));
                result.append(value.toObject().value("USERNAME").toString());
                result.append(value.toObject().value("PASSWORD").toString());
                result.append(value.toObject().value("DATABASE").toString());
            }
        }
    }
    return result;
}

void config::write(QJsonObject &json) const //Default
{
//    QJsonArray mysqlArray;
    QJsonObject mysqlObject;
    mysqlObject["HOST"] = "localhost";
    mysqlObject["PORT"] = 3306;
    mysqlObject["USERNAME"] = "root";
    mysqlObject["PASSWORD"] = "root";
    mysqlObject["DATABASE"] = "monita5";
//    mysqlArray.append(mysqlObject);
    json["MYSQL"] = mysqlObject;
}

bool config::loadConfig(config::SaveFormat saveFormat)
{
    QString path;
    if (saveFormat == Json) {
        path = PATH_CONFIGURATION_JSON;
    }
    QFile loadFile(path);
    if (!loadFile.exists()) {
        QDir dir;
        dir.mkpath(PATH_MONITA);
    }
    if (loadFile.open(QIODevice::ReadWrite)) {
        QByteArray readFile = loadFile.readAll();
        if (readFile.isEmpty()) {
            QJsonObject configObject;
            write(configObject);
            QJsonDocument saveDoc(configObject);
            loadFile.write(saveFormat == Json
                ? saveDoc.toJson()
                : saveDoc.toBinaryData());
            loadFile.close();
            loadConfig(config::Json);
        } else {
            data_json = readFile;
        }
        return true;
    } else {
        return false;
    }

//    read(loadDoc.object());
}

bool config::saveConfig(config::SaveFormat saveFormat) const
{
    QString path;
    if (saveFormat == Json) {
        path = PATH_CONFIGURATION_JSON;
    }
    QFile saveFile(path);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Couldn't save config ..";
        return false;
    }

    QJsonObject configObject;
    write(configObject);
    QJsonDocument saveDoc(configObject);
    saveFile.write(saveFormat == Json
        ? saveDoc.toJson()
        : saveDoc.toBinaryData());

    return true;
}
