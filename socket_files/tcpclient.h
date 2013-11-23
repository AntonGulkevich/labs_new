#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h> // close()

#include "const.h"

int prepareSocket(char *, int);
bool isFileExists(char *);
int getFileSize(char *);
