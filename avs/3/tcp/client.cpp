#include "client.h"

// public

Client::Client(QString hostname, qint16 port)
: hostname_(hostname), port_(port) {
    LOG << "Client init.";
}

Client::~Client() {
    LOG << "Base class destruct.";
}


// protected

QString Client::hostname() const {
    return hostname_;
}

void Client::hostname(QString hostname) {
    hostname_ = hostname;
}

quint16 Client::port() const {
    return port_;
}

void Client::port(quint16 port) {
    port_ = port;
}

// private


// private

// private slots

//void Client::recieve() const {
//    LOG << "Recieve.";

//    QDataStream in(tcpSocket_);
//    in.setVersion(QDataStream::Qt_5_1);
//    if (tcpSocket_->bytesAvailable() > 0) {
//        QString next;
//        in >> next;
//        qDebug() << next;
//        int value;
//        in >> value;
//        qDebug() << "recieve" << " " << value;
//    }
//}

//void Client::recieveFile() {
//    LOG << "Recieve file.";

//    QDataStream in(tcpSocket_);
//    in.setVersion(QDataStream::Qt_5_1);

//    QString filename;

//    if (!file_) {
//        in >> filename;
//        in >> bytes_.need;
//        LOG << "File not exists";
//        file_  = new QFile(path_ + filename);
//        file_->open(QIODevice::WriteOnly);
//        LOG << "created file: " << file_->fileName();
//    }

//    if (tcpSocket_->bytesAvailable() > 0) {
//        QByteArray read = tcpSocket_->read(tcpSocket_->bytesAvailable());
//        file_->write(read,read.size());
//        bytes_.get += read.size();
//    }

//    if (bytes_.need == bytes_.get)
//        stop();
//}

//void Client::saveFile() {
//    LOG << "Save file";
//    stop();
//    file_->close();
//    emit recieved(file_->size());
//    delete file_;
//}

