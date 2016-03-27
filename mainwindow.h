#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QString>

#include <kstatusnotifieritem.h>

#include "WebSocketListener.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, std::string input_token = "");
    ~MainWindow();

public slots:
    void error( QString errMsg );
    void websocket_quit();

private slots:
    void on_comboboxDevices_currentIndexChanged( int index );
    void on_buttonSubmit_clicked();
    void websocket_check();

    void on_pushButton_clicked();

private:
    void updateDevicesComboBox();
    void setupSignalSlotToWebSocketListener();
    WebSocketListener *websocket_listener;
    std::string token;
    QThread *websocket_thread;
    Ui::MainWindow *ui;
    int period = 2000;

};

#endif // MAINWINDOW_H
