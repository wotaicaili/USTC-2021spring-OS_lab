#include "shell.h"
#include "memTestCase.h"
#include "userInterface.h"

void myTsk0(void) {
    myPrintf(0x1, "*********************************\n");
	myPrintf(0x1, "*           New Task 0          *\n");
	myPrintf(0x1, "*          Hello World!         *\n");
	myPrintf(0x1, "*********************************\n");
}

void myTsk1(void) {
    myPrintf(0x3, "*********************************\n");
	myPrintf(0x3, "*           New Task 1          *\n");
	myPrintf(0x3, "*          Hello World!         *\n");
	myPrintf(0x3, "*********************************\n");
}

void myTsk2(void) {
    myPrintf(0x5, "*********************************\n");
	myPrintf(0x5, "*           New Task 2          *\n");
	myPrintf(0x5, "*          Hello World!         *\n");
	myPrintf(0x5, "*********************************\n");
}

void shell(void) {
    myPrintk(0x2, "*********************************\n");
	myPrintk(0x2, "*           Shell Task          *\n");
	myPrintk(0x2, "*         Starting shell...     *\n");
	myPrintk(0x2, "*********************************\n");

    initShell();
    memTestCaseInit();
    startShell();
}
