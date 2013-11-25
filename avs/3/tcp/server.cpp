#include "server.h"

// public

Server::Server(QString hostname, qint16 port, QString directory, QString msg)
    : hostname_(hostname), port_(port), directory_(directory), msg_(msg) {
    LOG << "Server init.";

//    timer_->setInterval(500);
//    connect(timer_, SIGNAL(timeout()), this, SLOT(send()));
}
Server::~Server() {
    LOG << "Base class destructed";
}


//void Server::start(QString hostname, qint16 port) {
//    LOG << "Start server (message).";

////    if (filename.isNull()) {
////        mode_ = message;
////    } else {
////        mode_ = file;
////        filename_ = filename;
////        emit sent(0);
////    }
//}

// protected

QString Server::hostname() const {
    return hostname_;
}

void Server::hostname(QString hostname) {
    hostname_ = hostname;
}

quint16 Server::port() const {
    return port_;
}

void Server::port(quint16 port) {
    port_ = port;
}

QString Server::directory() const {
    return directory_;
}
void Server::directory(QString directory) {
    directory_ = directory;
}

QString Server::msg() const {
    return msg_;
}
void Server::msg(QString msg) {
    msg_ = msg;
}

// private

//void Server::send() {
//    LOG << "Send (message).";

//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_4_0);
//    out << QString("hello msg");
//    out << rand();

//    tcpSocket_->write(block);
//}

//void Server::sendFile() {
//    LOG << "Send file.";

//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_4_0);
//    out << file_->fileName().split("/").back();
//    out << file_->size();

//    tcpSocket_->write(block);

//    QByteArray bytes = file_->readAll();

//    tcpSocket_->write(bytes);
//    tcpSocket_->flush();
//    emit bytesSent(bytes.size());

//}

