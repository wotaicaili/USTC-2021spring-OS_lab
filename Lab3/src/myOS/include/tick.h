#ifndef __TICK_H__
#define __TICK_H___

#include "interrupt.h"

#define TICK_FREQ FREQ_TIME_INTERRUPT

extern int kernel_tick_times;
void tick(void);
void setWallClockHook(void (*func)(void));

#endif
