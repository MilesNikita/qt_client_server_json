#include "auth.h"
#include "ui_auth.h"

auth::auth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::auth)
{
    ui->setupUi(this);
    maine = new MainWindow();
}

auth::~auth()
{
    delete ui;
}

void auth::on_pushButton_clicked()
{
    if ((ui->lineEdit->text() == "kof") && (ui->lineEdit_2->text() == "kof"))
    {
        maine->show();
        this->close();
        maine->opr(3);
    }
    else if ((ui->lineEdit->text() == "jur") && (ui->lineEdit_2->text() == "jur"))
    {
        maine->show();
        this->close();
        maine->opr(2);
    }
    else if ((ui->lineEdit->text() == "kursant") && (ui->lineEdit_2->text() == "kursant"))
    {
        maine->show();
        this->close();
        maine->opr(1);
    }
    else {
        ui->label->setStyleSheet("color:red");
        ui->label->setText("Неверный логин/пароль");
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
    }
}
