#ifndef DISASSEMBLER_H
#define	DISASSEMBLER_H

#include <QStringList>
#include <QFile>
#include <QMap>
#include <QDebug>

#undef LOG
#define LOG qDebug() << "disassembler: "
class Disassembler {
public:
    Disassembler(const QString &);
    void makeStrings();
    void writeStrings(const QString &);
    virtual ~Disassembler();
private:
    int size(quint64) const;

    QMap<qint8, QMap<bool, QString> > reg_; // registers
    QMap<qint8, QString> sr_; // segment registers
    QMap<quint64, QString> com1_, com2_;

    QStringList asmCommands_;

    QList< quint64 > hexCommands_;
};

#endif	/* DISASSEMBLER_H */

