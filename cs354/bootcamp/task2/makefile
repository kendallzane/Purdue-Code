all: calc_coord.o libcalc_coord.so task2

calc_coord.o: calc_coord.c calc_coord.h
	gcc -c -fpic calc_coord.c -lm
	
libcalc_coord.so: calc_coord.o
	gcc -shared -o libcalc_coord.so calc_coord.o -lm
	
task2: task2.c libcalc_coord.so
	export LD_LIBRARY_PATH=/homes/bowlesk/cs354/bootcamp/task2:$LD_LIBRARY_PATH
	gcc -L /homes/bowlesk/cs354/bootcamp/task2 -o task2 task2.c  -l calc_coord -lm
	unset LD_LIBRARY_PATH


