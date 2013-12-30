#ifndef NEWMESSAGE_H
#define NEWMESSAGE_H

#include <QDialog>
#include <QFileDialog>
#include "smtpclient.h"
#include "user.h"
#include "message.h"
#include "storage.h"

namespace Ui {
class NewMessage;
}

class NewMessage : public QDialog
{
    Q_OBJECT

public:
    explicit NewMessage(User *User, Storage< Message > *storage, QDialog *parent = 0);
    ~NewMessage();

private slots:
    void on_browsePB_clicked();
    void on_sendPB_clicked();
    void on_closePB_clicked();

private:
    Ui::NewMessage *ui_;
    QStringList files_;
    User *User_;
    Storage< Message > *smtpStorage_;
};

#endif // NEWMESSAGE_H
