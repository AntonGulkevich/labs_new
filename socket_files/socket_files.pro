TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    tcpserver.cpp \
    udpreciever.cpp \
    udpsender.cpp \
    tcpclient.cpp

HEADERS += \
    udpreciever.h \
    udpsender.h \
    tcpserver.h \
    const.h \
    tcpclient.h

