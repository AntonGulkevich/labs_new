#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTimer>
#include <QTableWidget>
#include <QHeaderView>
#include <QCloseEvent>
#include <QTimer>
#include <QRegExp>

#include "user.h"
#include "login.h"
#include "storage.h"
#include "message.h"
#include "writemessage.h"
#include "readmessage.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
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
    Message parsePOP3Message(QString stringMessage);

private slots:
    void startLogin();
    void on_actionNew_Message_triggered();
    void on_actionGet_Mail_triggered();
    void on_actionAbout_triggered();
    void on_smtpTW_doubleClicked(const QModelIndex &index);
    void on_popTW_doubleClicked(const QModelIndex &index);
    void on_actionDelete_triggered();
};

#endif // MAINWINDOW_H
