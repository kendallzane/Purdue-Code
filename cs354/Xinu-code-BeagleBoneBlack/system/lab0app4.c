/* lab0app4.c - lab0app4 */

#include <xinu.h>
#include <stdlib.h>
#include <lab0app4.h>

process master(void);
process slave(int n, int *addr);

/**
 * Master process for creating the Fibonacci array
 */
process master(void) 
{
	int fib[ARRAYSIZE];
	int input[ARRAYSIZE];
	int i;
	
	
	
	for (i = 0; i < ARRAYSIZE; i++) {
		
		//modifying the RAND_MAX macro in stdlib.h to 29 does not create sufficient RNG.
		//so, I must manually divide so that it resembles the kind of numbers that we want.
		
		int r = rand();
		r = r % 30;
		//int d = 2147483646 / 30;
		//r = (r / d);
		
		//if that 1/2147483646 chance happens... I think...
		//if (r == 30) {
		//	r = 29;
		//}
		input[i] = r;
		
	}
	
	for (i = 0; i < ARRAYSIZE; i++) {
		resume(create(slave, 8192, INITPRIO, "slave", 2, input[i], &(fib[i])));
	}
	
	kprintf("\n");
	
	for (i = 0; i < ARRAYSIZE; i++) {
		kprintf("n = %d, fib = %d\n", input[i], fib[i]);
	}
	
	return OK;
	
}

/**
 * Slave process for filling in the Fibonacci array
 */
process slave(int n, int *addr) 
{
	*addr = fib_n(n);
	return OK;
}