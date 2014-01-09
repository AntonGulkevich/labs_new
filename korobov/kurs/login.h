#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QInputDialog>
#include <QModelIndex>
#include "user.h"
#include "newuser.h"
#include "storage.h"


namespace Ui {
    class Login;
}

class Login : public QDialog {
    Q_OBJECT

public:
    explicit Login(User **User, QWidget *parent = 0);
    ~Login();
    static bool checkConnect(User *user);

private slots:
    void on_newUserPB_clicked();
    void on_delUserPB_clicked();
    void on_loginPB_clicked();
    void on_usersLW_doubleClicked(const QModelIndex &modelIndex);

private:
    Ui::Login *ui_;
    User **User_;
    Storage< User > *userStorage_;

    void login(int index);
    void updateLW();
    bool checkPassword(int id);
};

#endif // LOGIN_H
