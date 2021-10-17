#include "taskQueuePrio.h"
#include "task.h"
#include "types.h"
#include "tick.h"

taskQueuePrio *rdyQueueSJF;

int compare_exec_time(const myTCB *a, const myTCB *b) {
	if (getTskPara(EXEC_TIME, a->para) == getTskPara(EXEC_TIME, b->para))
		return getTskPara(ARRV_TIME, a->para) - getTskPara(ARRV_TIME, b->para);
	else
		return getTskPara(EXEC_TIME, a->para) - getTskPara(EXEC_TIME, b->para);
}

myTCB * nextTskSJF(void) {
    return taskQueuePrioNext(rdyQueueSJF);
}

void enqueueTskSJF(myTCB *tsk) {
    taskQueuePrioEnqueue(rdyQueueSJF, tsk);
}

myTCB * dequeueTskSJF(void) {
    return taskQueuePrioDequeue(rdyQueueSJF);
}

void schedulerInitSJF(void) {
    taskQueuePrioInit(&rdyQueueSJF, taskNum, compare_exec_time);
}

void scheduleSJF(void) {
	while (1) {
		
		myTCB *nextTsk;
		int idleTid;

		if (taskQueuePrioEmpty(rdyQueueSJF)) {
			if (!idleTsk) 
				idleTid = createTsk(idleTskBody);

			nextTsk = idleTsk = tcbPool[idleTid];
		}
		else
			nextTsk = dequeueTskSJF();

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