#ifndef CODER_H
#define CODER_H
#include <QDebug>
#include <QStringList>
#include <QtCore/qmath.h>
#define LOG qDebug() << "Coder::"

class Coder {
public:
    Coder();
    ~Coder();

    void setBlackLength(int);
    void setWhiteLength(int);
    QString code(QString);
    QString getKDS() const;
    QString getAPK() const;
    QString getHaffman() const;
    void setShow(int);

private:
    int black_;
    int white_;

    struct koef {
        double kds;
        double apk;
        double haffman;
    } *koef_;

    int show_;
    QMap<int, QMap<bool, QString> > H;

    void H_init();

};

#endif // CODER_H
