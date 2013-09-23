#include "server.h"

// public

Server::Server()
: hostname_(QString::null), port_(0), filename_(QString::null),
tcpServer_(new QTcpServer()), tcpSocket_(0),
status_(stopped), mode_(message), timer_(new QTimer()), file_(0) {
    LOG << "Server init.";

    timer_->setInterval(500);

    connect(timer_, SIGNAL(timeout()), this, SLOT(send()));
}

Server::~Server() {
    LOG << "Server destruct.";

    if (tcpServer_->isListening())
        tcpServer_->close();
    delete tcpServer_;

    if (timer_->isActive())
        timer_->stop();
    delete timer_;

    if (file_) {
        if (file_->isOpen())
            file_->close();
        delete file_;
    }
}

Server::estatus_ Server::status() const {
    return status_;
}

Server::emode_ Server::mode() const {
    return mode_;
}

void Server::start(QString hostname, qint16 port, QString filename) {
    LOG << "Start server (message).";

    hostname_ = hostname;
    port_ = port;
    if (filename.isNull()) {
        mode_ = message;
    } else {
        mode_ = file;
        filename_ = filename;
        emit sent(0);
    }

    tcpServer_->listen(QHostAddress(hostname_), port_);
    connect(tcpServer_, SIGNAL(newConnection()), this, SLOT(startSocket()), Qt::UniqueConnection);

    if (tcpServer_->isListening()) {
        status_ = started;
        emit statusChanged(status_, mode_);
    }
}

void Server::stop() {
    LOG << "Stop server.";

    if (tcpSocket_ && tcpSocket_->isOpen()) {
        tcpSocket_->abort();
        tcpSocket_->close();
    }

    if (tcpServer_->isListening())
        tcpServer_->close();

    status_ = stopped;
    emit statusChanged(status_, mode_);
    disconnect(this, SLOT(stopSocket()));

    timer_->stop();
}

// private

void Server::send() {
    LOG << "Send (message).";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << QString("hello msg");
    out << rand();

    tcpSocket_->write(block);
}

void Server::sendFile() {
    LOG << "Send file.";

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << file_->fileName().split("/").back();
    out << file_->size();

    tcpSocket_->write(block);

    QByteArray bytes = file_->readAll();

    tcpSocket_->write(bytes);
    tcpSocket_->flush();
    emit sent(bytes.size());

}

void Server::startSocket() {
    LOG << "Start socket.";

    tcpSocket_ = tcpServer_->nextPendingConnection();
    connect(tcpSocket_, SIGNAL(disconnected()), this, SLOT(stopSocket()));

    if (mode_ == message) {

        timer_->start();
    } else {
        file_ = new QFile(filename_);
        if (file_->open(QIODevice::ReadOnly)) {
            sendFile();
        }
    }
}

void Server::stopSocket() {
    LOG << "Stop socket.";

    stop();
}

