/* prime.c - isprime */

#include <xinu.h>

/**
 *  isprime  -  determine if a number is prime
 */
bool8 isprime(int32 n) 
{
	if (n <= 1) {
		return FALSE;
	}
	int i;
	for (i = 2; i < n; i++) {
		if (n % i == 0 && i != n) {
			return FALSE;
		}
	}
	return TRUE;
}