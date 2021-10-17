#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "task.h"

#define FCFS 0
#define PRIO 1
#define RR 2
#define SJF 3

typedef struct scheduler {
    unsigned int type;

	myTCB * (*nextTsk)(void);
	void (*enqueueTsk)(myTCB *tsk);
	myTCB * (*dequeueTsk)(void);
	void (*schedulerInit)(void);
	void (*schedule)(void);
	void (*tick_hook)(void);
} scheduler;

scheduler sysSch;

void initSysSch(void);

#endif