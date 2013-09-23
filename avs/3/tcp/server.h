#ifndef SERVER_H
#define SERVER_H

#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <QFile>

#undef LOG
#define LOG qDebug() << "Server: "

class Server : public QObject {

    Q_OBJECT
public:
    enum estatus_ {
        started, stopped
    };

    enum emode_ {
        file, message
    };

    Server();
    ~Server();
    estatus_ status() const;
    emode_ mode() const;
    bool isRun();
    void start(QString, qint16, QString filename = QString::null);
    void stop();
private:

    QString hostname_;
    qint16 port_;
    QString filename_;
    QTcpServer *tcpServer_;
    QTcpSocket *tcpSocket_;
    estatus_ status_;
    emode_ mode_;
    QTimer *timer_;
    QFile *file_;

private slots:
    void send();
    void sendFile();
    void startSocket();
    void stopSocket();

signals:
    void statusChanged(Server::estatus_, Server::emode_);
    void sent(int);
};

#endif // SERVER_H
