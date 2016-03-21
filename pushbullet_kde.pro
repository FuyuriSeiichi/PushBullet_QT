#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T06:46:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pushbullet_kde
TEMPLATE = app


SOURCES += main.cpp\
    pushbulletcontroller.cpp \
    mainwindow.cpp \
    LoginWindow.cpp \
    WebSocketListener.cpp

QMAKE_CXXFLAGS += -std=c++11    

HEADERS  += \
    pushbulletcontroller.h \
    Device.h \
    mainwindow.h \
    LoginWindow.h \
    websocket.h \
    WebSocketListener.h

FORMS    += \
    mainwindow.ui \
    loginwindow.ui

unix:!macx: LIBS += -L/usr/lib64/ -lcurl

INCLUDEPATH += /usr/include/curl
DEPENDPATH += /usr/include/curl

unix:!macx: LIBS += -L/usr/lib64/ -ljsoncpp

INCLUDEPATH += /usr/include/jsoncpp
DEPENDPATH += /usr/include/jsoncpp

unix:!macx: LIBS += -L/usr/lib64/ -lwebsockets

INCLUDEPATH += /usr/include
DEPENDPATH += /usr/include
