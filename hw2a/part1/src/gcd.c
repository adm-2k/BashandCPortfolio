#include <stdio.h>
#include <stdlib.h>
#include "iterative.h"
#include "recursive.h"

int main(int argc, char **argv)
{
    // Check if the number of command-line arguments is correct
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./gcd <integer m> <integer n>\n");
        return EXIT_FAILURE;
    }
    // Convert the command-line arguments to integers
    int m = atoi(argv[1]);
    int n = atoi(argv[2]);
    if (m == 0 && n == 0)
    {
        printf("gcd(0, 0) = undefined\n");
        return EXIT_SUCCESS;
    }
    // Calculate the GCD using the iterative and recursive functions
    int iterative_result = gcd_iterative(m, n);
    int recursive_result = gcd_recursive(m, n);
    // Print the results using the formated output function
    printf("Iterative: gcd(%d, %d) = %d\n", m, n, iterative_result);
    printf("Recursive: gcd(%d, %d) = %d\n", m, n, recursive_result);
    return EXIT_SUCCESS;
}
