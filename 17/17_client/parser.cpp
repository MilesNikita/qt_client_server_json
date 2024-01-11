#include "parser.h"
QStringList list1;
Parser::Parser () {

}

void Parser::slotGetSystemInfo ()
{
    QString info;
    QFile file;
    file.setFileName("spisok.json");
    qDebug() << file;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    info = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(info.toUtf8());
    QJsonObject json = doc.object();
    QJsonValue jv = json.value("spisok");
    QJsonObject obj;
    if (jv.isArray()){
        QJsonArray ja = jv.toArray();
        for (int i = 0; i < ja.count(); i++){
            QJsonObject sub = ja.at(i).toObject();
            QString save = sub.value("date").toString() + ";" + sub.value("tem").toString() + ";" + sub.value("atd").toString() + ";" + sub.value("veter").toString() + ";" + sub.value("radi").toString();
            list1 << save;
        }
    }
    obj["All"] = QString::number(list1.size());
    for (int i = 0; i < list1.length();i++)
    {
        obj[QString::number(i)] = list1.at(i);
    }
    qDebug() << obj;
    list1.clear();
    emit getResult (obj);
}

void Parser::upd()
{
    QString newtx = oche;
    QStringList lis = newtx.split(";");
    QString info;
    QFile file;
    file.setFileName("spisok.json");
    qDebug() << file;
    file.open(QIODevice::ReadWrite);
    info = file.readAll();
    QJsonObject doc;
    try {
        doc = QJsonDocument::fromJson(info.toUtf8()).object();
    }  catch (...) {
        qDebug() << "error";
    }
    QJsonObject json;
    if (!doc.contains("spisok"))
        return;
    QJsonValue jv = doc["spisok"];
    if (!jv.isArray())
    {
        return;
    }
    json["date"] = lis.at(0);
    json["tem"] = lis.at(1);
    json["atd"] = lis.at(2);
    json["veter"] = lis.at(3);
    json["radi"] = lis.at(4);
    auto arr = doc["spisok"].toArray();
    arr.append(json);
    doc["spisok"] = arr;
    qDebug() << doc;
    file.seek(0);
    file.write(QJsonDocument(doc).toJson());
    file.close();
    slotGetSystemInfo();
}
