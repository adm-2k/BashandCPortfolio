#include <stdio.h>
#include <stdlib.h>

// Function to print an integer as a binary number using bitwise operators and a loop
void binary_print(int x)
{
    unsigned int mask = 1 << 31; // Start with the highest bit
    int bit_count = 0;
    int space_counter = 0;

    for (int i = 0; i < 32; i++)
    {
        if (x & mask) // Check if the current bit is 1
        {
            putc('1', stdout); // Print 1 if the current bit is 1
        }
        else
        {
            putc('0', stdout); // Print 0 if the current bit is 0
        }
        mask >>= 1;      // Move the mask to the right by 1 bit
        bit_count++;     // Increment the bit count
        space_counter++; // Increment the space counter

        // Add a space after every 4 bits except the last one
        if (space_counter == 4 && bit_count != 32)
        {
            putc(' ', stdout);
            space_counter = 0; // Reset the space counter
        }
    }
    // Add a newline at the end
    putc('\n', stdout);
}

int main()
{
    int x;
    // Use printf and scanf to get an integer from the user
    printf("Enter an integer : ");
    scanf("%d", &x);

    // Print x in different formats:
    // Print x as a signed decimal
    printf("signed decimal   : %d\n", x);
    // Cast x to an unsigned int to print it as an unsigned decimal
    printf("unsigned decimal : %u\n", (unsigned int)x);
    // Print x as a hexadecimal using the %X format specifier
    printf("hexadecimal      : %X\n", x);
    // Print x as a binary number using the binary_print function
    printf("binary           : ");
    binary_print(x);
    return EXIT_SUCCESS;
}
