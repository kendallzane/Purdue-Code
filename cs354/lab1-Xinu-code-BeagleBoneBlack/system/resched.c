/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;
int doingBounded = 0;

/**
 *  resched  -  Reschedule processor to highest priority eligible process
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];


	
	if (preempt == 0 && doingBounded) {
		//kprintf("times up for pid %d\n", currpid);
		kill(currpid);
	}
	
	if (ptold->prstate != PR_CURR && doingBounded) {
		//kprintf("state change? %d\n", currpid);
		kill(currpid);
	}
	
	if (doingBounded) {
		//kprintf("about to hook: pid: %d, preempt: %d\n", currpid, preempt);
		hook(currpid, preempt);
	}
	
	if (ptold->prstate == PR_CURR && !doingBounded) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			return;
		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	
	ptnew = &proctab[currpid];
	
	int q = -1;
	int i = 0;
	
	if (ptnew->prprio == 10000) {

		//next process is a bounded one
		//look for it in our plan
		
		for (i = 0; i <= numGood; i++) {
			//kprintf("checking good[%d].pid (%d) vs currpid (%d)\n", i, good[i].pid, currpid);
			if (good[i].pid == currpid) {
				
				q = good[i].quantum;
				//kprintf("next to execute is a bounded job! pid = %d | quantum = %d\n", currpid, q);
				i = 100;
			}
		}
		
		if (!doingBounded) {
			//kprintf("Start of Bounded Jobs!\n");
			doingBounded = 1;
		}
		
	} else {
		if (doingBounded) {
			kprintf("Bounded jobs ended\n");
			
			//restore everything to its right place
			
			doingBounded = 0;
			executionBegan = 0;
			numBoundedJobs = 0;
			numGood = 0;
			numBad = 0;
		}
	}
	
	
	
	ptnew->prstate = PR_CURR;
	
	//is the job is bounded, use the calculated quantum
	
	if (doingBounded) {
		preempt = q;
	} else {
		preempt = QUANTUM; /* Reset time slice for process	*/
	}
			
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/**
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
