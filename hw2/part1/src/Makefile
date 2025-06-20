
CC = gcc
CFLAGS = -g -Wall -Werror -std=c17

all: gcd

gcd: gcd.o iterative.o recursive.o
	$(CC) $(CFLAGS) -o gcd gcd.o iterative.o recursive.o

gcd.o: gcd.c iterative.h recursive.h
iterative.o: iterative.c iterative.h
recursive.o: recursive.c recursive.h

clean:
	rm -f *.o gcd