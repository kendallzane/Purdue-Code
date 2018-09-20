/*  main.c  - main */

#include <xinu.h>

process	main(void)
{

	/* Commented for your benefit..
	 * You can uncomment this while testing
	 * However, keep this commented while submission
	recvclr();
	resume(create(shell, 8192, 50, "shell", 1, CONSOLE));

	while (TRUE) {
		receive();
		sleepms(200);
		kprintf("\n\nMain process recreating shell\n\n");
		resume(create(shell, 4096, 20, "shell", 1, CONSOLE));
	}
	*/
	
	int32 n_items = 30;
	
	//use gettime to get a seed
	uint32 seed = 0;
	gettime(&seed);
	//kprintf("our seed is %d\n", seed);
	srand(seed);
	
	
	pid32 c = create(consumer, INITSTK, 25, "consumer", 1, n_items);
	
	
	pid32 p = create(producer, INITSTK, 25, "producer", 2, c, n_items);
	
	resume(c);
	resume(p);
	//sleepms(3000);
	
	
	
	
	


	
	
	
	
	
	return OK;
    
}
