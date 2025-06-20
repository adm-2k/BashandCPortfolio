# Part 1

You will implement the following three methods to find the GCD of two numbers. ***You may not change any of these function signatures***.

``` c
int gcd_iterative(int m, int n);
int gcd_recursive(int m, int n);
int main(int argc, char **argv);
```

In both gcd_iterative and gcd_recursive, you will implement Euclid's method (feel free to search for the algorithm on Google). The iterative version will use a while loop, while the recursive will, of course, use recursion.

The function header for `gcd_iterative` can be found in `iterative.h` and the function header for `gcd_recursive` can be found in `recursive.h`. The function definitions must be in `iterative.c` and `recursive.c` respectively (which you will create).

The main method in `gcd.c` will parse the command line arguments and call both `gcd_iterative` and `gcd_recursive`. Don't forget that `gcd(m, n)` is equivalent to `gcd(|m|, |n|)` so you should never return a negative value.

If the number of provided command line arguments is not 2, you should print the following error message to `stderr` then exit in failure.

```
Usage: ./gcd <integer m> <integer n>
```

If two command line arguments are provided, you may assume that they will be valid integers. If the two integers are 0, you should print the following message to `stdout` then exit in success.
```
gcd(0, 0) = undefined
```
Otherwise, your output should match the sample executions below. Then, you should exit in success.

Your Makefile should generate an executable called `gcd`.

## Sample Executions

``` 
$ ./gcd 24 12
Iterative: gcd(24, 12) = 12
Recursive: gcd(24, 12) = 12
$ ./gcd -2 10
Iterative: gcd(-2, 10) = 2
Recursive: gcd(-2, 10) = 2
$ ./gcd 0 0
gcd(0, 0) = undefined
$ ./gcd 1 1 1
Usage: ./gcd <integer m> <integer n>
```

Note that your program must behave ***EXACTLY*** the same as the sample executions above. This means that the strings, ordering, and spacing must be the same.

## Requirements/Tips

- The only libraries you can include are `stdio.h` and `stdlib.h`. Any other libraries you include will be removed before we test your code.
- `gcd.c` may not include any source files directly. (i.e. you cannot do `#include "recursive.c"`)
- You may use the `abs` library function to find the absolute value of an int. Use `man 3 abs` for more information.
- You will need to use `atoi` to convert from `char *` to `int`. Use `man 3 atoi` for more information.
- Use the `EXIT_SUCCESS` and `EXIT_FAILURE` macros for returning from the main function of `gcd.c`.
- You are required to add header guards (conditional compilation directives) to each header file. (`recursive.h` and `iterative.h`).
