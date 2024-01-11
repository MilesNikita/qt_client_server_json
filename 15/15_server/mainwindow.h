#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QAxObject>
#include <QtAlgorithms>
#include <QVector>
#include "server.h"
#include <QRegExpValidator>
#include <QtAlgorithms>
#include <QDateTime>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void opred(int k);
    Server *server;
    QJsonObject StringToJson (QString a);
    QString JsonToString (QJsonObject *a);
    QJsonObject check (QJsonObject );
    void slotSendRequest (bool);
    void slotOperationFinish (bool, QPair<QString, int>);
    void slotProcess (QPair<QString, int>, QJsonObject);
    QPair<QString, int> curAg;

private slots:
    void appendline (QJsonObject &, QPair<QString, int> &);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

signals:
    void makeConnection (QPair<QString, int>);
    void sendMessage (QString, QPair<QString, int>);

private:
    Ui::MainWindow *ui;
    QAxObject *excel;
    QAxObject *workbooks;
    QAxObject *workbook;
    QAxObject *sheets;
    QAxObject *good;
};
#endif // MAINWINDOW_H
