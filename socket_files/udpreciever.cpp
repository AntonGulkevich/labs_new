#include "udpreciever.h"
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

	sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock <= 0) {
        cout << "Could not create socket!" << endl;
        return 0;
    } 
    cout << "Socket was created!" << endl;

    int bind_status = bind(sock,(struct sockaddr *)&address, sizeof(address));
    if (bind_status != 0) {
        cout << "Could not bind socket!" << endl;
        return 0;
    }

    int bytes_read;
    while (1) {
        bytes_read = recvfrom(sock, buffer, bufsize, 0, 0,0);
        if (bytes_read > 0)
            cout << buffer;
        if (!strcmp(buffer, "close"))  
            break;

    }

    return 0;
}