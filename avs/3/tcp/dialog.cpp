#include "dialog.h"
#include "ui_dialog.h"

/* public ***********************************************/
Dialog::Dialog(QWidget *parent) :
QDialog(parent),
  ui_(new Ui::Dialog), Server_(0), Client_(0) {
    LOG << "Dialog init.";

    ui_->setupUi(this);

    QNetworkInterface *inter = new QNetworkInterface();
    QList<QHostAddress> list;
    list = inter->allAddresses();
    QString hostname = "";
    for (int i = 0; i < list.size(); ++i) {
        QHostAddress tmp(list.at(i).toIPv4Address());
        if (tmp.toString().compare("127.0.0.1") && tmp.toString().compare("0.0.0.0"))
            hostname = tmp.toString();
    }

//    ui_->serverIPEdit->setText(hostname);
    ui_->serverIPEdit->setText("127.0.0.1");
    ui_->serverPortEdit->setText("11111");
    ui_->serverMsgEdit->setText("Hello");
//    ui_->serverFileEdit->setText("/home/dn/yabss.crt");

    ui_->clientIPEdit->setText("127.0.0.1");
    ui_->clientPortEdit->setText("11111");
    ui_->clientFileWidget->hide();
//    ui_->recieveFileEdit->setText("/tmp/");

}

Dialog::~Dialog() {
    LOG << "Dialog destruct.";
    if (Server_)
        delete Server_;

    if (Client_)
        delete Client_;

    delete ui_;
}

/* private slots ***********************************************/
// server side

//void Dialog::on_sendFileTB_clicked() {
//    LOG << "sendFileTB was clicked!";

//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//            "/home/dn");
//    if (fileName.isEmpty())
//        return;
//    ui_->sendFileEdit->setText((fileName));
//}


void Dialog::on_serverButton_clicked() {
    LOG << "serverButton was clicked!";
    QString proto = ui_-> serverProtoCB->currentText();
    QString hostname = ui_->serverIPEdit->text();
    quint16 port = ui_->serverPortEdit->text().toInt();
    QString message = ui_->serverMsgEdit->text();
    QString directory = ui_->serverDirEdit->text();

    if (Server_ == 0) {
        if (proto == "TCP") {
            Server_ = new TCPServer(hostname, port, directory, message);

            connect(Server_, SIGNAL(statusChanged(bool)),
                    this, SLOT(setServerUI(bool)));
            connect(Server_, SIGNAL(bytesSent(int)),
                    this, SLOT(setSentBytes(int)));
        }

        if (proto == "UDP") {

        }
    }

    if (!Server_->isRun()) {
        Server_->start();
        ui_->serverCommonWidget->setDisabled(true);
    } else {
        Server_->stop();
        ui_->serverCommonWidget->setDisabled(false);
        delete Server_;
        Server_ = 0;
    }
}


void Dialog::setServerUI(bool status) {
    LOG << "setServerUI.";
    ui_->serverButton->setText((status) ? "Stop" : "Start");
}

void Dialog::setSentBytes(int bytes) {
    LOG << "setSentBytes";
    ui_->serverSentBytesValue->setText("Bytes sent: " + QString::number(bytes));
}


// client side

//void Dialog::on_recieveFileTB_clicked() {
//    LOG << "recieveFileTB was clicked!";

//    QString folderName = QFileDialog::getExistingDirectory(this, tr("Recieve Folder"),
//            "/home/dn");
//    if (folderName.isEmpty())
//        return;
//    ui_->recieveFileEdit->setText(folderName);
//}


void Dialog::on_clientButton_clicked() {
    LOG << "clientButton was clicked!";
    QString proto = ui_->clientProtoCB->currentText();
    QString hostname = ui_-> clientIPEdit->text();
    quint16 port = ui_->clientPortEdit->text().toInt();
    QString filename = ui_->clientFileEdit->text();

    if (Client_ == 0) {
        if (proto == "TCP") {
            Client_ = new TCPClient(hostname, port);

                connect(Client_, SIGNAL(statusChanged(bool)),
                        this, SLOT(setClientUI(bool)));
                connect(Client_, SIGNAL(bytesRecieved(int)),
                        this, SLOT(setRecievedBytes(int)));
                connect(Client_, SIGNAL(msgRecieved(QString)),
                        this, SLOT(setRecievedMsg(QString)));
               Client_->start();
        }

        if (proto == "UDP") {

        }
    } else {
        if (Client_->isRun())  {
            Client_->stop();
            ui_->clientCommonWidget->setDisabled(false);
            delete Client_;
            Client_ = 0;
        }
    }

}


void Dialog::on_clientModeCB_currentIndexChanged(const QString &string) {
    LOG << "clientMode changed";
    if (string == "Message") {
        ui_->clientMsgWidget->show();
        ui_->clientFileWidget->hide();
    } else {
        ui_->clientMsgWidget->hide();
        ui_->clientFileWidget->show();
    }
}

void Dialog::setClientUI(bool status) {
    LOG << "setClientButton().";
     ui_->clientButton->setText((status) ? "Stop" : "Start");
     ui_->clientCommonWidget->setDisabled(status);
}

void Dialog::setRecievedBytes(int bytes) {
    LOG << "setRecievedBytes().";
    ui_->clientRecievedBytesValue->setText("Bytes recieved: " + QString::number(bytes));
}

void Dialog::setRecievedMsg(QString msg) {
    LOG << "setRecievedMsg().";
    ui_->clientMsgEdit->append(msg);
}
