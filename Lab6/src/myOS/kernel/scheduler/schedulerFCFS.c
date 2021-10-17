#include "taskQueueFIFO.h"
#include "task.h"
#include "types.h"
#include "tick.h"

taskQueueFIFO *rdyQueueFCFS;

myTCB * nextTskFCFS(void) {
    return taskQueueFIFONext(rdyQueueFCFS);
}

void enqueueTskFCFS(myTCB *tsk) {
    taskQueueFIFOEnqueue(rdyQueueFCFS, tsk);
}

myTCB * dequeueTskFCFS(void) {
    return taskQueueFIFODequeue(rdyQueueFCFS);
}

void schedulerInitFCFS(void) {
    taskQueueFIFOInit(rdyQueueFCFS);
}

void scheduleFCFS(void) {
	while (1) {
		
		myTCB *nextTsk;
		int idleTid;

		if (taskQueueFIFOEmpty(rdyQueueFCFS)) {
			if (!idleTsk) 
				idleTid = createTsk(idleTskBody);

			nextTsk = idleTsk = tcbPool[idleTid];
		}
		else
			nextTsk = dequeueTskFCFS();

		if (nextTsk == idleTsk && currentTsk == idleTsk)
			// current task is the idleTask, and the ready queue is empty
			continue; // do nothing
			
		// schedule, context switch
		if (currentTsk ) {
			if (currentTsk->para->exec_time * TICK_FREQ <= currentTsk->runTime || currentTsk == idleTsk)
				destroyTsk(currentTsk->tid);
			if (currentTsk == idleTsk)
				idleTsk = NULL;
		}

		nextTsk->state = TSK_RUNNING;
		currentTsk = nextTsk;
		
		context_switch(&BspContext, currentTsk->stkTop);
	}
}