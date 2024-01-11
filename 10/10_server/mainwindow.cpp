#include "mainwindow.h"
#include "ui_mainwindow.h"

int lolo = 1;
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
    ui->lineEdit_9->setToolTip("Введите фамили через запятую, при отсутствие введите 0");
    ui->lineEdit_10->setToolTip("Введите фамили через запятую, при отсутствие введите 0");
    ui->lineEdit_11->setToolTip("Введите фамили через запятую, при отсутствие введите 0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::opred(int k)
{
    if (k == 3)
    {
        ui->tab_2->setEnabled(false);
    }
    if (k == 1)
    {
        ui->tab->setEnabled(false);
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
    if (reply.contains ("All")) {
       appendline(reply, agent);
    }
}

void MainWindow::appendline(QJsonObject &obj, QPair<QString, int> &)
{
    int a = obj["All"].toString().toInt();
    int n = 0;
    int a1 = 0, b1 = a;
    int h = 1;
    QVector <double> x, y, x1, x2;
    for (int i = 0; i < a; i++){
        QStringList lis = obj.value(QString::number(i)).toString().split(";");
        if (lis.at(4) == "Наряд")
        {
            if (lis.at(5).size() > 1)
            {
                x.push_back(lis.at(5).split(",").size());
            }
            else
            {
                x.push_back(0);
            }
        }
        if (lis.at(6) == "Санчасть")
        {
            if (lis.at(7).size() > 1)
            {
                x1.push_back(lis.at(7).split(",").size());
            }
            else
            {
                x1.push_back(0);
            }
        }
        if (lis.at(8) == "Прогул")
        {
            if (lis.at(9).size() > 1)
            {
                x2.push_back(lis.at(9).split(",").size());
            }
            else
            {
                x2.push_back(0);
            }
        }
        y.push_back(n);
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
        ui->tableWidget_3->insertRow(ui->tableWidget_3->rowCount());
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(lis.at(4) + " : " + lis.at(5) + "; " + lis.at(6) + " : " + lis.at(7) + "; " + lis.at(8) + " : " + lis.at(9) + ";"));
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(lis.at(4) + " : " + lis.at(5) + "; " + lis.at(6) + " : " + lis.at(7) + "; " + lis.at(8) + " : " + lis.at(9) + ";"));
        ui->tableWidget_3->setItem(i, 0, new QTableWidgetItem(lis.at(0)));
        ui->tableWidget_3->setItem(i, 1, new QTableWidgetItem(lis.at(1)));
        ui->tableWidget_3->setItem(i, 2, new QTableWidgetItem(lis.at(2)));
        ui->tableWidget_3->setItem(i, 3, new QTableWidgetItem(lis.at(3)));
        ui->tableWidget_3->setItem(i, 4, new QTableWidgetItem(lis.at(4) + " : " + lis.at(5) + "; " + lis.at(6) + " : " + lis.at(7) + "; " + lis.at(8) + " : " + lis.at(9) + ";"));
        n++;
    }
    qDebug() << x << x1 << x2;
    ui->widget->clearGraphs();
    ui->widget->addGraph();
    ui->widget_2->clearGraphs();
    ui->widget_2->addGraph();
    ui->widget_3->clearGraphs();
    ui->widget_3->addGraph();
    ui->widget->graph(0)->setData(x, y);
    ui->widget_2->graph(0)->setData(x1, y);
    ui->widget_3->graph(0)->setData(x2, y);
    ui->widget->xAxis->setRange(a1, b1);
    ui->widget_2->xAxis->setRange(a1, b1);
    ui->widget_3->xAxis->setRange(a1, b1);
    ui->widget->yAxis->setRange(0, b1);
    ui->widget_2->yAxis->setRange(0, b1);
    ui->widget_3->yAxis->setRange(0, b1);
    ui->widget->replot();
    ui->widget_2->replot();
    ui->widget_3->replot();
}

void MainWindow::on_pushButton_clicked()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    QString ip = ui->lineEdit->text();
    int port = ui->lineEdit_2->text().toInt();
    auto pair = qMakePair (ip, port);
    curAg = pair;
    emit makeConnection (pair);
    QJsonObject obj;
    obj["start"] = "start";
    emit sendMessage (JsonToString (&obj), curAg);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_3->setRowCount(0);
    QString date = ui->dateEdit_3->text();
    QString gr = ui->comboBox->currentText();
    QString kol1 = ui->lineEdit_5->text();
    QString kol2 = ui->lineEdit_6->text();
    QString pr1 = ui->comboBox_2->currentText();
    QString ot1 = ui->lineEdit_9->text();
    QString pr2 = ui->comboBox_3->currentText();
    QString ot2 = ui->lineEdit_10->text();
    QString pr3 = ui->comboBox_4->currentText();
    QString ot3 = ui->lineEdit_11->text();
    QJsonObject obj;
    obj["update"] = date + ";" + gr + ";" + kol1 + ";" + kol2 + ";" + pr1 + ";" + ot1 + ";" + pr2 + ";" + ot2 + ";" + pr3 + ";" + ot3;
    qDebug() << obj;
    emit sendMessage (JsonToString (&obj), curAg);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString date1 = ui->dateEdit->text();
    QString date2 = ui->dateEdit_2->text();
    QDate dat1 = QDate::fromString(date1, "dd.MM.yyyy");
    QDate dat2 = QDate::fromString(date2, "dd.MM.yyyy");
    pword = new QAxObject("Word.Application");
    pword->setProperty("Visible", true);
    pdoc = pword->querySubObject("Documents");
    pdoc = pdoc->querySubObject("Add()");
    pactive = pword->querySubObject("ActiveDocument()");
    prange = pactive->querySubObject("Range()");
    prange->setProperty("Text", "Посещение сампоподготовки: \n");
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if ((QDate::fromString(ui->tableWidget->item(i,0)->text(),"dd.MM.yyyy") >= dat1) && (QDate::fromString(ui->tableWidget->item(i,0)->text(),"dd.MM.yyyy") <= dat2)){
            good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 1)->setProperty("Value", QVariant (ui->tableWidget->item(i, 0)->text()));
            good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 2)->setProperty("Value", QVariant (ui->tableWidget->item(i, 1)->text()));
            good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 3)->setProperty("Value", QVariant (ui->tableWidget->item(i, 2)->text()));
            good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 4)->setProperty("Value", QVariant (ui->tableWidget->item(i, 3)->text()));
            good->querySubObject ("Cells(QVariant,QVariant)", lolo + 1 + i , 5)->setProperty("Value", QVariant (ui->tableWidget->item(i, 4)->text()));
        }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    QString date1 = ui->dateEdit_4->text();
    QString date2 = ui->dateEdit_5->text();
    QDate dat1 = QDate::fromString(date1, "dd.MM.yyyy");
    QDate dat2 = QDate::fromString(date2, "dd.MM.yyyy");
    for (int i = 0; i < ui->tableWidget_3->rowCount(); i++)
    {
        if ((QDate::fromString(ui->tableWidget_3->item(i,0)->text(),"dd.MM.yyyy") >= dat1) && (QDate::fromString(ui->tableWidget_3->item(i,0)->text(),"dd.MM.yyyy") <= dat2)){
            ui->tableWidget_3->item(i,0)->setBackgroundColor("green");
            ui->tableWidget_3->item(i,1)->setBackgroundColor("green");
            ui->tableWidget_3->item(i,2)->setBackgroundColor("green");
            ui->tableWidget_3->item(i,3)->setBackgroundColor("green");
            ui->tableWidget_3->item(i,4)->setBackgroundColor("green");
        }
}
}
