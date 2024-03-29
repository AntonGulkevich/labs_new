#ifndef POP3CLIENT_H
#define POP3CLIENT_H

#include <QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>

class POP3Client : public QObject {
    Q_OBJECT
public:
    POP3Client(const QString &email, const QString &password,
               const QString &host, quint16 port, int timeout = 30000);
    ~POP3Client();

    bool init();
    bool login();
    bool getMsgList(QList< QPair<quint64, quint64> >& list);
    //    bool getMsgList(QString msgId, QPair<QString,int>& uIdList);
    bool getMessageTop(QString msgId, int nbLines, QString& msgTop);
    bool getMessage(quint64 msgId, quint64 msgSize, QString& msg);
    bool removeMessage(QString msgId);
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
    QString doCommand(QString command, quint64 bytesAvailable = 0);
    bool readResponse(QString& response, quint64 bytesAvailable = 0);
    bool sendUser(QString& user);
    bool sendPasswd(QString& password);

private slots:
    void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
};

#endif // POP3CLIENT_H
