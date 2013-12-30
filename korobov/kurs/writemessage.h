#ifndef WRITEMESSAGE_H
#define WRITEMESSAGE_H

#include <QDialog>
#include <QFileDialog>
#include "smtpclient.h"
#include "user.h"
#include "message.h"
#include "storage.h"

namespace Ui {
class WriteMessage;
}

class WriteMessage : public QDialog
{
    Q_OBJECT

public:
    explicit WriteMessage(User *User, Storage< Message > *storage, QDialog *parent = 0);
    ~WriteMessage();

private slots:
    void on_browsePB_clicked();
    void on_sendPB_clicked();
    void on_closePB_clicked();

private:
    Ui::WriteMessage *ui_;
    QStringList files_;
    User *User_;
    Storage< Message > *smtpStorage_;
};

#endif // WRITEMESSAGE_H
