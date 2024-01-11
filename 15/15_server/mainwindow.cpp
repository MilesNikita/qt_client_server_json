#include "mainwindow.h"
#include "ui_mainwindow.h"

int lolo = 1;
QJsonObject oi;
QVector<QDate> vec;
QMap <QDate, QString> ma;
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
    ui->lineEdit->setValidator(ipValidator);
    ui->lineEdit_2->setValidator(new QIntValidator (1000, 99999, this));
    ui->comboBox->setCurrentIndex(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::opred(int k)
{
    if (k == 3)
    {
        ui->tab->setEnabled(false);
    }
    if (k == 1)
    {
        ui->tab_2->setEnabled(false);
        ui->tab_3->setEnabled(false);
    }
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
    if ((reply.contains ("All")) && (reply.contains ("All_r"))){
       appendline(reply, agent);
    }
}

void MainWindow::appendline(QJsonObject &obj, QPair<QString, int> &)
{
    oi = obj;
    int a = obj["All"].toString().toInt();
    for (int i = 0; i < a; i++){
        QString lis = obj.value(QString::number(i)).toString();
        ui->comboBox->addItem(lis);
    }
    int b = obj["All_r"].toString().toInt();
    for (int i = 0; i < b; i++){
        QStringList list = obj.value("r" + QString::number(i)).toString().split(";");
        vec.append(QDate::fromString(list.at(1), "dd.MM.yyyy"));
        ma[QDate::fromString(list.at(1), "dd.MM.yyyy")] = list.at(0) + ";" + list.at(3);
        if (ui->lineEdit_3->text() != "")
        {
            if (list.at(3) == ui->lineEdit_3->text()){
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());
                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(list.at(0)));
                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(list.at(1)));
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(list.at(2)));
                ui->tableWidget->setItem(i, 4, new QTableWidgetItem(list.at(3)));
                if (list.at(4).toInt() == 1){
                    ui->tableWidget->setItem(i, 3, new QTableWidgetItem("Выполнено"));
                }
                else{
                    ui->tableWidget->setItem(i, 3, new QTableWidgetItem("Невыполнено"));
                }
            }
        }
        ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(list.at(0)));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(list.at(1)));
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(list.at(2)));
        ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(list.at(3)));
        if (list.at(4).toInt() == 1){
            ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem("Выполнено"));
        }
        else{
            ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem("Невыполнено"));
        }
        QDate d1 = QDate::currentDate();
        QStringList l = list.at(1).split(" ");
        if ((QDate::fromString(l.at(0), "dd.MM.yyyy") < d1) && (list.at(4).toInt() != 1)){
            ui->tableWidget_4->insertRow(ui->tableWidget_4->rowCount());
            ui->tableWidget_4->setItem(i, 0, new QTableWidgetItem(list.at(0)));
            ui->tableWidget_4->setItem(i, 1, new QTableWidgetItem(list.at(1)));
            ui->tableWidget_4->setItem(i, 2, new QTableWidgetItem(list.at(2)));
            ui->tableWidget_4->setItem(i, 4, new QTableWidgetItem(list.at(3)));
            if (list.at(4).toInt() == 1){
                ui->tableWidget_4->setItem(i, 3, new QTableWidgetItem("Выполнено"));
            }
            else{
                ui->tableWidget_4->setItem(i, 3, new QTableWidgetItem("Невыполнено"));
            }
        }
        if ((QDate::fromString(l.at(0), "dd.MM.yyyy") >= d1) && (list.at(4).toInt() == 1)){
            ui->tableWidget_3->insertRow(ui->tableWidget_3->rowCount());
            ui->tableWidget_3->setItem(i, 0, new QTableWidgetItem(list.at(0)));
            ui->tableWidget_3->setItem(i, 1, new QTableWidgetItem(list.at(1)));
            ui->tableWidget_3->setItem(i, 2, new QTableWidgetItem(list.at(2)));
            ui->tableWidget_3->setItem(i, 4, new QTableWidgetItem(list.at(3)));
            if (list.at(4).toInt() == 1){
                ui->tableWidget_3->setItem(i, 3, new QTableWidgetItem("Выполнено"));
            }
            else{
                ui->tableWidget_3->setItem(i, 3, new QTableWidgetItem("Невыполнено"));
            }
        }
        if ((QDate::fromString(l.at(0), "dd.MM.yyyy") >= d1) && (list.at(4).toInt() != 1)){
            ui->tableWidget_4->insertRow(ui->tableWidget_4->rowCount());
            ui->tableWidget_4->setItem(i, 0, new QTableWidgetItem(list.at(0)));
            ui->tableWidget_4->setItem(i, 1, new QTableWidgetItem(list.at(1)));
            ui->tableWidget_4->setItem(i, 2, new QTableWidgetItem(list.at(2)));
            ui->tableWidget_4->setItem(i, 4, new QTableWidgetItem(list.at(3)));
            if (list.at(4).toInt() == 1){
                ui->tableWidget_4->setItem(i, 3, new QTableWidgetItem("Выполнено"));
            }
            else{
                ui->tableWidget_4->setItem(i, 3, new QTableWidgetItem("Невыполнено"));
            }
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    ui->tableWidget_4->setRowCount(0);
    ui->comboBox->clear();
    QString ip = ui->lineEdit->text();
    int port = ui->lineEdit_2->text().toInt();
    auto pair = qMakePair (ip, port);
    curAg = pair;
    emit makeConnection (pair);
    QJsonObject obj;
    obj["start"] = "start";
    emit sendMessage (JsonToString (&obj), curAg);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    ui->tableWidget_4->setRowCount(0);
    ui->comboBox->clear();
    if(ui->lineEdit_9->text() == "")
    {
        QMessageBox::critical(this, "Ошибка", "Введите название мероприятия");
        return;
    }
    if (ui->lineEdit_10->text() == ""){
        QMessageBox::critical(this, "Ошибка", "Введите место проведения меропрития");
        return;
        QJsonObject obj;
        obj["start"] = "start";
        emit sendMessage (JsonToString (&obj), curAg);
    }
    QString name_m = ui->lineEdit_9->text();
    QString date = ui->dateTimeEdit->text();
    QString mesto = ui->lineEdit_10->text();
    QString otvet = ui->comboBox->currentText();
    QJsonObject obj;
    obj["update"] = name_m + ";" + date + ";" + mesto + ";" + otvet + ";" + "0";
    emit sendMessage (JsonToString (&obj), curAg);
    ui->lineEdit_9->clear();
    ui->lineEdit_10->clear();
    ui->dateTimeEdit->clear();
}

