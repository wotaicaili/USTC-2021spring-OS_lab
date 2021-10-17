#ifndef __SCHEDULER_RR_H__
#define __SCHEDULER_RR_H__

#include "task.h"

#define TIME_SLICE 2

myTCB * nextTskRR(void);
void enqueueTskRR(myTCB *tsk);
myTCB * dequeueTskRR(void);
void schedulerInitRR(void);
void scheduleRR(void);

void preemptCurrentTaskRR_hook(void);

#endif