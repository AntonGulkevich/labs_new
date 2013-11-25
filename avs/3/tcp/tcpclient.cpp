#include "tcpclient.h"

// public
TCPClient::TCPClient(QString hostname, qint16 port)
    :Client(hostname, port), tcpSocket_(new QTcpSocket())
{
    LOG << "TCPClient constructed.";
}

TCPClient::~TCPClient() {
    LOG << "Client destruct.";


    if (tcpSocket_->isOpen())
        tcpSocket_->close();
    delete tcpSocket_;
}

bool TCPClient::isRun() const {
    return tcpSocket_->isOpen();
}

void TCPClient::start() {
    LOG << "Start TCPClient.";

    tcpSocket_->connectToHost(hostname(), port());

    connect(tcpSocket_, SIGNAL(connected()),
            this, SLOT(socketConnected()));
    connect(tcpSocket_, SIGNAL(readyRead()),
            this, SLOT(readServer()));
    connect(tcpSocket_, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));

}

void TCPClient::stop() {
    LOG << "Stop TCPClient.";

    if (tcpSocket_->isOpen()) {
        tcpSocket_->close();
        emit(statusChanged(tcpSocket_->isOpen()));
    }

}

/*
 *

void Client::start() {
    LOG << "Start client (message)";

//    if (path.isNull()) {
//        mode_ = message;
//        connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(recieve()));
//    } else {
//        mode_ = file;
////        path_ = path;
//        bytes_.get = 0;
//        bytes_.need = 0;
//        connect(tcpSocket_, SIGNAL(readyRead()), this, SLOT(recieveFile()));
//        connect(tcpSocket_, SIGNAL(disconnected()), this, SLOT(saveFile()));
//        emit recieved(0);
//    }

    tcpSocket_->abort();
    tcpSocket_->connectToHost(hostname_, port_, QIODevice::ReadWrite);
    status_ = started;
    emit statusChanged(status_);
}

void Client::stop() {
    qDebug() << "Stop client";
    if (tcpSocket_->isOpen())
        tcpSocket_->close();
    status_ = stopped;
    emit statusChanged(status_);

}
 */

// private


// private slots

void TCPClient::socketConnected() {
    LOG << "socketConnected().";
    sendM_INIT();
    emit(statusChanged(tcpSocket_->isOpen()));
}

void TCPClient::socketError(QAbstractSocket::SocketError error)
{
    QString string = "Error: ";
    switch (error) {
    case QAbstractSocket::HostNotFoundError:
        string+= "The host was not found!";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        string+= "The remote host is closed!";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        string+= "The connection was refused!";
    default: string+= tcpSocket_->errorString();
    }

    LOG << string;
    emit(statusChanged(tcpSocket_->isOpen()));
}

void TCPClient::readServer() {
    LOG << "readServer().";
    QDataStream in(tcpSocket_);
    in.setVersion(QDataStream::Qt_5_1);
    while (tcpSocket_->bytesAvailable() > 0) {
        quint16 size;
        in >> size;

        quint8 msgType;
        in >> msgType;

        switch (msgType) {
            case t_msg::TCP_M_DATA: {
                quint16 label;
                in >> label;
                QString msg;
                in >> msg;
                LOG << label << msg;
                emit(msgRecieved(QString::number(label) + " " + msg));
                QTimer::singleShot(500, this, SLOT(sendM_INIT()));

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

void TCPClient::sendM_INIT() {
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_1);

    out << quint16(0); // offset
    out << quint8(t_msg::TCP_M_INIT); // msg_type field

    out << quint16(data.size() - sizeof(quint16)); // msg_length field

    tcpSocket_->write(data);
}
