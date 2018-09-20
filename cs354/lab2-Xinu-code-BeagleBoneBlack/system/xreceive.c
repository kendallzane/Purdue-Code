/* xreceive.c - xreceive */

#include <xinu.h>

/**
 *  xreceive  -  return a message from the message buffer of the process
 */
umsg32 xreceive(void) 
{
	sleepms(100);
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	
	//int blocked = 0;
	
	if (prptr->nHighPrio == 0 && prptr->nLowPrio == 0) {
		//blocked = 1;
		prptr->prstate = PR_XRECV;
		//kprintf("sending pid %d to wait\n", currpid);
		resched();		/* Block until message arrives	*/
		//sleepms(100);
	}
	
	
	if (prptr->nHighPrio > 0) {
		msg = prptr->message[BUFFERSIZE - prptr->nHighPrio];		/* Retrieve message		*/
		prptr->nHighPrio--;
	} else {
		msg = prptr->message[prptr->nLowPrio - 1];		/* Retrieve message		*/
		prptr->nLowPrio--;
	}
	
	if (!isempty(prptr->queue)) {
		pid32 td = dequeue(prptr->queue);
		//kprintf("dequeueing pid %d\n", td);
		struct	procent *toDeque = &proctab[td];
		toDeque->prstate = PR_READY;
		ready(td);
		resched();
	}
	restore(mask);
	//kprintf("receiving %d\n", msg);
	
	return msg;
}