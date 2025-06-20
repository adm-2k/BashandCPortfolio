#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicksort.h"

#define MAX_STRLEN 64 // Not including '\0'
#define MAX_ELEMENTS 1024

void print_usage(void)
{
    fprintf(stderr, "Usage: ./sort [-i|-d] [filename]\n");
    fprintf(stderr, "-i: Specifies the input contains ints.\n");
    fprintf(stderr, "-d: Specifies the input contains doubles.\n");
    fprintf(stderr, "filename: The file to sort. If no file is supplied, input is read from stdin.\n");
    fprintf(stderr, "No flags defaults to sorting strings.\n");
}

int main(int argc, char **argv)
{

    // Variable declarations
    int opt;
    int int_flag = 0;
    int dbl_flag = 0;
    char *filename = NULL;
    FILE *file = stdin;
    char *buffer[MAX_ELEMENTS];
    int count = 0;

    // Parse command line arguments
    while ((opt = getopt(argc, argv, "id")) != -1)
    {
        switch (opt)
        {
        case 'i': // Integer flag
            int_flag = 1;
            break;
        case 'd': // Double flag
            dbl_flag = 1;
            break;
        case '?': // Unknown option
            fprintf(stderr, "Error: Unknown option '-%c' received.\n", optopt);
            print_usage();
            return EXIT_FAILURE;
        }
    }

    // Series of manual checks for argument inputs

    // Check for multiple valid flags
    if (int_flag && dbl_flag) // check both flags are on
    {
        fprintf(stderr, "Error: Too many flags specified.\n");
        return EXIT_FAILURE;
    }

    // Check for multiple filenames
    if (optind < argc - 1)
    {
        fprintf(stderr, "Error: Too many files specified.\n");
        return EXIT_FAILURE;
    }

    // Get the filename if provided
    if (optind < argc)
    {
        filename = argv[optind];
        file = fopen(filename, "r");
        if (file == NULL)
        {
            fprintf(stderr, "Error: Cannot open '%s'. %s.\n", filename, strerror(errno));
            return EXIT_FAILURE;
        }
    }

    // Read input data into dynamically allocated strings
    char line[MAX_STRLEN]; // Buffer for reading lines
    while (fgets(line, MAX_STRLEN, file) != NULL && count < MAX_ELEMENTS)
    {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character

        // Allocate memory and copy the line
        buffer[count] = strdup(line);
        if (!buffer[count])
        {
            fprintf(stderr, "Error: Memory allocation failed.\n");
            return EXIT_FAILURE;
        }
        count++;
    }

    if (file != stdin)
    {
        fclose(file);
    }

    // Sort the data
    if (int_flag)
    {
        int int_array[MAX_ELEMENTS];
        for (int i = 0; i < count; i++)
        {
            int_array[i] = atoi(buffer[i]);
            free(buffer[i]); // Free allocated memory after use
        }
        quicksort(int_array, count, sizeof(int), int_cmp);
        for (int i = 0; i < count; i++)
        {
            printf("%d\n", int_array[i]);
        }
    }
    else if (dbl_flag)
    {
        double dbl_array[MAX_ELEMENTS];
        for (int i = 0; i < count; i++)
        {
            dbl_array[i] = atof(buffer[i]);
            free(buffer[i]); // Free allocated memory after use
        }
        quicksort(dbl_array, count, sizeof(double), dbl_cmp);
        for (int i = 0; i < count; i++)
        {
            printf("%f\n", dbl_array[i]);
        }
    }
    else
    {
        quicksort(buffer, count, sizeof(char *), str_cmp);
        for (int i = 0; i < count; i++)
        {
            printf("%s\n", buffer[i]);
            free(buffer[i]); // Free allocated memory after use
        }
    }

    return EXIT_SUCCESS;
}
