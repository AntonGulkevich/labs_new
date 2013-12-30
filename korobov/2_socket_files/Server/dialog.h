#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QHostAddress>
#include <QDebug>
#include <QUdpSocket>
#include <QFile>
#include <cmath>
#include "tcpserver.h"

#define DATAGRAM_SIZE 1024

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui_;
    int mode_;
    QHostAddress ip_;
    quint16 port_;
    QFile *sendFile_;

    TCPServer *TCPServer_;
    QUdpSocket *udpSocket_;

    void startTCP();
    void startUDP();
    void processDatagram(QByteArray datagram);

private slots:
    void on_startPB_clicked();
    void on_modeCB_currentIndexChanged(int mode);
    void recieveDatagram();
    void sendDatagram(QString type, QString info = QString::null, QByteArray package = 0);
    void sendDatagram(QByteArray data);
};

#endif // DIALOG_H
