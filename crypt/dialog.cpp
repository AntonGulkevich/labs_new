#include "dialog.h"
#include "ui_dialog.h"
#include "alg.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::Dialog)
{
    ui_->setupUi(this);
    primes_ = getPrimes(100000);
    primes_.insert(0, "Not selected");
    ui_->pAliceCB->addItems(primes_);
    ui_->qAliceCB->addItems(primes_);
    ui_->pBobCB->addItems(primes_);
    ui_->qBobCB->addItems(primes_);

    //    ui_->pAliceCB->setCurrentIndex(ui_->pAliceCB->findText("131"));
    //    ui_->qAliceCB->setCurrentIndex(ui_->qAliceCB->findText("139"));

    //    ui_->pBobCB->setCurrentIndex(ui_->pBobCB->findText("139"));
    //    ui_->qBobCB->setCurrentIndex(ui_->qBobCB->findText("149"));

    //    ui_->inputAliceTE->setText("192 70 106 59 177 123");
    //


    // send button block
    ui_->codeAndSendAlicePB->setEnabled(false);
    ui_->codeAndSendBobPB->setEnabled(false);

}

Dialog::~Dialog()
{
    delete ui_;
}

void Dialog::on_pAliceCB_currentIndexChanged(int index) {
    if (index > 0) {
        on_alice_pq_changed();
        ui_->qAliceCB->removeItem(ui_->qAliceCB->findText(ui_->pAliceCB->itemText(index)));
    }
}

void Dialog::on_qAliceCB_currentIndexChanged(int index) {
    if (index > 0) {
        on_alice_pq_changed();
        ui_->pAliceCB->removeItem(ui_->pAliceCB->findText(ui_->qAliceCB->itemText(index)));
    }
}

void Dialog::on_pBobCB_currentIndexChanged(int index) {
    if (index > 0) {
        bob_.p = ui_->pBobCB->currentText().toInt();

        ui_->qBobCB->clear();
        ui_->qBobCB->addItems(getPrimesByK(1000, alice_.k, bob_.p));
        ui_->qBobCB->removeItem(ui_->qBobCB->findText(ui_->pBobCB->itemText(index)));
    }
}

void Dialog::on_qBobCB_currentIndexChanged(int index) {
    if (index > 0) {
        on_bob_pq_changed();
        ui_->pBobCB->removeItem(ui_->pBobCB->findText(ui_->qBobCB->itemText(index)));
    }
}

void Dialog::on_alice_pq_changed() {
    qDebug() << "alice pq changed";
    ui_->aliceLW->clear();

    alice_.p = ui_->pAliceCB->currentText().toInt();
    alice_.q = ui_->qAliceCB->currentText().toInt();

    alice_.n = alice_.p * alice_.q;

    if (alice_.n != 0) {
        alice_.k = floor(log2(alice_.n));
        ui_->nAliceValueLabel->setText(QString::number(alice_.n));
        ui_->kAliceValueLabel->setText(QString::number(alice_.k));

        if (alice_.k != bob_.k) {
            qDebug() << "clear!";
            ui_->qBobCB->clear();
            ui_->qBobCB->addItems(getPrimesByK(1000, alice_.k, bob_.p));
            ui_->qBobCB->setCurrentIndex(0);
            qDebug() << "alice pq changed; bobcb size: " << ui_->qBobCB->count();
        }

        alice_.phi = (alice_.p - 1) * (alice_.q - 1) + 1; // = e * d

        alice_.ed_list = getDivisors(alice_.phi);
        qDebug() << alice_.ed_list.size();

        for (QList< QPair<int, int> >::Iterator it = alice_.ed_list.begin(); it != alice_.ed_list.end(); ++it) {
            ui_->aliceLW->addItem(QString("E: %1; D: %2").arg(it->first).arg(it->second));
        }
    }
}

void Dialog::on_bob_pq_changed() {
    qDebug() << "bob pq changed";
    ui_->bobLW->clear();


    bob_.q = ui_->qBobCB->currentText().toInt();

    bob_.n = bob_.p * bob_.q;

    bob_.k = floor(log2(bob_.n));
    ui_->nBobValueLabel->setText(QString::number(bob_.n));
    ui_->kBobValueLabel->setText(QString::number(bob_.k));
    bob_.phi = (bob_.p - 1) * (bob_.q - 1) + 1; // = e * d

    bob_.ed_list = getDivisors(bob_.phi);
    qDebug() << bob_.ed_list.size();

    for (QList< QPair<int, int> >::Iterator it = bob_.ed_list.begin(); it != bob_.ed_list.end(); ++it) {
        ui_->bobLW->addItem(QString("E: %1; D: %2").arg(it->first).arg(it->second));
    }
}

