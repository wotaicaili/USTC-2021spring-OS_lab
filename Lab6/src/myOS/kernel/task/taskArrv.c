#include "tick.h"
#include "timer.h"
#include "task.h"
#include "taskPara.h"
#include "taskQueuePrio.h"

taskQueuePrio *arrvQueue;

int compare_arrv_time(const myTCB *a, const myTCB *b) {
    return getTskPara(ARRV_TIME, a->para) - getTskPara(ARRV_TIME, b->para);
}

void startArrivedTask_hook(void) {
	if (taskQueuePrioEmpty(arrvQueue))
		return;

	myTCB *nextTask = taskQueuePrioNext(arrvQueue);
	if (get_current_time() >= getTskPara(ARRV_TIME, nextTask->para)) {
		tskStart(nextTask->tid);
		taskQueuePrioDequeue(arrvQueue);
	}
}

void taskArrvQueueInit(void) {
	taskQueuePrioInit(&arrvQueue, taskNum, compare_arrv_time);
	append2HookList(startArrivedTask_hook);
}

void enableTask(int tid) {
	if (tcbPool[tid]->para->arrv_time == 0)
		tskStart(tid);
	else
		taskQueuePrioEnqueue(arrvQueue, tcbPool[tid]);
}