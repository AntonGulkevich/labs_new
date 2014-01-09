#ifndef USER_H
#define USER_H
#include <QString>
#include <QHostAddress>
#include <QCryptographicHash>
#include <QDataStream>

class User {
public:
    User();
    User(const QString &email, const QString &password,
            const QString &popHost, const QString &smtpHost,
            const quint16 &popPort, const quint16 &smtpPort);
    User(const User& orig);

    QString email() const;
    QString password() const;
    QString popHost() const;
    QString smtpHost() const;
    quint16 popPort() const;
    quint16 smtpPort() const;
    bool auth() const;
    void auth(bool auth);
    QString id() const;
    //    friend QDataStream& operator<< (QDataStream& out, const User& object);
    //    friend QDataStream& operator<< (QDataStream& in, User& object);


private:
    QString email_;
    QString password_;
    QString popHost_;
    QString smtpHost_;
    quint16 popPort_;
    quint16 smtpPort_;
    QString id_;
    bool auth_;
};
QDataStream& operator<<(QDataStream& out, const User& object);
QDataStream& operator>>(QDataStream& in, User& object);
#endif // USER_H
