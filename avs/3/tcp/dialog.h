#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QNetworkInterface>
#include "server.h"
#include "client.h"

#undef LOG
#define LOG qDebug() << "Dialog: "

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    // server side
    void on_sendFileTB_clicked();
    void on_sendFileButton_clicked();
    void on_sendButton_clicked();
    void setSendButton(Server::estatus_, Server::emode_);
    void setSentBytes(int);

    // client side
    void on_recieveFileTB_clicked();
    void on_recieveFileButton_clicked();
    void on_recieveButton_clicked();
    void setRecieveButton(Client::estatus_, Client::emode_);
    void setRecievedBytes(int);

private:

    Ui::Dialog *ui_;
    Server *tcpServer_;
    Client *tcpClient_;

    QString current_;
    quint16 blockSize_;

};

#endif // DIALOG_H

