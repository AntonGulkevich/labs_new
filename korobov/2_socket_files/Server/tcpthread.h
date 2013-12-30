#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>

class TCPThread : public QThread
{
    Q_OBJECT
public:
    TCPThread(int socketDescriptor,  QFile *sendFile, QObject *parent);
    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor_;
    QByteArray data;
};

#endif // TCPTHREAD_H
