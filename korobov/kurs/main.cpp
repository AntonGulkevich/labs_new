#include <QApplication>

#include "mainwindow.h"

#include "storage.h"
#include "user.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Storage<User> *test = new Storage<User>("test.dat");
//    test->importStorage();
//    QList<User> list;
//    test->getObjects(list);
//    for (auto it : list) {
//        qDebug() << it.email();
//    }
//    test->add(User("test@mail.ru", "pass",
//                  "pophost", "smtphost",
//                  123, 1233));
//    test->exportStorage();
//    delete test;

    MainWindow w;
    return a.exec();
}

