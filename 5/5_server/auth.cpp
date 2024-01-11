#include "auth.h"
#include "ui_auth.h"

auth::auth(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::auth)
{
    ui->setupUi(this);
    m = new MainWindow();
}

auth::~auth()
{
    delete ui;
}

void auth::on_pushButton_clicked()
{
    if ((ui->lineEdit->text() == "da") && (ui->lineEdit_2->text() == "da"))
       {
           m->show();
           this->close();
           m->opr(3);
       }
       else if ((ui->lineEdit->text() == "pda") && (ui->lineEdit_2->text() == "pda"))
       {
           m->show();
           this->close();
           m->opr(2);
       }
       else if ((ui->lineEdit->text() == "feld") && (ui->lineEdit_2->text() == "feld"))
       {
           m->show();
           this->close();
           m->opr(1);
       }
       else {
           ui->label->setStyleSheet("color:red");
           ui->label->setText("Неверный логин/пароль");
           ui->lineEdit->clear();
           ui->lineEdit_2->clear();
       }
}
