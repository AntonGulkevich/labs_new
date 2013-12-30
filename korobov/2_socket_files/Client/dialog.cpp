#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::Dialog), wait_(new QTimer())
{
    ui_->setupUi(this);
    ui_->ipLE->setText("127.0.0.1");
    ui_->portLE->setText("11111");
    mode_ = ui_->modeCB->currentText();

}

Dialog::~Dialog()
{
    delete ui_;
}

void Dialog::startTCP() {
    tcpSocket_ = new QTcpSocket(this);
    connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(tcpReadData()));
    tcpSocket_->abort();
    tcpSocket_->connectToHost(ip_, port_);
}

void Dialog::startUDP() {
    qDebug() << "start udp";
    udpSocket_ = new QUdpSocket();
    udpSocket_->bind(QHostAddress("127.0.0.2"), port_);
    connect(udpSocket_, SIGNAL(readyRead()), this, SLOT(recieveDatagram()));
    sendDatagram(QString("OK"));
    wait_->setInterval(300);
    connect(wait_, SIGNAL(timeout()), this, SLOT(resendDatagram()));

}

void Dialog::processDatagram(QByteArray datagram) {
    qDebug() << "process obtained datagram";

    QDataStream in(&datagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_1);
    QString type;
    in >> type;
    if (type == "NAME") {
        QString filename;
        in >> filename;
        qDebug() << filename;
        file_.setFileName("/tmp/" + filename);
        lastRecievedN_ = -1;
        sendDatagram(QString("GET"), QString::number(0));
    }

    if (type == "DATA") {

        wait_->stop();
        QString n;
        in >> n;
        if (lastRecievedN_ + 1 == n.toInt()) {
            QByteArray package;
            in >> package;

            file_.open(QIODevice::Append);
            file_.write(package);
            file_.close();
            ++lastRecievedN_;
        }

        sendDatagram(QString("GET"), QString::number(lastRecievedN_ + 1));
        wait_->start();
        qDebug() << "get Data";
    }

    if (type == "END") {
        wait_->stop();
    }
}

void Dialog::tcpReadData() {
    qDebug() << "call";
    QDataStream in(tcpSocket_);
    in.setVersion(QDataStream::Qt_5_1);
    if (!file_.exists()) {
        QString filename;
        in >> filename;
        file_.setFileName("/tmp/" + QString::number(rand()) + filename);
    }

    file_.open(QIODevice::Append);

    QByteArray line = tcpSocket_->readAll();
    file_.write(line);
    file_.close();
}

void Dialog::on_startPB_clicked() {
    ip_ = QHostAddress(ui_->ipLE->text());
    port_ = ui_->portLE->text().toInt();

    ui_->startPB->setText("Stop");

    if (mode_ == "TCP") {
        startTCP();
    }
    if (mode_ == "UDP") {
        startUDP();
    }
}

void Dialog::on_modeCB_currentIndexChanged(const QString &mode) {
    mode_ = mode;
    qDebug() << "index changed;";
}


void Dialog::recieveDatagram() {
    qDebug() << "recieve";
    QByteArray datagram;
    while (udpSocket_->hasPendingDatagrams()) {
        datagram.resize(udpSocket_->pendingDatagramSize());
        udpSocket_->readDatagram(datagram.data(), datagram.size());
    }
    processDatagram(datagram);
}

void Dialog::sendDatagram(QString type, QString info) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_1);
    out << type;
    if (info != QString::null) {
        out << info;
    }
    sendDatagram(data);
}

void Dialog::sendDatagram(QByteArray data) {
    udpSocket_->writeDatagram(data.data(), data.size(), ip_, port_);
}
void Dialog::resendDatagram() {
    qDebug() << "resend datagram";
    sendDatagram(QString("GET"), QString::number(lastRecievedN_ + 1));
}
