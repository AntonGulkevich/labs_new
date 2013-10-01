#include "server.h"

using namespace std;
int main() {
    int listener;
    int bufsize = 1024;
    char buffer[bufsize];
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(55555);

    listener = socket(AF_INET,SOCK_STREAM,0);
    if (listener <= 0) {
        cout << "Could not create socket!" << endl;
        return 0;
    } 
    cout << "Socket was created!" << endl;

    int bind_status = bind(listener,(struct sockaddr *)&address, sizeof(address));
    if (bind_status != 0) {
        cout << "Could not bind socket!" << endl;
        return 0;
    }
    cout << "Socket was binded!" << endl;
    listen(listener, 1); // 1 - queue size

    int sock = accept(listener,NULL, NULL);

    if (sock > 0) {
        cout << "Client was connected!" << endl;
    }

    while (1) {
        recv(sock, buffer, bufsize, 0);
        cout << "Client: " << buffer << endl;
        cout << "Server: ";
        cin >> buffer;
        if (!strcmp(buffer, "quit")) {
            send(sock, "Good bye", bufsize, 0);
            break;
        }
        send(sock, buffer, bufsize, 0);
    }
    close(sock);
    close(listener);
    return 0;
}