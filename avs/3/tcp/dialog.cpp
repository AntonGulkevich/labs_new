#include "dialog.h"
#include "ui_dialog.h"

/* public ***********************************************/
Dialog::Dialog(QWidget *parent) :
QDialog(parent),
ui_(new Ui::Dialog) {
    LOG << "Dialog init.";

    ui_->setupUi(this);
    tcpServer_ = new Server();
    tcpClient_ = new Client();

    connect(tcpServer_, SIGNAL(statusChanged(Server::estatus_, Server::emode_)),
            this, SLOT(setSendButton(Server::estatus_, Server::emode_)));
    connect(tcpClient_, SIGNAL(statusChanged(Client::estatus_, Client::emode_)),
            this, SLOT(setRecieveButton(Client::estatus_, Client::emode_)));
    connect(tcpServer_, SIGNAL(sent(int)),
            this, SLOT(setSentBytes(int)));
    connect(tcpClient_, SIGNAL(recieved(int)),
            this, SLOT(setRecievedBytes(int)));


    QNetworkInterface *inter = new QNetworkInterface();
    QList<QHostAddress> list;
    list = inter->allAddresses();
    QString hostname = "";
    for (int i = 0; i < list.size(); ++i) {
        QHostAddress tmp(list.at(i).toIPv4Address());
        if (tmp.toString().compare("127.0.0.1") && tmp.toString().compare("0.0.0.0"))
            hostname = tmp.toString();
    }


    ui_->sendIPEdit->setText(hostname);
    ui_->sendPortEdit->setText("11111");
    ui_->sendFileEdit->setText("/home/dn/yabss.crt");

    ui_->recieveIPEdit->setText("127.0.0.1");
    ui_->recievePortEdit->setText("11111");
    ui_->recieveFileEdit->setText("/tmp/");

}

Dialog::~Dialog() {
    LOG << "Dialog destruct.";
    delete tcpServer_;
    delete tcpClient_;
    delete ui_;
}

/* private slots ***********************************************/
// server side

void Dialog::on_sendFileTB_clicked() {
    LOG << "sendFileTB was clicked!";

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
            "/home/dn");
    if (fileName.isEmpty())
        return;
    ui_->sendFileEdit->setText((fileName));
}

void Dialog::on_sendFileButton_clicked() {
    LOG << "sendFileButton was clicked!";

    QString hostname = ui_->sendIPEdit->text();
    qint16 port = ui_->sendPortEdit->text().toInt();
    QString filename = ui_->sendFileEdit->text();

    if (tcpServer_->status() == Server::stopped) {
        tcpServer_->start(hostname, port, filename);
    } else {
        tcpServer_->stop();
    }

}

void Dialog::on_sendButton_clicked() {
    LOG << "sendButton was clicked!";

    QString hostname = ui_->sendIPEdit->text();
    qint16 port = ui_->sendPortEdit->text().toInt();

    if (tcpServer_->status() == Server::stopped) {
        tcpServer_->start(hostname, port);
    } else {
        tcpServer_->stop();
    }
}

void Dialog::setSendButton(Server::estatus_ status, Server::emode_ mode) {
    LOG << "setSendButton.";
    QString strStatus = "";
    if (status == Server::stopped)
        strStatus = "Start";
    else
        strStatus = "Stop";

    if (mode == Server::message)
        ui_->sendButton->setText(strStatus);
    else
        ui_->sendFileButton->setText(strStatus);
}

void Dialog::setSentBytes(int bytes) {
    LOG << "setSentBytes";

    ui_->sentBytesLabel->setText("Bytes sent: " + QString::number(bytes));
}


// client side

void Dialog::on_recieveFileTB_clicked() {
    LOG << "recieveFileTB was clicked!";

    QString folderName = QFileDialog::getExistingDirectory(this, tr("Recieve Folder"),
            "/home/dn");
    if (folderName.isEmpty())
        return;
    ui_->recieveFileEdit->setText(folderName);
}

void Dialog::on_recieveFileButton_clicked() {
    LOG << "recieveFileButton was clicked!";

    QString hostname = ui_->recieveIPEdit->text();
    qint16 port = ui_->recievePortEdit->text().toInt();
    QString filename = ui_->recieveFileEdit->text();

    if (tcpClient_->status() == Client::stopped) {
        tcpClient_->start(hostname, port, filename);
    } else {
        tcpClient_->stop();
    }

}

void Dialog::on_recieveButton_clicked() {
    LOG << "recieveButton was clicked!";

    QString hostname = ui_->recieveIPEdit->text();
    qint16 port = ui_->recievePortEdit->text().toInt();

    if (tcpClient_->status() == Client::stopped) {
        tcpClient_->start(hostname, port);
    } else {
        tcpClient_->stop();
    }
}

void Dialog::setRecieveButton(Client::estatus_ status, Client::emode_ mode) {
    LOG << "setRecieveButton.";
    QString strStatus = "";
    if (status == Client::stopped)
        strStatus = "Start";
    else
        strStatus = "Stop";

    if (mode == Client::message)
        ui_->recieveButton->setText(strStatus);
    else
        ui_->recieveFileButton->setText(strStatus);
}

void Dialog::setRecievedBytes(int bytes) {
    LOG << "setRecievedBytes";

    ui_->recievedBytesLabel->setText("Bytes recieved: " + QString::number(bytes));
}
