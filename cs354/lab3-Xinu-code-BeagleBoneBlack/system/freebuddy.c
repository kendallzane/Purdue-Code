/* freebuddy.c - freebuddy */

#include <xinu.h>

/** Returns OK if the memory was freed successfully, and SYSERR
 * if the argument was not valid (e.g. blkaddr did not point to an allocated
 * block)
 * blkaddr: Pointer to (the lower address of) the block of (usable) memory to
 * be freed
 */
syscall freebuddy(char *blkaddr) 
{
	intmask	mask;
	mask = disable();
	//kprintf("trying to free position %d\n", (void*)blkaddr - minbuddy);
	blkaddr -= 16;
	//kprintf("shifting a byte over for padding, position is now %d\n", (void*)blkaddr - minbuddy);
	//kprintf("freeing a block - let's find what list our block is in.\n");
	int location = -1;
	
	struct	memblk	*curr;
	//struct	memblk	*buddy = 0;
	
	int i = 15;
	for (i = 15; i >= 0; i--) {
		//kprintf("looking through memlists[%d]\n", i);
		curr = freelists[i].mnext;
		while (curr != NULL) {
			if ((void*)curr == (void*)blkaddr) {
				//kprintf("found our block!\n");
				location = i;
			}
			curr = curr->mnext;
		}
		if (location >= 0) {
			i = -1;
		}
	}
	
	if (location >= 0) {
		//kprintf("we found a block to free! freeing it\n");
	} else {
		//kprintf("not finding the block to free...\n");
		restore(mask);
		return SYSERR;
	}
	
	int foundFusion = 0;
	curr = (struct memblk *)blkaddr;
	struct	memblk	*prev;
	
		curr->used = FALSE;
		curr->owner = -100;
	
		
		//kprintf("begin fusing attempt process\n");
		
		struct	memblk	*curr2;
		struct	memblk	*next2;
		//struct	memblk	*buddy;
		
		for (i = location; i <= 14; i++) {
			foundFusion = 0;
			curr = freelists[i].mnext;
			prev = &freelists[i];
			//kprintf("looking for fusion candidates in freelists[%d]\n", i);
			while (curr != NULL && foundFusion == 0) {
				//kprintf("checking a block...\n");
				//kprintf("block has positional offset %d\n", ((void*)curr - minbuddy) % (32 * (power(2,i+1))));
				if (((void*)curr - minbuddy) % (32 * (power(2 , i + 1))) == 0) {						// if this is a left block
					if (curr->used == FALSE) {														// and is free
						//kprintf("found an unused left block. does it have a buddy?\n");
						if (curr->mnext != NULL && (void*)curr->mnext == (void*)curr + (32 * (power(2 , i)))) {		// and the next block exists and is right next to this block
							//kprintf("found the buddy for a block\n");
							//curr->buddy = curr->mnext;
							//curr->buddy->buddy = curr;
							if (curr->mnext->used == FALSE) {										// if that block is unused, fuse
								//kprintf("the buddy is also unused. found two buddies to fuse!\n");
								curr->mlength = curr->mlength*2;
								prev->mnext = curr->mnext->mnext;
								
								//kprintf("insert into list above this (freelists[%d])\n", i+1);
								//kprintf("inserting block with position: %d\n", ((void*)curr - minbuddy));
								
								
								curr2 = &freelists[i + 1];
								next2 = freelists[i + 1].mnext;
								//kprintf("looking through list above this\n");
								int listPos = 0;
								while ((next2 != NULL) && (next2 < curr)) {
									//kprintf("going past block with position: %d\n", (void*)next2 - minbuddy);
									curr2 = next2;
									next2 = next2->mnext;
									listPos++;
									
								}
								//kprintf("done traversing, inserting block now\n");
								if (listPos == 0) {
									curr->mnext = freelists[i + 1].mnext; 
									freelists[i + 1].mnext = curr;
								} else {
									curr->mnext = next2;
									curr2->mnext = curr;
								}
								
								foundFusion = 1;
								
							} else {
								//kprintf("the buddy for the block is still in use\n");
							}
						} else {
							//kprintf("no potential buddy found\n");
						}
					} else {
						//kprintf("this block is still in use\n");
					}
				} else {
					//kprintf("it was a right block. no need to check for fusion\n");
				}
				if (!foundFusion) {
					prev = curr;
					curr = curr->mnext;
				}
			}
			if (!foundFusion) {
				//kprintf("no fusion on this line, so exit\n");
				i = 15;
			}
		}
		//kprintf("\ndone looking for blocks to fuse\n\n");
		restore(mask);
		return OK;
	
	
	
}