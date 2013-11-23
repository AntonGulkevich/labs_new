#include "tcpclient.h"

using namespace std;

bool isFileExists(char *fileName, int sock) { 
    packet fileExistsRequest;
    fileExistsRequest.code = FILE_EXISTS_REQUEST;
    fileExistsRequest.content = new char[sizeof(fileName)];
    strcpy(fileExistsRequest.content, fileName);
    fileExistsRequest.contentLength = sizeof(fileExistsRequest.content);

    send(sock, &fileExistsRequest, sizeof(fileExistsRequest), 0);

    return true;
}


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

//    int bytes_recieved;
//    int bytes_send;

    cout << "Input file name: ";
    cin >> buffer;
    char fileName[strlen(buffer) + 1];
    strcpy(fileName, buffer);
    fileName[strlen(buffer)] = '\0';

    if (isFileExists(fileName, sock)) {
        cout << "file exists";
    } else {
        cout << "file not exists";
    }


//    while (1) {
//        cout << "Client: ";
//        cin >> buffer;
//        if (!strcmp(buffer, "quit")) {
//            send(sock, "Good bye", bufsize, 0);
//            break;
//        }
//        bytes_send = send(sock, buffer, bufsize, 0);
//        if (bytes_send == 0)
//            break;
//        bytes_recieved = recv(sock, buffer, bufsize, 0);
//        if (bytes_recieved > 0)
//            cout << "Server: " << buffer << endl;
//    }

    close(sock);
}
