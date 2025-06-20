# Homework 2

There are two parts to this homework. In part 1, you will write a program that computes the GCD of two numbers.
In part 2, you will write a program that converts a user-supplied integer into different formats. Each part has an instructions.md file which explains what you need to do.

    part1/instructions.md
    part2/instructions.md

All your changes must be done under the `src` directory for each part. For example, all your code for part 2 must be under `part2/src`.

Your code should be organized as follows.

    part1
        \_ src
            \_ recursive.h
            \_ recursive.c
            \_ iterative.h
            \_ iterative.c
            \_ gcd.c
            \_ Makefile
        \_ instructions.md
    part2
        \_ src
            \_ convert.c
            \_ Makefile
        \_ instructions.md
    README.md

All files and directories must be named exactly as above case-sensitive. ***You should not commit any extraneous files, such as object files or executables.***

For all homeworks, each part where you write a program must include a Makefile that abides by the following specifications:

- `make` must build the correct executable, detailed in each individual part. We will make no other attempt to compile your program other than typing `make`.
- `make clean` should remove all executables and intermediate build artifacts.
- Compiles with gcc.
- Uses `-g -Wall -Werror -pedantic-errors` flags for all compilations.
- Defines the correct dependencies for the programs. For example, in part 1, if you build everything, change `iterative.c`, and run `make` again, `recursive.c` should not recompile. You can simulate changing a file by using the `touch` command.

You are allowed to consult example Makefiles used in class. However, you are responsible for knowing how all components work. Furthermore, you should be able to write a simple Makefile from scratch.

All executables should compile without warnings.

All submissions must include at least five git commits with meaningful messages.

## Submission

To submit:

    git commit -am "hw2 completed"
    git push origin main

After submitting, you should re-test with your submitted version. Details on how to do that are [here](https://github.com/cs3157-borowski/guides/blob/main/submission.md).

## Acknowledgments

Parts of this homework were originally created by Dr. Brian Borowski and Prof. Jae Lee. They were modified by Mateo Maturana and Xurxo Riesco in Spring 2023.
