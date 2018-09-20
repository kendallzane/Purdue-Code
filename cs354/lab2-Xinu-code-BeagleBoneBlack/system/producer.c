/* producer.c - producer */

#include <xinu.h>

/**
 *  producer  -  produce a ton of prime number messages
 */
process producer(pid32 cons_pid, int32 n_items) 
{
	int i = 0;
	for (i = 0; i < n_items; i++) {
		int r = rand();
		r = r % 1024;
		//kprintf("%d", r);
		bool8 isPrime = isprime(r);
		if (isPrime) {
			//kprintf("!");
			umsg32 msg = (umsg32)r;
			resume(create(xsend, INITSTK, 1, "xsend", 3, cons_pid, msg, TRUE));
		} else {
			umsg32 msg = (umsg32)r;
			
			resume(create(xsend, INITSTK, 1, "xsend", 3, cons_pid, msg, FALSE));
		}
		sleepms(50);
		
		//kprintf(",");
	}
	//kprintf("\n");
	return OK;
}