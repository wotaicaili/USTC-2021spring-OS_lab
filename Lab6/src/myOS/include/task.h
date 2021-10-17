#ifndef __TASK_H__
#define __TASK_H__

#include "taskPara.h"
#include "userApp.h"

#define STACK_SIZE 0x1000
#define initTskBody myMain // connect initTask with myMain

// task states
#define TSK_WAITING 0
#define TSK_READY 1
#define TSK_RUNNING 2

void initTskBody(void);
void CTX_SW(void);

// task control block TCB
typedef struct myTCB {
    int tid; // task id
    int state; // task state
    unsigned int runTime; // the time this task has been executed
    unsigned int lastScheduledTime; 
    unsigned long *stkTop; // the top of the stack
    unsigned long *stkMAX; // the MAX address of the stack, reside in the lower address
    void (*entry)(void); // the entry of task function body
    tskPara *para;
    struct myTCB *next;
} myTCB;

myTCB **tcbPool; // tcb pool
myTCB *firstFreeTCB; // the first free tcb in the pool
myTCB *idleTsk; // idle task pointer
myTCB *currentTsk; // current task pointer

unsigned long *BspContextBase;
unsigned long *BspContext;
unsigned int taskNum;

void idleTskBody(void);
void TaskManagerInit(void);
void tskEnd(void);

int createTsk(void (*tskBody)(void));
void destroyTsk(int tid);
void context_switch(unsigned long **prevTskStkAddr, unsigned long *nextTskStk);
void tskStart(int tid);

#endif
