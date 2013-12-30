#include "newmessage.h"
#include "ui_newmessage.h"

//public
NewMessage::NewMessage(User *User, Storage< Message > *storage, QDialog *parent) :
    QDialog(parent),
    ui_(new Ui::NewMessage), User_(User), smtpStorage_(storage)
{
    ui_->setupUi(this);
    ui_->toLE->setText("danil@seventest.ru");
    ui_->subjectLE->setText("test");
    ui_->messageTE->setText("example message");
}
// public
NewMessage::~NewMessage()
{
    delete ui_;
}
// private slots
void NewMessage::on_browsePB_clicked()
{
    files_.clear();

    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec())
        files_ = dialog.selectedFiles();

    QString fileListString;
    foreach(QString file, files_)
        fileListString.append( "\"" + QFileInfo(file).fileName() + "\" " );

    ui_->attachLE->setText(fileListString);
}
// private slots
void NewMessage::on_sendPB_clicked()
{
    SMTPClient client(User_->email(), User_->password(),
                      User_->smtpHost(), User_->smtpPort());
    bool sent = client.send(User_->email(), ui_->toLE->text(),
                            ui_->subjectLE->text(), ui_->messageTE->toPlainText(), files_);

    QTime time = QTime::currentTime();

    if (sent) {
        Message message(User_->email(), ui_->toLE->text(),
                        ui_->subjectLE->text(), ui_->messageTE->toPlainText(),
                        files_, time);

        smtpStorage_->add(message);

        accept();
    }

}
// private slots
void NewMessage::on_closePB_clicked()
{
    close();
}
