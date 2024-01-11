#include "auth.h"
#include "ui_auth.h"

auth::auth(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::auth)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    mai = new MainWindow();
}

auth::~auth()
{
    delete ui;
}

void auth::on_pushButton_clicked()
{
    if ((ui->lineEdit->text()=="nk") && (ui->lineEdit_2->text()=="nk"))
        {
            mai->show();
            this->close();
            mai->opred(3);
        }
        else if ((ui->lineEdit->text()=="prepod") && (ui->lineEdit_2->text()=="prepod"))
        {
            mai->show();
            this->close();
            mai->opred(1);
        }
        else
        {
            ui->label_3->setStyleSheet("color:red");
            ui->label_3->setText("Неверный логин/пароль");
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
        }
}
