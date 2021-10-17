#include "myPrintk.h"
#include "vga.h"
#include "uart.h"
#include "interrupt.h"
#include "wallClock.h"
#include "mem.h"

// user app
void myMain(void);

// wait the user to prepare uart device
void pressAnyKeyToStart(void) {
	myPrintk(0x5, "Prepare uart device\n");
	myPrintk(0x5, "Then, press any key to start...\n");

	uart_get_char();
}

void osStart(void) {

	// before start
	disable_interrupt();
	clear_screen();
	pressAnyKeyToStart();

	// start OS
	clear_screen();
	myPrintk(0x2, "start running......\n");

	// enalbe interrupt
	init8253();
	init8259A();
	enable_interrupt();
	setWallClock(0, 0, 0);

	// init mem
	pMemInit();

	// start user app
	myMain();

	// shut down
	myPrintk(0x2, "stop running......shutdown\n");
	while(1);
}