void MainWindow::on_pushButton_2_clicked()
{
    qSort(vec.begin(), vec.end());
    excel = new QAxObject ("Excel.Application");
    excel->setProperty ("Visible", true);
    excel->setProperty ("DisplayAlerts", "0");
    workbooks = excel->querySubObject ("Workbooks");
    workbook = workbooks->querySubObject ("Add()");
    sheets = workbook->querySubObject ("Sheets");
    good = sheets->querySubObject ("Item(int)", 1);
    good->setProperty ("Name", "Отчет");
    good->querySubObject ("Cells(QVariant,QVariant)", lolo, 1)->setProperty("Value", "Название мероприятия");
    good->querySubObject ("Cells(QVariant,QVariant)", lolo, 2)->setProperty("Value", "Ответственный");
    for (int i = 0; i < vec.size(); i++)
    {
            good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 1)->setProperty("Value", QVariant (ma[vec.at(i)].split(";").at(0)));
            good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 2)->setProperty("Value", QVariant (ma[vec.at(i)].split(";").at(1)));
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->tableWidget->setRowCount(0);
    QString str = ui->lineEdit_3->text();
    if (str == "")
    {
        QMessageBox::critical(this, "Ошибка", "Введите свою фамилию и инициалы");
        return;
    }
    QJsonObject obj = oi;
    int b = obj["All_r"].toString().toInt();
    for (int i = 0; i < b; i++){
        QStringList list = obj.value("r" + QString::number(i)).toString().split(";");
        if (list.at(3) == str)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(list.at(0)));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(list.at(1)));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(list.at(2)));
            ui->tableWidget->setItem(i, 4, new QTableWidgetItem(list.at(3)));
            if (list.at(4).toInt() == 1){
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem("Выполнено"));
            }
            else{
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem("Невыполнено"));
            }
        }
    }
    ui->tableWidget->setEnabled(true);
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    ui->lineEdit_4->setText(ui->tableWidget->item(row, 0)->text());
}

void MainWindow::on_pushButton_6_clicked()
{
    if (ui->lineEdit_4->text() == ""){
        QMessageBox::critical(this, "Ошибка", "Выберите строку для того чтобы поставить выполнено");
        return;
    }
    QString str = ui->lineEdit_4->text();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    ui->tableWidget_4->setRowCount(0);
    ui->comboBox->clear();
    QJsonObject obj;
    obj["vi"] = str + ";" + ui->lineEdit_3->text();
    emit sendMessage (JsonToString (&obj), curAg);
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->tableWidget_2->setSortingEnabled(true);
    ui->tableWidget_2->sortItems(1, Qt::DescendingOrder);
    ui->tableWidget_3->setSortingEnabled(true);
    ui->tableWidget_3->sortItems(1, Qt::DescendingOrder);
    ui->tableWidget_4->setSortingEnabled(true);
    ui->tableWidget_4->sortItems(1, Qt::DescendingOrder);
}

void MainWindow::on_pushButton_7_clicked()
{
    excel = new QAxObject ("Excel.Application");
    excel->setProperty ("Visible", true);
    excel->setProperty ("DisplayAlerts", "0");
    workbooks = excel->querySubObject ("Workbooks");
    workbook = workbooks->querySubObject ("Add()");
    sheets = workbook->querySubObject ("Sheets");
    good = sheets->querySubObject ("Item(int)", 1);
    good->setProperty ("Name", "Отчет");
    good->querySubObject ("Cells(QVariant,QVariant)", lolo, 1)->setProperty("Value", "Название мероприятия");
    good->querySubObject ("Cells(QVariant,QVariant)", lolo, 2)->setProperty("Value", "Дата");
    good->querySubObject ("Cells(QVariant,QVariant)", lolo, 3)->setProperty("Value", "Ответственный");
    good->querySubObject ("Cells(QVariant,QVariant)", lolo, 4)->setProperty("Value", "Выполнение");
    for (int i = 0; i < ui->tableWidget_2->rowCount(); i++)
    {
        if ((ui->tableWidget_2->item(i, 3)->text() == "Невыполнено") && ((QDate::fromString(ui->tableWidget_2->item(i,0)->text().split(" ").at(0), "dd.MM.yyyy")) >  QDate::currentDate())){
                good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 1)->setProperty("Value", QVariant (ui->tableWidget_2->item(i, 0)->text()));
                good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 2)->setProperty("Value", QVariant (ui->tableWidget_2->item(i, 1)->text()));
                good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 3)->setProperty("Value", QVariant (ui->tableWidget_2->item(i, 4)->text()));
                good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 3)->setProperty("Value", QVariant (ui->tableWidget_2->item(i, 3)->text()));
        }

    }
}
