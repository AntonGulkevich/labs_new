#ifndef READMESSAGE_H
#define READMESSAGE_H

#include <QDialog>
#include <QDebug>
#include "message.h"
namespace Ui {
class ReadMessage;
}

class ReadMessage : public QDialog {
    Q_OBJECT

public:
    explicit ReadMessage(Message *message, QWidget *parent = 0);
    ~ReadMessage();

private:
    Ui::ReadMessage *ui_;
    Message *Message_;
};

#endif // READMESSAGE_H
