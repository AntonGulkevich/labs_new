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
    client.cpp

HEADERS  += dialog.h \
    server.h \
    client.h

FORMS    += dialog.ui
