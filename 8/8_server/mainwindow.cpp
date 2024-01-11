#include "mainwindow.h"
#include "ui_mainwindow.h"

QString file_save, file_open;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
   ui->setupUi(this);
   server = new Server();
   connect (server, &Server::operationFinished, this, &MainWindow::slotOperationFinish);
   connect (this, &MainWindow::sendMessage, server, &Server::sendToClient);
   connect (server, &Server::recievedMessage, this, &MainWindow::slotProcess);
   connect (this, &MainWindow::makeConnection, server, &Server::slotMakeConnection);
   QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
   QRegExp ipRegex ("^" + ipRange + "\\." + ipRange + "\\." + ipRange + "\\." + ipRange + "$");
   QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
   ui->lineEdit_9->setValidator(ipValidator);
   ui->lineEdit_10->setValidator(new QIntValidator (1000, 99999, this));
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

QString MainWindow::JsonToString (QJsonObject *a)
{
    return QString (QJsonDocument (*a).toJson (QJsonDocument::Compact));
}

QJsonObject MainWindow::StringToJson (QString a)
{
    return QJsonDocument::fromJson (a.toUtf8()).object();
}

void MainWindow::slotProcess (QPair<QString, int> agent, QJsonObject reply)
{
    qDebug() << JsonToString(&reply);
    if (reply.contains ("All")) {
       appendline(reply, agent);
    }
    if (reply.contains("Spisok")){
        appendspisok(reply, agent);
    }
}

void MainWindow::appendline (QJsonObject &obj, QPair<QString, int> &agent)
{
    int a = obj["All"].toString().toInt();
    qDebug() << a;
    for (int i = 0; i < a; i++){
        QStringList lis = obj.value(QString::number(i)).toString().split(";");
        ui->tableWidget_4->insertRow(ui->tableWidget_4->rowCount());
        ui->tableWidget_4->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget_4->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget_3->insertRow(ui->tableWidget_3->rowCount());
        ui->tableWidget_3->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget_3->setItem(i, 1, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget_3->setItem(i, 2, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget_3->setItem(i, 3, new QTableWidgetItem(lis.at(4)));
        ui->tableWidget_3->setItem(i, 4, new QTableWidgetItem(lis.at(5)));
        ui->tableWidget_3->setItem(i, 5, new QTableWidgetItem(lis.at(6)));
        ui->tableWidget_3->setItem(i, 6, new QTableWidgetItem(lis.at(7)));
        ui->tableWidget_3->setItem(i, 7, new QTableWidgetItem(lis.at(8)));
        ui->tableWidget_3->setItem(i, 8, new QTableWidgetItem(lis.at(9)));
        ui->comboBox->addItem(lis.at(0));
        ui->comboBox_2->addItem(lis.at(0));
        ui->comboBox_3->addItem(lis.at(0));
    }
}

void MainWindow::appendspisok(QJsonObject &obj, QPair<QString, int> &agent)
{
    int a = obj["Spisok"].toString().toInt();
    qDebug() << a;
    for (int i = 0; i < a; i++){
        QStringList lis = obj.value(QString::number(i)).toString().split(";");
        if (lis.at(0) != ""){
         ui->tableWidget->insertRow(ui->tableWidget->rowCount());
         ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
         ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
         ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
         ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
         ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem(lis.at(4)));
         ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(lis.at(6)));
         ui->tableWidget_2->setItem(i, 5, new QTableWidgetItem(QString::number(sr_ocenka(lis.at(2).toDouble(), lis.at(4).toDouble(), lis.at(6).toDouble()))));
         ui->tableWidget->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
         ui->tableWidget->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
         ui->tableWidget->setItem(i, 2, new QTableWidgetItem(lis.at(3)));
         ui->tableWidget->setItem(i, 3, new QTableWidgetItem(lis.at(2)));
         ui->tableWidget->setItem(i, 4, new QTableWidgetItem(lis.at(5)));
         ui->tableWidget->setItem(i, 5, new QTableWidgetItem(lis.at(4)));
         ui->tableWidget->setItem(i, 6, new QTableWidgetItem(lis.at(7)));
         ui->tableWidget->setItem(i, 7, new QTableWidgetItem(lis.at(6)));
         ui->tableWidget->setItem(i, 8, new QTableWidgetItem(QString::number(sr_ocenka(lis.at(2).toDouble(), lis.at(4).toDouble(), lis.at(6).toDouble()))));
        }
    }
}

void MainWindow::opred(int k){
    if (k == 2){
        ui->tab->setEnabled(false);
    }
}

