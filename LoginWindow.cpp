#include "LoginWindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"

extern PushBulletController *pb_handler;

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_Submit_clicked()
{
    pb_handler->setAccessToken( this->ui->lineeditAccessToken ->text().toStdString() );
    pb_handler->listDevices(); // This update this->device_list !!
  //  pb_handler->getPushes( true, "1458259200" );
    MainWindow *mainWindow = new MainWindow( this, pb_handler->access_token );
    this->hide();
    mainWindow->show();
}
