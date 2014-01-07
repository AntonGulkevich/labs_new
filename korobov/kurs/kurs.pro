#-------------------------------------------------
#
# Project created by QtCreator 2013-12-26T01:48:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = kurs
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    user.cpp \
    smtpclient.cpp \
    pop3client.cpp \
    newuser.cpp \
    login.cpp \
    message.cpp \
    messagewindow.cpp \
    writemessage.cpp \
    readmessage.cpp

HEADERS  += mainwindow.h \
    user.h \
    smtpclient.h \
    pop3client.h \
    newuser.h \
    login.h \
    storage.h \
    message.h \
    messagewindow.h \
    writemessage.h \
    readmessage.h

FORMS    += mainwindow.ui \
    login.ui \
    newuser.ui \
    writemessage.ui \
    readmessage.ui
