 /* hook.c - hook */

#include <xinu.h>

/**
 *------------------------------------------------------------------------
 *  hook  -  hook to print the job details
 *------------------------------------------------------------------------
 */
void	hook(
	  pid32		pid,		/* ID of process to insert	*/
	  uint32	quantum_left	/* Quantum time left		*/
	)
{
	
	//TODO : Hook implementation to be added
	//kprintf("hook: pid is %d, quantum_left is %d\n", pid, quantum_left);
}
