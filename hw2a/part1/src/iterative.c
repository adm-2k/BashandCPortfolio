
#include "iterative.h"
#include <stdlib.h>
#include <stdio.h>

int gcd_iterative(int m, int n)
{
    m = abs(m);
    n = abs(n);
    // Ensure m is greater than or equal to n
    if (m < n)
    {
        // Swap m and n
        int temp = m;
        m = n;
        n = temp;
    }
    while (n != 0)
    {
        int temp = n;
        n = m % n;
        m = temp;
    }
    return m;
}