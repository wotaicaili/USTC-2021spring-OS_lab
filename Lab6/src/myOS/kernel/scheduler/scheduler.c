#include "scheduler.h"
#include "schedulerFCFS.h"
#include "schedulerRR.h"
#include "schedulerPrio.h"
#include "schedulerSJF.h"
#include "types.h"
#include "vga.h"
#include "tick.h"

void initSysSch(void) {
    switch (sysSch.type) {
        case FCFS:
            sysSch.schedulerInit = schedulerInitFCFS;
            sysSch.nextTsk = nextTskFCFS;
            sysSch.enqueueTsk = enqueueTskFCFS;
            sysSch.dequeueTsk = dequeueTskFCFS;
            sysSch.schedule = scheduleFCFS;
            sysSch.tick_hook = NULL;
            put_chars("FCFS", 0x6, VGA_SCREEN_HEIGHT - 1, VGA_SCREEN_WIDTH  / 2 - 2);
            break;
        case RR:
            sysSch.schedulerInit = schedulerInitRR;
            sysSch.nextTsk = nextTskRR;
            sysSch.enqueueTsk = enqueueTskRR;
            sysSch.dequeueTsk = dequeueTskRR;
            sysSch.schedule = scheduleRR;
            sysSch.tick_hook = preemptCurrentTaskRR_hook;
            append2HookList(preemptCurrentTaskRR_hook);
            put_chars(" RR ", 0x6, VGA_SCREEN_HEIGHT - 1, VGA_SCREEN_WIDTH  / 2 - 2);
            break;
        case PRIO:
            sysSch.schedulerInit = schedulerInitPrio;
            sysSch.nextTsk = nextTskPrio;
            sysSch.enqueueTsk = enqueueTskPrio;
            sysSch.dequeueTsk = dequeueTskPrio;
            sysSch.schedule = schedulePrio;
            sysSch.tick_hook = NULL;
            put_chars("PRIO", 0x6, VGA_SCREEN_HEIGHT - 1, VGA_SCREEN_WIDTH  / 2 - 2);
            break;
        case SJF:
            sysSch.schedulerInit = schedulerInitSJF;
            sysSch.nextTsk = nextTskSJF;
            sysSch.enqueueTsk = enqueueTskSJF;
            sysSch.dequeueTsk = dequeueTskSJF;
            sysSch.schedule = scheduleSJF;
            sysSch.tick_hook = NULL;
            put_chars("SJF", 0x6, VGA_SCREEN_HEIGHT - 1, VGA_SCREEN_WIDTH  / 2 - 2);
            break;
    }
    sysSch.schedulerInit();
}