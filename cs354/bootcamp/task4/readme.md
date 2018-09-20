FUNCTION POINTERS
==================

Programming Assignment
=======================
Implement `print_even` and `print_odd` in `print_seq.c` where `print_even`
prints all even natural numbers(excluding 0)  upto the `limit` parameter 
while `print_odd` prints all odd natural numbers upto the `limit` parameter.

Fill in the main function in `task4.c` to take in a single integer 
parameter as a commandline argument and create a function pointer `fp`
that has `void` return type and takes in an `int` argument. 

If the parameter is odd, call `print_odd` using the function pointer `fp`,
passing the same parameter as it's argument . If the 
parameter is even, call `print_even` passing the same parameter as it's 
argument.

You can use the makefile provided to compile your program.

A sample run would look as below:
```
$ ./test 6
$ 2 4 6 
``` 

 
