#-------------------------------------------------
#
# Project created by QtCreator 2013-12-26T01:48:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kurs
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    user.cpp \
    smtpclient.cpp \
    newmessage.cpp \
    pop3client.cpp \
    newuser.cpp \
    login.cpp \
    message.cpp \
    openmessage.cpp

HEADERS  += mainwindow.h \
    user.h \
    smtpclient.h \
    newmessage.h \
    pop3client.h \
    newuser.h \
    login.h \
    storage.h \
    message.h \
    openmessage.h

FORMS    += mainwindow.ui \
    login.ui \
    newuser.ui \
    newmessage.ui \
    openmessage.ui
