#include "udpserver.h"

UDPServer::UDPServer(QString hostname, qint16 port, QString directory, QString msg)
    : Server(hostname, port, directory, msg)
{
    LOG << "UDPServer constructed";
}

UDPServer::~UDPServer() {
        LOG << "Server destruct.";

//        if (UDPServer_->isListening())
//            UDPServer_->close();

//        delete UDPServer_;
}

void UDPServer::start() {
//    LOG << "Start UDPServer";
//    UDPServer_->listen(QHostAddress(hostname()), port());
//    connect(UDPServer_, SIGNAL(newConnection()), this, SLOT(startSocket()), Qt::UniqueConnection);

//    if (UDPServer_->isListening()) {
//        emit statusChanged(true);
//    }
}

void UDPServer::stop() {

}
// private

void UDPServer::sendMsg(QAbstractSocket *socket) {
//    LOG << "sendMsg()";
//    QTcpSocket *tcpSocket = dynamic_cast<QTcpSocket *>(socket);

//    QByteArray data;
//    QDataStream out(&data, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_5_1);
//    out << QString(msg());
//    out << rand();

}

// private slots

void UDPServer::startSocket() {
    LOG << "Start TCP socket.";

//    QTcpSocket *tcpSocket = UDPServer_->nextPendingConnection();

//    connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));
//    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readClient()));


//    if (mode_ == message) {

//        timer_->start();
//    } else {
//        file_ = new QFile(filename_);
//        if (file_->open(QIODevice::ReadOnly)) {
//            sendFile();
//        }
//    }
}


void UDPServer::readClient() {
//    QTcpSocket *tcpSocket = (QTcpSocket *)sender();
////    QTcpSocket *tcpSocket = qobject_cast<QTcpSocket *>sender();
//    QDataStream in(tcpSocket);
//    in.setVersion(QDataStream::Qt_5_1);

//    while (tcpSocket->bytesAvailable() > 0) {
//        emsgType msgType;
//        in >> msgType;
//        switch (msgType) {
//        case TCP_MSG: {
//            QTimer timer;
//            timer.setInterval(500);
//            connect(&timer, SIGNAL(timeout()), this, SLOT(sendMsg(tcpSocket)));
//            timer.start();
//            break;
//        }
//        case TCP_FILE: {
//            sendFile();
//        }
//        default:
//            break;
//        }
//    }
}
