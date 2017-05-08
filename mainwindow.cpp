#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pushbulletcontroller.h"
//#include "websocket.h"
#include <iostream>
#include "RegisterDeviceDialog.h"

extern PushBulletController *pb_handler;

MainWindow::MainWindow(QWidget *parent, std::string input_token) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->websocket_thread = new QThread;
    this->websocket_listener = new WebSocketListener;
    websocket_listener->moveToThread( websocket_thread );
    this->token = input_token;
    this->setupSignalSlotToWebSocketListener();
    this->updateDevicesComboBox();
    websocket_thread->start();
 //   wss_connect();
}

MainWindow::~MainWindow()
{
    //delete websocketListener;
    delete ui;
}

void MainWindow::updateDevicesComboBox()
{
    if ( pb_handler->devices_list != NULL ) {
        for ( vector<Device>::const_iterator iter = pb_handler->devices_list->begin();
              iter != pb_handler->devices_list->end();
              iter ++ ) {
            if ( iter->nickname.empty() == false ) {
                QString qstrNickname = QString::fromUtf8( iter->nickname.c_str() );
                QVariant qvarArg( QString( iter->iden.c_str() ) );
                this->ui->comboboxDevices->addItem( qstrNickname, qvarArg );
            }
        }
    }
}

void MainWindow::on_comboboxDevices_currentIndexChanged( int index )
{
    pb_handler->deviceSelected = pb_handler->devices_list->begin() + index;
    std::cout << "Device selected:" << pb_handler->deviceSelected->nickname << std::endl;
}

void MainWindow::on_buttonSubmit_clicked()
{
    string *text;
    string *title = new string( ui->lineeditTitle->text().toUtf8() );

    QTabWidget *tabwidgetFunctions = ui->tabwidgetFunctions;

    std::string type = "";

    //std::cout << tabwidgetFunctions->currentWidget()->accessibleName().toStdString() << std::endl;

    if ( tabwidgetFunctions->currentWidget()->accessibleName() == "note" ) {
        type = std::string( "note" );
        text = new string( ui->plaintexteditNote->toPlainText().toUtf8() );
    }
    else {
        type = std::string( "link" );
        text = new string( ui->plaintexteditLink->toPlainText().toUtf8() );
    }
    if ( pb_handler->deviceSelected->iden == "ALL" )
        pb_handler->push( type.c_str(), *title, *text, "ALL" );
    else
        pb_handler->push( type.c_str(), *title, *text, pb_handler->deviceSelected->iden );
}

void MainWindow::setupSignalSlotToWebSocketListener()
{
    connect( websocket_listener, SIGNAL( error(QString) ),
             this, SLOT( error(QString) ) );
    connect( websocket_thread, SIGNAL( started() ),
             websocket_listener, SLOT( start() ) );
    connect( websocket_listener, SIGNAL( finished() ),
             websocket_thread, SLOT( quit() ) );
    connect( websocket_thread, SIGNAL( finished() ),
             this, SLOT( websocket_quit() ) );
}

void MainWindow::websocket_check()
{
    std::cout << "Checking websocket" << std::endl;
    //lws_service( context, period );
}

void MainWindow::error( QString errMsg )
{
    qDebug( errMsg.toUtf8() );
}

void MainWindow::websocket_quit()
{
    qDebug( "websocket thread just quits!" );
}

//void MainWindow::on_pushButton_clicked()
//{

//}

void MainWindow::on_actionRegisterDevice_triggered()
{
  //std::cout << "ACTION REGISTER DEVICE TRIGGERED!" << std::endl;
  RegisterDeviceDialog *registerDeviceDialog = new RegisterDeviceDialog();
  registerDeviceDialog->exec();
}
