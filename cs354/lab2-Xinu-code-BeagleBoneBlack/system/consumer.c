/* consumer.c - consumer */

#include <xinu.h>

/**
 *  consumer  -  receive a set number of messages from the buffer
 */
process consumer(int32 n_items)
{
	/*
	struct	procent *receiver;
	receiver = &proctab[currpid];
	
	//kprintf("consumer process: %d\n", currpid);
	//what does the message queue look like initially?
	
	
	
	
	int highP = receiver->nHighPrio;
	int lowP = receiver->nLowPrio;
	
	kprintf("num low prio: %d\n", lowP);
	kprintf("num high prio: %d\n", highP);
	kprintf("table look a liddle sumting like dis\n\n");
	sleepms(2000);
	int j = 0;
	for (j = 0; j < BUFFERSIZE; j++) {
		kprintf("~~~~~~~~~~~~~~~~~~~~\n");
		kprintf("~%d ", j);
		if (j < lowP) {
			kprintf("L	%d", receiver->message[j]);
		} else if (BUFFERSIZE - j - 1< highP) {
			kprintf("H	%d", receiver->message[j]);
		} else {
			kprintf("(empty)");
		}
		kprintf("\n");
	}
	kprintf("~~~~~~~~~~~~~~~~~~~~\n\n");
	*/
	
	int i = 0;
	for(i = 0; i<n_items; i++) {
		
		umsg32 msg = xreceive();
		
		kprintf("i = %d, message = %d\n", i, msg);
		
	}

	

	
	return OK;
	
}