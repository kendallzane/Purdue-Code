# Debugging practice

Write a GDB script to do the following to the program `debuggingPractice.c`.

1. Break at the assignment of the loop in main and print the value of the general purpose registers.
2. Print "OVER\n" in GDB when the variable `i` is greater than 5. Do note that we expect a newline at the end of the print statement.
3. Set the variable `i` to `0xCAFE` when `i` reaches the value 10.

Before turning in your script it is **IMPORTANT** that you have the following lines be the start of your GDB script:

```
set pagination off
set width 0
set height 0
set verbose off
```

Without the above lines we cannot grade the submissions. We shall provide you with the compiled executable `debuggingPractice.out` for consistency.

To run a GDB script you have the following options:

1. Run the command `gdb --batch --command=debugging.gdb debuggingPractice.out` to run as a dedicated script.
2. You also have the option of running the command `source debugging.gdb` within a GDB session. Do not that with this method it is possible that you would have to restart GDB when you modify the script.
