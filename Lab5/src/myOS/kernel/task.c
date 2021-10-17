#include "task.h"
#include "myPrintk.h"
#include "kmalloc.h"
#include "types.h"

//============================== FCFS Data Structure ==============================//

typedef struct rdyQueueNode {
    myTCB *TCB;
    struct rdyQueueNode *next;
} rdyQueueNode;

// FCFS ready queue for tasks
typedef struct rdyQueueFCFS {
    rdyQueueNode *head;
    rdyQueueNode *tail;
} rdyQueueFCFS;
rdyQueueFCFS rdyQueue;

// init the ready queue
void rqFCFSInit(void) {
    rdyQueue.head = rdyQueue.tail = NULL;
}

// to judge whether the ready queue is empty
int rqFCFSIsEmpty(void) {
    return (rdyQueue.head == NULL && rdyQueue.tail == NULL);
}

// get the task at the head of the ready queue, not deqeue
myTCB * nextFCFSTsk(void) {
	if (rqFCFSIsEmpty())
		return idleTsk;
	else
    	return rdyQueue.head->TCB;
}

// enqueue a task to the ready queue, insert it to the tail
void tskEnqueueFCFS(myTCB *tsk) {
    rdyQueueNode *newNode = (rdyQueueNode *)kmalloc(sizeof(rdyQueueNode));
    if (!newNode)
        return;

    newNode->TCB = tsk;
    newNode->next = NULL;

    if (rqFCFSIsEmpty())
        rdyQueue.head = rdyQueue.tail = newNode;
    else {
        rdyQueue.tail->next = newNode;
        rdyQueue.tail = newNode;
    }
}

// dequeue the task at the head of the ready queue
void tskDequeueFCFS(void) {
    if (rqFCFSIsEmpty())
        return;

    else {
        rdyQueueNode *node = rdyQueue.head;
        if (rdyQueue.head == rdyQueue.tail)
            rdyQueue.head = rdyQueue.tail = NULL;
        else
            rdyQueue.head = node->next;
        kfree((unsigned long)node);
    }
}

//============================== Context Switch ==============================//

unsigned long **prevTSK_StackPtrAddr;
unsigned long *nextTSK_StackPtr;
void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk) {
	prevTSK_StackPtrAddr = prevTskStkAddr;
	nextTSK_StackPtr = nextTskStk;
	CTX_SW();
}

void tskEnd(void);

// init the stack of a newly-created task, make it prepared for its first CTX_SW
void stack_init(unsigned long **stk, void (*task)(void)) {
	// push tskbdy() entry address into eip
	*(*stk)-- = (unsigned long)tskEnd; 

	// push tskbdy() entry address into eip
	*(*stk)-- = (unsigned long)task;

	// pushf
	*(*stk)-- = (unsigned long)0x0202; // flag registers

	// pusha
	*(*stk)-- = (unsigned long)0xAAAAAAAA; // eax
	*(*stk)-- = (unsigned long)0xCCCCCCCC; // ecx
	*(*stk)-- = (unsigned long)0xDDDDDDDD; // edx
	*(*stk)-- = (unsigned long)0xBBBBBBBB; // ebx
	*(*stk)-- = (unsigned long)0x44444444; // esp
	*(*stk)-- = (unsigned long)0x55555555; // ebp
	*(*stk)-- = (unsigned long)0x66666666; // esi
	**stk     = (unsigned long)0x77777777; // edi
}

//============================== TCB Operations ==============================//

// task states
#define TSK_WAITING 0
#define TSK_READY 1
#define TSK_RUNNING 2

// allocate a free tcb for the task, build up and init its stack
int createTsk(void (*tskBody)(void)) {
	if (!firstFreeTCB)
		return -1;
	
	myTCB *newTsk = firstFreeTCB;
	firstFreeTCB = firstFreeTCB->next;

	newTsk->entry = tskBody;
	newTsk->stkMAX = (unsigned long *)kmalloc(STACK_SIZE);
	if (!newTsk->stkMAX)
		return -1;	
	newTsk->stkTop = newTsk->stkMAX + STACK_SIZE - 1;
	
	stack_init(&newTsk->stkTop, tskBody);

	return newTsk->tid;
}

// free a tcb, return it back to the tcb pool
void destroyTsk(int tid) {
	kfree((unsigned long)tcbPool[tid]->stkMAX); //free the stack
	tcbPool[tid]->entry = NULL;
	tcbPool[tid]->state = TSK_WAITING;
	tcbPool[tid]->stkMAX = NULL;
	tcbPool[tid]->stkTop = NULL;
	tcbPool[tid]->next = firstFreeTCB;
	firstFreeTCB = tcbPool[tid];
}

//============================== Task Operations ==============================//

// start a task, insert it to the ready queue
void tskStart(int tid) {
	tcbPool[tid]->state = TSK_READY;
	tskEnqueueFCFS(tcbPool[tid]);
}

// kernel stack and its top ptr
unsigned long *BspContextBase;
unsigned long *BspContext;

/* end a task
 * this function is pushed into the stack of a task in advance 
 * so it will be called automatically when the tskBdy() returns 
 */
void tskEnd(void) {
	// dequeue the current task from the ready queue
	tskDequeueFCFS();
	// return to the context which resides in the kernel stack
	context_switch(&currentTsk->stkTop, BspContext);
}

//============================== FCFS Schedule ==============================//

void scheduleFCFS(void) {
	while (1) {
		myTCB *nextTsk = nextFCFSTsk();

		if (nextTsk == idleTsk && currentTsk == idleTsk)
			// current task is the idleTask, and the ready queue is empty
			return; // do nothing
			
		// schedule, context switch
		if (currentTsk)
			destroyTsk(currentTsk->tid);

		nextTsk->state = TSK_RUNNING;
		currentTsk = nextTsk;
			
		context_switch(&BspContext, currentTsk->stkTop);
	}
}

void schedule(void) {
	scheduleFCFS();
}

//============================== others ==============================//

void startMultitask(void) {
	// build up the kernal stack
	BspContextBase = (unsigned long *)kmalloc(10 * STACK_SIZE);
	BspContext = BspContextBase + STACK_SIZE - 1;
	
	currentTsk = NULL;

	// start multitasking through schedule
	schedule();
}

void idleTskBody(void) {
	myPrintf(0x6, "*********************************\n");
	myPrintf(0x6, "*           Idle Task           *\n");
	myPrintf(0x6, "*    Waiting for scheduling...  *\n");
	myPrintf(0x6, "*********************************\n");

	while (1)
		schedule();
}

void TaskManagerInit(void) {
	myTCB *prevTCB = NULL;
	for (int i = 0; i < TASK_NUM; i++) {
		tcbPool[i] = (myTCB *)kmalloc(sizeof(myTCB));
		tcbPool[i]->tid = i;
		tcbPool[i]->entry = NULL;
		tcbPool[i]->state = TSK_WAITING;
		tcbPool[i]->stkMAX = NULL;
		tcbPool[i]->stkTop = NULL;
		tcbPool[i]->next = NULL;
		if (prevTCB) 
			prevTCB->next = tcbPool[i];
		prevTCB = tcbPool[i];
	}
	firstFreeTCB = tcbPool[0];

	int initTid = createTsk(initTskBody);
	int idleTid = createTsk(idleTskBody);
	idleTsk = tcbPool[idleTid];

	tskStart(initTid);
	
	startMultitask();
}
