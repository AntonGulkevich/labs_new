#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QTableWidget>
#include <QHeaderView>
#include "user.h"
#include "login.h"
#include "newmessage.h"
#include "storage.h"
#include "openmessage.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui_;

    Login *Login_;
    User *User_;
    Storage<Message> *popStorage_;
    Storage<Message> *smtpStorage_;

    void closeEvent(QCloseEvent *);
    void showTable(QTableWidget *tableWidget, Storage<Message> *storage, bool isTo = true);
    void showRecord(QTableWidget *tableWidget, Message *message, int index, bool isTo);
    Message& parsePOP3Message(QString stringMessage);

private slots:
    void loginDialog_finished(int code);
    void on_actionNew_Message_triggered();
    void on_actionGet_Mail_triggered();
    void on_actionDelete_triggered();
    void on_actionAbout_triggered();
    void on_smtpTW_doubleClicked(const QModelIndex &index);
};

#endif // MAINWINDOW_H
