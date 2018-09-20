C-Structs
==========

A struct in the C programming language (and many derivatives) is a 
composite data type declaration that defines a physically 
grouped list of variables to be placed under one name in a 
block of memory, allowing the different variables to 
be accessed via a single pointer, or the struct declared 
name which returns the same address. [1]

Structure Padding
------------------

As you'll work through the assignment for this task, you'll notice
that the size of the structs sometimes is larger than just the
cumulative size of it's data members. The reason why it happens is
because of the padding bytes that are inserted by the compiler within
the structure to satisfy the alignment requirement of the struct's 
constituent datatypes.

Chars can start on any byte address, 2-byte shorts must start on an
even address, 4-byte ints or floats must start on an address
divisible by 4, and 8-byte longs or doubles must start on an 
address divisible by 8. A struct instance will have the alignment
of it's widest scalar member. [2]

Program Assignment
===================

Create a program that instantiates each of the following
three structs:
```
struct simple_struct_1 {
  long long a;
  char b;
  long c;
};

struct simple_struct_2 {
  char d;
  struct simple_struct_2 *e;
  short f;
};


struct complex_struct_1 {
  long long ssn;
  char gender;
  struct inner_struct_1 {
    char *address;
    short age;
  }in;
};
```
After instantiation of these structs, figure out how the structs are laid
down in memory, taking into account the padding bytes added to the struct
by writing a program `task3.c` that prints the addresses at which the
individual members of the struct are present and the size of the struct itself.
Ensure that you run this program on the XINU machines and compile it as a 64-bit
executable to ensure consistent data type size and word length.

Turnin a `task3.txt` which shows how each struct is laid down in memory. 
A sample solution would look as given below. If in the simple_struct_1
`a` occupies 111 bytes and has 222 padding bytes after which `b` occupies
333 bytes with 0 padding bytes after it and `c` occupies 444 bytes with
555 bytes of padding after it. Notice that the padding bytes have the 
suffix `P` attached with them. 
 
```
simple_struct_1 = { 111, 222P, 333, 444, 555P }
``` 
Ensure that when you're submitting your solution you submit both 
`task3.c` and `task3.txt`. `task3.txt` should conform to the above specified
format for our automatic grader to evaluate your solution. Each struct layout
should be on a newline.

References
===========

[1] https://en.wikipedia.org/wiki/Struct_(C_programming_language)
[2] http://www.catb.org/esr/structure-packing/
