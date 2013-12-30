#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
QDialog(parent),
ui_(new Ui::Dialog), coder_(new Coder()) {
    ui_->setupUi(this);
    ui_->blackEdit->setText(QString::number(3));
    ui_->whiteEdit->setText(QString::number(6));
    ui_->kdsRB->setChecked(true);
}

Dialog::~Dialog() {
    delete ui_;
}

void Dialog::on_codeButton_clicked() {
    coder_->setBlackLength(ui_->blackEdit->text().toInt());
    coder_->setWhiteLength(ui_->whiteEdit->text().toInt());
    if (ui_->kdsRB->isChecked())
        coder_->setShow(0);
    if (ui_->apkRB->isChecked())
        coder_->setShow(1);
    if (ui_->haffmanRB->isChecked())
        coder_->setShow(2);

    ui_->codeTB->setText(coder_->code(ui_->seriesTB->toPlainText()));

    ui_->kdsLabel->setText(coder_->getKDS());
    ui_->apkLabel->setText(coder_->getAPK());
    ui_->haffmanLabel->setText(coder_->getHaffman());
}
