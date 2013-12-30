#include "tcpthread.h"

TCPThread::TCPThread(int socketDescriptor,  QFile *sendFile, QObject *parent)
    : QThread(parent), socketDescriptor_(socketDescriptor)
{
    qDebug() << "create thread!!";

    if(sendFile->open(QFile::ReadOnly)){
        qDebug() << "file exists";

        QDataStream out(&data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_1);
        out << QFileInfo(sendFile->fileName()).fileName();

        QByteArray fileData = sendFile->readAll();
        data.append(fileData);
        sendFile->close();
    }
}

void TCPThread::run() {
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor_)) {
        emit error(tcpSocket.error());
        return;
    }

    qint64 x = 0;
    while(x < data.size()){
        qint64 y= tcpSocket.write(data);
        x+= y;
        qDebug()<< x << " to: " << socketDescriptor_;
    }

    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();


}
