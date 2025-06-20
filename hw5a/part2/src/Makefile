CC = gcc
CFLAGS = -g -Wall -Werror -pedantic-errors -std=c17
VALGRIND = valgrind --leak-check=full --track-origins=yes

all: pfind spfind

pfind: pfind.o
	$(CC) $(CFLAGS) -o pfind pfind.o

spfind: spfind.o
	$(CC) $(CFLAGS) -o spfind spfind.o

pfind.o: pfind.c
	$(CC) $(CFLAGS) -c pfind.c

spfind.o: spfind.c
	$(CC) $(CFLAGS) -c spfind.c

clean:
	rm -f *.o pfind spfind

valgrind: spfind
	$(VALGRIND) ./spfind

.PHONY: all clean valgrind