# Part 2

You will write a program that takes a user-supplied integer and prints it to stdout in four different formats: signed decimal, unsigned decimal, hexadecimal, and binary.

Your main method will read a signed decimal integer from the user through stdin, and then print the number in the four formats. You may assume that the user will enter a valid integer, so no error checking is necessary.

Your Makefile should generate an executable called `convert`.

## Sample Executions

```
$ ./convert 
Enter an integer : 1

signed decimal   : 1
unsigned decimal : 1
hexadecimal      : 1
binary           : 0000 0000 0000 0000 0000 0000 0000 0001
$ ./convert
Enter an integer : -1

signed decimal   : -1
unsigned decimal : 4294967295
hexadecimal      : ffffffff
binary           : 1111 1111 1111 1111 1111 1111 1111 1111
$ ./convert
Enter an integer : 256

signed decimal   : 256
unsigned decimal : 256
hexadecimal      : 100
binary           : 0000 0000 0000 0000 0000 0001 0000 0000
```


Note that your program must behave ***EXACTLY*** the same as the sample executions above. This means that the strings, ordering, and spacing must be the same.

## Requirements/Tips

- The only libraries you can include are `stdio.h` and `stdlib.h`. Any other libraries you include will be removed before we test your code.
- You **must** use `putc` to print the binary representation. Use `man 3 putc` for more information.
  - For example, to write the character '1' to `stdout`, use `putc('1', stdout)`
- To convert from decimal to binary, you must use bitwise or bit shifting operations. 
- You must not use the `mod (%)` operator at all within `convert.c`.
- You must use `scanf("%d", &x)` to read an integer inputted through stdin (into a variable `x`). Feel free to name the variable however you wish.
- There is **never** a space after the last readable character on a line of output. There is, however, a new-line character after the last binary digit on the last line.
