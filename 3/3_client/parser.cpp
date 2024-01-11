#include "parser.h"
QStringList list1;
Parser::Parser () {

}

void Parser::slotGetSystemInfo ()
{
    QString info;
    QFile file;
    file.setFileName("1.json");
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
            QString save = sub.value("fio").toString() + "%;%" + sub.value("group").toString() + "%;%" + sub.value("fp").toString() + "%;%" + sub.value("zoc").toString() + "%;%" + sub.value("tes").toString() + "%;%" + sub.value("mp").toString() + "%;%" + sub.value("os").toString() + "%;%" + sub.value("ball").toString();
            list1 << save;
        }
    }
    obj["All"] = QString::number(list1.size());
    for (int i = 0; i < list1.length();i++)
    {
        obj[QString::number(i)] = list1.at(i);
    }
    list1.clear();
    qDebug() << obj;
    emit getResult (obj);
}

void Parser::upd()
{
    QString newtx = oche;
    QStringList lis = newtx.split("%;%");
    QString info;
    QFile file;
    file.setFileName("1.json");
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
    json["fio"] = lis.at(0);
    json["group"] = lis.at(1);
    json["fp"] = lis.at(2);
    json["zoc"] = lis.at(3);
    json["tes"] = lis.at(4);
    json["mp"] = lis.at(5);
    json["os"] = lis.at(6);
    json["ball"] = lis.at(7);
    auto arr = doc["spisok"].toArray();
    arr.append(json);
    doc["spisok"] = arr;
    qDebug() << doc;
    file.seek(0);
    file.write(QJsonDocument(doc).toJson());
    file.close();
    slotGetSystemInfo();
}
