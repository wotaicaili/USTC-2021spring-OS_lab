#include "myPrintk.h"
#include "vga.h"
#include "uart.h"
#include "interrupt.h"
#include "wallClock.h"
#include "mem.h"
#include "task.h"

// wait the user to prepare uart device
void pressAnyKeyToStart(void) {
	myPrintk(0x5, "Prepare uart device\n");
	myPrintk(0x5, "Then, press any key to start...\n");

	uart_get_char();
}

void osStart(void) {
	// before
	disable_interrupt();
	clear_screen();
	pressAnyKeyToStart();

	// init
	clear_screen();
	myPrintk(0x2, "Initializing the OS...\n");
	init8253();
	init8259A();
	enable_interrupt();
	pMemInit();
	setWallClock(0, 0, 0);

	// start
	myPrintk(0x2, "Starting the OS...\n");
	TaskManagerInit();

	/*
	myPrintk(0x2, "Stop running... shutdown\n");
	while(1); 
	*/
}
