#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "pushbulletcontroller.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_Submit_clicked();

private:
    Ui::LoginWindow *ui;

//    PushBulletController *pb_handler;
};

#endif // LOGINWINDOW_H
