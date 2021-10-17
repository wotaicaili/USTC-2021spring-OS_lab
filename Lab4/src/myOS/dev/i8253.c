#include "io.h"
#include "interrupt.h"

// enable timer interrupt through 8259's port
void enableTimerInt(void) {
    outb(0x21, inb(0x21) & 0xFE);
}

// disable timer interrupt through 8259's port
void disableTimerInt(void) {
    outb(0x21, inb(0x21) & 0xFF);
}

void init8253(void) {
    disableTimerInt();
    unsigned short freq_div = FREQ_8253 / FREQ_TIME_INTERRUPT;

    // control byte
    outb(0x43, 0x34);

    // frequency division parameter
    outb(0x40, (unsigned char)freq_div);
    outb(0x40, (unsigned char)(freq_div >> 8));

    enableTimerInt();
}
