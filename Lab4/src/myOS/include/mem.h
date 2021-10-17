#ifndef __MEM_H__
#define __MEM_H__

extern unsigned long uMemHandler;
extern unsigned long kMemHandler;

unsigned long align4(unsigned long value);

void pMemInit(void);

unsigned long dPartitionInit(unsigned long start, unsigned long size);
unsigned long dPartitionAlloc(unsigned long dpHandler, unsigned long size);
unsigned long dPartitionFree(unsigned long dpHandler, unsigned long start);
void dPartitionWalkByAddr(unsigned long dpHandler);

unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n);
unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n);
unsigned long eFPartitionAlloc(unsigned long EFPHandler);
unsigned long eFPartitionFree(unsigned long EFPHandler, unsigned long mbStart);
void eFPartitionWalkByAddr(unsigned long EFPHandler);

#endif