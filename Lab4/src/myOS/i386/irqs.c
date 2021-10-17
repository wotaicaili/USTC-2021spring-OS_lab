#include "vga.h"
#include "tick.h"

void enable_interrupt(void) {
	__asm__ __volatile__("Call enableInterrupt");
}

void disable_interrupt(void) {
	__asm__ __volatile__("Call disableInterrupt");
}

void ignoreIntBody(void) {
	put_chars("unknown interrupt", 0x4, VGA_SCREEN_HEIGHT - 1, 0);
}

void timeIntBody(void) {
	tick();
}