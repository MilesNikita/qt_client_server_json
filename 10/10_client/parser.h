#ifndef PARSER_H
#define PARSER_H
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QJsonDocument>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QObject>
#include <QThread>
#include <QString>
#include <QDirIterator>
#include <agent.h>

class Parser : public QObject
{
    Q_OBJECT

public:
    Parser ();
    QString str;
    QString oche;

public slots:
    void slotGetSystemInfo();
    void upd();

signals:
    void getResult (QJsonObject);

};

#endif // PARSER_H
