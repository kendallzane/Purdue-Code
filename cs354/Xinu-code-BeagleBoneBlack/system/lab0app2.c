/* lab0app2.c - lab0app2.c */

#include <xinu.h>

/**
 * More of just printing out a few things to the shell output.
 */
process lab0app2(void)
{
	kprintf("\nSquare of PID of current process is: %d\n", getpid() * getpid());
	kprintf("PID of current process is: %d\n", getpid());
}