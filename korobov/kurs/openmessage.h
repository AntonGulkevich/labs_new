#ifndef OPENMESSAGE_H
#define OPENMESSAGE_H

#include <QDialog>
#include <QDebug>
#include "message.h"
namespace Ui {
class OpenMessage;
}

class OpenMessage : public QDialog
{
    Q_OBJECT

public:
    explicit OpenMessage(Message *message, QWidget *parent = 0);
    ~OpenMessage();

private:
    Ui::OpenMessage *ui_;
    Message *Message_;
};

#endif // OPENMESSAGE_H
