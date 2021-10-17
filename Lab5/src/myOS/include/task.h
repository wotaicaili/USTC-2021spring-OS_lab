#ifndef __TASK_H__
#define __TASK_H__

#ifndef USER_TASK_NUM
#include "userApp.h"
#endif

#define STACK_SIZE 0x1000
#define TASK_NUM (2 + USER_TASK_NUM) // at least: 0-idle, 1-init
#define initTskBody myMain // connect initTask with myMain

void initTskBody(void);
void CTX_SW(void);

// task control block TCB
typedef struct myTCB {
    int tid; // task id
    int state; // task state
    unsigned long *stkTop; // the top of the stack
    unsigned long *stkMAX; // the MAX address of the stack, reside in the lower address
    void (*entry)(void); // the entry of task function body
    struct myTCB *next;
} myTCB;

myTCB *tcbPool[TASK_NUM];
myTCB *firstFreeTCB;

myTCB *idleTsk; // idle task pointer
myTCB *currentTsk; // current task pointer

void TaskManagerInit(void);

int createTsk(void (*tskBody)(void));
void tskStart(int tid);

#endif
