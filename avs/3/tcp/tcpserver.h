#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "server.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QDebug>
#include <QTimer>
#include <QFile>

#undef LOG
#define LOG qDebug() << "TCPServer: "

class TCPServer : public Server
{
public:
    TCPServer(QString hostname = "127.0.0.1", qint16 port = 0,
              QString directory = "/tmp/", QString msg = "Hello");
    ~TCPServer();

    bool isRun() const;
    void start();
    void stop();

private:
    QTcpServer *tcpServer_;
    void sendMsg(QAbstractSocket *socket);
    void sendFile(QAbstractSocket *socket, const QString filename);

private slots:
    void startSocket();
    void readClient();
    // messages
    void sendM_DATA(QAbstractSocket *socket);
};

#endif // TCPSERVER_H
