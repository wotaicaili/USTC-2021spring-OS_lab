#ifndef __TICK_H__
#define __TICK_H___

#include "interrupt.h"

#define TICK_FREQ FREQ_TIME_INTERRUPT

void tick(void);

void append2HookList(void (*func)(void));
unsigned int get_tick_times();

#endif
