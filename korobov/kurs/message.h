#ifndef MESSAGE_H
#define MESSAGE_H
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
class Message
{
public:
    Message();
    Message(const QString &from, const QString &to,
            const QString &subj, const QString &body,
            const QStringList &files, QDateTime &time,
            QString messageId = QString::null, bool read = true);
    Message(const Message& message);
    ~Message();

    QString from() const;
    QString to() const;
    QString subj() const;
    QString body() const;
    QStringList files() const;
    QDateTime datetime() const;
    QString id() const;
    QString messageId() const;
    void messageId(QString);
    bool read() const;
    void read(bool read);

private:
    QString from_;
    QString to_;
    QString subj_;
    QString body_;
    QStringList files_;
    QDateTime datetime_;
    QString id_;
    QString messageId_;
    bool read_;

};
QDataStream& operator<< (QDataStream& out, const Message& object);
QDataStream& operator>> (QDataStream& in, Message& object);

#endif // MESSAGE_H
