#include "mainwindow.h"
#include "ui_mainwindow.h"

int k = 1;
QMap <QString, int> map;
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
    ui->lineEdit_3->setValidator(ipValidator);
    ui->lineEdit_4->setValidator(new QIntValidator (1000, 99999, this));
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
    if (reply.contains("All_r"))
    {
        appendspi(reply, agent);
    }
}

void MainWindow::appendline (QJsonObject &obj, QPair<QString, int> &agent)
{
    int a = obj["All"].toString().toInt();
    for (int i = 0; i < a; i++){
        QStringList lis = obj.value(QString::number(i)).toString().split(";");
        qDebug() << lis;
        map[lis.at(0)] = lis.at(1).toInt();
        ui->comboBox->addItem(lis.at(0));
        ui->comboBox_2->addItem(lis.at(0));
        ui->comboBox_3->addItem(lis.at(0));
        ui->comboBox_4->addItem(lis.at(0));
    }

}

void MainWindow::appendspi(QJsonObject &obj, QPair<QString, int> &agent)
{
    int b = obj["All_r"].toString().toInt();
    for (int i = 0; i < b; i++){
        QStringList lis = obj.value("spis" + QString::number(i)).toString().split(';');
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(lis.at(4)));
        ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(lis.at(4)));
        ui->tableWidget_3->insertRow(ui->tableWidget_3->rowCount());
        ui->tableWidget_3->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget_3->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget_3->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget_3->setItem(i, 3, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget_3->setItem(i, 4, new QTableWidgetItem(lis.at(4)));
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

void MainWindow::opr(int k)
{
    if (k == 3){
           ui->tab_2->setEnabled(false);
           ui->tab_3->setEnabled(false);
        }
    if (k == 2){
        ui->tab->setEnabled(false);
        ui->tab_3->setEnabled(false);
    }
    if (k == 1){
        ui->tab->setEnabled(false);
        ui->tab_2->setEnabled(false);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    map.clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    QString ip = ui->lineEdit_3->text();
    int port = ui->lineEdit_4->text().toInt();
    auto pair = qMakePair (ip, port);
    curAg = pair;
    emit makeConnection (pair);
    QJsonObject obj;
    obj["start"] = "start";
    obj["get"] = "get";
    emit sendMessage (JsonToString (&obj), curAg);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    QString name = ui->comboBox->currentText();
    QString date = ui->dateEdit->text();
    QString chislo = ui->lineEdit_5->text();
    if (chislo.toInt() != map[name]){
        QMessageBox::critical(this,"Ошибка","В указанном курсе " + QString::number(map[name]) + " человек");
        ui->lineEdit_5->clear();
        return;
    }
    QString neww = ui->lineEdit_6->text();
    if (neww.toInt() > map[name]){
        QMessageBox::critical(this,"Ошибка","В указанном курсе не может быть больше " + QString::number(map[name]) + " человек");
        ui->lineEdit_6->clear();
        return;
    }
    QJsonObject obj;
    obj["update"] = name + ";" + date + ";" + chislo + ";" + neww;
    emit sendMessage (JsonToString (&obj), curAg);
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    QString name = ui->comboBox_2->currentText();
    QString date = ui->dateEdit_2->text();
    QString otset = ui->lineEdit_8->text();
    QJsonObject obj;
    obj["bol"] = name + ";" + date + ";" + otset;
    emit sendMessage (JsonToString (&obj), curAg);
}

void MainWindow::on_pushButton_clicked()
{
    QString number = ui->comboBox_3->currentText();
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if (ui->tableWidget->item(i, 0)->text() == number)
        {
            ui->tableWidget->item(i,0)->setBackgroundColor("red");
            ui->tableWidget->item(i,1)->setBackgroundColor("red");
            ui->tableWidget->item(i,2)->setBackgroundColor("red");
            ui->tableWidget->item(i,3)->setBackgroundColor("red");
            ui->tableWidget->item(i,4)->setBackgroundColor("red");
        }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    QString number = ui->comboBox_4->currentText();
    excel = new QAxObject ("Excel.Application");
    excel->setProperty ("Visible", true);
    excel->setProperty ("DisplayAlerts", "0");
    workbooks = excel->querySubObject ("Workbooks");
    workbook = workbooks->querySubObject ("Add()");
    sheets = workbook->querySubObject ("Sheets");
    good = sheets->querySubObject ("Item(int)", 1);
    good->setProperty ("Name", "Сохранение");
    good->querySubObject ("Cells(QVariant,QVariant)", k, 1)->setProperty("Value", "Номер курса");
    good->querySubObject ("Cells(QVariant,QVariant)", k, 2)->setProperty("Value", "Дата");
    good->querySubObject ("Cells(QVariant,QVariant)", k, 3)->setProperty("Value", "Количество л/с по списку");
    good->querySubObject ("Cells(QVariant,QVariant)", k, 4)->setProperty("Value", "Количество л/с на лицо");
    good->querySubObject ("Cells(QVariant,QVariant)", k, 5)->setProperty("Value", "Причина");
    for (int i = 0; i < ui->tableWidget->rowCount(); i++){
        if (ui->tableWidget->item(i, 0)->text() == number){
               good->querySubObject ("Cells(QVariant,QVariant)", k + 1 + i , 1)->setProperty("Value", QVariant (ui->tableWidget->item(i, 0)->text()));
               good->querySubObject ("Cells(QVariant,QVariant)", k + 1 + i , 2)->setProperty("Value", QVariant (ui->tableWidget->item(i, 1)->text()));
               good->querySubObject ("Cells(QVariant,QVariant)", k + 1 + i , 3)->setProperty("Value", QVariant (ui->tableWidget->item(i, 2)->text()));
               good->querySubObject ("Cells(QVariant,QVariant)", k + 1 + i , 4)->setProperty("Value", QVariant (ui->tableWidget->item(i, 3)->text()));
               good->querySubObject ("Cells(QVariant,QVariant)", k + 1 + i , 5)->setProperty("Value", QVariant (ui->tableWidget->item(i, 4)->text()));
               k++;
        }
           }
}
