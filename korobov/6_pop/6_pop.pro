#-------------------------------------------------
#
# Project created by QtCreator 2013-12-26T00:45:17
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x
TARGET = 6_pop
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    pop3client.cpp \
    message.cpp

HEADERS  += dialog.h \
    pop3client.h \
    message.h

FORMS    += dialog.ui \
    message.ui
