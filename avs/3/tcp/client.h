#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QDataStream>
#include <QDebug>
#include <QTimer>
#include <QFile>

#include "const.h"

#undef LOG
#define LOG qDebug() << "Client: "

class Client : public QObject {

    Q_OBJECT
public:

    Client(QString hostname, qint16 port);
    virtual ~Client();

    virtual bool isRun() const = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

protected:
    QString hostname() const;
    void hostname(QString hostname);
    quint16 port() const;
    void port(quint16 port);

private:
    QString hostname_;
    qint16 port_;
//    QString filename_;

//    QString path_;
    QAbstractSocket *tcpSocket_;
//    QFile *file_;

//    struct {
//        qint64 get;
//        qint64 need;
//    } bytes_;

private slots:
    virtual void socketConnected() = 0;
    virtual void socketError(QAbstractSocket::SocketError error) = 0;
    virtual void readServer() = 0;
    // messages
    virtual void sendM_INIT() = 0;
//    virtual void readMsg() = 0;
//    virtual void readFile() = 0;

//    void recieve() const;
//    void recieveFile();
//    void saveFile();
signals:
    void statusChanged(bool status);
    void bytesRecieved(int bytes);
    void msgRecieved(QString msg);
};

#endif // CLIENT_H
