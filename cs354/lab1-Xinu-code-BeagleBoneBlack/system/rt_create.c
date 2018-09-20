/* create.c - create, newpid */

#include <xinu.h>
//#include <bounded.h>



struct job bounded[26];
struct job good[25];
struct job bad[25];
int numBoundedJobs;
int numGood;
int numBad;
int executionBegan;


/**
 *  rt_create  -  Create a bounded process
 */
int	rt_create(
	  void		*funcaddr,	/* procedure address		*/
	  uint32	size,		/* stack size in bytes		*/
	  uint32	execution_time,	/* time needed to execute */
	  uint32	deadline,	/* deadline from time of creation */
	  char		*name,		/* name (for debugging)		*/
	  int32		arg		/* argument passed to create()	*/
	)
{
	intmask	mask;
	mask = disable();
	
	if (executionBegan != 0) {
		//kprintf("ERROR for %s: rt_plan_and_resume has already been called\n", name);
		return(SYSERR);
	}
	
	if (deadline <= 0) {
		//kprintf("ERROR for %s: deadline <= 0\n", name);
		return(SYSERR);
	}
	
	if (execution_time <= 0) {
		//kprintf("ERROR for %s: execution_time <= 0\n", name);
		return(SYSERR);
	}
	
	if (deadline < execution_time) {
		//kprintf("ERROR for %s: deadline < execution_time\n", name);
		return(SYSERR);
	}
	
	if (numBoundedJobs < 25) {
		
		pid32 pid = create(funcaddr, size, 10000, name, 1, arg); //10000 priority is dummy value for bounded jobs
		
		
		
		
		bounded[numBoundedJobs].name = name;
		bounded[numBoundedJobs].execution_time = execution_time;
		bounded[numBoundedJobs].deadline = deadline;
		bounded[numBoundedJobs].pid = pid;
		numBoundedJobs++;
		/*
		kprintf("Added new job: %s\n", name);
		kprintf("PID = %d\n", pid);
		kprintf("Num jobs: %d\n\n", numBoundedJobs);
		*/
	} else {
		//kprintf("ERROR for %s: max number of jobs exceeded (%d)\n", name);
		return(SYSERR);
	}
	
	
	restore(mask);
	return(OK);
	
	
}
