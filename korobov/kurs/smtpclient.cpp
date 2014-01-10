#include "smtpclient.h"

// public

SMTPClient::SMTPClient(const QString &email, const QString &password,
                       const QString &host, quint16 port, int timeout) :
    stream_(0), socket_(new QSslSocket(this)),
    email_(email), password_(password), host_(host), port_(port), timeout_(timeout) {
    //    connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(socket_, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect(socket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(disconnected()));

}
// public

SMTPClient::~SMTPClient() {
    qDebug() << "~SMTPClient()";
    socket_->deleteLater();
    socket_->close();
    if (socket_) delete socket_;
    if (stream_) delete stream_;
}

// public

bool SMTPClient::send(const QString &from, const QString &to,
                      const QString &subject, const QString &body, QStringList files) {
    message_ = "To: " + to + "\n";
    message_ += "From: " + from + "\n";
    message_.append("Subject: " + subject + "\n");

    message_.append("MIME-Version: 1.0\n");

    QString boundary = QString(QCryptographicHash::hash(QTime::currentTime()
                                                        .toString()
                                                        .toUtf8(),
                                                        QCryptographicHash::Md5)
                               .toHex()).left(10);

    message_.append("Content-Type: multipart/mixed; boundary=" + boundary + "\n\n");

    message_.append("--" + boundary + "\n");
    message_.append("Content-Type: text/plain\nContent-Transfer-Encoding: base64\n\n");

    QByteArray text(body.toStdString().c_str());
    message_.append(text.toBase64());

    message_.append("\n\n");

    if (!files.isEmpty()) {

        foreach(QString filePath, files) {
            QFile file(filePath);
            if (file.exists()) {
                if (!file.open(QIODevice::ReadOnly)) {
                    QMessageBox::warning(0, tr("Qt Simple SMTP client"), tr("Couldn't open the file\n\n"));
                    return false;
                }
                QByteArray bytes = file.readAll();
                message_.append("--" + boundary + "\n");
                message_.append("Content-Type: application/octet-stream\nContent-Disposition: attachment; filename="
                                + QFileInfo(file.fileName()).fileName()
                                + ";\nContent-Transfer-Encoding: base64\n\n");
                message_.append(bytes.toBase64());
                message_.append("\n");
            }
        }
    }

    message_.append("--" + boundary + "--\n");
    message_.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    message_.replace(QString::fromLatin1("\r\n.\r\n"), QString::fromLatin1("\r\n..\r\n"));

    from_ = from;
    rcpt_ = to;
    if (init() && login()) {
        return (mail_msg() && rcpt_msg() && data_msg() && body_msg() && quit());
    }
    return false;
}

// public

bool SMTPClient::init() {
    return (init_msg() && handshake_msg());
}

// public

bool SMTPClient::login() {
    return (auth_msg() && user_msg() && pass_msg());
}

// public

bool SMTPClient::quit() {
    return quit_msg();
}


// private

QString SMTPClient::doCommand(QString command) {
    *stream_ << "HELO" << "\r\n";
    stream_->flush();
    QString response;
    if (!readResponse(response))
        return "";
    return response;
}

// private

bool SMTPClient::readResponse(QString &response) {
    bool complete = false;
    bool couldRead = socket_->waitForReadyRead(timeout_);
    QString responseLine;
    do {
        responseLine = socket_->readLine();
        response += responseLine;
    } while (socket_->canReadLine());
    qDebug() << response;
    response.truncate(3);

    return couldRead;
}

void SMTPClient::unexpectedResponse(QString response) {
    QMessageBox::warning(0, "Mail Client",
                         "Unexpected reply from SMTP server:\n\n" + response);
    state_ = Close;
}

// private

bool SMTPClient::init_msg() {
    qDebug() << "init msg()";
    state_ = Init;
    qDebug() << "connect to: " << host_ << ":" << port_;
    socket_->connectToHostEncrypted(host_, port_);
    if (!socket_->waitForConnected(timeout_)) {
        qDebug() << socket_->errorString();
        return false;
    }

    stream_ = new QTextStream(socket_);


    if (state_ == Init) {
        QString response;
        if (readResponse(response) && response == "220") {
            qDebug() << "HELO";
            *stream_ << "EHLO" << "\r\n";
            stream_->flush();
            state_ = HandShake;
            return true;
        } else {
            unexpectedResponse(response);
        }
    }
    return false;
}

