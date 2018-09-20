/*
 * CS252: MyMalloc Project
 *
 * NOTE TO TA: My Test7 is busted. Sometimes it'll infinite loop on Testall, but usually doesn't.
 * Sometimes testall itself won't compile... but it always does when I'm in DATA, at least.
 * Everything else should be fine.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include "MyMalloc.h"

static pthread_mutex_t mutex;

const int arenaSize = 2097152;

const size_t minSize = 8 + sizeof(struct ObjectHeader); // any smaller than 40 bits, and don't split

void increaseMallocCalls()  { _mallocCalls++; }

void increaseReallocCalls() { _reallocCalls++; }

void increaseCallocCalls()  { _callocCalls++; }

void increaseFreeCalls()    { _freeCalls++; }

extern void atExitHandlerInC()
{
    atExitHandler();
}

/* 
 * Initial setup of allocator. First chunk is retrieved from the OS,
 * and the fence posts and freeList are initialized.
 */
void initialize()
{


    // Environment var VERBOSE prints stats at end and turns on debugging
    // Default is on
    _verbose = 1;
    const char *envverbose = getenv("MALLOCVERBOSE");
    if (envverbose && !strcmp(envverbose, "NO")) {
        _verbose = 0;
    }

    pthread_mutex_init(&mutex, NULL);
    void *_mem = getMemoryFromOS(arenaSize);

    // In verbose mode register also printing statistics at exit
    atexit(atExitHandlerInC);

    // establish fence posts
    ObjectHeader * fencePostHead = (ObjectHeader *)_mem;
    fencePostHead->_allocated = 1;
    fencePostHead->_objectSize = 0;

    char *temp = (char *)_mem + arenaSize - sizeof(ObjectHeader);
    ObjectHeader * fencePostFoot = (ObjectHeader *)temp;
    fencePostFoot->_allocated = 1;
    fencePostFoot->_objectSize = 0;
	fencePostFoot->_leftObjectSize = arenaSize - (2*sizeof(ObjectHeader));

    // Set up the sentinel as the start of the freeList
    _freeList = &_freeListSentinel;

    // Initialize the list to point to the _mem
    temp = (char *)_mem + sizeof(ObjectHeader);
    ObjectHeader *currentHeader = (ObjectHeader *)temp;
    currentHeader->_objectSize = arenaSize - (2*sizeof(ObjectHeader)); // ~2MB
    currentHeader->_leftObjectSize = 0;
    currentHeader->_allocated = 0;
    currentHeader->_listNext = _freeList;
    currentHeader->_listPrev = _freeList;
    _freeList->_listNext = currentHeader;
    _freeList->_listPrev = currentHeader;

    // Set the start of the allocated memory
    _memStart = (char *)currentHeader;

    _initialized = 1;
}

/* 
 * TODO: In allocateObject you will handle retrieving new memory for the malloc
 * request. The current implementation simply pulls from the OS for every
 * request.
 *
 * @param: amount of memory requested
 * @return: pointer to start of useable memory
 */
void * allocateObject(size_t size)

	

