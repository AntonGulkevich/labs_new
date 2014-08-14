#include "dialog.h"
#include "pop3client.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    POP3Client POP3("redmine.dcti@gmail.com", "",
                    "pop.googlemail.com", 995);
    if (POP3.init()) {
        if (POP3.login()) {

            if (POP3.getMsgList(list_)) {
                for (auto node : list_) {
                    ui->listWidget->addItem(QString("%1 %2").arg(node.first).arg(node.second));
                }
            }
        }
    }
    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(showMessage(QModelIndex)));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::showMessage(QModelIndex index)
{
    Message m(list_[index.row()].first, list_[index.row()].second);
    m.exec();
}
