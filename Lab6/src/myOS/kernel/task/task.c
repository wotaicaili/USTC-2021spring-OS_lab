#include "task.h"
#include "taskPara.h"
#include "taskArrv.h"
#include "myPrintk.h"
#include "kmalloc.h"
#include "types.h"
#include "tick.h"
#include "interrupt.h"
#include "scheduler.h"

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

// allocate a free tcb for the task, build up and init its stack
int createTsk(void (*tskBody)(void)) {
	if (!firstFreeTCB)
		return -1;
	
	myTCB *newTsk = firstFreeTCB;
	firstFreeTCB = firstFreeTCB->next;

	initTskPara(&newTsk->para);
	newTsk->runTime = 0;
	newTsk->entry = tskBody;
	newTsk->lastScheduledTime = 0;
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
	kfree((unsigned long)tcbPool[tid]->para);
	tcbPool[tid]->runTime = 0;
	tcbPool[tid]->lastScheduledTime = 0;
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
	sysSch.enqueueTsk(tcbPool[tid]);
}

/* end a task
 * this function is pushed into the stack of a task in advance 
 * so it will be called automatically when the tskBdy() returns 
 */
void tskEnd(void) {
	// dequeue the current task from the ready queue
	// return to the context which resides in the kernel stack
	context_switch(&currentTsk->stkTop, BspContext);
}

//============================== Schedule ==============================//

void schedule(void) {
	sysSch.schedule();
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
	myPrintk(0x8, "*********************************\n");
	myPrintk(0x8, "*           Idle Task           *\n");
	myPrintk(0x8, "*     Waiting for new task...   *\n");
	myPrintk(0x8, "*********************************\n");
}

void updateCurrentTskRunTime_hook(void) {
	//if (get_tick_times() % TICK_FREQ != 0)
		//return;
	disable_interrupt();
	currentTsk->runTime++;
	enable_interrupt();
}

void TaskManagerInit(void) {

	switch (sysSch.type) {
		case FCFS:
			taskNum = FCFS_TASK_NUM + 2;
			break;
		case PRIO:
			taskNum = PRIO_TASK_NUM + 2;
			break;
		case RR:
			taskNum = RR_TASK_NUM + 2;
			break;
		case SJF:
			taskNum = SJF_TASK_NUM + 2;
			break;
	}

	myTCB *prevTCB = NULL;
	tcbPool = (myTCB **)kmalloc(taskNum * sizeof(myTCB *));
	for (int i = 0; i < taskNum; i++) {
		tcbPool[i] = (myTCB *)kmalloc(sizeof(myTCB));
		tcbPool[i]->tid = i;
		tcbPool[i]->entry = NULL;
		tcbPool[i]->state = TSK_WAITING;
		tcbPool[i]->stkMAX = NULL;
		tcbPool[i]->stkTop = NULL;
		tcbPool[i]->next = NULL;
		tcbPool[i]->para = NULL;
		tcbPool[i]->runTime = 0;
		tcbPool[i]->lastScheduledTime = 0;
		if (prevTCB) 
			prevTCB->next = tcbPool[i];
		prevTCB = tcbPool[i];
	}
	firstFreeTCB = tcbPool[0];

	initSysSch();
	taskArrvQueueInit();

	int initTid = createTsk(initTskBody);
	idleTsk = NULL;
	tskStart(initTid);
	enable_interrupt();

	append2HookList(updateCurrentTskRunTime_hook);
	append2HookList(showCurrentTskParaInfo_hook);
	
	startMultitask();
}
