#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

// open or close interrupt state
void enable_interrupt(void);
void disable_interrupt(void);

// interrupt controller 8259
void init8259A(void);

// time interrupt 8253
#define FREQ_8253 1193180
#define FREQ_TIME_INTERRUPT 100

void init8253(void);
void enableTimerInt(void);
void disableTimerInt(void);

#endif
