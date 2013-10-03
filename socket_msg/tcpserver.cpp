#include "tcpserver.h"

using namespace std;
int main(int argc, char **argv) {
    int listener;
    int bufsize = 1024;
    char buffer[bufsize];
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(55555);
    if (argc == 1)
        address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (argc == 2)
        inet_aton(argv[1], &(address.sin_addr));


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
    if (sock > 0) 
        cout << "Client was connected!" << endl;

    int bytes_recieved;
    int bytes_send;
    while (1) {
        bytes_recieved = recv(sock, buffer, bufsize, 0);
        if (bytes_recieved > 0)
            cout << "Client: " << buffer << endl;
        cout << "Server: ";
        cin >> buffer;
        if (!strcmp(buffer, "quit")) {
            send(sock, "Good bye", bufsize, 0);
            break;
        }
        bytes_send = send(sock, buffer, bufsize, 0);

        if (bytes_send == 0)
            break;
    }
    close(sock);
    close(listener);
    return 0;
}