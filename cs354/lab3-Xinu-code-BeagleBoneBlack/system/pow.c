#include <xinu.h>
uint32 power(uint32 x, uint32 y) {
	int r = x;
	if (y == 0) {
		return 1;
	}
	while (y > 1) {
		r = x*r;
		y--;
	}
	return r;
}

void visualize(void) 
{

	int i = 15;
	for (i = 15; i >= 0; i--) {
		
		kprintf("LIST[%d] - ", i);
		
		struct	memblk	*curr;
		curr = freelists[i].mnext;
		int j = 0;
		while (curr != NULL) {
			kprintf("[#%d - %d ", j, (void*)curr - minbuddy);
			if (!curr->used) {
				kprintf("(free)");
			} else {
				kprintf("(used)");
			}
			if (curr->owner != -100) {
				kprintf(" <PID: %d>", curr->owner);
			} else {
				kprintf(" <NO OWNER>", curr->owner);
			}
			
			kprintf("] ");
			j++;
			curr = curr->mnext;
		}
		if (j == 0) {
			kprintf("[EMPTY]");
		}
		kprintf("\n");
		
	}
	kprintf("\n");
}

void func1(void)
{
	
	int i = 0;
	for (i = 0; i < 100; i++) {
		getbuddy(1);
	}
	visualize();
	//kprintf("commiting soduku\n");
	sleepms(1000);
	//kill(currpid);
	
}