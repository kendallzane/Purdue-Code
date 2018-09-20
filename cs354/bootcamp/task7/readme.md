# Knowledge questions

## There are 5 segment a typical representation of a C program's memory layout. For each segment, answer the following questions:

1. What is the name of the segment?
2. What is the order of the segment?
3. What direction does the memory grow (if applicable)?
4. What are the permissions of the segment initialized to?
5. What information lives in that segment?

:NAMES:
The names of the segments of memory are TEXT, DATA, BSS, HEAP, and STACK.

:ORDER:
TEXT, DATA, BSS, HEAP, and STACK is the order of the memory segments from the lowest address (TEXT) to the highest address (STACK).

:DIRECTION:
The HEAP grows upwards, and the STACK grows downwards. The others do not grow.

:PERMISSIONS:
TEXT  - Read, Execute
DATA  - Read, Write, Execute
BSS   - Read, Write
HEAP  - Read, Write
STACK - Read, Write

:INFORMATION:
TEXT  - Stores the instructions that the program runs
DATA  - Contains initialized global variables
BSS   - Contains uninitialized global variables
HEAP  - This is the memory returned when the program requests it with malloc or new
STACK - Stores local variables, as well as return addresses


## Which registers are needed to keep track of the bounds of a function's call frame?

EBP is the "base pointer", and ESP is the top of the stack.

## Given this information, how would you reference the third argument passed on the stack from a called function using an offset from one of these registers.
Know that the first argument in this case is the function argument and not the return address. (hint: be specific as to which register stays consistent throughout the life of a function)?

Assuming that the parameters are 4 bytes large each, you could reference the third argument by [ebp + 12], which will point 3 bytes ahead of EBP. The ESP will stay the same.