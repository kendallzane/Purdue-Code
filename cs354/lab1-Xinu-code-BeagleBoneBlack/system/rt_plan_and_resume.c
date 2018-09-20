/* rt_plan_and_resume.c - rt_plan_and_resume */

#include <xinu.h>
//#include <bounded.h>
//int numBoundedJobs;

/**
 *  rt_plan_and_resume  -  plan and execute all of the created bounded processes
 */
void	rt_plan_and_resume(void)
{
	intmask	mask;
	mask = disable();
	
	if (numBoundedJobs == 0) {
		return(SYSERR);
	}
	
	executionBegan = 1;
	
	
	//begin sorting
	//double bubble sort... I know
	
	
	
	int i = 0;
	int j = 0;
	for (i = 0; i < numBoundedJobs; i++) {
		for (j = 0; j < numBoundedJobs-1; j++) {
			if (bounded[j].deadline > bounded[j + 1].deadline) {
				bounded[26] = bounded[j];
				bounded[j] = bounded[j + 1];
				bounded[j + 1] = bounded[26];
			}
		}
	}
	
	for (i = 0; i < numBoundedJobs; i++) {
		for (j = 0; j < numBoundedJobs-1; j++) {
			if (bounded[j].deadline == bounded[j + 1].deadline) {
				
				if (bounded[j].execution_time < bounded[j + 1].execution_time) {
				//if (bounded[j].execution_time > bounded[j + 1].execution_time) {
					//kprintf("deadline swap!\n");
					bounded[26] = bounded[j];
					bounded[j] = bounded[j + 1];
					bounded[j + 1] = bounded[26];
				}
			}
		}
	}
	
	
	int numJobs = numBoundedJobs; //numBoundedJobs increases after this line... very bizzare.
	numBoundedJobs = numJobs;

	
	//begin determining the quantums, and weeding out the ones that can't be executed
	
	int time = 0;
	
	for (i = 0; i < numJobs; i++) {
		
		int endTime = time + bounded[i].execution_time;
		if (endTime > bounded[i].deadline) {
			endTime = bounded[i].deadline;
		}
		if (time == endTime) {
			//can't execute this one
			bad[numBad] = bounded[i];
			bad[numBad].startTime = time;
			numBad++;
			
		} else {
			//can execute this one
			good[numGood] = bounded[i];
			good[numGood].startTime = time;
			good[numGood].quantum = endTime - time;
			numGood++;
			
			time = endTime;

		}
		
	}

	//begin printing out the plan
	
	for (i = 0; i < numGood; i++) {
		kprintf("%s : %d\n", good[i].name, good[i].startTime);
	}
	for (i = 0; i < numBad; i++) {
		kprintf("%s : cannot schedule\n", bad[i].name);
		kill(bad[i].pid);
	}
	
	//put everything into the ready list
	//idk what I'm doing but it works?
	
	numBoundedJobs = numJobs;
	for (i = 0; i < numGood; i++) {
		//copied from ready.c
		register struct procent *prptr;
		prptr = &proctab[good[i].pid];
		prptr->prstate = PR_READY;
		insert(good[i].pid, readylist, prptr->prprio);
		//kprintf("...\n");
	}

	//execute the first bounded job
	
	resched();
	
	
	restore(mask);
	
}