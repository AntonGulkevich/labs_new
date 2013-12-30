#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <cmath>

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
    void on_pAliceCB_currentIndexChanged(int index);
    void on_qAliceCB_currentIndexChanged(int index);
    void on_pBobCB_currentIndexChanged(int index);
    void on_qBobCB_currentIndexChanged(int index);

    void on_aliceLW_currentRowChanged(int currentRow);
    void on_bobLW_currentRowChanged(int currentRow);

    void on_codeAndSendAlicePB_clicked();
    void on_codeAndSendBobPB_clicked();
    void on_encodeBobPB_clicked();
    void on_encodeAlicePB_clicked();

private:
    Ui::Dialog *ui_;
    QStringList primes_;

    struct par {
        int p;
        int q;
        int n;
        int k;
        int e;
        int d;
        int phi;
        QList< QPair<int, int> > ed_list;
    };

    par alice_;
    par bob_;

    void on_alice_pq_changed();
    void on_bob_pq_changed();
};

#endif // DIALOG_H