void Dialog::on_aliceLW_currentRowChanged(int currentRow) {

    if (ui_->aliceLW->currentIndex().row() != -1 && ui_->bobLW->currentIndex().row() != -1) {
        ui_->codeAndSendAlicePB->setEnabled(true);
        ui_->codeAndSendBobPB->setEnabled(true);
    } else {
        ui_->codeAndSendAlicePB->setEnabled(false);
        ui_->codeAndSendBobPB->setEnabled(false);
    }

    if (-1 < currentRow && currentRow < alice_.ed_list.size()) {
        alice_.e = alice_.ed_list.at(currentRow).first;
        alice_.d = alice_.ed_list.at(currentRow).second;
    }

    qDebug() << "E: " << alice_.e << " D : " << alice_.d;
}

void Dialog::on_bobLW_currentRowChanged(int currentRow) {

    if (ui_->aliceLW->currentIndex().row() != -1 && ui_->bobLW->currentIndex().row() != -1) {
        ui_->codeAndSendAlicePB->setEnabled(true);
        ui_->codeAndSendBobPB->setEnabled(true);
    } else {
        ui_->codeAndSendAlicePB->setEnabled(false);
        ui_->codeAndSendBobPB->setEnabled(false);
    }

    if (-1 < currentRow && currentRow < bob_.ed_list.size()) {
        bob_.e = bob_.ed_list.at(currentRow).first;
        bob_.d = bob_.ed_list.at(currentRow).second;
    }
    qDebug() << "E: " << bob_.e << " D : " << bob_.d;
}

void Dialog::on_codeAndSendAlicePB_clicked() {
    QString aliceInput = ui_->inputAliceTE->toPlainText();
    QStringList outputWords;

    if (ui_->modeAliceCB->currentText() == "Numbers") {
        QStringList inputWords = aliceInput.split(" ");
        for (QStringList::Iterator it = inputWords.begin(); it != inputWords.end(); ++it) {
            int code1 = coder(it->toInt(), bob_.e, bob_.n);
            int code2 = coder(code1, alice_.d, alice_.n);
            outputWords << QString::number(code2);
        }
    } else {
        for (int i = 0; i < aliceInput.length(); ++i) {
            int code1 = coder(aliceInput.at(i).unicode(), bob_.e, bob_.n);
            int code2 = coder(code1, alice_.d, alice_.n);
            outputWords << QString::number(code2);
        }
    }

    ui_->outputAliceTE->setText(outputWords.join(" "));
    ui_->inputBobTE->setText(outputWords.join(" "));
}

void Dialog::on_codeAndSendBobPB_clicked() {
    QString bobInput = ui_->inputBobTE->toPlainText();
    QStringList outputWords;
    if (ui_->modeAliceCB->currentText() == "Numbers") {
        QStringList inputWords = bobInput.split(" ");
        for (QStringList::Iterator it = inputWords.begin(); it != inputWords.end(); ++it) {
            int code1 = coder(it->toInt(), alice_.e, alice_.n);
            int code2 = coder(code1, bob_.d, bob_.n);
            outputWords << QString::number(code2);
        }
    } else {
        for (int i = 0; i < bobInput.length(); ++i) {
            int code1 = coder(bobInput.at(i).unicode(), alice_.e, alice_.n);
            int code2 = coder(code1, bob_.d, bob_.n);
            outputWords << QString::number(code2);
        }
    }

    ui_->outputBobTE->setText(outputWords.join(" "));
    ui_->inputAliceTE->setText(outputWords.join(" "));
}

void Dialog::on_encodeAlicePB_clicked() {
    QString aliceInput = ui_->inputAliceTE->toPlainText();
    QStringList inputWords = aliceInput.split(" ");
    QStringList outputWords;
    for (QStringList::Iterator it = inputWords.begin(); it != inputWords.end(); ++it) {
        int code1 = coder(it->toInt(), bob_.e, bob_.n);
        int code2 = coder(code1, alice_.d, alice_.n);

        if (ui_->modeAliceCB->currentText() == "Numbers") {
            outputWords << QString::number(code2);
        } else {
            outputWords << QChar(code2);
        }
    }

    if (ui_->modeAliceCB->currentText() == "Numbers")
        ui_->outputAliceTE->setText(outputWords.join(" "));
    else
        ui_->outputAliceTE->setText(outputWords.join(""));
}

void Dialog::on_encodeBobPB_clicked() {
    QString bobInput = ui_->inputBobTE->toPlainText();
    QStringList inputWords = bobInput.split(" ");
    QStringList outputWords;
    for (QStringList::Iterator it = inputWords.begin(); it != inputWords.end(); ++it) {
        int code1 = coder(it->toInt(), alice_.e, alice_.n);
        int code2 = coder(code1, bob_.d, bob_.n);
        if (ui_->modeBobCB->currentText() == "Numbers") {
            outputWords << QString::number(code2);
        } else {
            outputWords << QChar(code2);
        }
    }

    if (ui_->modeBobCB->currentText() == "Numbers")
        ui_->outputBobTE->setText(outputWords.join(" "));
    else
        ui_->outputBobTE->setText(outputWords.join(""));
}

