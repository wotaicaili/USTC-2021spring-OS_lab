#include "tick.h"

unsigned int timer_seconds;

void update_timer(void) {
    if (get_tick_times() % TICK_FREQ != 0)
        return;

    timer_seconds++;
}

void init_timer(void) {
    timer_seconds = 0;
    append2HookList(update_timer);
}

unsigned int get_current_time(void) {
    return timer_seconds;
}