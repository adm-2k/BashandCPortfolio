# Homework 4

In this homework, you will write a C program to sort strings, ints, or doubles supplied by stdin or a file.
Parts A and B will build the quicksort
library, while Parts C and D will build the user-facing program. 
*Note that your quicksort library must be generic and work independently.*


The instructions can be found under

    instructions.md

in the top level directory. All your changes must be done under the `src`
directory.

You code should be organized as follows. (Ignoring all markdown files)

    src
      \_ quicksort.h
      \_ quicksort.c
      \_ sort.c
      \_ Makefile

All files and directories must be named exactly as above case-sensitive. ***You
should not commit any extraneous files, such as object files or executables.***

All rules about Makefiles and compilation are listed in HW2. As with HW3, your
program must also not leak memory or run with memory errors. Details on how to
check this can be found in HW3. 

You must allocate memory with `malloc` and cannot use `calloc`, `memset`, or other similar functions.
Starting with HW4, if `malloc` fails you should print to stderr that `malloc` failed, clean up all allocated memory, then exit in failure.

All submissions must include at least five git commits with **meaningful**
messages.

## Submission

To submit:

    git commit -am "hw4 completed"
    git push origin main

After submitting, you should re-test with your submitted version. Details on how
to do that are
[here](https://github.com/cs3157-borowski/guides/blob/main/submission.md).

## Acknowledgments

This homework was originally created by Dr. Brian Borowski. Parts of this homework were modified by Palash Sharma in Summer 2023.
