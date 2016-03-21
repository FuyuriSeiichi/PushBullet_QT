#include "LoginWindow.h"
#include "pushbulletcontroller.h"
#include <QApplication>
//#include "WebSocketListener.h"

PushBulletController *pb_handler;

void initWebSocketListener();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pb_handler = new PushBulletController();
    LoginWindow w;
    w.show();
    return a.exec();
}
