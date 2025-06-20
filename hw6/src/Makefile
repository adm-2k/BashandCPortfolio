CC = gcc
CFLAGS = -g -Wall -Werror -pedantic-errors -std=c17
VALGRIND = valgrind --leak-check=full --track-origins=yes

# Object files
OBJ = minishell.o parse.o

# Default target
all: minishell

# Link the object files to create the executable
minishell: $(OBJ)
	$(CC) $(CFLAGS) -o minishell $(OBJ)

# Compile .c files into .o files
%.o: %.c minishell.h parse.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f *.o minishell

# Run the program with Valgrind
valgrind: minishell
	$(VALGRIND) ./minishell

.PHONY: all clean valgrind
