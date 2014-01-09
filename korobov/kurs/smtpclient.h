#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include <QTime>
#include <QCryptographicHash>

class SMTPClient : public QObject {
    Q_OBJECT
public:
    SMTPClient(const QString &email, const QString &password,
            const QString &host, quint16 port, int timeout = 30000);

    ~SMTPClient();

    bool send(const QString &from, const QString &to,
            const QString &subject, const QString &body, QStringList files);
    bool init();
    bool login();
    bool quit();

private:
    QTextStream *stream_;
    QSslSocket *socket_;

    QString from_;
    QString rcpt_;
    QString response_;
    QString message_;


    QString email_;
    QString password_;
    QString host_;
    quint16 port_;
    int timeout_;

    enum states {
        Init, HandShake, Auth, User, Pass, Mail, Rcpt, Data, Body, Quit, Close
    };

    int state_;
    QString doCommand(QString command);
    bool readResponse(QString& response);
    void unexpectedResponse(QString response);

    // protocol
    bool init_msg();
    bool handshake_msg();
    bool auth_msg();
    bool user_msg();
    bool pass_msg();
    bool mail_msg();
    bool rcpt_msg();
    bool data_msg();
    bool body_msg();
    bool quit_msg();

private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
    //    void readyRead();


};

#endif // SMTPCLIENT_H
