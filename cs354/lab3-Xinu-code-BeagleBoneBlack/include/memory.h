/* memory.h - roundmb, truncmb, freestk */

#define	PAGE_SIZE	4096

/*----------------------------------------------------------------------
 * roundmb, truncmb - Round or truncate address to memory block size
 *----------------------------------------------------------------------
 */
#define	roundmb(x)	(char *)( (7 + (uint32)(x)) & (~7) )
#define	truncmb(x)	(char *)( ((uint32)(x)) & (~7) )

/*----------------------------------------------------------------------
 *  freestk  --  Free stack memory allocated by getstk
 *----------------------------------------------------------------------
 */
#define	freestk(p,len)	freemem((char *)((uint32)(p)		\
				- ((uint32)roundmb(len))	\
				+ (uint32)sizeof(uint32)),	\
				(uint32)roundmb(len) )

struct	memblk	{			/* See roundmb & truncmb	*/
	struct	memblk	*mnext;		/* Ptr to next free memory blk	*/
	//struct	memblk	*buddy;		/* Ptr to buddy	*/
	uint32	mlength;		/* Size of blk (includes memblk)*/
	bool8	used;			/* Is this block "split" (unusable)? */
	pid32 owner;
	};
	
extern	struct	memblk	memlist;	/* Head of free memory list	*/



extern	struct	memblk	buddy15;	/* 1 MB list*/
extern	struct	memblk	buddy14;	/* 512K list*/
extern	struct	memblk	buddy13;	/* 256K list*/
extern	struct	memblk	buddy12;	/* 128K list*/
extern	struct	memblk	buddy11;	/* 64K list*/
extern	struct	memblk	buddy10;	/* 32K list*/
extern	struct	memblk	buddy9;		/* 16K list*/
extern	struct	memblk	buddy8;		/* 8K list*/
extern	struct	memblk	buddy7;		/* 4K list*/
extern	struct	memblk	buddy6;		/* 2K list*/
extern	struct	memblk	buddy5;		/* 1K list*/
extern	struct	memblk	buddy4;		/* 512B list*/
extern	struct	memblk	buddy3;		/* 256B list*/
extern	struct	memblk	buddy2;		/* 128B list*/
extern	struct	memblk	buddy1;		/* 64B list*/
extern	struct	memblk	buddy0;		/* 32B list*/



extern	void	*minheap;		/* Start of heap		*/
extern	void	*maxheap;		/* Highest valid heap address	*/
extern  void	*minbuddy;

extern  void	*maxbuddy;

/* Added by linker */

extern	int	text;			/* Start of text segment	*/
extern	int	etext;			/* End of text segment		*/
extern	int	data;			/* Start of data segment	*/
extern	int	edata;			/* End of data segment		*/
extern	int	bss;			/* Start of bss segment		*/
extern	int	ebss;			/* End of bss segment		*/
extern	int	end;			/* End of program		*/
