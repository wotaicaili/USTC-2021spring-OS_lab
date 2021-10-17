#include "shell.h"
#include "memTestCase.h"
#include "userInterface.h"

void initFCFSCases(void);
void initRRCases(void);
void initPrioCases(void);
void initSJFCases(void);

void shell(void) {
    myPrintk(0x2, "*********************************\n");
	myPrintk(0x2, "*           Shell Task          *\n");
	myPrintk(0x2, "*         Starting shell...     *\n");
	myPrintk(0x2, "*********************************\n");

    initShell();
    memTestCaseInit();
    startShell();
}

void myMain(void) {
    myPrintf(0x8, "*********************************\n");
	myPrintf(0x8, "*      Init Task: myMain()      *\n");
	myPrintf(0x8, "*     Initing user tasks...     *\n");
	myPrintf(0x8, "*********************************\n");
    switch (sysSch.type) {
        case FCFS: { // FCFS test cases
            if (startShellFlag)
                shell();
            else
                initFCFSCases();
            break;
        }
        case RR: { // RR test cases
            initRRCases();
            break;
        }
        case PRIO: { // prio test cases
            initPrioCases();
            break;
        }
        case SJF: { // SJF test cases
            initSJFCases();
            break;
        }
    }
}
