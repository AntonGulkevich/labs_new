#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <QFile>

#undef LOG
#define LOG qDebug() << "Client: "

class Client : public QObject {

    Q_OBJECT
public:
    enum estatus_ {
        started, stopped
    };

    enum emode_ {
        file, message
    };

    Client();
    ~Client();
    estatus_ status() const;
    emode_ mode() const;
    void start(QString, qint16, QString filename = QString::null);
    void stop();
private:
    QString hostname_;
    qint16 port_;
    QString path_;
    QTcpSocket *tcpSocket_;
    QFile *file_;
    estatus_ status_;
    emode_ mode_;
    struct {
        qint64 get;
        qint64 need;
    } bytes_;
private slots:
    void recieve() const;
    void recieveFile();
    void saveFile();
signals:
    void statusChanged(Client::estatus_, Client::emode_);
    void recieved(int);
};

#endif // CLIENT_H
