#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
QDialog(parent),
ui_(new Ui::Dialog), coder_(new Coder()) {
    ui_->setupUi(this);
//    ui_->blackEdit->setText(QString::number(3));
//    ui_->whiteEdit->setText(QString::number(5));
//    ui_->seriesTB->setText("6 7 64 128\n0 14 31 2 158");
    ui_->blackEdit->setText(QString::number(2));
    ui_->whiteEdit->setText(QString::number(6));
    ui_->seriesTB->setText("1396 31 31 192 63 15\n0 3 127 7 255 640 696");
}

Dialog::~Dialog() {
    delete ui_;
}

void Dialog::on_codeButton_clicked() {
    coder_->setBlackLength(ui_->blackEdit->text().toInt());
    coder_->setWhiteLength(ui_->whiteEdit->text().toInt());
    if (ui_->rleRB->isChecked())
        coder_->setShow(0);
    if (ui_->posRB->isChecked())
        coder_->setShow(1);
    if (ui_->haffmanRB->isChecked())
        coder_->setShow(2);

    ui_->codeTB->setText(coder_->code(ui_->seriesTB->toPlainText()));

    ui_->rleLabel->setText(coder_->getRLE());
    ui_->posLabel->setText(coder_->getPos());
    ui_->haffmanLabel->setText(coder_->getHaffman());
}

void Dialog::on_encodeButton_clicked() {
    coder_->setBlackLength(ui_->blackEdit->text().toInt());
    coder_->setWhiteLength(ui_->whiteEdit->text().toInt());
}
