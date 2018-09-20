/* meminit.c - meminit */

#include <xinu.h>

void	*minheap;	/* Start address of heap	*/
void	*maxheap;	/* End address of heap		*/

void	*minbuddy;	/* Minheap for ba */
void	*maxbuddy;	/* Maxheap for ba */

struct memblk freelists[16];

/*------------------------------------------------------------------------
 * meminit - Initialize the free memory list for BeagleBone Black
 *------------------------------------------------------------------------
 */
void	meminit(void)
{
	//thank you, C, for forcing me to make this as ugly as possible
	
	
	freelists[0] = buddy0;
	freelists[1] = buddy1;
	freelists[2] = buddy2;
	freelists[3] = buddy3;
	freelists[4] = buddy4;
	freelists[5] = buddy5;
	freelists[6] = buddy6;
	freelists[7] = buddy7;
	freelists[8] = buddy8;
	freelists[9] = buddy9;
	freelists[10] = buddy10;
	freelists[11] = buddy11;
	freelists[12] = buddy12;
	freelists[13] = buddy13;
	freelists[14] = buddy14;
	freelists[15] = buddy15;
	
	
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
		
	
	//initialize the 1 MB block
	freelists[15].mnext = (struct memblk *)minbuddy;
	memptr = freelists[15].mnext;
	
	memptr->mnext = (struct memblk *)NULL;
	freelists[15].mlength = memptr->mlength = 32 * (power(2, 15));
	//kprintf("initializing mem array for list of size %d\n", freelists[i].mlength);
	freelists[15].used = FALSE;
	memptr->used = FALSE;
	memptr->owner = -100;
	
	int i = 0;
	for (i = 0; i < 15; i++) {
		freelists[i].mnext = (struct memblk *)NULL;
		freelists[i].mlength = 32 * (power(2, i));
		freelists[i].owner = -100; 
	}
	
}
