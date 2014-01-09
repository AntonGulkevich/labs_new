#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>
#include "user.h"
#include "login.h"
#include "smtpclient.h"
#include "pop3client.h"

namespace Ui {
class NewUser;
}

class NewUser : public QDialog {
    Q_OBJECT

public:
    explicit NewUser(User **User, QWidget *parent = 0);
    ~NewUser();

private slots:
    void on_okPB_clicked();
    void on_closePB_clicked();

private:
    User **User_;
    Ui::NewUser *ui_;
};

#endif // NEWUSER_H