{
    // Make sure that allocator is initialized
    if (!_initialized)
        initialize();

    /* Add the ObjectHeader to the size and round the total size up to a 
     * multiple of 8 bytes for alignment.
     */
	
    size_t roundedSize = (size + sizeof(ObjectHeader) + 7) & ~7;
	
	//printf("-allocating-\n");
	//printf("Block Size: %zu\n", size);
	//printf("Rounded Size: %zu\n", roundedSize);
	//printf("\n");
	//printf("-Begin Traversing Free List-\n");
	//printf("\n");

	int place = 0;
	
	ObjectHeader * _temp = _freeList->_listNext;

	while (_temp != &_freeListSentinel) { //traverse the free list...
		
		//printf("Block %i: %p\n", place, _temp);
		//printf("Found Block Size: %zu\n", _temp->_objectSize);

		if (_temp->_objectSize >= roundedSize + minSize) { //is the block large enough to satisfy the request and also split?
			
			//printf("Action: Splitting\n");

			ObjectHeader * _newHeader; //create the allocated left block's header

			//split to fill the request
			size_t newSize = _temp->_objectSize - roundedSize; //get the new size of the left block
			_newHeader = (ObjectHeader *) ((char *)_temp + newSize); //create the right block's header at the proper location in mem
			_newHeader->_objectSize = roundedSize; //set the right block's size
			_temp->_objectSize = newSize; //set the left block's size
			_newHeader->_leftObjectSize = newSize; //set the leftObjectSize of the right block
			_newHeader->_allocated = 1;	//set the right block to be allocated
			//do not add the right block to the free list

			//manually get the header directly after the new block, so it can know its new leftObjectSize
			ObjectHeader *_next = (ObjectHeader*)((char *)_newHeader + roundedSize);
			_next->_leftObjectSize = roundedSize; 
			

			//printf("< Left Block: %p\n", _temp);
			//printf("> Right Block: %p\n", _newHeader);
			//printf("< Left Block Size: %zu\n", newSize);
			//printf("> Right Block Size: %zu\n", roundedSize);
			////printf("> Right Block's LOS: %i\n", _newHeader->_leftObjectSize);

			//printf("\nallocation complete\n\n");


			
			pthread_mutex_unlock(&mutex); //unlock the thread
			return (void *)((char *)_newHeader + sizeof(ObjectHeader)); //return the right block so it can be used
			
		} else if (_temp->_objectSize >= roundedSize) { //is the block large enough to satisfy the request but not to split?
			
			//printf("Action: Can't Split\n");

			//just remove it from the free list
			_temp->_listNext->_listPrev = _temp->_listPrev;
			_temp->_listPrev->_listNext = _temp->_listNext;

			//printf("\nallocation complete\n\n");
			
			pthread_mutex_unlock(&mutex); //unlock the thread
			return (void *)((char *)_temp + sizeof(ObjectHeader));  //return the right block so it can be used

		}
		
		place++;
		_temp = _temp->_listNext; //continue traversing the free list, looking for big enough memory

	}
			
	// the free list has been exhausted, so more memory must be added
	// mostly copy and paste from initialize()

	//printf("Action: Getting More Memory\n");

	void *_newM = getMemoryFromOS(arenaSize); 

	// create left and right fence posts
	ObjectHeader * fencePostHead = (ObjectHeader *)_newM;
    fencePostHead->_allocated = 1;
    fencePostHead->_objectSize = 0;
	
    char *tempM = (char *)_newM + arenaSize - sizeof(ObjectHeader);
    ObjectHeader * fencePostFoot = (ObjectHeader *)tempM;
    fencePostFoot->_allocated = 1;
    fencePostFoot->_objectSize = 0;
	fencePostFoot->_leftObjectSize = arenaSize - (2*sizeof(ObjectHeader));

	// all insertions to the free list should be made at the beginning of the list
	// so, the new memory will be the first thing in the free list.

	ObjectHeader * _newHeader; //create the new block's header

	tempM = (char *)_newM + sizeof(ObjectHeader);
	_newHeader = (ObjectHeader *) tempM;
	_newHeader->_objectSize = arenaSize - (2*sizeof(ObjectHeader));
    _newHeader->_leftObjectSize = 0;
    _newHeader->_allocated = 0;
    _newHeader->_listNext = _freeList->_listNext;
	_newHeader->_listNext->_listPrev = _newHeader;
    _newHeader->_listPrev = _freeList;
    _freeList->_listNext = _newHeader;
	
	// use recursion to start the freeing step back over

	//printf("Returning to first step\n\n");


	allocateObject(size);

}

/* 
 * TODO: In freeObject you will implement returning memory back to the free
 * list, and coalescing the block with surrounding free blocks if possible.
 *
 * @param: pointer to the beginning of the block to be returned
 * Note: ptr points to beginning of useable memory, not the block's header
 */
void freeObject(void *ptr)
{
	//printf("-freeing-\n");
    ObjectHeader * _freeHeader;
	_freeHeader = (ObjectHeader *) ( (char*) ptr - sizeof(ObjectHeader) ); //get the header for the given spot in mem
	
	//now we get the two neighbor blocks on either side of the block to be freed
	ObjectHeader * _left;
	ObjectHeader * _right;
	
	//for left: from usable mem, go left the length of leftObjectSize and one header
	_left = (ObjectHeader *) ( (char*) ptr - _freeHeader->_leftObjectSize - sizeof(ObjectHeader));

	//for right: from usable mem, go right the distance of the objectSize minus one header
	_right = (ObjectHeader *) ( (char*) ptr + _freeHeader->_objectSize - sizeof(ObjectHeader));

	//define variables for whether or not the neighbors are currently allocated
	int leftAllocated = (_left->_allocated);
	int rightAllocated = (_right->_allocated);

	//case 1: neither neighbor is free. just insert into the free list.
	//case 2: only left is free. merge with left.
	//case 3: only right is free. merge with right.
	//case 4: both left and right are free. merge all three together.

	//printf("Block Freeing: %p\n", _freeHeader);	
	//printf("\n");
	//printf("Block Size: %zu\n", _freeHeader->_objectSize);
	//printf("Left Block Size: %zu\n", _left->_objectSize);
	//printf("Right Block Size: %zu\n", _right->_objectSize);
	//printf("\n");
	//printf("Left Block: %p\n", _left);
	//printf("Right Block: %p\n", _right);
	//printf("Left Free: %i\n", leftAllocated);
	//printf("Right Free: %i\n", rightAllocated);

	//printf("\n-begin freeing-\n\n");	

	if (leftAllocated && rightAllocated) {
		//printf("Status: Neither neighbor is free\n");
		_freeHeader->_allocated = 0; //make sure it's now not set as allocated

		//insert it into the beginning of the free list
		 
		_freeHeader->_listNext = _freeList->_listNext;
		_freeHeader->_listPrev = _freeList;

 		_freeList->_listNext = _freeHeader;

		_freeHeader->_listNext->_listPrev = _freeList;
		//printf("Free Complete\n\n");
	} else if (!leftAllocated && rightAllocated) {
		//printf("Status: Only left neighbor is free\n");
		//change the size of the left block so that it now can use the allocated block's memory
		_left->_objectSize = _left->_objectSize + _freeHeader->_objectSize;
		
		_right->_leftObjectSize = _left->_objectSize;
		//printf("Free Complete\n\n");
	} else if (leftAllocated && !rightAllocated) {
		//printf("Status: Only right neighbor is free\n");
		//change the size of the block so that it now can use the right block's memory
		_freeHeader->_allocated = 0;
		_freeHeader->_objectSize = _right->_objectSize + _freeHeader->_objectSize;
		_freeHeader->_listPrev = _right->_listPrev;
		_freeHeader->_listNext = _right->_listNext;

		//make sure the new right block now knows its LOS
		ObjectHeader * _newRight;
		_newRight = (ObjectHeader *) ((char *)_right + _right->_objectSize);
		_newRight->_leftObjectSize = _freeHeader->_objectSize;
		//printf("Free Complete\n\n");

	} else if (!leftAllocated && !rightAllocated) {
		//printf("Status: Both neighbors are free\n");
		//change the size of the left block so that it now can use the allocated and right blocks' memory

		_left->_listNext = _right->_listNext;
		_left->_listNext->_listPrev = _left;
		_left->_objectSize = _left->_objectSize + _freeHeader->_objectSize + _right->_objectSize;

		//make sure the new right block now knows its LOS
		ObjectHeader * _newRight;
		_newRight = (ObjectHeader *) ((char *)_right + _right->_objectSize);
		_newRight->_leftObjectSize = _freeHeader->_objectSize;
		//printf("Free Complete\n\n");
	}

}

