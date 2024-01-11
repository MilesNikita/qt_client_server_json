#include "parser.h"
QStringList list1, list2;
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
            QString save = sub.value("name").toString() + ";" + sub.value("chislo").toString();
            list1 << save;
        }
    }
    obj["All"] = QString::number(list1.size());
    for (int i = 0; i < list1.length();i++)
    {
        obj[QString::number(i)] = list1.at(i);
    }
    list1.clear();
    QString info1;
    QFile file1;
    file1.setFileName("rashod.json");
    file1.open(QIODevice::ReadOnly | QIODevice::Text);
    info1 = file1.readAll();
    file1.close();
    QJsonDocument doc1 = QJsonDocument::fromJson(info1.toUtf8());
    QJsonObject json1 = doc1.object();
    QJsonValue jv1 = json1.value("spisok");
    if (jv1.isArray()){
        QJsonArray ja1 = jv1.toArray();
        for (int i = 0; i < ja1.count(); i++){
            QJsonObject sub = ja1.at(i).toObject();
            QString save = sub.value("name").toString() + ";" + sub.value("date").toString() + ";" + sub.value("chislo").toString() + ";" + sub.value("new").toString() + ";" + sub.value("otset").toString();
            list2 << save;
        }
    }
    obj["All_r"] = QString::number(list2.size());
    for (int i = 0; i < list2.length();i++)
    {
        obj["spis" + QString::number(i)] = list2.at(i);
    }
    list2.clear();
    qDebug() << obj;
    emit getResult (obj);
}

void Parser::upd()
{
    QString newtx = oche;
    QStringList lis = newtx.split(";");
    QString info;
    QFile file;
    file.setFileName("rashod.json");
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
    json["name"] = lis.at(0);
    json["date"] = lis.at(1);
    json["chislo"] = lis.at(2);
    json["new"] = lis.at(3);
    auto arr = doc["spisok"].toArray();
    arr.append(json);
    doc["spisok"] = arr;
    qDebug() << doc;
    file.seek(0);
    file.write(QJsonDocument(doc).toJson());
    file.close();
    slotGetSystemInfo();
}

void Parser::oste()
{
     QString newtx = bb;
     QStringList list = bb.split(";");
     QString info;
     QFile file;
     file.setFileName("rashod.json");
     file.open(QIODevice::ReadWrite);
     info = file.readAll();
     QByteArray data = file.readAll();
     QJsonParseError error;
     QJsonDocument docs = QJsonDocument::fromJson(data, &error);
     QJsonObject doc;
     file.close();
     try {
         doc = QJsonDocument::fromJson(info.toUtf8()).object();
     }  catch (...) {
         qDebug() << "error";
     }
     if (!doc.contains("spisok"))
         return;
     QJsonValue jv = doc["spisok"];
     if (!jv.isArray())
     {
         return;
     }
     QJsonObject js;
     js.insert("spisok", QJsonArray{});
     QJsonArray ja1 = jv.toArray();
     for (int i = 0; i < ja1.count(); i++){
         QJsonObject sub = ja1.at(i).toObject();
         if ((sub["name"] == list.at(0)) && (sub["date"] == list.at(1))){
                sub["otset"] = list.at(2);

         }
         QJsonArray arr = js["spisok"].toArray();
         arr.append(sub);
         js["spisok"] = arr;
         docs = QJsonDocument(js);
         file.open(QIODevice::WriteOnly);
         file.write(docs.toJson());
         file.close();
     }
     slotGetSystemInfo();
}