double MainWindow::sr_ocenka(double a1, double a2, double a3)
{
    double sum = 0;
    sum += a1 + a2 + a3;
    return (sum / 3);
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->tableWidget_4->setRowCount(0);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    QString ip = ui->lineEdit_9->text();
    int port = ui->lineEdit_10->text().toInt();
    auto pair = qMakePair (ip, port);
    curAg = pair;
    emit makeConnection (pair);
    QJsonObject obj;
    obj["start"] = "start";
    emit sendMessage (JsonToString (&obj), curAg);
}

void MainWindow::on_pushButton_clicked()
{
    ui->tableWidget_4->setRowCount(0);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    QString fio = ui->lineEdit->text();
    QString gr = ui->lineEdit_2->text();
    QString upr1 = ui->comboBox->currentText();
    QString ochenka1 = ui->lineEdit_4->text();
    QString upr2 = ui->comboBox_2->currentText();
    QString ochenka2 = ui->lineEdit_5->text();
    QString upr3 = ui->comboBox_3->currentText();
    QString ochenka3 = ui->lineEdit_8->text();
    QJsonObject obj;
    obj["update"] = fio + ";" + gr + ";" + ochenka1 + ";" + upr1 + ";" + ochenka2 + ";" + upr2 + ";" + ochenka3 + ";" + upr3;
    emit sendMessage (JsonToString (&obj), curAg);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_8->clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    QString gr = ui->lineEdit_7->text();
    for (int i = 0; i < ui->tableWidget_2->rowCount(); i++){
        if (ui->tableWidget_2->item(i,1)->text() == gr)
        {
            ui->tableWidget_2->item(i,0)->setBackgroundColor("red");
            ui->tableWidget_2->item(i,1)->setBackgroundColor("red");
            ui->tableWidget_2->item(i,2)->setBackgroundColor("red");
            ui->tableWidget_2->item(i,3)->setBackgroundColor("red");
            ui->tableWidget_2->item(i,4)->setBackgroundColor("red");
            ui->tableWidget_2->item(i,5)->setBackgroundColor("red");
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    double sum1 = 0, sum2 = 0, sum3 = 0;
    pword = new QAxObject("Word.Application");
    pword->setProperty("Visible", true);
    pdoc = pword->querySubObject("Documents");
    pdoc = pdoc->querySubObject("Add()");
    pactive = pword->querySubObject("ActiveDocument()");
    prange = pactive->querySubObject("Range()");
    prange->setProperty("Text", "Итоговая ведомость: \n");
    for (int i = 0; i < ui->tableWidget_2->rowCount(); i++){
        prange->dynamicCall("InsertAfter(Text)","ФИО: " + ui->tableWidget_2->item(i,0)->text() + " Номер учебной группы: " + ui->tableWidget_2->item(i,1)->text() + " Оценка за 1-е упражнение: " + ui->tableWidget_2->item(i,2)->text() + " Оценка за 2-е упражнение: " + ui->tableWidget_2->item(i,3)->text() + " Оценка за 3-е упражнение: " +ui->tableWidget_2->item(i,4)->text() + " Средний балл: " +ui->tableWidget_2->item(i,5)->text()+ "\n");
    }
    for (int i = 0; i < ui->tableWidget_2->rowCount(); i++)
    {
        sum1 += ui->tableWidget_2->item(i,2)->text().toDouble();
        sum2 += ui->tableWidget_2->item(i,3)->text().toDouble();
        sum3 += ui->tableWidget_2->item(i,4)->text().toDouble();
    }
    prange->dynamicCall("InsertAfter(Text)", "Средний балл за 1 упражнение: " + QString::number(sum1/ ui->tableWidget_2->rowCount()) + "\n");
    prange->dynamicCall("InsertAfter(Text)", "Средний балл за 2 упражнение: " + QString::number(sum2/ ui->tableWidget_2->rowCount()) + "\n");
    prange->dynamicCall("InsertAfter(Text)", "Средний балл за 3 упражнение: " + QString::number(sum3/ ui->tableWidget_2->rowCount()) + "\n");
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    ui->lineEdit_3->setText(ui->tableWidget->item(row, 0)->text() + " " + ui->tableWidget->item(row, 1)->text());
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->tableWidget_4->setRowCount(0);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    if (ui->lineEdit_3->text() == ""){
        QMessageBox::critical(this, "Ошибка", "Выберите строку для удаления из таблицы");
        return;
    }
    QString str = ui->lineEdit_3->text();
    QJsonObject obj;
    obj["del"] = str;
    emit sendMessage (JsonToString (&obj), curAg);
}
