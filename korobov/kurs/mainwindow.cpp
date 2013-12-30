#include "mainwindow.h"
#include "ui_mainwindow.h"
// public
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    Login_(0),
    User_(0)
{
    ui_->setupUi(this);
    show();
    Login_ = new Login(&User_, this);
    Login_->setModal(true);
    Login_->show();
    connect(Login_, SIGNAL(finished(int)), this, SLOT(loginDialog_finished(int)));
    ui_->menuBar->setEnabled(false);

    QHeaderView *popHeader = ui_->popTW->horizontalHeader();
    QHeaderView *smtpHeader = ui_->smtpTW->horizontalHeader();


    popHeader->resizeSections(QHeaderView::Stretch);
//    smtpHeader->resizeSections(QHeaderView::Stretch);

    ui_->popTW->setColumnCount(4);
    ui_->popTW->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->smtpTW->setColumnCount(4);
    ui_->smtpTW->setSelectionBehavior(QAbstractItemView::SelectRows);

}

// public
MainWindow::~MainWindow()
{
    qDebug() << "~";
    delete ui_;
    if (Login_) delete Login_;
    if (User_) delete User_;
}

// private
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (User_) {
        popStorage_->exportStorage();
        smtpStorage_->exportStorage();
    }
    //        event->accept();
}



// private
void MainWindow::showTable(QTableWidget *tableWidget, Storage<Message> *storage, bool isTo)
{
    for (int i = tableWidget->rowCount() - 1; i >= 0; --i)
        tableWidget->removeRow(i);

    int i = 0;
    for (storage->iterator = storage->begin(); storage->iterator != storage->end(); ++(storage->iterator))
    {
        tableWidget->insertRow(i);
        showRecord(tableWidget, &(*(storage->iterator)), i, isTo);
        i++;
    }

}

// private
void MainWindow::showRecord(QTableWidget *tableWidget, Message *message, int index, bool isTo)
{
    qDebug() << "show table";

    QFont bold;
    bold.setBold(true);

    QTableWidgetItem *attachLength = new QTableWidgetItem(QString::number(message->files().length()));
    attachLength->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    if (message->read() == false) attachLength->setFont(bold);
    tableWidget->setItem(index, 0, attachLength);

    QTableWidgetItem *time = new QTableWidgetItem(message->datetime().toString());
    time->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    if (message->read() == false) time->setFont(bold);
    tableWidget->setItem(index, 1, time);

    QTableWidgetItem *subj = new QTableWidgetItem(message->subj());
    subj->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    if (message->read() == false) subj->setFont(bold);
    tableWidget->setItem(index, 2, subj);

    if (isTo) {
        QTableWidgetItem *to = new QTableWidgetItem(message->to());
        to->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        if (message->read() == false) to->setFont(bold);
        tableWidget->setItem(index, 3, to);
    } else {
        QTableWidgetItem *from = new QTableWidgetItem(message->from());
        from->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        if (message->read() == false) from->setFont(bold);
        tableWidget->setItem(index, 3, from);
    }
}

// private
Message MainWindow::parsePOP3Message(QString stringMessage)
{
    QString to = User_->email();
    QString from, subj, body;
    QStringList files;
    QDateTime datetime;
    bool read;

    QStringList lines = stringMessage.split("\r\n");
    foreach (QString line, lines) {
        if (line.startsWith("From: ")) {
            to = line.section("From: ", 1);
        }

        if (line.startsWith("Subject: ")) {
            subj = line.section("Subject: ", 1);
        }

        if (line.startsWith("Date: ")) {
            QString tmp = line.section("Date: ", 1);
            tmp = tmp.section(",", 1).trimmed();
            tmp.chop(6);
            datetime = QDateTime::fromString(tmp, "d MMM yyyy hh:mm:ss");
        }


//        QString str = "Fri, 24 May 2013 14:48:00 +0400";
//        str = str.section(",", 1).trimmed();
//                str.chop(6);

//        qDebug() << str;
//        QDateTime dt = QDateTime::fromString(str, "d MMM yyyy hh:mm:ss");
//        qDebug() << dt.toString();



//        qDebug() << to << " " << subj << " " << time;
    }
    Message readMessage(from, User_->email(), subj, "", QStringList(), datetime, false);
    qDebug() << readMessage.id();
    return readMessage;
}

// private slots
void MainWindow::loginDialog_finished(int code)
{
    if (code == 0) {
        close();
    } else if (code == 1) {
        ui_->menuBar->setEnabled(true);
        popStorage_ = new Storage<Message>("pop" + User_->id());
        popStorage_->importStorage();
        smtpStorage_ = new Storage<Message>("smtp" + User_->id());
        smtpStorage_->importStorage();
        showTable(ui_->popTW, popStorage_);
        showTable(ui_->smtpTW, smtpStorage_);
    }
}

// private slots
void MainWindow::on_actionNew_Message_triggered()
{
    WriteMessage *writeMessage = new WriteMessage(User_, smtpStorage_);
    if (writeMessage->exec() == QDialog::Accepted) {
        showTable(ui_->smtpTW, smtpStorage_);
    }
}
// private slots
void MainWindow::on_actionGet_Mail_triggered()
{
    POP3Client POP3 ((User_)->email(), (User_)->password(),
                     (User_)->popHost(), (User_)->popPort());
    QStringList uIdList;
    if (POP3.init()) {
        if (POP3.login()) {
            if (POP3.getMsgList(uIdList)) {
                QString message;
                foreach (QString str, uIdList) {
//                   POP3.getMessageTop(str, 10, top);
                    POP3.getMessage(str, message);
                    Message retr(parsePOP3Message(message));
//                    qDebug() << retr.id();
                    popStorage_->add(retr);
                }
            }
        }
    }
    showTable(ui_->popTW, popStorage_);
}

// private slots
void MainWindow::on_actionDelete_triggered()
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


void MainWindow::on_smtpTW_doubleClicked(const QModelIndex &index)
{
    int i = index.row();
    Message *m = &(smtpStorage_->getObject(i));
    ReadMessage *readMessage = new ReadMessage(m, this);
    if (readMessage->exec() == QDialog::Rejected) {
        delete readMessage;
    }

}

void MainWindow::on_popTW_doubleClicked(const QModelIndex &index)
{
    int i = index.row();
    Message *m = &(popStorage_->getObject(i));
    ReadMessage *readMessage = new ReadMessage(m, this);
    if (readMessage->exec() == QDialog::Rejected) {
        delete readMessage;
    }
}
