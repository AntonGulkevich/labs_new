#include "readmessage.h"
#include "ui_readmessage.h"

ReadMessage::ReadMessage(Message *message, QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::ReadMessage), Message_(message)
{
    ui_->setupUi(this);
    ui_->toLE->setText(message->to());
    ui_->subjectLE->setText(message->subj());
    ui_->timeLE->setText(message->datetime().toString());
    ui_->messageTE->setText(message->body());

    QString attach = "";
    for (QString str : message->files()) {
        attach.append(str);
    }
    ui_->attachLE->setText(attach);

}

ReadMessage::~ReadMessage()
{
    qDebug() << "read message destructed";
    delete ui_;
}
