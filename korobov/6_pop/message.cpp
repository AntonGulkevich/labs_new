#include "message.h"
#include "ui_message.h"

Message::Message(int index, int size, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Message)
{
    POP3Client POP3("redmine.dcti@gmail.com", "",
                    "pop.googlemail.com", 995);

    ui->setupUi(this);
    if (POP3.init()) {
        if (POP3.login()) {
            QString stringMessage;
            POP3.getMessage(index, size, stringMessage);

            QString body;
            QString boundary;
            QStringList files;
            QDateTime datetime;
            QString messageId;

            QStringList lines = stringMessage.split("\r\n");
            //    for (auto line = ;)

            foreach(QString line, lines) {
                if (line.startsWith("From: ")) {
                    ui->toLE->setText(line.section("From: ", 1));
                }

                if (line.startsWith("Subject: ")) {
                    ui->subjectLE->setText(line.section("Subject: ", 1));
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
                    ui->timeLE->setText(datetime.toString());
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
                        QDir filesPath("/tmp");
                        QDir userPath(filesPath.absolutePath() + "/pop3");

                        if (!userPath.exists()) {
                            filesPath.mkdir("pop3");
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
            ui->messageTE->setText(body);
            QString attach = "";
            for (QString str : files) {
                attach.append(str + "; ");
            }
            ui->attachLE->setText(attach);
        }
    }
}

Message::~Message()
{
    qDebug() << "~Message";
    delete ui;
}
