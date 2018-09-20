
#include <xinu.h>

void visualize(void) 
{

	for (int i = 15; i >= 0; i--) {
		
		kprintf("LIST[%d] - ");
		
		struct	memblk	*curr;
		curr = freelists[i].next;
		int j = 0;
		while (curr != NULL) {
			kprintf("[#%d - %d ", j, &curr);
			if (curr->free) {
				kprintf("(free)");
			} else {
				kprintf("(used)");
			}
			j++;
			kprintf("]");
		}
		if (j == 0) {
			kprintf("[EMPTY]");
		}
		kprintf("\n");
		
	}
	
}