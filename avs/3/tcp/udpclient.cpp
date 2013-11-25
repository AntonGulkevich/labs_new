#include "udpclient.h"

// public
UDPClient::UDPClient(QString hostname, qint16 port)
    : Client(hostname, port), udpSocket_(new QUdpSocket())
{
     LOG << "UDPClient constructed.";
}

UDPClient::~UDPClient() {
    LOG << "Client destruct.";

    if (udpSocket_->isOpen())
        udpSocket_->close();

    delete udpSocket_;
}

bool UDPClient::isRun() const {
    return udpSocket_->isOpen();
}

void UDPClient::start() {
    LOG << "Start UDPClient.";

//    tcpSocket_->connectToHost(hostname(), port());
}

void UDPClient::stop() {
    LOG << "Stop UDPClient.";
}

// private

// private slots
void UDPClient::startSocket() {
    LOG << "startSocket().";
}

void UDPClient::slotConnected() {
    LOG << "slotConnected().";
}

void UDPClient::readServer() {
    LOG << "readServer().";
}
