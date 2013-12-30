#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::Dialog),
    mode_(0), TCPServer_(0), udpSocket_(0)
{
    ui_->setupUi(this);
    ui_->ipLE->setText("127.0.0.1");
    ui_->portLE->setText("11111");
//    sendFile_ = new QFile("/home/dn/Downloads/MSLabs.zip");
    sendFile_ = new QFile("/home/dn/1.zip");
    qDebug() << sendFile_->size();
    qDebug() << floor(sendFile_->size() / DATAGRAM_SIZE);
}

Dialog::~Dialog()
{
    delete ui_;
    delete sendFile_;
}

void Dialog::startTCP() {
    qDebug() << "starting tcp server";

    TCPServer_ = new TCPServer(this, ip_, port_, sendFile_);
    TCPServer_->start();
}

void Dialog::startUDP() {
    qDebug() << "starting udp sender";
    udpSocket_ = new QUdpSocket();
    udpSocket_->bind(QHostAddress("127.0.0.1"), port_);
    connect(udpSocket_, SIGNAL(readyRead()), this, SLOT(recieveDatagram()));
}

void Dialog::processDatagram(QByteArray datagram) {
    qDebug() << "process obtained datagram";

    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_1);
    QString type;
    in >> type;
    qDebug() << type;
    if (type == "OK") {
        if (sendFile_->exists()) {
            sendDatagram(QString("NAME"), QFileInfo(sendFile_->fileName()).fileName());
        }

    } else if (type == "GET") {
        sendFile_->open(QFile::ReadOnly);
        QString n;
        in >> n;
        qDebug() <<  n;
        if (floor(sendFile_->size() / DATAGRAM_SIZE) < n.toDouble()) {
            sendDatagram(QString("END"));
        } else {
            sendFile_->seek(DATAGRAM_SIZE * n.toInt());
            QByteArray package = sendFile_->read(DATAGRAM_SIZE);
            sendDatagram(QString("DATA"), n, package);
        }
        sendFile_->close();
    }
}

void Dialog::on_startPB_clicked() {
    qDebug() << "start button clicked!";
    ip_ = QHostAddress(ui_->ipLE->text());
    port_ = ui_->portLE->text().toInt();

    ui_->startPB->setText("Stop");

    qDebug() << mode_;
    if (mode_ == 0) {
        startTCP();
    }
    if (mode_ == 1) {
        startUDP();
    }

}

void Dialog::on_modeCB_currentIndexChanged(int mode) {
    qDebug() << "mode changed";
    mode_ = mode;
}

void Dialog::recieveDatagram() {
    QByteArray datagram;
    while (udpSocket_->hasPendingDatagrams()) {
        datagram.resize(udpSocket_->pendingDatagramSize());
        udpSocket_->readDatagram(datagram.data(), datagram.size(), &ip_);
    }
    processDatagram(datagram);

}

void Dialog::sendDatagram(QString type, QString info, QByteArray package) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_1);
    out << type;
    if (info != QString::null) {
        out << info;
        if (!data.isNull()) {
            out << package;
        }
    }
    sendDatagram(data);
}

void Dialog::sendDatagram(QByteArray data) {
    udpSocket_->writeDatagram(data.data(), data.size(), ip_, port_);
}
