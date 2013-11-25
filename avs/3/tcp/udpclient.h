#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include "client.h"

#include <QUdpSocket>
#include <QDataStream>
#include <QDebug>
#include <QTimer>
#include <QFile>

#undef LOG
#define LOG qDebug() << "UDPClient: "

class UDPClient : public Client
{
public:
    UDPClient(QString hostname, qint16 port);
    ~UDPClient();

    bool isRun() const;
    void start();
    void stop();
private:
    QUdpSocket *udpSocket_;

private slots:
    void startSocket();
    void slotConnected();
    void readServer();
};

#endif // UDPCLIENT_H
