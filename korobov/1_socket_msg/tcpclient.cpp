#include "tcpclient.h"

using namespace std;
int main(int argc, char **argv) {
    int sock;
    int bufsize = 1024;
    char buffer[bufsize];

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(55555);
    if (argc == 1)
        address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (argc == 2)
        inet_aton(argv[1], &(address.sin_addr));

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

    int bytes_recieved;
    int bytes_send;
    while (1) {
        cout << "Client: ";
        cin.getline(buffer, bufsize);
        buffer[strlen(buffer)] = '\0';

        if (!strcmp(buffer, "quit")) {
            send(sock, "Good bye", strlen(buffer)+1, 0);
            break;
        }
        bytes_send = send(sock, buffer, strlen(buffer)+1, 0);
        if (bytes_send == 0)
            break;
        
        bytes_recieved = recv(sock, buffer, bufsize, 0);
        if (bytes_recieved > 0)
            cout << "Server: " << buffer << "(" << bytes_recieved << ")" << endl;
    }

    close(sock);
}
