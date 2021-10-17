#include "userInterface.h"

void myTsk0(void);
void myTsk1(void);
void myTsk2(void);
void shell(void);

void myMain(void) {
    int shellTid = createTsk(shell);
    int newTskTid0 = createTsk(myTsk0);
    int newTskTid1 = createTsk(myTsk1);
    int newTskTid2 = createTsk(myTsk2);
    tskStart(newTskTid0);
    tskStart(newTskTid1);
    tskStart(newTskTid2); 
    tskStart(shellTid);
}
