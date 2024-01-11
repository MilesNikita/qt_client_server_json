#include "mainwindow.h"
#include "ui_mainwindow.h"

double obk = 0;
double kol_ot = 0, kol_hor = 0, kol_tr = 0, kol_dv = 0;
int lolo = 1;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new Server();
    int k = 0;
    connect (server, &Server::operationFinished, this, &MainWindow::slotOperationFinish);
    connect (this, &MainWindow::sendMessage, server, &Server::sendToClient);
    connect (server, &Server::recievedMessage, this, &MainWindow::slotProcess);
    connect (this, &MainWindow::makeConnection, server, &Server::slotMakeConnection);
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->lineEdit_18->setValidator(ipValidator);
    ui->lineEdit_17->setValidator(new QIntValidator (1000, 99999, this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSendRequest (bool result)
{
    if (result) {
        QMessageBox::about (this, "", "Запрос отправлен агенту.");
    } else {
        QMessageBox::about (this, "", "Не удалось отправить запрос агенту.");
    }
}

void MainWindow::slotOperationFinish (bool res, QPair<QString, int>)
{
    if (!res) {
        QMessageBox::about (this, "", "Операция не выполнена.");
    }
}

void MainWindow::slotProcess (QPair<QString, int> agent, QJsonObject reply)
{
    qDebug() << JsonToString(&reply);
    if (reply.contains ("All")) {
        appendline(reply, agent);
    }
}

void MainWindow::appendline (QJsonObject &obj, QPair<QString, int> &agent)
{
    int a = obj["All"].toString().toInt();
    obk = a;
    qDebug() << a;
    for (int i = 0; i < a; i++){
        QStringList lis = obj.value(QString::number(i)).toString().split("%;%");
        if (lis.at(7) == "Отличник")
            kol_ot++;
        if (lis.at(7) == "Хорошист")
            kol_hor++;
        if (lis.at(7) == "Троечник")
            kol_tr++;
        if (lis.at(7) == "Двоичник")
            kol_dv++;
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(lis.at(4)));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(lis.at(5)));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(lis.at(6)));
        ui->tableWidget->setItem(i, 7, new QTableWidgetItem(lis.at(7)));
        ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(lis.at(4)));
        ui->tableWidget_2->setItem(i, 5, new QTableWidgetItem(lis.at(5)));
        ui->tableWidget_2->setItem(i, 6, new QTableWidgetItem(lis.at(6)));
        ui->tableWidget_2->setItem(i, 7, new QTableWidgetItem(lis.at(7)));
        ui->tableWidget_3->insertRow(ui->tableWidget_3->rowCount());
        ui->tableWidget_3->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget_3->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget_3->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget_3->setItem(i, 3, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget_3->setItem(i, 4, new QTableWidgetItem(lis.at(4)));
        ui->tableWidget_3->setItem(i, 5, new QTableWidgetItem(lis.at(5)));
        ui->tableWidget_3->setItem(i, 6, new QTableWidgetItem(lis.at(6)));
        ui->tableWidget_3->setItem(i, 7, new QTableWidgetItem(lis.at(7)));
        }
        ui->lineEdit_4->setText(QString::number(kol_ot));
        ui->lineEdit_5->setText(QString::number(kol_hor));
        ui->lineEdit_6->setText(QString::number(kol_tr));
        ui->lineEdit_7->setText(QString::number(kol_dv));
        ui->lineEdit_8->setText(QString::number((double(kol_ot/a))));
        ui->lineEdit_9->setText(QString::number((double(kol_hor/a))));
        ui->lineEdit_10->setText(QString::number((double(kol_tr/a))));
        ui->lineEdit_11->setText(QString::number((double(kol_dv/a))));
}

QString MainWindow::JsonToString (QJsonObject *a)
{
    return QString (QJsonDocument (*a).toJson (QJsonDocument::Compact));
}

QJsonObject MainWindow::StringToJson (QString a)
{
    return QJsonDocument::fromJson (a.toUtf8()).object();
}

void MainWindow::opr(int k)
{
    if (k == 3){
        ui->tab_2->setEnabled(false);
    }
    if (k == 2){
        ui->tab->setEnabled(false);
    }
    if (k == 1){
        ui->tab->setEnabled(false);
        ui->tab_2->setEnabled(false);
    }
}

