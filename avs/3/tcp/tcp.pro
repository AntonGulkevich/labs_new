#-------------------------------------------------
#
# Project created by QtCreator 2013-04-11T00:59:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcp
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    server.cpp \
    client.cpp \
    tcpserver.cpp \
    udpserver.cpp \
    tcpclient.cpp \
    udpclient.cpp

HEADERS  += dialog.h \
    server.h \
    client.h \
    tcpserver.h \
    udpserver.h \
    tcpclient.h \
    udpclient.h \
    const.h

FORMS    += dialog.ui
