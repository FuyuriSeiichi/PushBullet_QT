#include "LoginWindow.h"
#include "pushbulletcontroller.h"
#include <QApplication>
#include <QString>
#include <QDir>
#include <fstream>
#include "mainwindow.h"

//#include "WebSocketListener.h"

PushBulletController *pb_handler;

bool is_file_exist(const char *fileName)
{
    std::ifstream infile( fileName );
    return infile.good();
}

QString loadCfg()
{
  QString fileSettings( QDir::homePath() + "/.config/pushbullet.cfg" );
  QSettings settings( fileSettings, QSettings::NativeFormat );
  QString access_token = settings.value( "Access-Token", "" ).toString();
  if ( !access_token.isEmpty() ) {
    //this->hide();
    //this->close();
  }
  return access_token;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pb_handler = new PushBulletController();
    QString access_token = loadCfg();
    if ( !access_token.isEmpty() ) {
      pb_handler->setAccessToken( access_token.toStdString() );
      pb_handler->listDevices(); // This update this->device_list !!
      MainWindow *mw = new MainWindow( NULL, access_token.toStdString() );
      mw->show();
    }
    else {
      LoginWindow *w = new LoginWindow();
      w->show();
    }
    return a.exec();
}
