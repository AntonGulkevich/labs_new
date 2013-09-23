#include "client.h"

// public

Client::Client()
: hostname_(QString::null), port_(0), path_(QString::null),
  tcpSocket_(new QTcpSocket()), file_(0),
status_(stopped), mode_(message) {
    LOG << "Client init.";


}

Client::~Client() {
    LOG << "Client destruct.";

    if (tcpSocket_->isOpen())
        tcpSocket_->close();
    delete tcpSocket_;
}

Client::estatus_ Client::status() const {
    return status_;
}

Client::emode_ Client::mode() const {
    return mode_;
}

void Client::start(QString hostname, qint16 port, QString path) {
    LOG << "Start client (message)";

    hostname_ = hostname;
    port_ = port;
    if (path.isNull()) {
        mode_ = message;
        connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(recieve()));
    } else {
        mode_ = file;
        path_ = path;
        bytes_.get = 0;
        bytes_.need = 0;
        connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(recieveFile()));
        connect(tcpSocket_, SIGNAL(disconnected()), this, SLOT(saveFile()));
        emit recieved(0);
    }

    tcpSocket_->abort();
    tcpSocket_->connectToHost(hostname_, port_, QIODevice::ReadWrite);
    status_ = started;
    emit statusChanged(status_, mode_);
}

void Client::stop() {
    qDebug() << "Stop client";
    if (tcpSocket_->isOpen())
        tcpSocket_->close();
    status_ = stopped;
    emit statusChanged(status_, mode_);

}
// private

// private slots

void Client::recieve() const {
    LOG << "Recieve.";

    QDataStream in(tcpSocket_);
    in.setVersion(QDataStream::Qt_4_0);
    if (tcpSocket_->bytesAvailable() > 0) {
        QString next;
        in >> next;
        qDebug() << next;
        int value;
        in >> value;
        qDebug() << "recieve" << " " << value;
    }
}

void Client::recieveFile() {
    LOG << "Recieve file.";

    QDataStream in(tcpSocket_);
    in.setVersion(QDataStream::Qt_4_0);

    QString filename;

    if (!file_) {
        in >> filename;
        in >> bytes_.need;
        LOG << "File not exists";
        file_  = new QFile(path_ + filename);
        file_->open(QIODevice::WriteOnly);
        LOG << "created file: " << file_->fileName();
    }

    if (tcpSocket_->bytesAvailable() > 0) {
        QByteArray read = tcpSocket_->read(tcpSocket_->bytesAvailable());
        file_->write(read,read.size());
        bytes_.get += read.size();
    }

    if (bytes_.need == bytes_.get)
        stop();
}

void Client::saveFile() {
    LOG << "Save file";
    stop();
    file_->close();
    emit recieved(file_->size());
    delete file_;
}

