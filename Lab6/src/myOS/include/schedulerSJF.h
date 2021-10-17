#ifndef __SCHEDULER_SJF_H__
#define __SCHEDULER_SJF_H__

#include "task.h"

myTCB * nextTskSJF(void);
void enqueueTskSJF(myTCB *tsk);
myTCB * dequeueTskSJF(void);
void schedulerInitSJF(void);
void scheduleSJF(void);

#endif