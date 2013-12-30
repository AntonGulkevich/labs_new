#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QTime>
#include <QDebug>
class Message
{
public:
    Message();
    Message(const QString &from, const QString &to,
            const QString &subj, const QString &body,
            const QStringList &files, QTime &time, bool read = true);
    Message(const Message& message);
    ~Message();

    QString from() const;
    QString to() const;
    QString subj() const;
    QString body() const;
    QStringList files() const;
    QTime time() const;
    QString id() const;
    bool read() const;
    void read(bool read);

private:
    QString from_;
    QString to_;
    QString subj_;
    QString body_;
    QStringList files_;
    QTime time_;
    QString id_;
    bool read_;

};
QDataStream& operator<< (QDataStream& out, const Message& object);
QDataStream& operator>> (QDataStream& in, Message& object);

#endif // MESSAGE_H