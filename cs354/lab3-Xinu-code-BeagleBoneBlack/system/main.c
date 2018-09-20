/*  main.c  - main */

#include <xinu.h>

process	main(void)
{

	/* Run the Xinu shell */
	struct	memblk	test;
	kprintf("got to main...\n");
	kprintf("size of mem is %d\n", sizeof(test));
	
	//visualize();
	
	//int b = 500000;;
	//getbuddy(b);
	/*
	char* a = getbuddy(20 * 1024);
	visualize();
	char* b = getbuddy(30 * 1024);
	visualize();
	char* c = getbuddy(200 * 1024);
	visualize();
	char* d = getbuddy(200 * 1024);
	visualize();
	freebuddy(c);
	visualize();
	freebuddy(a);
	visualize();
	freebuddy(b);
	visualize();
	freebuddy(d);
	visualize();
	*/
	
	resume(create(func1, 8192, 100, "job1", 0));
	
	sleepms(1000);
	visualize();
	/*
	char* e = getbuddy(1);
	visualize();
	freebuddy(e);
	visualize();
	*/
	return OK;
    
}
