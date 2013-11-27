#include "tcpserver.h"

TCPServer::TCPServer(QString hostname, qint16 port, QString directory, QString msg)
    : Server(hostname, port, directory, msg), tcpServer_(new QTcpServer())
{
    LOG << "TCPServer constructed";
}

TCPServer::~TCPServer() {
        LOG << "Server destruct.";

        if (tcpServer_->isListening())
            tcpServer_->close();

        delete tcpServer_;
}

bool TCPServer::isRun() const {
    return tcpServer_->isListening();
}

void TCPServer::start() {
    LOG << "Start TCPServer.";

    tcpServer_->listen(QHostAddress(hostname()), port());
    connect(tcpServer_, SIGNAL(newConnection()), this, SLOT(startSocket()), Qt::UniqueConnection);

    if (tcpServer_->isListening()) {
        emit statusChanged(tcpServer_->isListening());
    }
}

void TCPServer::stop() {
    LOG << "Stop TCPServer.";
    if (tcpServer_->isListening()) {
        tcpServer_->close();
        emit statusChanged(tcpServer_->isListening());
    }
}

// private
void TCPServer::sendMsg(QAbstractSocket *socket) {
    LOG << "sendMsg()";
    sendM_DATA(socket);
}

void TCPServer::sendFile(QAbstractSocket *socket, const QString filename) {
    LOG << "sendFile()";
}

// private slots

void TCPServer::startSocket() {
    LOG << "Start TCP socket.";

    QTcpSocket *tcpSocket = tcpServer_->nextPendingConnection();

    connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readClient()));


//    if (mode_ == message) {

//        timer_->start();
//    } else {
//        file_ = new QFile(filename_);
//        if (file_->open(QIODevice::ReadOnly)) {
//            sendFile();
//        }
//    }
}


void TCPServer::readClient() {
    QTcpSocket *tcpSocket = (QTcpSocket *)sender();
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_5_1);

    while (tcpSocket->bytesAvailable() > 0) {
        quint8 msgType;
        in >> msgType;
        switch (msgType) {
            case t_msg::TCP_M_INIT: {
                sendMsg(tcpSocket);
                break;
            }
//        case TCP_FILE: {
//            sendFile();
//        }
        default:
            break;
        }
    }
}

// messages
void TCPServer::sendM_DATA(QAbstractSocket *socket) {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_1);

    out << quint16(0); // offset
    out << quint8(t_msg::TCP_M_DATA); // msg_type field

    out << quint16(rand());
    out << QString(msg());

    out.device()->seek(0);
    out << quint16(data.size() - sizeof(quint16)); // msg_length field

    socket->write(data);
}

