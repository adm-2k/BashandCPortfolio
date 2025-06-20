#include <string.h>
#include <stdbool.h>
#include <stdio.h>

int line_splitter(char *line, char **argv, int max)
{
    int argc = 0; // Number of arg
    char *token = line;

    while (*token && argc < max - 1)
    {
        // Skip leading whitespace
        while (*token == ' ' || *token == '\t')
        {
            token++;
        }
        if (*token == '\0')
        {
            break; // End of line
        }

        // Handling quoted strings
        if (*token == '"')
        {
            token++;              // Skip the opening quote
            argv[argc++] = token; // Store the start of the token

            while (*token && *token != '"')
            {
                token++; // Move to the next character
            }
            //
            //
            if (*token == '"')
            {
                *token = '\0'; // Null-terminate the quoted argument
                token++;       // Move past the closing quote
            }
            else
            {
                fprintf(stderr, "Error: Mismatched quotes in input.\n");
                return -1; // Error: Mismatched quotes
            }
        }
        else
        {
            // Handle unquoted strings
            argv[argc++] = token;

            // Find the next space or end of input
            while (*token && *token != ' ' && *token != '\t')
            {
                token++;
            }
        }

        // Null-terminate the current argument
        if (*token)
        {
            *token = '\0';
            token++;
        }
    }

    if (argc >= max - 1)
    {
        fprintf(stderr, "Error: Too many arguments.\n");
        return -1; // Error: Argument limit exceeded
    }

    argv[argc] = NULL; // exec requires a NULL-terminated array of strings
    return argc;       // Return the number of arguments
}
