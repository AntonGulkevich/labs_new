CC=g++
CFLAGS=-Wall
all: client server

client: client.o
client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp -o client.o

server: server.o
server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp -o server.o

clean:
	rm -rf *.o client server