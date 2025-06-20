CC = gcc
CFLAGS = -g -Wall -Werror -pedantic-errors -std=c17
VALGRIND = valgrind --leak-check=full --track-origins=yes

all: pfind

pfind: pfind.o
	$(CC) $(CFLAGS) -o pfind pfind.o

pfind.o: pfind.c
	$(CC) $(CFLAGS) -c pfind.c

clean:
	rm -f *.o pfind

valgrind: pfind
	$(VALGRIND) ./pfind

.PHONY: all clean valgrind
