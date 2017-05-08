#include "RegisterDeviceDialog.h"
#include "ui_newdevice.h"
#include "pushbulletcontroller.h"
#include <curl.h>

extern PushBulletController *pb_handler;

RegisterDeviceDialog::RegisterDeviceDialog( QWidget *parent ) :
  QDialog( parent ),
  ui(new Ui::RegisterDeviceDialog)
{
  ui->setupUi(this);

}

RegisterDeviceDialog::~RegisterDeviceDialog()
{
  delete ui;
}

void RegisterDeviceDialog::on_buttonNewDeviceCancel_clicked()
{
  //q( "HELLO" );
  qDebug( "Cancel registering" );
  this->close();
}

void RegisterDeviceDialog::on_buttonNewDeviceSubmit_clicked()
{
  qDebug( "SUBMIT registering" );
  // ui->lineeditTitle->text().toUtf8()
  string result = pb_handler->registerDevice( this->ui->lineeditDeviceNickname->text().toStdString(),
                                                this->ui->lineeditDeviceManufacturer->text().toStdString(),
                                                this->ui->lineeditDeviceModel->text().toStdString(),
                                                "laptop" );
  this->close();
}
