#ifndef __TASK_QUEUE_FIFO_H__
#define __TASK_QUEUE_FIFO_H__

#include "task.h"

typedef struct queueNodeFIFO {
    myTCB *TCB;
    struct queueNodeFIFO *next;
} queueNodeFIFO;

typedef struct taskQueueFIFO {
    queueNodeFIFO *head;
    queueNodeFIFO *tail;
} taskQueueFIFO;

void taskQueueFIFOInit(taskQueueFIFO *queue);
int taskQueueFIFOEmpty(taskQueueFIFO *queue);
myTCB * taskQueueFIFONext(taskQueueFIFO *queue);
void taskQueueFIFOEnqueue(taskQueueFIFO *queue, myTCB *tsk);
myTCB * taskQueueFIFODequeue(taskQueueFIFO *queue);

#endif
