#ifndef SERVER_H
#define SERVER_H

#include <QString>
#include <QAbstractSocket>

#include "const.h"

#undef LOG
#define LOG qDebug() << "Server: "

class Server : public QObject {

    Q_OBJECT
public:
    Server(QString hostname, qint16 port, QString directory, QString msg);
    virtual ~Server();

    virtual bool isRun() const = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

protected:
    QString hostname() const;
    void hostname(QString hostname);
    quint16 port() const;
    void port(quint16 port);
    QString directory() const;
    void directory(QString directory);
    QString msg() const;
    void msg(QString msg);

private:
    QString hostname_;
    qint16 port_;
    QString directory_;
    QString msg_;

    virtual void sendMsg(QAbstractSocket *socket) = 0;
    virtual void sendFile(QAbstractSocket *socket, const QString filename) = 0;

private slots:
    virtual void startSocket() = 0;
    virtual void readClient() = 0;
    // messages
    virtual void sendM_DATA(QAbstractSocket *socket) = 0;

signals:
    void statusChanged(bool status);
    void bytesSent(int bytes);

};

#endif // SERVER_H
