#ifndef UDPSERVER_H
#define UDPSERVER_H
#include "server.h"

#include <QDataStream>
#include <QDebug>
#include <QTimer>
#include <QFile>


#undef LOG
#define LOG qDebug() << "UDPServer: "


class UDPServer : public Server
{
public:
    UDPServer(QString hostname = "127.0.0.1", qint16 port = 0,
              QString directory = "/tmp/", QString msg = "Hello");
    ~UDPServer();

    void start();
    void stop();

private:
//    QTcpServer *tcpServer_;

private slots:
    void startSocket();
    void readClient();
    void sendMsg(QAbstractSocket *socket);

};

#endif // UDPSERVER_H