QString MainWindow::sr_bal(int oc1, int oc2, int oc3, int oc4, int oc5)
{
    QVector <int> vec;
    vec.append(oc1);
    vec.append(oc2);
    vec.append(oc3);
    vec.append(oc4);
    vec.append(oc5);
    if ((vec.at(0) == 2) || (vec.at(1) == 2) || (vec.at(2) == 2) || (vec.at(3) == 2) || (vec.at(4) == 2))
    {
        return ("Двоичник");
    }
    else if ((vec.at(0) == 3) || (vec.at(1) == 3) || (vec.at(2) == 3) || (vec.at(3) == 3) || (vec.at(4) == 3)){
        return ("Троечник");
    }
    else if ((vec.at(0) == 4) || (vec.at(1) == 4) || (vec.at(2) == 4) || (vec.at(3) == 4) || (vec.at(4) == 4)){
        return ("Хорошист");
    }
    else{
        return ("Отличник");
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    QString fio = ui->lineEdit->text();
    QString gr = ui->lineEdit_15->text();
    QString fp = ui->lineEdit_2->text();
    QString zoc = ui->lineEdit_3->text();
    QString tes = ui->lineEdit_12->text();
    QString mp = ui->lineEdit_13->text();
    QString os = ui->lineEdit_14->text();
    QJsonObject obj;
    obj["update"] = fio + "%;%" + gr + "%;%" + fp + "%;%" + zoc + "%;%" + tes + "%;%" + mp + "%;%" + os + "%;%" + sr_bal(fp.toInt(), zoc.toInt(), tes.toInt(), mp.toInt(), os.toInt());
    emit sendMessage (JsonToString (&obj), curAg);
    ui->lineEdit->clear();
    ui->lineEdit_15->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_12->clear();
    ui->lineEdit_13->clear();
    ui->lineEdit_14->clear();

}

void MainWindow::on_pushButton_3_clicked()
{
   excel = new QAxObject ("Excel.Application");
   excel->setProperty ("Visible", true);
   excel->setProperty ("DisplayAlerts", "0");
   workbooks = excel->querySubObject ("Workbooks");
   workbook = workbooks->querySubObject ("Add()");
   sheets = workbook->querySubObject ("Sheets");
   good = sheets->querySubObject ("Item(int)", 1);
   good->setProperty ("Name", "Сохранение");
   good->querySubObject ("Cells(QVariant,QVariant)", lolo, 1)->setProperty("Value", "ФИО");
   good->querySubObject ("Cells(QVariant,QVariant)", lolo, 2)->setProperty("Value", "Итоговая оценка");
   good->querySubObject ("Cells(QVariant,QVariant)", 1, 3)->setProperty("Value", "Количество отличников");
   good->querySubObject ("Cells(QVariant,QVariant)", 1, 4)->setProperty("Value", "Количество хорошистов");
   good->querySubObject ("Cells(QVariant,QVariant)", 1, 5)->setProperty("Value", "Количество троичников");
   good->querySubObject ("Cells(QVariant,QVariant)", 1, 6)->setProperty("Value", "Количество двоичников");
   good->querySubObject ("Cells(QVariant,QVariant)", 1, 7)->setProperty("Value", "Процент отличников");
   good->querySubObject ("Cells(QVariant,QVariant)", 1, 8)->setProperty("Value", "Процент хорошистов");
   good->querySubObject ("Cells(QVariant,QVariant)", 1, 9)->setProperty("Value", "Процент троичников");
   good->querySubObject ("Cells(QVariant,QVariant)", 1, 10)->setProperty("Value", "Процент двоичников");
   for (int i = 0; i < ui->tableWidget->rowCount(); i++){
           good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 1)->setProperty("Value", QVariant (ui->tableWidget->item(i, 0)->text()));
           good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 2)->setProperty("Value", QVariant (ui->tableWidget->item(i, 7)->text()));
           lolo++;
       }
    good->querySubObject ("Cells(QVariant,QVariant)", 2, 3)->setProperty("Value", QVariant (ui->lineEdit_4->text()));
    good->querySubObject ("Cells(QVariant,QVariant)", 2, 4)->setProperty("Value", QVariant (ui->lineEdit_5->text()));
    good->querySubObject ("Cells(QVariant,QVariant)", 2, 5)->setProperty("Value", QVariant (ui->lineEdit_6->text()));
    good->querySubObject ("Cells(QVariant,QVariant)", 2, 6)->setProperty("Value", QVariant (ui->lineEdit_7->text()));
    good->querySubObject ("Cells(QVariant,QVariant)", 2, 7)->setProperty("Value", QVariant (QString::number(ui->lineEdit_4->text().toDouble() / obk)));
    good->querySubObject ("Cells(QVariant,QVariant)", 2, 8)->setProperty("Value", QVariant (QString::number(ui->lineEdit_5->text().toDouble() / obk)));
    good->querySubObject ("Cells(QVariant,QVariant)", 2, 9)->setProperty("Value", QVariant (QString::number(ui->lineEdit_6->text().toDouble() / obk)));
    good->querySubObject ("Cells(QVariant,QVariant)", 2, 10)->setProperty("Value", QVariant (QString::number(ui->lineEdit_7->text().toDouble() / obk)));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    kol_ot = 0;
    kol_dv = 0;
    kol_hor = 0;
    kol_tr = 0;
    QString ip = ui->lineEdit_18->text();
    int port = ui->lineEdit_17->text().toInt();
    auto pair = qMakePair (ip, port);
    curAg = pair;
    emit makeConnection (pair);
    QJsonObject obj;
    obj["start"] = "start";
    emit sendMessage (JsonToString (&obj), curAg);
}
