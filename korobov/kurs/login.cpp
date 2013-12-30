#include "login.h"
#include "ui_login.h"

Login::Login(User **user, QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::Login),
    User_(user), userStorage_(new Storage< User >("user.dat"))
{
    ui_->setupUi(this);
    userStorage_->importStorage();

    updateLW();
}

Login::~Login()
{
    if (userStorage_) delete userStorage_;
    delete ui_;
}

// public static
bool Login::checkConnect(User *user)
{
    return true;
    // check POP3
    bool pop3 = false;
    POP3Client POP3(user->email(), user->password(),
                    user->popHost(), user->popPort());
    if (POP3.init()) {
        if (POP3.login()) {
            qDebug() << "pop3 ok!";
            pop3 = true;
            POP3.quit();
        } else {
            QMessageBox::critical(0,"Error", "Wrong Password");
        }
    } else {
        QMessageBox::critical(0, "Error", "No connection to POP3 server!");
    }

    // check SMTP    readResponse(response);
    bool smtp = false;
    SMTPClient SMTP(user->email(), user->password(),
                    user->smtpHost(), user->smtpPort());
    if (SMTP.init()) {
        if (SMTP.login()) {
            qDebug() << "smtp ok!";
            smtp = true;
            SMTP.quit();
        } else {
            QMessageBox::critical(0, "Error", "Wrong Password");
        }
        qDebug() << "smtp connected";
    } else {
        QMessageBox::critical(0, "Error", "No connection to SMTP server!");
    }

    return pop3 && smtp;
}

// public slot
void Login::on_newUserPB_clicked()
{
    NewUser *newUser = new NewUser(User_, this);

    int exec = newUser->exec();
    if (exec == QDialog::Accepted) {
        userStorage_->add(**User_);
        userStorage_->exportStorage();
        updateLW();
                delete User_;
    }
}

// public slot
void Login::on_delUserPB_clicked()
{
    int index = ui_->usersLW->currentIndex().row();
    qDebug() << index;
    if (index > -1) {
        if (checkPassword(index)) {
            userStorage_->remove(index);
            userStorage_->exportStorage();
            updateLW();
        }
    }

}

// public slot
void Login::on_loginPB_clicked()
{
    login(ui_->usersLW->currentIndex().row());
}

void Login::on_usersLW_doubleClicked(const QModelIndex &modelIndex)
{
    login(modelIndex.row());
}

// private
void Login::login(int index)
{
    if (index > -1) {
        if (*User_) delete *User_;

        *User_ = new User(userStorage_->getObject(index));
        if (checkPassword(index)) {
            if (checkConnect(*User_)) {
                accept();
            }
        }
    }
}


// private
void Login::updateLW()
{
    ui_->usersLW->clear();
    int size = userStorage_->size();
    for (int i = 0; i < size; ++i) {
        ui_->usersLW->addItem(userStorage_->getObject(i).email());
    }
}

// private
bool Login::checkPassword(int id)
{
    bool ok = false;
    QString obtainPassword = QInputDialog::getText(
                this, "Input password", "Password", QLineEdit::Password, userStorage_->getObject(id).password(), &ok); // tmp!!!
    bool verify = (obtainPassword == userStorage_->getObject(id).password());
    if (ok && !verify) {
        QMessageBox::critical(this, "Error", "Wrong Password");
    }

    return ok && verify;
}



