#include "mainwindow.h"
#include "ui_mainwindow.h"
// public
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    uiMain_(new Ui::MainWindow),
    Login_(0),
    User_(0)
{
    uiMain_->setupUi(this);
    if (!User_) {
        Login_ = new Login(&User_);
        Login_->show();
        setDisabled(true);
        setWindowOpacity(0.5);
        Login_->move(Login_->geometry().center().x() - x(), Login_->geometry().center().y() - y());
        connect(Login_, SIGNAL(destroyed()), this, SLOT(loginWindowClosed()));
    }
}
// public
MainWindow::~MainWindow()
{
    delete uiMain_;
    if (Login_) delete Login_;
    if (User_) delete User_;
}

// private slots
void MainWindow::loginWindowClosed()
{
    if (!User_) {
        close();
    } else {
        setDisabled(false);
        setWindowOpacity(0);
            qDebug() << User_->email();
    }
    qDebug() << "____________closed";
}
// private slots
void MainWindow::on_actionNew_Message_triggered()
{
    NewMessage *newMessage = new NewMessage(User_);
    newMessage->show();
}
// private slots
void MainWindow::on_actionGet_Mail_triggered()
{

}
// private slots
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::warning(0, "About Mail Client",
                       "<h2>Mail Client 0.8.3-2</h2>"
                          "<p>Copyright &copy; 2013 Nikonov Danil from SP-91."
                          "<p>Mail Client is a small application that "
                          "demonstrates operations with POP3 and SMTP servers.");
}


