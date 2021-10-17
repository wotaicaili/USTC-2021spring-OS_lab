#include "myPrintk.h"
#include "mem.h"

// a free eFparted-memory block
typedef struct EEB {
	unsigned long nextStart;
} EEB;

// the handler of a eFparted-memory
typedef struct eFPartition {
	unsigned long totalN;
	unsigned long perSize;	
	unsigned long firstFree;
} eFPartition;

// show the info of a EEB
void showEEB(EEB *eeb) {
	myPrintk(0x7, "EEB (start = 0x%x, next = 0x%x)\n", eeb, eeb->nextStart);
}

// show the info of a EFPHandler
void showeFPartition(eFPartition *efp) {
	myPrintk(0x5, "eFPartition (start = 0x%x, totalN = 0x%x, perSize = 0x%x, firstFree = 0x%x)\n", efp, efp->totalN, efp->perSize, efp->firstFree);
}

// print the info of efpHandler and all of its free blocks
void eFPartitionWalkByAddr(unsigned long EFPHandler) {
	eFPartition *handler = (eFPartition *)EFPHandler;
	showeFPartition(handler);

	unsigned long addr = handler->firstFree;
	EEB *block;
	while (addr) {
    	block = (EEB *)addr;
		showEEB(block);
		addr = block->nextStart;
	}
}

// align a value by 4 bytes
unsigned long align4(unsigned long value) {
    if (value & 1) value += 1;
    if (value & 2) value += 2;
    return value;
}

// calculate the total size of a eFparted-memory
unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n) {
	if (perSize <= 0) // illegal perSize
		return 0;

	unsigned long perSizeAlign = align4(perSize);
	return perSizeAlign * n + sizeof(eFPartition);
}

// init a eFparted-memory, return its handler
unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n) {
	if (perSize <= 0) // illegal perSize
		return 0;

	unsigned long perSizeAlign = align4(perSize);

	eFPartition *handler = (eFPartition *)start;
	handler->perSize = perSizeAlign;
	handler->totalN = n;
	unsigned long addr = start + sizeof(eFPartition);
	handler->firstFree = addr;

	// divide this entire memory block into equal-sized blocks
    EEB *block;
    for (int i = 0; i < n; i++) {
        block = (EEB *)addr; // store the data structure in free memory itself
        addr += perSizeAlign;
		block->nextStart = addr;
    }
	block->nextStart = 0;

    return start;
}

// alloc the first free memory block of EFPHandler, return its start address
// if no free block, return 0
unsigned long eFPartitionAlloc(unsigned long EFPHandler) {
	eFPartition *handler = (eFPartition *)EFPHandler;

	// no free block to alloc
	if (!handler->firstFree)
		return 0;

    EEB *block = (EEB *)handler->firstFree;
	handler->firstFree = block->nextStart;

	// return the start of alloced memory
	return (unsigned long)block;
}

// free the memory block marked by mbstart. if free failed, return 0
unsigned long eFPartitionFree(unsigned long EFPHandler, unsigned long mbStart) {
	// illegal mbStart
	if (mbStart < EFPHandler + sizeof(eFPartition))
		return 0;

	eFPartition *handler = (eFPartition *)EFPHandler;

	// illegal mbStart
	if (mbStart > EFPHandler + eFPartitionTotalSize(handler->perSize, handler->totalN))
		return 0;;
	
	unsigned long addr = handler->firstFree;
	unsigned long addrPre = 0;
	unsigned long addrNext = 0;
	EEB *block;

	// find the closest pre and next free blocks of start
	while (addr) {
		block = (EEB *)addr;
		if (addr < mbStart)
			addrPre = addr;
		else if (addr > mbStart) {
			addrNext = addr;
			break;
		}
		addr = block->nextStart;
	}

	// free
	block = (EEB *)mbStart;
	if (addrNext)  // if the next free block is found
		block->nextStart = addrNext;
	else // no free blocks behind start
		block->nextStart = 0;

	if (addrPre) { // if the pre free block is found
		EEB *blockPre = (EEB *)addrPre;
		blockPre->nextStart = mbStart;
	}
	else // no free blocks before start
		handler->firstFree = mbStart;

	return 1;
}