// private

bool SMTPClient::handshake_msg() {
    qDebug() << "handshake msg()";
    if (state_ == HandShake) {
        QString response;
        if (readResponse(response) && response == "250") {
            if (socket_->mode() == 0) {
                qDebug() << "HELO ENC";
                socket_->startServerEncryption();
                if (!socket_->waitForEncrypted(timeout_)) {
                    qDebug() << socket_->errorString();
                    state_ = Close;
                    return false;
                }
            }
            *stream_ << "HELO" << "\r\n";
            stream_->flush();
            state_ = Auth;
            return true;
        } else {
            unexpectedResponse(response);
        }
    }
    return false;
}

// private

bool SMTPClient::auth_msg() {
    qDebug() << "auth msg()";
    if (state_ == Auth) {
        qDebug() << "wait read response";
        QString response;
        if (readResponse(response) && response == "250") {
            qDebug() << "AUTH LOGIN";
            *stream_ << "AUTH LOGIN" << "\r\n";
            stream_->flush();
            state_ = User;
            return true;
        } else {
            unexpectedResponse(response);
        }
    }
    return false;
}

// private

bool SMTPClient::user_msg() {
    qDebug() << "user msg()";
    if (state_ == User) {
        QString response;
        if (readResponse(response) && response == "334") {
            qDebug() << "EMAIL";
            *stream_ << QByteArray().append(email_).toBase64() << "\r\n";
            stream_->flush();
            state_ = Pass;
            return true;
        } else {
            unexpectedResponse(response);
        }
    }
    return false;
}

// private

bool SMTPClient::pass_msg() {
    qDebug() << "pass msg()";
    if (state_ == Pass) {
        QString response;
        if (readResponse(response) && response == "334") {
            qDebug() << "PASS";
            *stream_ << QByteArray().append(password_).toBase64() << "\r\n";
            stream_->flush();
            state_ = Mail;
            return true;
        }
    }
    return false;
}

// private

bool SMTPClient::mail_msg() {
    qDebug() << "mail msg()";
    if (state_ == Mail) {
        QString response;
        if (readResponse(response) && response == "235") {
            qDebug() << "MAIL";
            *stream_ << "MAIL FROM:<" << from_ << ">\r\n";
            stream_->flush();
            state_ = Rcpt;
            return true;
        } else {
            unexpectedResponse(response);
        }
    }

    return false;
}

// private

bool SMTPClient::rcpt_msg() {
    qDebug() << "rcpt msg()";
    if (state_ == Rcpt) {
        QString response;
        if (readResponse(response) && response == "250") {
            *stream_ << "RCPT TO:<" << rcpt_ << ">\r\n"; //r
            stream_->flush();
            state_ = Data;
            return true;
        } else {
            unexpectedResponse(response);
        }
    }
    return false;
}

// private

bool SMTPClient::data_msg() {
    qDebug() << "data msg()";
    if (state_ == Data) {
        QString response;
        if (readResponse(response) && response == "250") {
            *stream_ << "DATA\r\n";
            stream_->flush();
            state_ = Body;
            return true;
        } else {
            unexpectedResponse(response);
        }
    }
    return false;
}

// private

bool SMTPClient::body_msg() {
    qDebug() << "body msg";
    if (state_ == Body) {
        QString response;
        if (readResponse(response) && response == "354") {
            *stream_ << message_ << "\r\n.\r\n";
            stream_->flush();
            state_ = Quit;
            return true;
        } else {
            unexpectedResponse(response);
        }
    }
    return false;
}

// private

bool SMTPClient::quit_msg() {
    qDebug() << "quit message";
    if (state_ == Quit) {
        QString response;
        if (readResponse(response) && response == "250") {
            *stream_ << "QUIT\r\n";
            stream_->flush();
            state_ = Close;
            return true;
        } else {
            unexpectedResponse(response);
        }
    }
    return false;
}
// private slot

void SMTPClient::stateChanged(QAbstractSocket::SocketState socketState) {
    qDebug() << "stateChanged " << socketState;
}

// private slot

void SMTPClient::errorReceived(QAbstractSocket::SocketError socketError) {
    qDebug() << "error " << socketError;
}

// private slot

void SMTPClient::disconnected() {
    qDebug() << "disconneted";
    qDebug() << "error " << socket_->errorString();
}

// private slot

void SMTPClient::connected() {
    qDebug() << "Connected ";
}

