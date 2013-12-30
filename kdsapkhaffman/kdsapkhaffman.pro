QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kdsapkhaffman
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    coder.cpp

HEADERS  += dialog.h \
    coder.h

FORMS    += dialog.ui
