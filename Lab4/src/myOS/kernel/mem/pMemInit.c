#include "myPrintk.h"
#include "mem.h"
#include "malloc.h"

// pMem = the available memory 
unsigned long pMemStart;
unsigned long pMemSize;
unsigned long pMemHandler;

// kernel and user memory for kmalloc and malloc
unsigned long kMemStart;
unsigned long kMemHandler;
unsigned long uMemStart;
unsigned long uMemHandler;

// test the pMem
void memTest(unsigned long start, unsigned long grainSize) {

	if (start < 0x100000)
		start = 0x100000;
	if (grainSize < 2)
		grainSize = 2;

	unsigned long addr = start;
    unsigned short data;
    unsigned short *addr_head, *addr_tail;
    unsigned short test_data1 = 0xAA55;
    unsigned short test_data2 = 0x55AA;
	int flag_fail = 0; // whether the test fails
	pMemSize = 0;
	pMemStart = start;

	// get the size of pMem
    while (!flag_fail) {
        flag_fail = 0;

        addr_head = (unsigned short *)addr; // the first 2 bytes of current grain
        addr_tail = (unsigned short *)(addr + grainSize - 2); // the last 2 bytes of current grain

        data = *addr_head; // read original data
        *addr_head = test_data1; // write 0xAA55
        if (*addr_head != test_data1)
            flag_fail = 1;
        *addr_head = test_data2; // write 0x55AA
        if (*addr_head != test_data2)
            flag_fail = 1;
        *addr_head = data; // write back original data

        data = *addr_tail; // read original data
        *addr_tail = test_data1; // write 0xAA55
        if (*addr_tail != test_data1)
            flag_fail = 1;
        *addr_tail = test_data2; // write 0x55AA
        if (*addr_tail != test_data2)
            flag_fail = 1;
        *addr_tail = data; // write back original data
        
        if (!flag_fail) {
            addr += grainSize;
            pMemSize += grainSize;
        }
    }
	
	myPrintk(0x7, "MemStart: %x\n", pMemStart);
	myPrintk(0x7, "MemSize:  %x\n", pMemSize);
}

extern unsigned long _end;
void pMemInit(void) {
	unsigned long _end_addr = (unsigned long) &_end;
	memTest(0x100000, 0x1000);
	myPrintk(0x7, "_end:     %x\n", _end_addr);
	if (pMemStart <= _end_addr) {
		pMemSize -= _end_addr - pMemStart;
		pMemStart = _end_addr;
	}

    // use dPartition algorithm to init pMem
	pMemHandler = dPartitionInit(pMemStart, pMemSize);

    // on the foundation of pMemHandler, alloc two large memory blocks for malloc and kmalloc
    kMemStart = dPartitionAlloc(pMemHandler, pMemSize / 3);
    uMemStart = dPartitionAlloc(pMemHandler, pMemSize / 3);
    // also use dPartition algorithm to init kMem and uMem
    kMemHandler = dPartitionInit(kMemStart, pMemSize / 3);
    uMemHandler = dPartitionInit(uMemStart, pMemSize / 3);
}
