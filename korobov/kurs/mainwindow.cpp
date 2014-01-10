#include "mainwindow.h"
#include "ui_mainwindow.h"
// public

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    Login_(),
    User_(0),
    popStorage_(0), smtpStorage_(0) {
    ui_->setupUi(this);
    show();

    ui_->menuBar->setEnabled(false);

    QHeaderView *popHeader = ui_->popTW->horizontalHeader();
    QHeaderView *smtpHeader = ui_->smtpTW->horizontalHeader();

    popHeader->resizeSections(QHeaderView::Stretch);
    smtpHeader->resizeSections(QHeaderView::Stretch);

    ui_->popTW->setColumnCount(4);
    ui_->popTW->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->smtpTW->setColumnCount(4);
    ui_->smtpTW->setSelectionBehavior(QAbstractItemView::SelectRows);

    QTimer::singleShot(200, this, SLOT(startLogin()));

}

// public

MainWindow::~MainWindow() {
    qDebug() << "~MainWindow()";
    if (Login_) delete Login_;
    if (User_) delete User_;
    if (popStorage_) delete popStorage_;
    if (smtpStorage_) delete smtpStorage_;
    delete ui_;
}

// private

void MainWindow::closeEvent(QCloseEvent *event) {
    if (User_) {
        popStorage_->exportStorage();
        smtpStorage_->exportStorage();
    }
    event->accept();
}

// private

void MainWindow::showTable(QTableWidget *tableWidget, Storage<Message> *storage, bool isTo) {
    for (int i = tableWidget->rowCount() - 1; i >= 0; --i)
        tableWidget->removeRow(i);

    int i = 0;
    QList<Message> tmp;
    if (storage->getObjects(tmp)) {
        for (auto record : tmp) {
            tableWidget->insertRow(i);
            showRecord(tableWidget, &record, i, isTo);
            i++;
        }
    }
}

// private

