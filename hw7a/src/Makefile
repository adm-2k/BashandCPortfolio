CC = gcc
CFLAGS = -g -Wall -Werror -pedantic-errors -std=c17
VALGRIND = valgrind --leak-check=full --track-origins=yes
IP      ?= 127.0.0.1
PORT    ?= 21356


all: chatclient

chatclient: chatclient.o
	$(CC) $(CFLAGS) -o chatclient chatclient.o

chatclient.o: chatclient.c util.h
	$(CC) $(CFLAGS) -c chatclient.c

clean:
	rm -f *.o chatclient

valgrind: chatclient
	$(VALGRIND) ./chatclient $(IP) $(PORT)

.PHONY: all clean valgrind