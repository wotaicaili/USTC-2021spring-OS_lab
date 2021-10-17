#include "myPrintk.h"
#include "vga.h"
#include "interrupt.h"
#include "wallClock.h"

void myMain(void);

void osStart(void) {
	init8253();
	init8259A();
	enable_interrupt();

	clear_screen();
	setWallClock(0, 0, 0);

	myPrintk(0x2, "START RUNNING......\n");
	myMain();
	myPrintk(0x2, "STOP RUNNING......ShutDown\n");
	
	while(1);
}
