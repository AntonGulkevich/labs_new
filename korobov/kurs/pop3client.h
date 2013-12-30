#ifndef POP3CLIENT_H
#define POP3CLIENT_H

#include <QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>

class POP3Client : public QObject
{
    Q_OBJECT
public:
    POP3Client(const QString &email, const QString &password,
               const QString &host, quint16 port, int timeout = 30000);
    ~POP3Client();

    bool init();
    bool login();
    bool getMsgList(QStringList& uIdList);
//    bool getMsgList(QString msgId, QPair<QString,int>& uIdList);
    bool getMessageTop(QString msgId, int nbLines, QString& msgTop);
    bool getMessage(QString msgId, QString& msg);
    bool quit();

signals:
    void status(QString status);
    void status();

private:
    QSslSocket *socket_;
    bool auth_;

    QString email_;
    QString password_;
    QString host_;
    quint16 port_;
    int timeout_;

    enum states {
        NotConnected, Authorization, Transaction, Update
    };

    int state_;
    QString doCommand(QString command);
    bool readResponse(QString& response);
    bool sendUser(QString& user);
    bool sendPasswd(QString& password);

private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
};

#endif // POP3CLIENT_H