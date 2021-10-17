#include "taskQueuePrio.h"
#include "types.h"
#include "interrupt.h"
#include "kmalloc.h"

// init the priority queue, determine the compare function
void taskQueuePrioInit(taskQueuePrio **queue, int capacity, int (*cmp)(const myTCB *a, const myTCB *b)) {
    *queue = (taskQueuePrio *)kmalloc(sizeof(taskQueuePrio));
    (*queue)->tcb = (myTCB **)kmalloc((capacity + 1) * sizeof(myTCB *));
    (*queue)->tail = 0;
    (*queue)->capacity = capacity;
    (*queue)->cmp = cmp;
}

int taskQueuePrioEmpty(taskQueuePrio *queue) {
    int bool = (queue->tail == 0);
    return bool;
}

myTCB * taskQueuePrioNext(taskQueuePrio *queue) {
    if (taskQueuePrioEmpty(queue))
		return NULL;
	else
    	return queue->tcb[1];
}

void taskQueuePrioEnqueue(taskQueuePrio *queue, myTCB *tsk) {

    disable_interrupt();
    int i = ++queue->tail;
    while (queue->cmp(queue->tcb[i / 2], tsk) > 0) {
        queue->tcb[i] = queue->tcb[i / 2];
        i = i / 2;
    }
    queue->tcb[i] = tsk;
    enable_interrupt();
}

myTCB * taskQueuePrioDequeue(taskQueuePrio *queue) {
    if (taskQueuePrioEmpty(queue))
        return NULL;

    disable_interrupt();
    myTCB *task = queue->tcb[1];
    queue->tail--;
    int i = 2;
    while (i <= queue->tail) {

        if (i < queue->tail && queue->cmp(queue->tcb[i], queue->tcb[i + 1]) > 0)
            i++;

        if (i <= queue->tail && queue->cmp(queue->tcb[queue->tail + 1], queue->tcb[i]) > 0)
            queue->tcb[i / 2] = queue->tcb[i];
        else
            break;

        i *= 2;
    }
    queue->tcb[i / 2] = queue->tcb[queue->tail + 1];
    enable_interrupt();
    return task;
}