#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include "client.h"

#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QTimer>
#include <QFile>

#undef LOG
#define LOG qDebug() << "TCPClient: "

class TCPClient : public Client
{
public:
    TCPClient(QString hostname, qint16 port);
    ~TCPClient();

    bool isRun() const;
    void start();
    void stop();
private:
    QTcpSocket *tcpSocket_;

private slots:
    void socketConnected();
    void socketError(QAbstractSocket::SocketError error);
    void readServer();

    // messages
    void sendM_INIT();

};

#endif // TCPCLIENT_H


