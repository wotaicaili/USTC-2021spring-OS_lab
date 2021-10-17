#include "taskQueueFIFO.h"
#include "types.h"
#include "interrupt.h"
#include "kmalloc.h"

// init the ready queue
void taskQueueFIFOInit(taskQueueFIFO *queue) {
    queue->head = queue->tail = NULL;
}

// to judge whether the ready queue is empty
int taskQueueFIFOEmpty(taskQueueFIFO *queue) {
    return (queue->head == NULL && queue->tail == NULL);
}

// get the task at the head of the ready queue, not deqeue
myTCB * taskQueueFIFONext(taskQueueFIFO *queue) {
	if (taskQueueFIFOEmpty(queue))
		return NULL;
	else
    	return queue->head->TCB;
}

// enqueue a task to the ready queue, insert it to the tail
void taskQueueFIFOEnqueue(taskQueueFIFO *queue, myTCB *tsk) {
    queueNodeFIFO *newNode = (queueNodeFIFO *)kmalloc(sizeof(queueNodeFIFO));
    if (!newNode)
        return;

    newNode->TCB = tsk;
    newNode->next = NULL;

    disable_interrupt();
    if (taskQueueFIFOEmpty(queue))
        queue->head = queue->tail = newNode;
    else {
        queue->tail->next = newNode;
        queue->tail = newNode;
    }
    enable_interrupt();
}

// dequeue the task at the head of the ready queue
myTCB * taskQueueFIFODequeue(taskQueueFIFO *queue) {
    if (taskQueueFIFOEmpty(queue))
        return NULL;

    else {
        queueNodeFIFO *node = queue->head;
        myTCB *task = node->TCB;

        disable_interrupt();
        if (queue->head == queue->tail)
            queue->head = queue->tail = NULL;
        else
            queue->head = node->next;
        enable_interrupt();
        
        kfree((unsigned long)node);

        return task;
    }
}