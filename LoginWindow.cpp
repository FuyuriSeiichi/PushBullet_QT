#include "LoginWindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include <fstream>
#include <QDir>

extern PushBulletController *pb_handler;

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    fileSettings = QString( QDir::homePath() + "/.config/pushbullet.cfg" );
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::loadCfg()
{
    QSettings settings( fileSettings, QSettings::NativeFormat );
    QString access_token = settings.value( "Access-Token", "" ).toString();
    if ( !access_token.isEmpty() ) {
        pb_handler->setAccessToken( access_token.toStdString() );
        pb_handler->listDevices(); // This update this->device_list !!
        MainWindow *mainWindow = new MainWindow( this, pb_handler->access_token );
	this->saveCfg();
        //this->hide();
	//this->close();
	QMainWindow::close();
	mainWindow->show();
    }
}

void LoginWindow::saveCfg()
{
    QSettings settings( fileSettings, QSettings::NativeFormat );
   // QString sText = ( pb_handler->access_token ) ? m_pEdit->text() : "";
    if ( pb_handler->access_token != ""  ) {
        settings.setValue( "Access-Token", QString::fromStdString( pb_handler->access_token ) );
    }
    settings.sync();
}

bool LoginWindow::is_file_exist(const char *fileName)
{
    std::ifstream infile( fileName );
    return infile.good();
}

void LoginWindow::showEvent( QShowEvent *event )
{
   QMainWindow::show();
   if ( is_file_exist( fileSettings.toStdString().c_str() ) ) {
       loadCfg();  // This will skip LoginWindow & move on to mainWindow.
   }
}

void LoginWindow::on_Submit_clicked()
{
    pb_handler->setAccessToken( this->ui->lineeditAccessToken ->text().toStdString() );
    pb_handler->listDevices(); // This update this->device_list !!
    MainWindow *mainWindow = new MainWindow( this, pb_handler->access_token );
    this->hide();
    this->saveCfg();
    mainWindow->show();
}
