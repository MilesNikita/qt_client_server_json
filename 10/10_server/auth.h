#ifndef AUTH_H
#define AUTH_H

#include <QMainWindow>
#include <mainwindow.h>
namespace Ui {
class auth;
}

class auth : public QMainWindow
{
    Q_OBJECT

public:
    explicit auth(QWidget *parent = nullptr);
    ~auth();

private slots:
    void on_pushButton_clicked();

private:
    Ui::auth *ui;
    MainWindow *mai;
};

#endif // AUTH_H
