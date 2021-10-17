#ifndef __SCHEDULER_FCFS_H__
#define __SCHEDULER_FCFS_H__

#include "task.h"

myTCB * nextTskFCFS(void);
void enqueueTskFCFS(myTCB *tsk);
myTCB * dequeueTskFCFS(void);
void schedulerInitFCFS(void);
void scheduleFCFS(void);

#endif