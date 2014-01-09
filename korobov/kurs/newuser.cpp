#include "newuser.h"
#include "ui_newuser.h"

NewUser::NewUser(User **User, QWidget *parent) :
QDialog(parent),
User_(User),
ui_(new Ui::NewUser) {
    ui_->setupUi(this);
    ui_->emailLE->setText("redmine.dcti@gmail.com");
    ui_->passwordLE->setText("AdminRedmineDCTI");
    ui_->smtpHostLE->setText("smtp.googlemail.com");
    ui_->smtpPortLE->setText("465");
    ui_->popHostLE->setText("pop.googlemail.com");
    ui_->popPortLE->setText("995");
}

NewUser::~NewUser() {
    qDebug() << "destruct newuser";
    delete ui_;
}

// private slot

void NewUser::on_okPB_clicked() {
    *User_ = new User(ui_->emailLE->text(), ui_->passwordLE->text(),
            ui_->popHostLE->text(), ui_->smtpHostLE->text(),
            ui_->popPortLE->text().toInt(), ui_->smtpPortLE->text().toInt());
    if (Login::checkConnect(*User_)) {
        accept();
    }
}

//private slot

void NewUser::on_closePB_clicked() {
    reject();
}
