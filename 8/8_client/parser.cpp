#include "parser.h"
QStringList list1, spis;
Parser::Parser () {

}

void Parser::slotGetSystemInfo ()
{
    QString info;
    QFile file;
    file.setFileName("upr.json");
    qDebug() << file;
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    info = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(info.toUtf8());
    QJsonObject json = doc.object();
    QJsonValue jv = json.value("upr");
    QJsonObject obj;
    if (jv.isArray()){
        QJsonArray ja = jv.toArray();
        for (int i = 0; i < ja.count(); i++){
            QJsonObject sub = ja.at(i).toObject();
            QString save = sub.value("title").toString() + ";" + sub.value("name").toString() + ";" + sub.value("ball1").toString() + ";" + sub.value("ochenka1").toString() + ";" + sub.value("ball2").toString() + ";" + sub.value("ochenka2").toString() + ";" + sub.value("ball3").toString() + ";" + sub.value("ochenka3").toString() + ";" + sub.value("ball4").toString() + ";" + sub.value("ochenka4").toString();
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
    QString info1;
    QFile file1;
    file1.setFileName("spisok.json");
    file1.open(QIODevice::ReadOnly | QIODevice::Text);
    info1 = file1.readAll();
    file1.close();
    QJsonDocument doc1 = QJsonDocument::fromJson(info1.toUtf8());
    QJsonObject json1 = doc1.object();
    QJsonValue jv1 = json1.value("spisok");
    QJsonObject obj1;
    if (jv1.isArray()){
        QJsonArray ja = jv1.toArray();
        for (int i = 0; i < ja.count(); i++){
            QJsonObject sub1 = ja.at(i).toObject();
            QString save = sub1.value("fio").toString() + ";" + sub1.value("group").toString() + ";" + sub1.value("ocenka1").toString() + ";" + sub1.value("upr1").toString() + ";" +sub1.value("ocenka2").toString() + ";" +sub1.value("upr2").toString() + ";" +sub1.value("ocenka3").toString() + ";" +sub1.value("upr3").toString();
            spis << save;
        }
    }
    obj1["Spisok"] = QString::number(spis.size());

    for (int i = 0; i < spis.length();i++)
    {
        obj1[QString::number(i)] = spis.at(i);
    }
    qDebug() << obj1;
    spis.clear();
    emit getResult (obj1);
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
    json["fio"] = lis.at(0);
    json["group"] = lis.at(1);
    json["ocenka1"] = lis.at(2);
    json["upr1"] = lis.at(3);
    json["ocenka2"] = lis.at(4);
    json["upr2"] = lis.at(5);
    json["ocenka3"] = lis.at(6);
    json["upr3"] = lis.at(7);
    auto arr = doc["spisok"].toArray();
    arr.append(json);
    doc["spisok"] = arr;
    qDebug() << doc;
    file.seek(0);
    file.write(QJsonDocument(doc).toJson());
    file.close();
    slotGetSystemInfo();
}

void Parser::delet()
{
    QString de = dd;
    QStringList list = de.split(" ");
    QString info;
    QFile file;
    file.setFileName("spisok.json");
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
        if ((sub["fio"] == list.at(0)) && (sub["group"] == list.at(1))){
            sub["fio"] = "";
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
