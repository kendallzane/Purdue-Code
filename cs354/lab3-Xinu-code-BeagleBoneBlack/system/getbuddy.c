/* getbuddy.c - getbuddy */

#include <xinu.h>

/** Returns a pointer to the lowest usable (see section on metadata)
 * address of the memory block allocated
 * nbytes: Size of request
 */
char  	*getbuddy(
	  uint32	nbytes		/* Size of memory requested	*/
	)
{
	//kprintf("padding %d bytes to ", nbytes);
	nbytes += 0x10;
	//kprintf("%d bytes\n", nbytes);
	intmask	mask;
	mask = disable();
	int size = 1048576;
	
	if (nbytes <= 16 || nbytes > size) {
		restore(mask);
		return (char *)SYSERR;
	}
	
	int found = 0;
	int power = 16;
	
	//kprintf("rounded %d bytes to ...\n", nbytes);
	
	if (nbytes < 32) {
		nbytes = 32;
		power = 0;
	} else {
	
		while (size > 16 && !found) {
			if (!found) {
				//kprintf("does %d = %d?\n", nbytes, size);
				if (nbytes == size) {
					//kprintf("yes, so nbytes = %d\n", nbytes, size);
					found = 1;
				} else {
					//kprintf("no; does %d > %d/2 = %d?\n", nbytes, size, size/2);
					if (nbytes > size / 2) {
						//kprintf("yes, so nbytes = %d\n", size);
						nbytes = size;
						found = 1;
					}
					
				}
				//kprintf("no, keep looking\n");
				size = size / 2;
				power--;
			}
		}
	
	}
	
	//kprintf("... %d bytes, which is 32 * (2^%d) bytes\n\n", nbytes, power);
	
	int top = 15;
	int bottom = power;
	
	struct	memblk	*prev, *curr, *split;
	struct	memblk	*prev2, *curr2;
	
	prev = &freelists[bottom];
	curr = freelists[bottom].mnext;
	
	int direction = 0; //0 = continue up, 1 = continue down
	
	while (1) {
		//kprintf("looking through freelists[%d]\n", bottom);
		//sleepms(20);
		while (curr != NULL && direction == 0) {
			//kprintf("analyzing a block of size: %d\n", curr->mlength);
			if (curr->used == FALSE) {
				if (curr->mlength == nbytes) {
					//kprintf("found a block to return!\n\n");
					curr->used = TRUE;
					curr->owner = currpid;
					
					//kprintf("position of block to return is %d\n", (void*)curr - minbuddy);
					//curr += 16 * (sizeof(char));
					//kprintf("returning 16 bytes past this: %d\n", (void*)curr - minbuddy);
					restore(mask);
					return (char *)(curr) + 16;
				} else {
					//kprintf("found a block, but its too big. need to split.\n");
					split = (struct memblk *)((uint32) curr + (curr->mlength) / 2);
					split->owner = -100;
					split->mlength = (curr->mlength) / 2;
					curr->mlength = (curr->mlength) / 2;
					//curr->buddy = split;
					//split->buddy = curr;
					prev->mnext = curr->mnext;
					curr->used = FALSE;
					split->used = FALSE;
					
					//kprintf("block split. insert into proper location in lower list.\n");
					
					prev2 = &freelists[bottom - 1];			/* Walk along free list	*/
					curr2 = freelists[bottom - 1].mnext;
					
					while ((curr2 != NULL) && (curr2 < curr)) {
						prev2 = curr2;
						curr2 = curr2->mnext;
					}
					
					split->mnext = prev2->mnext;
					curr->mnext = split;
					prev2->mnext = curr;
					
					direction = 1;
					
					
					
				}
				
				
			} else {
				//kprintf("this block is in use...\n");
			}
			if (direction == 1) {
				//kprintf("go down one list to find the block we split.\n\n");
				bottom--;
				prev = &freelists[bottom];
				curr = freelists[bottom].mnext;
			} else {
				prev = curr;
				curr = curr->mnext;
			}
		}
		if (direction == 0) {
			//kprintf("reached the end of our list and found nothing.\n");
			if (bottom == top) {
				//kprintf("there's nothing in our list that we can return.\n");
				restore(mask);
				return (char *)SYSERR;
			} else {
				//kprintf("looking up one list.\n\n");
				bottom++;
				prev = &freelists[bottom];
				curr = freelists[bottom].mnext;
			}
		}
		direction = 0;
	}
	
	//kprintf("something... has gone wrong...\n");
	
	restore(mask);
	
	return (char *)OK;
	
}