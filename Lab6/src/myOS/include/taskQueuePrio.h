#ifndef __TASK_QUEUE_PRIO_H__
#define __TASK_QUEUE_PRIO_H__

#include "task.h"

typedef struct taskQueuePrio {
    myTCB **tcb;
    int tail;
    int capacity;
    int (*cmp)(const myTCB *a, const myTCB *b);
} taskQueuePrio;

void taskQueuePrioInit(taskQueuePrio **queue, int capacity, int (*cmp)(const myTCB *a, const myTCB *b));
int taskQueuePrioEmpty(taskQueuePrio *queue);
myTCB * taskQueuePrioNext(taskQueuePrio *queue);
void taskQueuePrioEnqueue(taskQueuePrio *queue, myTCB *tsk);
myTCB * taskQueuePrioDequeue(taskQueuePrio *queue);

#endif
