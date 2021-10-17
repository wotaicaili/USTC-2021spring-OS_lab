#include "myPrintk.h"
#include "mem.h"

// the handler of a dParted-memory
typedef struct dPartition {
	unsigned long size;
	unsigned long firstFree; 
} dPartition;

// a block of dParted-memory
typedef struct EMB {
	unsigned long size; // need to be protected when alloc EMB
	union {
		unsigned long nextStart;    // if free, pointer to next block
        unsigned long userData;		// if allocated, belongs to user
	};	                           
} EMB;

// show the info of a dpHandler
void showdPartition(dPartition *dp) {
	myPrintk(0x5, "dPartition (start = 0x%x, size = 0x%x, firstFreeStart = 0x%x)\n", dp, dp->size,dp->firstFree);
}

// show the info of a EMB
void showEMB(EMB *emb) {
	myPrintk(0x3, "EMB (start = 0x%x, size = 0x%x, nextStart = 0x%x)\n", emb, emb->size, emb->nextStart);
}

// print the info of dpHandler and all of its free blocks
void dPartitionWalkByAddr(unsigned long dpHandler) {
	dPartition *handler = (dPartition *)dpHandler;
	showdPartition(handler);

	unsigned long addr = handler->firstFree;
	EMB *block;
	while (addr) {
    	block = (EMB *)addr;
		showEMB(block);
		addr = block->nextStart;
	}
}

// align a value by 8 bytes
unsigned long align8(unsigned long value) {
    if (value & 1) value += 1;
    if (value & 2) value += 2;
	if (value & 4) value += 4;
    return value;
}

// init a dparted-memory, return its handler
unsigned long dPartitionInit(unsigned long start, unsigned long totalSize) {
	if (totalSize <= sizeof(dPartition) + sizeof(EMB)) // total size too small
		return 0;

	dPartition *handler = (dPartition *)start;
	handler->size = totalSize;
	handler->firstFree = start + sizeof(dPartition);

	// create a block list node for this entire momory
	EMB *block = (EMB *)handler->firstFree;
	block->size = totalSize - sizeof(dPartition);
	block->nextStart = 0;

	return start;
}

// using first fit algorithm to alloc a memory block with specific size
unsigned long dPartitionAllocFirstFit(unsigned long dpHandler, unsigned long size) {
	if (size <= 0) // illegal alloc size
		return 0;

	dPartition *handler = (dPartition *)dpHandler;
	if (!handler->firstFree) // no free block
		return 0;

	// align the size by 8 bytes (the size of data structure EMB)
	unsigned long sizeAlign = align8(size);
	unsigned long addr = handler->firstFree;
	unsigned long addrPre = 0;
	EMB *block;
	EMB *blockPre;
	
	while (addr) {
		block = (EMB *)addr;
		blockPre = (EMB *)addrPre;

		// we need to protect "size" field of EMB
		if (block->size >= sizeAlign + sizeof(unsigned long) && 
			block->size <= sizeAlign + sizeof(unsigned long) + sizeof(EMB)) {
			// directly alloc this block
			if (addrPre == 0) // this block is the first free block of dpHandler
				handler->firstFree = block->nextStart;
			else
				blockPre->nextStart = block->nextStart;

			return addr + sizeof(unsigned long); // user can't change "size" field of EMB
		}

		else if (block->size > sizeAlign + sizeof(unsigned long) + sizeof(EMB)) {
			// split this block into two
			unsigned long addrNewNode = addr + sizeof(unsigned long) + sizeAlign;
			EMB *blockNew = (EMB *)(addrNewNode);
			blockNew->size = block->size - sizeAlign - sizeof(unsigned long);
			blockNew->nextStart = block->nextStart;
			block->size -= blockNew->size;
			
			if (addrPre == 0) // this block is the first free block of dpHandler
				handler->firstFree = addrNewNode;
			else
				blockPre->nextStart = addrNewNode;

			return addr + sizeof(unsigned long); // user can't change "size" field of EMB
		}

		addrPre = addr;
		addr = block->nextStart;
	}

	// no fitted block
	return 0;
}

// free the memory block marked by start
unsigned long dPartitionFreeFirstFit(unsigned long dpHandler, unsigned long start) {

	// the real start of EMB, because the start given to user is the end of "size" field in EMB
	start -= sizeof(unsigned long);

	if (start < dpHandler + sizeof(dPartition)) // illegal start
		return 0;

	dPartition *handler = (dPartition *)dpHandler;

	if (start >= dpHandler + handler->size) // illegal start
		return 0;
	
	unsigned long addr = handler->firstFree;
	unsigned long addrPre = 0;
	unsigned long addrNext = 0;
	EMB *block;

	// find the closest pre and next free blocks of start
	while (addr) {
		block = (EMB *)addr;
		if (addr < start)
			addrPre = addr;
		else if (addr > start) {
			addrNext = addr;
			break;
		}
		addr = block->nextStart;
	}

	// free
	block = (EMB *)start;
	if (addrNext) { // if the next free block is found
		if (addrNext == start + block->size) { // adjacent, merge to next
			EMB *blockNext = (EMB *)addrNext;
			block->size += blockNext->size;
			block->nextStart = blockNext->nextStart;
		}
		else // not adjacent, dont't merge
			block->nextStart = addrNext;
	}
	else // no free blocks behind start
		block->nextStart = 0;

	if (addrPre) { // if the pre free block is found
		EMB *blockPre = (EMB *)addrPre;
		if (start == addrPre + blockPre->size) { // adjacent, merge to pre
			blockPre->size += block->size;
			blockPre->nextStart = block->nextStart;
		}
		else // not adjacent, dont't merge
			blockPre->nextStart = start;
	}
	else // no free blocks before start
		handler->firstFree = start;

	return 1;
}

// alloc a memory block with specific size
unsigned long dPartitionAlloc(unsigned long dpHandler, unsigned long size) {
	return dPartitionAllocFirstFit(dpHandler, size);
}

// free the memory block marked by start
unsigned long dPartitionFree(unsigned long dpHandler, unsigned long start) {
	return dPartitionFreeFirstFit(dpHandler, start);
}
