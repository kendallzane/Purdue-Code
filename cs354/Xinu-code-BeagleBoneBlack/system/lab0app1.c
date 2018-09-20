/* lab0app1.c - lab0app1 */

#include <xinu.h>

/**
 * Just printing out a few things to the shell output.
 */
process lab0app1(void) 
{
	kprintf("\nIT ALL RETURNS TO NOTHING\n");
	kprintf("PID of current process is: %d\n", getpid());
}