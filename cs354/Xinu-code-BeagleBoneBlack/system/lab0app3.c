/* lab0app3.c - lab0app3.c */

#include <xinu.h>

/**
 * Testing out how priorit works in XINU.
 */
process lab0app3(char c) 
{
	while(1) {
		kprintf("PID: %d, Input: %c\n", getpid(), c);
		sleep(1);
	}
}