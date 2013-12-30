#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "tcpthread.h"

class TCPServer : QTcpServer
{
    Q_OBJECT
public:
    TCPServer(QObject *parent, QHostAddress &ip, quint16 port, QFile *sendFile);
    ~TCPServer();
    void start();

protected:
    void incomingConnection(qintptr socketDescriptor);

private:
    QHostAddress ip_;
    quint16 port_;
    QFile *sendFile_;
private slots:
    void finishThread();
};

#endif // TCPSERVER_H
