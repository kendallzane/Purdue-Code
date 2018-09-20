/*  main.c  - main */

#include <xinu.h>
#include <lab0.h>

/**
 * Main Process
 */
process	main(void)
{

	/* Run the Xinu shell */

	recvclr();
	//resume(create(shell, 8192, 50, "shell", 1, CONSOLE));
	sleep(1);
	resume(create(lab0app1, INITSTK, INITPRIO, "lab0app1", 0, NULL));
	sleep(1);
	resume(create(lab0app2, INITSTK, INITPRIO, "lab0app2", 0, NULL));
	sleep(1);
	resume(create(master, 8192, INITPRIO, "lab0app4", 0, NULL));
	
	/*
	resume(create(lab0app3, INITSTK, 20, "lab0app3 - A", 1, 'A'));
	resume(create(lab0app3, INITSTK, 20, "lab0app3 - B", 1, 'B'));
	resume(create(lab0app3, INITSTK, 20, "lab0app3 - C", 1, 'C'));
	*/
	
	/* Wait for shell to exit and recreate it */
	/*
	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	
	*/ 
  return OK;
}
