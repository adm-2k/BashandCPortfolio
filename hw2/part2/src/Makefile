CC = gcc
CFLAGS = -g -Wall -Wextra -std=c17
TARGET = convert

all: $(TARGET)

$(TARGET): convert.o
	$(CC) $(CFLAGS) -o $(TARGET) convert.o

convert.o: convert.c
	$(CC) $(CFLAGS) -c convert.c

clean:
	rm -f $(TARGET) *.o