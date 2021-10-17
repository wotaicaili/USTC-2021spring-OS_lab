#include "taskQueuePrio.h"
#include "task.h"
#include "types.h"
#include "tick.h"

taskQueuePrio *rdyQueuePrio;

int compare_priority(const myTCB *a, const myTCB *b) {
	if (getTskPara(PRIORITY, a->para) == getTskPara(PRIORITY, b->para))
		return getTskPara(ARRV_TIME, a->para) - getTskPara(ARRV_TIME, b->para);
	else
		return getTskPara(PRIORITY, a->para) - getTskPara(PRIORITY, b->para);
}

myTCB * nextTskPrio(void) {
    return taskQueuePrioNext(rdyQueuePrio);
}

void enqueueTskPrio(myTCB *tsk) {
    taskQueuePrioEnqueue(rdyQueuePrio, tsk);
}

myTCB * dequeueTskPrio(void) {
    return taskQueuePrioDequeue(rdyQueuePrio);
}

void schedulerInitPrio(void) {
    taskQueuePrioInit(&rdyQueuePrio, taskNum, compare_priority);
}

void schedulePrio(void) {
	while (1) {
		
		myTCB *nextTsk;
		int idleTid;

		if (taskQueuePrioEmpty(rdyQueuePrio)) {
			if (!idleTsk) 
				idleTid = createTsk(idleTskBody);

			nextTsk = idleTsk = tcbPool[idleTid];
		}
		else
			nextTsk = dequeueTskPrio();

		if (nextTsk == idleTsk && currentTsk == idleTsk)
			// current task is the idleTask, and the ready queue is empty
			continue; // do nothing
			
		// schedule, context switch
		if (currentTsk) {
			if (currentTsk->para->exec_time * TICK_FREQ <= currentTsk->runTime || currentTsk == idleTsk);
				destroyTsk(currentTsk->tid);
			if (currentTsk == idleTsk)
				idleTsk = NULL;
		}

		nextTsk->state = TSK_RUNNING;
		currentTsk = nextTsk;
		
		context_switch(&BspContext, currentTsk->stkTop);

	}
}