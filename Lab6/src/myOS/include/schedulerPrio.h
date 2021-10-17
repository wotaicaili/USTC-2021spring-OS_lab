#ifndef __SCHEDULER_PRIO_H__
#define __SCHEDULER_PRIO_H__

#include "task.h"

myTCB * nextTskPrio(void);
void enqueueTskPrio(myTCB *tsk);
myTCB * dequeueTskPrio(void);
void schedulerInitPrio(void);
void schedulePrio(void);

#endif