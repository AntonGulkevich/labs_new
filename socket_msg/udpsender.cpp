#include "udpsender.h"
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

	char msg1[] = "test!!\n";
    sendto(sock, msg1, sizeof(msg1), 0, 
    	(struct sockaddr *)&address, sizeof(address));

    

    return 0;
}