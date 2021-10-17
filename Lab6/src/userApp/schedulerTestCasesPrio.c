#include "userInterface.h"

void myTskPrio0(void) {
    task_execute(4);
}

void myTskPrio1(void) {
    task_execute(3);
}

void myTskPrio2(void) {
    task_execute(3);
}

void myTskPrio3(void) {
    task_execute(3);
}

void myTskPrio4(void) {
    task_execute(3);
}

void myTskPrio5(void) {
    task_execute(3);
}


void initPrioCases(void) {
    // correct execute order:
    // init -> 1 -> 3 -> 2 -> 4 -> 5 -> 6 -> idle (the numbers are tid)

    int newTskTid0 = createTsk(myTskPrio0); // its tid will be 1
    setTskPara(PRIORITY, 3, tcbPool[newTskTid0]->para);
    setTskPara(ARRV_TIME, 0, tcbPool[newTskTid0]->para);
    setTskPara(EXEC_TIME, 4, tcbPool[newTskTid0]->para); 

    int newTskTid1 = createTsk(myTskPrio1); // its tid will be 2
    setTskPara(PRIORITY, 1, tcbPool[newTskTid1]->para);
    setTskPara(ARRV_TIME, 1, tcbPool[newTskTid1]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid1]->para); 

    int newTskTid2 = createTsk(myTskPrio2); // its tid will be 3
    setTskPara(PRIORITY, 0, tcbPool[newTskTid2]->para);
    setTskPara(ARRV_TIME, 1, tcbPool[newTskTid2]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid2]->para); 

    int newTskTid3 = createTsk(myTskPrio3); // its tid will be 4
    setTskPara(PRIORITY, 4, tcbPool[newTskTid3]->para);
    setTskPara(ARRV_TIME, 1, tcbPool[newTskTid3]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid3]->para);

    int newTskTid4 = createTsk(myTskPrio4); // its tid will be 5
    setTskPara(PRIORITY, 4, tcbPool[newTskTid4]->para);
    setTskPara(ARRV_TIME, 4, tcbPool[newTskTid4]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid4]->para);

    int newTskTid5 = createTsk(myTskPrio5); // its tid will be 6
    setTskPara(PRIORITY, 4, tcbPool[newTskTid5]->para);
    setTskPara(ARRV_TIME, 6, tcbPool[newTskTid5]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid5]->para);

    enableTask(newTskTid5);
    enableTask(newTskTid1);
    enableTask(newTskTid2);
    enableTask(newTskTid4);
    enableTask(newTskTid3);
    enableTask(newTskTid0);
}
