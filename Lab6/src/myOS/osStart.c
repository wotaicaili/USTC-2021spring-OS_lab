#include "myPrintk.h"
#include "vga.h"
#include "uart.h"
#include "interrupt.h"
#include "wallClock.h"
#include "mem.h"
#include "task.h"
#include "scheduler.h"
#include "timer.h"
#include "schedulerRR.h"

int startShellFlag = 0;

// wait the user to prepare uart device
void pressAnyKeyToStart(void) {
	myPrintk(0x7, "\nPlease prepare your uart device\n");
	myPrintk(0x7, "\nThen, enter a number to choose the mode you want to enter\n");
	myPrintk(0x7, "\nNote: In scheduling algorithm testing mode, we will not start the shell\n\n");
	myPrintk(0x6, "[0] - FCFS scheduling test\n");
	myPrintk(0x5, "[1] - PRIO scheduling test - no preemption\n");
	myPrintk(0x1, "[2] - SJF  scheduling test - no preemption\n");
	myPrintk(0x3, "[3] - RR   scheduling test - time slice %d seconds\n", TIME_SLICE);
	myPrintk(0x2, "[4] - Shell\n", 3);

	unsigned char input;

	while (1) {
		input = uart_get_char();
		if (input <= '4' && input >= '0')
			break;
	}
	switch (input) {
		case '0':
			sysSch.type = FCFS;
			break;
		case '1':
			sysSch.type = PRIO;
			break;
		case '2':
			sysSch.type = SJF;
			break;
		case '3':
			sysSch.type = RR;
			break;
		case '4':
			sysSch.type = FCFS;
			startShellFlag = 1;
			break;
	}
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
	pMemInit();
	init_timer();
	setWallClock(0, 0, 0);	

	myPrintk(0x2, "Starting the OS...\n");
	
	TaskManagerInit();
}
