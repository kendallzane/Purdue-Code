/* producerTEST.c - producerTEST */

#include <xinu.h>

/**
 *  producerTEST  -  produce a ton of messages
 */
process producerTEST(pid32 cons_pid, int32 n_items) 
{
	int i = 0;
	for (i = 0; i < n_items/2; i++) {
		umsg32 msg = (umsg32)(1000 + i);
		resume(create(xsend, INITSTK, 1, "xsend", 3, cons_pid, msg, FALSE));
		sleepms(50);
		
	}
	for (i = 0; i < n_items/2; i++) {
		umsg32 msg = (umsg32)(2000 + i);
		resume(create(xsend, INITSTK, 1, "xsend", 3, cons_pid, msg, FALSE));
		sleepms(50);
		
	}
	//kprintf("\n");
	return OK;
}