#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QFile>
#include <QTimer>

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

private slots:


private:
    Ui::Dialog *ui_;
    QString mode_;
    QHostAddress ip_;
    QHostAddress localIP_;
    quint16 port_;

    QTcpSocket *tcpSocket_;
    QUdpSocket *udpSocket_;
    QFile file_;
    int lastRecievedN_;
    QTimer *wait_;

    void startTCP();
    void startUDP();
    void processDatagram(QByteArray datagram);

private slots:
    void tcpReadData();
    void on_startPB_clicked();
    void on_modeCB_currentIndexChanged(const QString &mode);
    void recieveDatagram();
    void sendDatagram(QString type, QString info = QString::null);
    void sendDatagram(QByteArray data);
    void resendDatagram();

};

#endif // DIALOG_H
