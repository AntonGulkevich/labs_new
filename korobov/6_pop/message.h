#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDialog>
#include <QDebug>
#include "pop3client.h"
#include <QDateTime>
#include <QDir>
namespace Ui {
class Message;
}

class Message : public QDialog
{
    Q_OBJECT

public:
    explicit Message(int index, int size, QWidget *parent = 0);
    ~Message();

private:
    Ui::Message *ui;
};

#endif // MESSAGE_H
