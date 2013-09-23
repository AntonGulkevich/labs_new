#ifndef CODE_H
#define CODE_H
#include <QDebug>
#include <QList>
#include <QString>
#include <QVariant>
#define LOG qDebug() << "Code::"

class Code : public QObject
{
    Q_OBJECT
public:
    Code(const QString);
    enum level {H, HM, M, LM, L};
    enum type {NRZ, NRZI, Manchester, AMI, _2B1Q, HDB3 };
    void setSeq(const QString);
    QList<Code::level> getLevels(Code::type) const;

private:
    QString sequence_;
public slots:
    void textChanged(QString);
signals:
    void sequenceUpdated();

};

#endif // CODE_H
