CC = gcc
# CFLAGS = -g -Wall -Werror -pedantic-errors -std=c17
VALGRIND = valgrind --leak-check=full --track-origins=yes

all: chatserver

chatserver: chatserver.o
	$(CC) $(CFLAGS) -o chatserver chatserver.o

chatserver.o: chatserver.c util.h
	$(CC) $(CFLAGS) -c chatserver.c

clean:
	rm -f *.o chatserver

valgrind: chatserver
	$(VALGRIND) ./chatserver

.PHONY: all clean valgrind