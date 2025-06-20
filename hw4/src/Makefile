CC = gcc
CFLAGS = -g -Wall -Werror -pedantic-errors -std=c17
VALGRIND = valgrind --leak-check=full --track-origins=yes

all: sort

sort: sort.o quicksort.o
	$(CC) $(CFLAGS) -o sort sort.o quicksort.o

sort.o: sort.c quicksort.h
	$(CC) $(CFLAGS) -c sort.c

quicksort.o: quicksort.c quicksort.h
	$(CC) $(CFLAGS) -c quicksort.c

clean:
	rm -f *.o sort

valgrind: sort
	$(VALGRIND) ./sort

.PHONY: all clean valgrind