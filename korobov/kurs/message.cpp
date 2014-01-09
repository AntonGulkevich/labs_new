#include "message.h"
// public

Message::Message()
: from_(QString::null), to_(QString::null),
subj_(QString::null), body_(QString::null),
files_(QStringList()), datetime_(QDateTime()),
messageId_(QString::null), read_(true) {
}

// public

Message::Message(const QString &from, const QString &to,
        const QString &subj, const QString &body,
        const QStringList &files, QDateTime &datetime,
        QString messageId, bool read)
: from_(from), to_(to),
subj_(subj), body_(body),
files_(files), datetime_(datetime),
messageId_(messageId), read_(read) {
    id_ = QString(QCryptographicHash::hash(
            from_.toUtf8() + to_.toUtf8() + datetime_.toString().toUtf8(),
            QCryptographicHash::Md5).toHex());
}
// public

Message::Message(const Message& orig)
: from_(orig.from_), to_(orig.to_),
subj_(orig.subj_), body_(orig.body_),
files_(orig.files_), datetime_(orig.datetime_),
id_(orig.id_),
messageId_(orig.messageId_), read_(orig.read_) {
}

Message::~Message() {
    qDebug() << "destruct message";
}

// public

QString Message::from() const {
    return from_;
}
// public

QString Message::to() const {
    return to_;
}
// public

QString Message::subj() const {
    return subj_;
}
// public

QString Message::body() const {
    return body_;
}
// public

QStringList Message::files() const {
    return files_;
}
// public

QDateTime Message::datetime() const {
    return datetime_;
}
// public

QString Message::id() const {
    return id_;
}
// public

bool Message::read() const {
    return read_;
}
// public

void Message::read(bool read) {
    read_ = read;
}
// public

QString Message::messageId() const {
    return messageId_;
}
// public

void Message::messageId(QString messageId) {
    messageId_ = messageId;
}

//public

QDataStream& operator<<(QDataStream& out, const Message& object) {
    out << object.from() << object.to()
            << object.subj() << object.body()
            << object.files() << object.datetime()
            << object.messageId() << object.read();
    return out;
}

// public

QDataStream& operator>>(QDataStream& in, Message& object) {

    QString from, to;
    QString subj, body;
    QStringList files;
    QDateTime datetime;
    bool read;
    QString messageId;
    in >> from >> to >> subj >> body >> files >> datetime >> messageId >> read;
    object = Message(from, to, subj, body, files, datetime, messageId, read);
    return in;
}
