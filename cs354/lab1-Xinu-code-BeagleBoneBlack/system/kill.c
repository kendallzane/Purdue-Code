/* kill.c - kill */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  kill  -  Kill a process and remove it from the system
 *------------------------------------------------------------------------
 */
syscall	kill(
	  pid32		pid		/* ID of process to kill	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	int32	i;			/* Index into descriptors	*/

	mask = disable();
	if (isbadpid(pid) || (pid == NULLPROC)
	    || ((prptr = &proctab[pid])->prstate) == PR_FREE) {
		restore(mask);
		return SYSERR;
	}

	if (--prcount <= 1) {		/* Last user process completes	*/
		xdone();
	}

	send(prptr->prparent, pid);
	for (i=0; i<3; i++) {
		close(prptr->prdesc[i]);
	}
	freestk(prptr->prstkbase, prptr->prstklen);

	switch (prptr->prstate) {
	case PR_CURR:
		prptr->prstate = PR_FREE;	/* Suicide */
		resched();

	case PR_SLEEP:
	case PR_RECTIM:
		unsleep(pid);
		prptr->prstate = PR_FREE;
		break;

	case PR_WAIT:
		semtab[prptr->prsem].scount++;
		/* Fall through */

	case PR_READY:
		getitem(pid);		/* Remove from queue */
		/* Fall through */

	default:
		prptr->prstate = PR_FREE;
	}

	restore(mask);
	
	//kprintf("looking for a kill\n");
	int j = 0;
	int r = -1;
	for (j = 0; j < numBoundedJobs; j++) {
		if (bounded[j].pid == pid && pid != 0) {
			r = j;
		}			
	}
	
	if (r != -1 && !executionBegan) {
		/*printf("killing pid:%d, removing from list\n\n", bounded[r].pid);
		for (j = 0; j < numBoundedJobs; j++) {
			kprintf("priority: %d\n", j);
			kprintf("name: %s\n", bounded[j].name);
			kprintf("execution_time: %d\n", bounded[j].execution_time);
			kprintf("deadline: %d\n", bounded[j].deadline);
			kprintf("pid: %d\n\n", bounded[j].pid);
		}
		*/
	
		for (j = r; j< numBoundedJobs - 1; j++) {
			bounded[j] = bounded[j + 1];
		}
	
		numBoundedJobs--;
		/*
		printf("after killing pid:%d\n\n", bounded[r].pid);
		for (j = 0; j < numBoundedJobs; j++) {
			kprintf("priority: %d\n", j);
			kprintf("name: %s\n", bounded[j].name);
			kprintf("execution_time: %d\n", bounded[j].execution_time);
			kprintf("deadline: %d\n", bounded[j].deadline);
			kprintf("pid: %d\n\n", bounded[j].pid);
		}
		*/
	}
	return OK;
}
