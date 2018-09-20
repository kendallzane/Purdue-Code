/* xsend.c - xsend */

#include <xinu.h>

/**
 *  xsend  -  send a message to the message buffer of a process
 */
syscall	xsend(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg,		/* Contents of message		*/
	  bool8		high_prio
	)
{	
	sleepms(100);
	//kprintf("(id %d)", currpid);

	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *sender;		/* Ptr to process's table entry	*/
	struct	procent *receiver;
	
	mask = disable();
/* 	if (isbadpid(pid)) {
		kprintf("I see no such pid %d\n", pid);
		restore(mask);
		return SYSERR;
	} */
	
	sender = &proctab[currpid];
	receiver = &proctab[pid];
	
	//kprintf("msg: %d\n", msg);
	
 	//If the message to be sent cannot be stored in the receiver's buffer
	
	//kprintf("MESSAGE: %d,	", msg);
	
	
	if (receiver->nHighPrio + receiver->nLowPrio == BUFFERSIZE) {
		//change its state to non-ready.
		//kprintf("	(OVERFLOW)\n");
		
		enqueue(currpid, receiver->queue);
		sender->prstate = PR_XSEND;
		
		resched();
		
		//return SYSERR;
	}
	

	
	if(!high_prio) {
		receiver->message[receiver->nLowPrio] = msg;
		receiver->nLowPrio++;
	} else {
		receiver->message[BUFFERSIZE - receiver->nHighPrio - 1] = msg;
		receiver->nHighPrio++;
	} 
	

	
	//kprintf("num low prio: %d\n", receiver->nLowPrio);
	//kprintf("num high prio: %d\n", receiver->nHighPrio);
	
	if (receiver->prstate == PR_XRECV) {
		//kprintf("resuming receive process: %d\n", pid);
		ready(pid);
	}
	
	//sleepms(100);
	restore(mask);
	return OK;
}