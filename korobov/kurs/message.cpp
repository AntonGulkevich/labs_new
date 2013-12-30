#include "message.h"
// public
Message::Message()
    : from_(QString::null), to_(QString::null),
      subj_(QString::null), body_(QString::null),
      files_(QStringList()), time_(QTime()), read_(true)
{
}

// public
Message::Message(const QString &from, const QString &to,
                 const QString &subj, const QString &body,
                 const QStringList &files, QTime &time, bool read)
    : from_(from), to_(to),
      subj_(subj), body_(body),
      files_(files), time_(time), read_(read)
{
    id_ = QString( QCryptographicHash::hash(
                       from_.toUtf8() + to_.toUtf8() + time_.toString().toUtf8(),
                       QCryptographicHash::Md5).toHex() );
}
// public
Message::Message(const Message& orig)
    : from_(orig.from_), to_(orig.to_),
      subj_(orig.subj_), body_(orig.body_),
      files_(orig.files_), time_(orig.time_),
      id_(orig.id_), read_(orig.read_)

{
}

Message::~Message() {
    qDebug() << "destruct message";
}

// public
QString Message::from() const { return from_; }
// public
QString Message::to() const { return to_; }
// public
QString Message::subj() const { return subj_; }
// public
QString Message::body() const { return body_; }
// public
QStringList Message::files() const { return files_; }
// public
QTime Message::time() const { return time_; }
// public
QString Message::id() const { return id_; }
// public
bool Message::read() const { return read_; }
// public
void Message::read(bool read) { read_ = read; }

//public
QDataStream& operator<< (QDataStream& out, const Message& object)
{
    out << object.from() << object.to()
        << object.subj() << object.body()
        << object.files() << object.time()
        << object.read();
    return out;
}

// public
QDataStream& operator>> (QDataStream& in, Message& object)
{

    QString from, to;
    QString subj, body;
    QStringList files;
    QTime time;
    bool read;
    in >> from >> to >> subj >> body >> files >> time >> read;
    object = Message(from, to, subj, body, files, time, read);
    return in;
}
