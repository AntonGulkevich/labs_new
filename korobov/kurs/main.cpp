#include <QApplication>

#include "mainwindow.h"

//#include "storage.h"
//#include "user.h"
#include <QDebug>
#include <QByteArray>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}

