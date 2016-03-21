#ifndef WEBSOCKETLISTENER_H
#define WEBSOCKETLISTENER_H

#include <QObject>
#include <QString>

class WebSocketListener : public QObject
{
    Q_OBJECT
public:
    explicit WebSocketListener( QObject *parent = 0 );
    ~WebSocketListener();

signals:
    void finished();
    void error( QString errMsg );

public slots:
    void start();

private:
    int counter = 0;

protected:
    void timerEvent(QTimerEvent *);
};

#endif // WEBSOCKETLISTENER_H
