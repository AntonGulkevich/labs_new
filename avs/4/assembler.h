#ifndef ASSEMBLER_H
#define	ASSEMBLER_H
#include <QStringList>
#include <QFile>
#include <QMap>
#include <QDebug>

#undef LOG
#define LOG qDebug() << "assembler: "

using namespace std;
class Assembler {
public:
    Assembler(const QString &);
    void makeHEX();
    void writeHEX(const QString &);

    virtual ~Assembler();
private:
    QMap<QString, qint8> reg_; // registers
    QMap<QString, qint8> sr_; // segment registers
    QMap<QString, bool> w_; // word bit

    typedef QMap<QString, QMap<QString, quint64> > com1;
    typedef QMap<QString, QMap<QString, QMap<QString, quint64> > > com2;

    com1 com1_;
    com2 com2_;
    typedef QList< QStringList > com;
    com asmCommands_;
    QStringList hexCommands_;
};

#endif	/* ASSEMBLER_H */
