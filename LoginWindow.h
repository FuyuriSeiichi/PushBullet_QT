#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QDir>
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
    QString fileSettings;
    Ui::LoginWindow *ui;
    void loadCfg();
    void saveCfg();
    bool is_file_exist( const char *fileName );
    void showEvent( QShowEvent* event );

//    PushBulletController *pb_handler;
};

#endif // LOGINWINDOW_H
