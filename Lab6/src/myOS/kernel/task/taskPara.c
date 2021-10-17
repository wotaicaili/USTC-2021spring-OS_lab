#include "taskPara.h"
#include "kmalloc.h"
#include "task.h"
#include "tick.h"
#include "myPrintk.h"

extern int startShellFlag;

void task_execute(unsigned int wait_time) {
	while (currentTsk->runTime < getTskPara(EXEC_TIME, currentTsk->para) * TICK_FREQ);
}

void showCurrentTskParaInfo_hook(void) {
	if (get_tick_times() % TICK_FREQ != 0 || startShellFlag)
		return;

	if (currentTsk == idleTsk) // dont't show idleTsk's info
		return;

	myPrintk(0x7, "*********************************\n");
	myPrintk(0x7, "*  Current task tid : %-2d        *\n", currentTsk->tid);
	myPrintk(0x7, "*  Priority         : %-2d        *\n", getTskPara(PRIORITY, currentTsk->para));
	myPrintk(0x7, "*  Arrive time      : %-2d        *\n", getTskPara(ARRV_TIME, currentTsk->para));
	myPrintk(0x7, "*  Execute time     : %-2d / %2d   *\n", (currentTsk->runTime + 50) / TICK_FREQ, getTskPara(EXEC_TIME, currentTsk->para));
	myPrintk(0x7, "*********************************\n");
}

void initTskPara(tskPara **para) {
	*para = (tskPara *)kmalloc(sizeof(tskPara));
	(*para)->arrv_time = 0;
	(*para)->exec_time = 0;
	(*para)->priority = 0;
}

void setTskPara(unsigned int option, unsigned int value, tskPara *para) {
	switch (option) {
		case PRIORITY:
			para->priority = value > MAX_PRIORITY ? MAX_PRIORITY : value;
			break;
		case EXEC_TIME:
			para->exec_time = value > MAX_EXEC_TIME ? MAX_EXEC_TIME : value;
			break;
		case ARRV_TIME:
			para->arrv_time = value;
			break;
		default:
			break;
	}
}

unsigned int getTskPara(unsigned int option, tskPara *para) {
	switch (option) {
		case PRIORITY:
			return para->priority;
		case EXEC_TIME:
			return para->exec_time;
		case ARRV_TIME:
			return para->arrv_time;
		default:
			return 0;
	}
}
