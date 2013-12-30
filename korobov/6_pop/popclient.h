#ifndef POPCLIENT_H
#define POPCLIENT_H

class QtSSLSocket;
class POPClient
{
public:
    POPClient(  );
};





class QPushButton;
class QTextEdit;
class QLabel;
class QLineEdit;
class QtSSLSocket;
class QSocket;

class SSLClient : public QWidget
{
    Q_OBJECT

public:
    SSLClient(QWidget *parent = 0, const char *name = 0);

    QSize sizeHint() const;

private slots:
    void doConnect();
    void readData();
    void sendData();
    void connectionClosed();
    void error(int);
    void connectToHost(const QString &host, int port);
    void connectedToHost();
    void checkCertError(int, const QString &str);

private:
    QString host;
    int port;
    QPushButton *quitButton;
    QTextEdit *terminal;
    QLabel *statusLabel;
    QLineEdit *userInput;
    //QtSSLSocket *sslsocket;
    QSocket *socket;
};



#endif // POPCLIENT_H
