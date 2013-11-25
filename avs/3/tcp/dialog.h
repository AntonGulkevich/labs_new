#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QNetworkInterface>
#include "tcpserver.h"
#include "tcpclient.h"
#include "udpserver.h"
#include "udpclient.h"

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
//    void on_sendFileTB_clicked();
    void on_serverButton_clicked();
    void setServerUI(bool status);
    void setSentBytes(int bytes);

    // client side
//    void on_recieveFileTB_clicked();
    void on_clientButton_clicked();
    void on_clientModeCB_currentIndexChanged(const QString &string);
    void setClientUI(bool status);
    void setRecievedBytes(int bytes);
    void setRecievedMsg(QString msg);

private:

    Ui::Dialog *ui_;
    Server *Server_;
    Client *Client_;

};

#endif // DIALOG_H

