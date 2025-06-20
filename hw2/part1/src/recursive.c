
#include "recursive.h"
#include <stdlib.h>
#include <stdio.h>

int gcd_recursive(int m, int n)
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
    if (n == 0)
    {
        return m;
    }
    return gcd_recursive(n, m % n);
}