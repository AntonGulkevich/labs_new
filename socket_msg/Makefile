CC=g++
CFLAGS=-Wall
all: tcpclient tcpserver udpsender udpreciever
tcpclient: tcpclient.o
tcpclient.o: tcpclient.cpp
	$(CC) $(CFLAGS) -c tcpclient.cpp -o tcpclient.o

tcpserver: tcpserver.o
tcpserver.o: tcpserver.cpp
	$(CC) $(CFLAGS) -c tcpserver.cpp -o tcpserver.o

udpsender: udpsender.o
udpsender.o: udpsender.cpp
	$(CC) $(CFLAGS) -c udpsender.cpp -o udpsender.o

udpreciever: udpreciever.o
udpreciever.o: udpreciever.cpp
	$(CC) $(CFLAGS) -c udpreciever.cpp -o udpreciever.o

clean:
	rm -rf *.o tcpclient tcpserver udpsender udpreciever