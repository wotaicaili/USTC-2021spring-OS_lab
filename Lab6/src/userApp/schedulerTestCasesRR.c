#include "userInterface.h"

void myTskRR0(void) {
    task_execute(14);
}

void myTskRR1(void) {
    task_execute(4);
}

void myTskRR2(void) {
    task_execute(4);
}

void myTskRR3(void) {
    task_execute(3);
}

void myTskRR4(void) {
    task_execute(4);
}

void myTskRR5(void) {
    task_execute(4);
}

void initRRCases(void) {

    int newTskTid0 = createTsk(myTskRR0); // tid 1
    setTskPara(ARRV_TIME, 0, tcbPool[newTskTid0]->para);
    setTskPara(EXEC_TIME, 14, tcbPool[newTskTid0]->para); 

    int newTskTid1 = createTsk(myTskRR1); // tid 2
    setTskPara(ARRV_TIME, 1, tcbPool[newTskTid1]->para);
    setTskPara(EXEC_TIME, 4, tcbPool[newTskTid1]->para); 

    int newTskTid2 = createTsk(myTskRR2); // tid 3
    setTskPara(ARRV_TIME, 2, tcbPool[newTskTid2]->para);
    setTskPara(EXEC_TIME, 4, tcbPool[newTskTid2]->para); 

    int newTskTid3 = createTsk(myTskRR3); // tid 4
    setTskPara(ARRV_TIME, 15, tcbPool[newTskTid3]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid3]->para); 

    int newTskTid4 = createTsk(myTskRR4); // tid 5
    setTskPara(ARRV_TIME, 15, tcbPool[newTskTid4]->para);
    setTskPara(EXEC_TIME, 4, tcbPool[newTskTid4]->para); 

    int newTskTid5 = createTsk(myTskRR5); // tid 6
    setTskPara(ARRV_TIME, 26, tcbPool[newTskTid5]->para);
    setTskPara(EXEC_TIME, 4, tcbPool[newTskTid5]->para); 
    
    enableTask(newTskTid1);
    enableTask(newTskTid0);
    enableTask(newTskTid5);
    enableTask(newTskTid4);
    enableTask(newTskTid3);
    enableTask(newTskTid2);
}
