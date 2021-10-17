#include "mem.h"

// malloc for user
unsigned long malloc(unsigned long size) {
    return dPartitionAlloc(uMemHandler, size);
}

// free for user
unsigned long free(unsigned long start) {
    return dPartitionFree(uMemHandler, start);
}

// kmalloc for kernel, seperate from malloc
unsigned long kmalloc(unsigned long size) {
    return dPartitionAlloc(kMemHandler, size);
}

// kfree for kernel, seperate from free
unsigned long kfree(unsigned long start) {
    return dPartitionFree(kMemHandler, start);
}