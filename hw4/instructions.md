# Quicksort

## Overview

The goal of this assignment is to create an executable `sort` such that 

    ./sort [-i|-d] [filename]

sorts the values that are in the file, if supplied, and stdin, if the filename
argument is not present. There are many components to this assignment, so it is
recommended that you complete them in order.

## Part A  - Helper Functions

You will complete the implementations of the following 3 static functions in
`quicksort.c`.

```c
static void swap(void *a, void *b, size_t size);
static int lomuto(void *array, int left, int right, size_t elem_sz,
                  int (*cmp) (const void*, const void*));
static void quicksort_helper(void *array, int left, int right, size_t elem_sz,
                             int (*cmp) (const void*, const void*));
```

The source file (`quicksort.c`) contains detailed comments specifying the
behavior of these functions. Your implementation should follow the specified
behavior. These functions will be used as helper functions.

You may not have any other include statements in `quicksort.c` other than what
is already included in the skeleton code (`stdio.h`, `string.h`, `quicksort.h`).
Any other include statements will be removed before grading, which may cause
your code to not compile.

For `lomuto` and `quicksort_helper` you may refer to the following pseudocode.

```
lomuto(A[left...right]) --> int:
    // Input: array A[left..right] where left is the left index and right is
    // the right index
    // Output: partition of A[left..right] and the new position of the pivot
    p <- A[left]
    s <- left
    for i <- left + 1 to right do
        if A[i] < p
            s <- s + 1
            swap(A[s], A[i])
    swap(A[left], A[s])
    return s
```

```
quicksort_helper(A[left..right]):
    // Input: array A[left..right] 
    // Output: array A[left..right] sorted in nondecreasing order
    if left < right
        s <- lomuto(A[left..right]) // s is a split position
        quicksort_helper(A[left...s-1])
        quicksort_helper(A[s+1...right])
```

## Part B - Completing the Quicksort Implementation

In order to complete the quicksort implementation, you have to 
implement the following functions that are defined in `quicksort.h`.

```c
int int_cmp(const void *a, const void *b);
int dbl_cmp(const void *a, const void *b);
int str_cmp(const void *a, const void *b);
void quicksort(void *array, size_t len, size_t elem_sz,
               int (*cmp) (const void*, const void*));
```

These functions should be implemented in `quicksort.c`. The header file contains
detailed comments specifying the behavior of these functions.

You should not edit `quicksort.h`. As with part A, no additional include
statements can be added to `quicksort.c`.

## Part C - Error Checking

In this part, we will start building the user facing program `sort` in `sort.c`.
This program will be called using `./sort [-i|-d] [filename]`. 

- The -i flag specifies that the input to be sorted contains integers.
- The -d flag specifies that the input to be sorted contains doubles.
- No flag specifies that the input will be sorted as strings.
- `filename` should be a ***relative*** path to a .txt file where each line is
  an element to be sorted. It is an optional command line argument. If it is not
  present, the input is read from stdin.

You must use getopt to parse the command line arguments. Run `man 3 getopt` for
more information. The necessary header files are already included.

Your program must be able to handle all the following error cases.

1. Invalid flag -- print usage message with error at the top and return
   `EXIT_FAILURE`.
    ```
    $ ./sort -z
    Error: Unknown option '-z' received.
    Usage: ./sort [-i|-d] [filename]
       -i: Specifies the input contains ints.
       -d: Specifies the input contains doubles.
       filename: The file to sort. If no file is supplied, input is read from
                 stdin.
       No flags defaults to sorting strings.
    ```

2. Invalid filename -- print error message and use strerror (`man 3 strerror`)
   to provide details after the first period. Return `EXIT_FAILURE`. *Note: the
   filename in the output must include the relative path.*
    ```
    $ ./sort -d notfound.txt
    Error: Cannot open 'notfound.txt'. No such file or directory.
    ```

3. Multiple filenames -- print error message and return `EXIT_FAILURE`.
    ```
    $ ./sort file1.txt file2.txt
    Error: Too many files specified.
    ```

4. Multiple valid flags -- print error message and return `EXIT_FAILURE`.
    ```
    $ ./sort -id ints.txt
    Error: Too many flags specified.
    ```

5. Multiple flags with invalid flag -- print error message for invalid flag and
   return `EXIT_FAILURE`.
    ```
    $ ./sort -iz ints.txt
    Error: Unknown option '-z' received.
    Usage: ./sort [-i|-d] [filename]
       -i: Specifies the input contains ints.
       -d: Specifies the input contains doubles.
       filename: The file to sort. If no file is supplied, input is read from
                 stdin.
       No flags defaults to sorting strings.
    ```

## Part D - File Reading and Sorting

The contents of the input files or stdin will be correct. You may safely assume
that each line contains a string, int, or double, and that every line in the input
will be of the same data type. There will be at most 1024 lines in the input, and each
line will contain up to 64 printable (visible) characters. Every line in the input,
including the last line, will end in a newline character.

When the user supplies the correct arguments, the program should sort the
supplied data. Seen below, suppose `tinyints.txt` contains integers 1 through 4
in random order, and when sorted with this program, the output is the integers 1
through 4 in non-decreasing order, each on a separate line:

```
$ ./sort -i input/tinyints.txt
1
2
3
4
```

If no file is specified, the program waits for input on stdin. In the example
below, the user enters integers 4, 3, 2, and 1. CTRL+D is used to stop the program
from reading additional input. The integers 1, 2, 3, and 4 are then printed right
below the input:

```
$ ./sort -i
4
3
2
1
<type CTRL+D>
1
2
3
4
```

stdin can also be redirected from a file or pipe:

```
$ ./sort -i < input/tinyints.txt
1
2
3
4
$ cat input/tinyints.txt | ./sort -i
1
2
3
4
```

Here is an example of the program sorting strings:

```
$ ./sort input/strings.txt
000
ARE
are
biscuits
club
hello
```

Doubles should be printed with default precision:

```
$ ./sort -d input/doubles.txt
1.000000
1.000025
1.001000
3.789500
5.167500
12.444444
600.170000
```

Of course, if the user tries to sort strings with the -i or -d flag, the output
is not expected to be correct. The same is true for any mismatch between the
flag specified and the actual input data.

When reading from the files, you must use relative paths. Any absolute paths will
likely not work on the grading machine.  

You should not have any other #include statements in `sort.c` 
besides what is already in the skeleton code.

Any calls to the `qsort` library function will be replaced with `quicksort` from
your implementation.
