#include "client.h"

using namespace std;
int main() {
    int sock;
    int bufsize = 1024;
    char buffer[bufsize];

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(55555);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    sock = socket(AF_INET,SOCK_STREAM,0);
    if (sock <= 0) {
        cout << "Could not create socket!" << endl;
        return 0;
    } 
    cout << "Socket was created!" << endl;
    
    int connect_status = connect(sock,(struct sockaddr *)&address, sizeof(address));
    if (connect_status < 0) {
         cout << "Could not connect!" << endl;
         return 0;
    }    
    cout << "The connection was accepted with the server..." << endl;

    while (1) {
        cout << "Client: ";
        cin >> buffer;
        if (!strcmp(buffer, "quit")) {
            send(sock, "Good bye", bufsize, 0);
            break;
        }
        send(sock, buffer, bufsize, 0);
        recv(sock, buffer, bufsize, 0);
        cout << "Server: " << buffer << endl;
    }

    close(sock);
}