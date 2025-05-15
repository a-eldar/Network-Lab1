CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lrt

all: client server

client: client.c common.h
	$(CC) $(CFLAGS) -o client client.c $(LDFLAGS)

server: server.c common.h
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -f client server 