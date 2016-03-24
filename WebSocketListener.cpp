#include "WebSocketListener.h"
#include "websocket.h"
#include <iostream>

WebSocketListener::WebSocketListener(QObject *parent) : QObject(parent)
{
 //   std::cout << "WebSocketListner receive token: " << token << std::endl;
}

WebSocketListener::~WebSocketListener()
{
    qDebug( "WebSocketListener DESTRUCT" );
}

void WebSocketListener::start()
{
    qDebug( "WebSocketListener:: DO_SOMETHING" );
    wss_connect();
    this->startTimer(2000);
}


void WebSocketListener::timerEvent(QTimerEvent *timerEvent)
{
    //qDebug( "WebSocketListener:: POLLING timerEvent" );
    //lws_service( context, 0 );
}

