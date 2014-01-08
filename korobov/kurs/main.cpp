#include <QApplication>

#include "mainwindow.h"

//#include "storage.h"
//#include "user.h"
#include <QRegExp>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}

