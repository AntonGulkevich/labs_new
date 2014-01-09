#include "user.h"

// public

User::User()
: email_(QString::null), password_(QString::null),
popHost_(QString::null), smtpHost_(QString::null),
popPort_(0), smtpPort_(0) {
}

// public

User::User(const QString &email, const QString &password,
        const QString &popHost, const QString &smtpHost,
        const quint16 &popPort, const quint16 &smtpPort)
: email_(email), password_(password),
popHost_(popHost), smtpHost_(smtpHost),
popPort_(popPort), smtpPort_(smtpPort),
auth_(false) {
    id_ = QString(QCryptographicHash::hash(
            email_.toUtf8() + popHost_.toUtf8() + smtpHost_.toUtf8(),
            QCryptographicHash::Md5).toHex());
}

// public

User::User(const User& orig)
: email_(orig.email_), password_(orig.password_),
popHost_(orig.popHost_), smtpHost_(orig.smtpHost_),
popPort_(orig.popPort_), smtpPort_(orig.smtpPort_),
id_(orig.id_) {

}

// public

QString User::email() const {
    return email_;
}
// public

QString User::password() const {
    return password_;
}
// public

QString User::popHost() const {
    return popHost_;
}
// public

QString User::smtpHost() const {
    return smtpHost_;
}
// public

quint16 User::popPort() const {
    return popPort_;
}
// public

quint16 User::smtpPort() const {
    return smtpPort_;
}
// public

bool User::auth() const {
    return auth_;
}
// public

void User::auth(bool auth) {
    auth_ = auth;
}
// public

QString User::id() const {
    return id_;
}
//public

QDataStream& operator<<(QDataStream& out, const User& object) {
    out << object.email() << object.password()
            << object.popHost() << object.smtpHost()
            << object.popPort() << object.smtpPort();
    return out;
}

// public

QDataStream& operator>>(QDataStream& in, User& object) {

    QString email, password;
    QString popHost, smtpHost;
    quint16 popPort, smtpPort;
    in >> email >> password >> popHost >> smtpHost >> popPort >> smtpPort;
    object = User(email, password, popHost, smtpHost, popPort, smtpPort);
    return in;
}