void MainWindow::showRecord(QTableWidget *tableWidget, Message *message, int index, bool isTo) {
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

Message MainWindow::parsePOP3Message(QString stringMessage) {
    QString to = User_->email();
    QString from, subj, body;
    QString boundary;
    QStringList files;
    QDateTime datetime;
    QString messageId;

    QStringList lines = stringMessage.split("\r\n");
    //    for (auto line = ;)

    foreach(QString line, lines) {
        if (line.startsWith("From: ")) {
            from = line.section("From: ", 1);
        }

        if (line.startsWith("Subject: ")) {
            subj = line.section("Subject: ", 1);
        }

        if (line.startsWith("Date: ")) {
            QString tmp = line.section("Date: ", 1);
            /// Tue, (07 Jan 2014 13:10:40) (-)(08)(00) PST
            /// Fri, (24 May 2013 14:48:00) (+)(04)(00)
            QRegExp reg("\\D*, (.*) (\\+|\\-)(\\d{2})(\\d{2}).*$");

            if (reg.indexIn(tmp) > -1) {
                int offsetHour = reg.cap(3).toInt();
                int offsetMin = reg.cap(4).toInt();
                if (reg.cap(2) == "-") {
                    offsetHour *= -1;
                    offsetMin *= -1;
                }

                datetime = QDateTime::fromString(reg.cap(1), "d MMM yyyy hh:mm:ss");
                datetime = datetime.addSecs((4 - offsetHour) * 3600);
                datetime = datetime.addSecs((0 - offsetMin) * 60);
            }
        }

        if (line.startsWith("Content-Type: ")) {
            QRegExp b(".*; boundary=(.*)");
            if (b.indexIn(line) > -1) {
                boundary = b.cap(1);
            }
        }

        if (line.startsWith("Message-ID: ")) {
            messageId = line.section("Message-ID: ", 1);
        }
    }

    // parse body
    QStringList bodyParts = stringMessage.split("--" + boundary);
    bodyParts.removeAt(0);

    for (auto part : bodyParts) {
        QString contentType, encodeType;
        QStringList lines = part.split("\r\n", QString::SkipEmptyParts);

        contentType = lines[0].section("Content-Type: ", 1);

        if (contentType.startsWith("text/plain")) {
            contentType = "text";

            encodeType = lines[1].section("Content-Transfer-Encoding: ", 1);
            if (encodeType == "base64") {
                for (int i = 2; i < lines.size(); ++i) {
                    QByteArray text(lines[i].toStdString().c_str());
                    body.append(QByteArray(QByteArray::fromBase64(text)).data());
                }
            }
        }
        else if (contentType.startsWith("application/octet-stream")) {
            contentType = "file";
            QString filename = lines[1].section("filename=", 1);
            filename.chop(1);
            files.append(filename);

            encodeType = lines[2].section("Content-Transfer-Encoding: ", 1);
            if (encodeType == "base64") {
                QDir filesPath("/home/dn/mailclient/files");
                QDir userPath(filesPath.absolutePath() + "/" + to);

                if (!userPath.exists()) {
                    filesPath.mkdir(to);
                }
                QFile saveFile(userPath.absolutePath() + "/" + messageId + "/" + filename);
                userPath.mkdir(messageId);
                saveFile.open(QIODevice::WriteOnly);

                for (int i = 3; i < lines.size(); ++i) {
                    QByteArray data(lines[i].toStdString().c_str());
                    saveFile.write(QByteArray::fromBase64(data));
                }

                saveFile.close();
            }

        }
        else {
            continue;
        }
    }

    Message readMessage(from, to, subj, body, files, datetime, messageId, false);
    return readMessage;
}

// private slot

void MainWindow::startLogin() {
    Login_ = new Login(&User_, this);
    Login_->setModal(true);
    int code = Login_->exec();
    if (code == QDialog::Accepted) {
        ui_->menuBar->setEnabled(true);
        popStorage_ = new Storage<Message>("pop" + User_->id());
        popStorage_->importStorage();
        smtpStorage_ = new Storage<Message>("smtp" + User_->id());
        smtpStorage_->importStorage();
        showTable(ui_->popTW, popStorage_);
        showTable(ui_->smtpTW, smtpStorage_);
    } else if (code == QDialog::Rejected) {
        if (User_) {
            delete User_;
            User_ = 0;
        }
        close();
    }
}

// private slots

void MainWindow::on_actionNew_Message_triggered() {
    WriteMessage *writeMessage = new WriteMessage(User_, smtpStorage_);
    if (writeMessage->exec() == QDialog::Accepted) {
        showTable(ui_->smtpTW, smtpStorage_);
    }
}
// private slots

void MainWindow::on_actionGet_Mail_triggered() {
    POP3Client POP3((User_)->email(), (User_)->password(),
                    (User_)->popHost(), (User_)->popPort());
    QList< QPair<QString, quint64> > list;
    if (POP3.init()) {
        if (POP3.login()) {
            if (POP3.getMsgList(list)) {
                QString message;
                for (QPair<QString, quint64> node : list) {
                    POP3.getMessage(node.first, node.second, message);
                    qDebug() << "[[[[" << message << "]]]]";
                    if (!message.isEmpty()) {
                        Message retr(parsePOP3Message(message));

                        auto func = [](Message& a, Message & b) {
                            return (a.id() == b.id());
                        };
                        if (!popStorage_->isObject(retr, func)) {
                            popStorage_->add(retr);
                        }
                    }
                }
            }
        }
    }
    showTable(ui_->popTW, popStorage_);
}

// private slots

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::warning(0, "About Mail Client",
                         "<h2>Mail Client 0.8.3-2</h2>"
                         "<p>Copyright &copy; 2013 Nikonov Danil from SP-91."
                         "<p>Mail Client is a small application that "
                         "demonstrates operations with POP3 and SMTP servers.");
}

void MainWindow::on_smtpTW_doubleClicked(const QModelIndex &index) {
    int i = index.row();
    Message *m = &(smtpStorage_->getObject(i));
    ReadMessage *readMessage = new ReadMessage(m, this);
    if (readMessage->exec() == QDialog::Rejected) {
        delete readMessage;
        showTable(ui_->smtpTW, smtpStorage_);
    }

}

void MainWindow::on_popTW_doubleClicked(const QModelIndex &index) {
    int i = index.row();
    Message *m = &(popStorage_->getObject(i));
    ReadMessage *readMessage = new ReadMessage(m, this);
    if (readMessage->exec() == QDialog::Rejected) {
        delete readMessage;
        showTable(ui_->popTW, popStorage_);
    }
}


void MainWindow::on_actionDelete_triggered()
{
    QTableWidget *tables[2] = {ui_->popTW, ui_->smtpTW};
    Storage<Message> *storages[2] = {popStorage_, smtpStorage_};

    int tab = ui_->tabWidget->currentIndex();
    QTableWidget *table = tables[tab];
    Storage<Message> *storage = storages[tab];

    int row = table->currentRow();
    storage->remove(row);
    showTable(table, storage);

    if (tab == 0) { // POP3 only
        POP3Client POP3((User_)->email(), (User_)->password(),
                        (User_)->popHost(), (User_)->popPort());
        if (POP3.init()) {
            if (POP3.login()) {
                if (POP3.removeMessage(QString::number(row + 1))) {
                    POP3.quit();
                }

            }
        }
    }
}
