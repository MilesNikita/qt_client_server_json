#ifndef AUTH_H
#define AUTH_H

#include <QWidget>
#include <mainwindow.h>

namespace Ui {
class auth;
}

class auth : public QWidget
{
    Q_OBJECT

public:
    explicit auth(QWidget *parent = nullptr);
    ~auth();

private slots:
    void on_pushButton_clicked();

private:
    Ui::auth *ui;
    MainWindow *m;
};

#endif // AUTH_H
