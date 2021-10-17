#include "taskQueueFIFO.h"
#include "task.h"
#include "types.h"
#include "schedulerRR.h"
#include "timer.h"
#include "tick.h"

taskQueueFIFO *rdyQueueRR;

myTCB * nextTskRR(void) {
    return taskQueueFIFONext(rdyQueueRR);
}

void enqueueTskRR(myTCB *tsk) {
    taskQueueFIFOEnqueue(rdyQueueRR, tsk);
}

myTCB * dequeueTskRR(void) {
    return taskQueueFIFODequeue(rdyQueueRR);
}

void schedulerInitRR(void) {
    taskQueueFIFOInit(rdyQueueRR);
}

void scheduleRR(void) {
	while (1) {

		myTCB *nextTsk;
		int idleTid;

		if (taskQueueFIFOEmpty(rdyQueueRR)) {
			if (!idleTsk) 
				idleTid = createTsk(idleTskBody);

			nextTsk = idleTsk = tcbPool[idleTid];
		}
		else
			nextTsk = dequeueTskRR();

		if (nextTsk == idleTsk && currentTsk == idleTsk) {
			continue;
		}
		
		if (currentTsk) {
			if (currentTsk->para->exec_time * TICK_FREQ <= currentTsk->runTime || currentTsk == idleTsk)
				destroyTsk(currentTsk->tid);
			if (currentTsk == idleTsk)
				idleTsk = NULL;
		}

		nextTsk->state = TSK_RUNNING;
		currentTsk = nextTsk;
		
		currentTsk->lastScheduledTime = get_current_time();
		context_switch(&BspContext, currentTsk->stkTop);
	}
}

void preemptCurrentTaskRR_hook(void) {
	if (currentTsk == idleTsk)
		return;
	
	if (currentTsk->runTime % (TIME_SLICE * TICK_FREQ) == 0 && get_current_time() > currentTsk->lastScheduledTime) {
		currentTsk->state = TSK_READY;
		enqueueTskRR(currentTsk);
		context_switch(&currentTsk->stkTop, BspContext);
	}
}