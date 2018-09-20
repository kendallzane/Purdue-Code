/*  main.c  - main */

#include <xinu.h>

process	main(void)
{

	/* Run the Xinu shell */

	rt_create(func1, 8192, 20, 100, "job1", 1);
    rt_create(func2, 8192, 20, 80, "job2", 2);
    rt_create(func3, 8192, 60, 90, "job3", 3);
	rt_create(func4, 8192, 70, 100, "job4", 4);
    rt_create(func5, 8192, 30, 130, "job5", 5);
	
	//pid32 pid = bounded[3].pid;
	//kill(pid);
	//kprintf("hello\n");
	rt_plan_and_resume();
	
	rt_create(func1, 8192, 20, 100, "job1", 1);
    rt_create(func2, 8192, 20, 80, "job2", 2);
    rt_create(func3, 8192, 60, 90, "job3", 3);
	rt_create(func4, 8192, 70, 100, "job4", 4);
    rt_create(func5, 8192, 30, 130, "job5", 5);
	
	//pid32 pid = bounded[3].pid;
	//kill(pid);
	//kprintf("hello\n");
	rt_plan_and_resume();
	return OK;
    
}
