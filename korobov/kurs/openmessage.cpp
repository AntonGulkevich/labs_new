#include "openmessage.h"
#include "ui_openmessage.h"

OpenMessage::OpenMessage(Message *message, QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::OpenMessage), Message_(message)
{
    ui_->setupUi(this);
    ui_->toLE->setText(message->to());
    ui_->subjectLE->setText(message->subj());
    ui_->timeLE->setText(message->time().toString());
    ui_->messageTE->setText(message->body());

    QString attach = "";
    foreach (QString str, message->files()) {
        attach.append(str);
    }
    ui_->attachLE->setText(attach);

}

OpenMessage::~OpenMessage()
{
    qDebug() << "open message destructed";
    delete ui_;
}
