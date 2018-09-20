/* meminit.c - meminit */

#include <xinu.h>

void	*minheap;	/* Start address of heap	*/
void	*maxheap;	/* End address of heap		*/

void	*minbuddy;	/* Minheap for ba */
void	*maxbuddy;	/* Maxheap for ba */

/*------------------------------------------------------------------------
 * meminit - Initialize the free memory list for BeagleBone Black
 *------------------------------------------------------------------------
 */
void	meminit(void)
{
	//kprintf("meminit time!\n");
	/*
	struct memblk freelists[16] = {buddy0, buddy1, buddy2, buddy3, 
						buddy4, buddy5, buddy6, buddy7,
						buddy8, buddy9, buddy10, buddy11,
						buddy12, buddy13, buddy14, buddy15};
	
	*/
	struct	memblk *memptr;	/* Memory block pointer	*/

	/* Initialize the minheap and maxheap variables */

	minheap = (void *)&end;
	minbuddy = minheap;
	
	/* Move minheap 1Mb over for Lab 3 */
	minheap = (void *)(minheap + 0x100000);

	maxbuddy = minheap;
	maxheap = (void *)MAXADDR;

	/* Initialize the memory list as one big block */

	memlist.mnext = (struct memblk *)minheap;
	memptr = memlist.mnext;

	memptr->mnext = (struct memblk *)NULL;
	memlist.mlength = memptr->mlength =
		(uint32)maxheap - (uint32)minheap;
	/*	
	int i = 0;
	for (i = 0; i < 16; i++) {
		//initialize all buddy lists at the same spot
		freelists[i].mnext = (struct memblk *)minbuddy;
		memptr = freelists[i].mnext;
		memptr->mnext = (struct memblk *)NULL;
		
		freelists[i].mlength = 32 * (power(2, i));
		kprintf("initializing mem array for list of size %d\n", freelists[i].mlength);
		freelists[i].used = FALSE;
	}
	*/
}
