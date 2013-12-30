#include "tcpserver.h"

TCPServer::TCPServer(QObject *parent, QHostAddress &ip, quint16 port, QFile *sendFile):
    QTcpServer(parent), ip_(ip), port_(port), sendFile_(sendFile) {

}

TCPServer::~TCPServer() {
}

void TCPServer::start() {
    if (!this->listen(ip_, port_))  {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port_ << "...";
    }
}

void TCPServer::incomingConnection(qintptr socketDescriptor) {
    qDebug() << "incomingConnection: " << socketDescriptor;
    qDebug() << sendFile_->size();
    TCPThread *thread = new TCPThread(socketDescriptor, sendFile_, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), this, SLOT(finishThread()));
    thread->start();
}

void TCPServer::finishThread() {
    qDebug() << "finish transmit";
}
