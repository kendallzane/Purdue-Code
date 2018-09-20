/* fibonacci.c - fibonacci */

#include <xinu.h>

/**
 * Calculate the nth Fibonacci number, without recursion
 */
int fib_n(int n) 
{
	
	if (n == 0) {
		return 0;
	}
	
	if (n == 1) {
		return 1;
	}
	
	int current = 1;
	int previous = 0;
	int i;
	for (i = 2; i <= n; i++) {
		int temp = current;
		current += previous;
		previous = temp;
	}
	
	return current;
}