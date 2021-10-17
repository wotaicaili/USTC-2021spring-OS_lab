#include "userInterface.h"

void myTskFCFS0(void) {
    task_execute(4);
}

void myTskFCFS1(void) {
    task_execute(5);
}

void myTskFCFS2(void) {
    task_execute(3);
}

void myTskFCFS4(void) {
    task_execute(2);
}

void initFCFSCases(void) {
    // correct execute order:
    // init -> 4 -> 2 -> 1 -> idle -> 3 -> idle (the numbers are tid)

    int newTskTid0 = createTsk(myTskFCFS0); // its tid will be 1
    setTskPara(ARRV_TIME, 3, tcbPool[newTskTid0]->para);
    setTskPara(EXEC_TIME, 4, tcbPool[newTskTid0]->para); 

    int newTskTid1 = createTsk(myTskFCFS1); // its tid will be 2
    setTskPara(ARRV_TIME, 1, tcbPool[newTskTid1]->para);
    setTskPara(EXEC_TIME, 5, tcbPool[newTskTid1]->para); 

    int newTskTid2 = createTsk(myTskFCFS2); // its tid will be 3
    setTskPara(ARRV_TIME, 13, tcbPool[newTskTid2]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid2]->para); 

    int newTskTid4 = createTsk(myTskFCFS4); // its tid will be 4
    setTskPara(ARRV_TIME, 0, tcbPool[newTskTid4]->para);
    setTskPara(EXEC_TIME, 2, tcbPool[newTskTid4]->para); 
    
    enableTask(newTskTid0);
    enableTask(newTskTid4);
    enableTask(newTskTid1);
    enableTask(newTskTid2);
}
