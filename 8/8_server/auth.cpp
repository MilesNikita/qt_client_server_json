#include "auth.h"
#include "ui_auth.h"

auth::auth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::auth)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    maine = new MainWindow();
    ui->lineEdit->setText("admin");
    ui->lineEdit_2->setText("admin");
}

auth::~auth()
{
    delete ui;
}

void auth::on_pushButton_clicked()
{
    if ((ui->lineEdit->text()=="admin") && (ui->lineEdit_2->text()=="admin"))
    {
        maine->show();
        this->close();
    }
    else if ((ui->lineEdit->text()=="kof") && (ui->lineEdit_2->text()=="kof"))
    {
        maine->show();
        this->close();
        maine->opred(2);
    }
    else if ((ui->lineEdit->text()=="kursant") && (ui->lineEdit_2->text()=="kursant"))
    {
        maine->show();
        this->close();
        maine->opred(2);
    }
    else
    {
        ui->label_3->setStyleSheet("color:red");
        ui->label_3->setText("Неверный логин/пароль");
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
    }
}
