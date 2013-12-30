#-------------------------------------------------
#
# Project created by QtCreator 2013-12-24T23:33:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    tcpserver.cpp \
    tcpthread.cpp

HEADERS  += dialog.h \
    tcpserver.h \
    tcpthread.h

FORMS    += dialog.ui
