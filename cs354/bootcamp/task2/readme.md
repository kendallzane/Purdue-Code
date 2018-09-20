Dynamic Shared Library Creation and Compilation
================================================

A shared library is a file that is intended to be shared
by executable files. Modules used by a program are loaded
from individual shared objects into memory at load time
or run time rather than being copied by a linker when it
creates a single monolithic executable file for the program. [1]

Steps to create a dynamically shared library
---------------------------------------------

- Compile the library as position independent code
```
gcc -c -fpic foo.c
```
- Create a shared library (`libfoo.so`) from the resulting object file
```
gcc -shared -o libfoo.so foo.o
```
    - Ensure that the shared library name starts with `lib` and ends with `.so`
      suffix. GCC assumes that a shared library will follow these norms

- Set LD_LIBRARY_PATH to the path where the shared library exists
```
export LD_LIBRARY_PATH=/path/to/foo:$LD_LIBRARY_PATH
```
- Link the shared library with a program that uses it
```
gcc -L/path/to/foo/ -o test main.c -lfoo
```  
    - `-L` specifies where to find the shared library
    - `-l` specifies the name of the library (name of the shared library without
      the `lib` and the `.so` part

- Make sure to unset the LD_LIBRARY_PATH once you're done testing your code

Programming Assignment
=======================

Create a co-ordinate geometry helper dynamic shared library `libcalc_coord.so` 
that has the following functions:

- `double slope (double x1, double y1, double x2, double y2)` : Given the
  co-ordinates of two points on a line [ (x1,y1) & (x2,y2) ] calculate the 
  slope of a line using the formula `(y2-y1) / (x2 - x1)`

- `double distance (double A, double B, double C, double x1, double y1)` - Given
   the parameters of the standard equation of a line (Ax + By + C = 0) and 
   the co-ordinates of a point  (x1, y1), calculate the perpendicular distance
   of the point from the line using [this formula](http://www.intmath.com/plane-analytic-geometry/perpendicular-distance-point-line.php) 
   
Implement the above mentioned library functions inside `calc_coord.c`

Implement 3 test cases for each of the library functions inside `task2.c`

Complete the makefile supplied to you with this task such that on
running `make` a shared library `libcalc_coord.so` is created which
is then linked with `task2.c` to create an executable named `task2`.
You will need to supply additional targets and dependencies inside the 
makefile in order to complete it. 

 