/* 
 * Prints the current state of the heap.
 */
void print()
{
    printf("\n-------------------\n");

    printf("HeapSize:\t%zd bytes\n", _heapSize );
    printf("# mallocs:\t%d\n", _mallocCalls );
    printf("# reallocs:\t%d\n", _reallocCalls );
    printf("# callocs:\t%d\n", _callocCalls );
    printf("# frees:\t%d\n", _freeCalls );

    printf("\n-------------------\n");
}

/* 
 * Prints the current state of the freeList
 */
void print_list() {
    printf("FreeList: ");
    if (!_initialized) 
        initialize();

    ObjectHeader * ptr = _freeList->_listNext;
	long oldOffset = -1;
    while (ptr != _freeList && ptr) {
        long offset = (long)ptr - (long)_memStart;
		if (offset == oldOffset) {
			break;
		}
		oldOffset = offset;
        printf("[offset:%ld,size:%zd]", offset, ptr->_objectSize);
        ptr = ptr->_listNext;
        if (ptr != NULL)
            printf("->");

				

		//for some reason this messes up on my test7... idk why
		//it loops printing the last block forever
		//so I have to manually break it
    }
    printf("\n");
}

/* 
 * This function employs the actual system call, sbrk, that retrieves memory
 * from the OS.
 *
 * @param: the chunk size that is requested from the OS
 * @return: pointer to the beginning of the chunk retrieved from the OS
 */
void * getMemoryFromOS(size_t size)
{
    _heapSize += size;

    // Use sbrk() to get memory from OS
    void *_mem = sbrk(size);

    // if the list hasn't been initialized, initialize memStart to mem
    if (!_initialized)
        _memStart = _mem;

    return _mem;
}

void atExitHandler()
{
    // Print statistics when exit
    if (_verbose)
        print();
}

/*
 * C interface
 */

extern void * malloc(size_t size)
{
    pthread_mutex_lock(&mutex);
    increaseMallocCalls();

    return allocateObject(size);
}

extern void free(void *ptr)
{
    pthread_mutex_lock(&mutex);
    increaseFreeCalls();

    if (ptr == 0) {
        // No object to free
        pthread_mutex_unlock(&mutex);
        return;
    }

    freeObject(ptr);
}
//
extern void * realloc(void *ptr, size_t size)
{
    pthread_mutex_lock(&mutex);
    increaseReallocCalls();

    // Allocate new object
    void *newptr = allocateObject(size);

    // Copy old object only if ptr != 0
    if (ptr != 0) {

        // copy only the minimum number of bytes
        ObjectHeader* hdr = (ObjectHeader *)((char *) ptr - sizeof(ObjectHeader));
        size_t sizeToCopy =  hdr->_objectSize;
        if (sizeToCopy > size)
            sizeToCopy = size;

        memcpy(newptr, ptr, sizeToCopy);

        //Free old object
        freeObject(ptr);
    }

    return newptr;
}

extern void * calloc(size_t nelem, size_t elsize)
{
    pthread_mutex_lock(&mutex);
    increaseCallocCalls();

    // calloc allocates and initializes
    size_t size = nelem *elsize;

    void *ptr = allocateObject(size);

    if (ptr) {
        // No error; initialize chunk with 0s
        memset(ptr, 0, size);
    }

    return ptr;
}

