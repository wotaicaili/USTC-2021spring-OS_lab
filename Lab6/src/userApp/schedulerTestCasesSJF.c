#include "userInterface.h"

void myTskSJF0(void) {
    task_execute(2);
}

void myTskSJF1(void) {
    task_execute(5);
}

void myTskSJF2(void) {
    task_execute(4);
}

void myTskSJF3(void) {
    task_execute(3);
}

void myTskSJF4(void) {
    task_execute(3);
}

void myTskSJF5(void) {
    task_execute(3);
}

void myTskSJF6(void) {
    task_execute(10);
}


void initSJFCases(void) {
    // correct execute order:
    // init -> 1 -> 3 -> 2 -> 4 -> 6 -> 5 -> 7 -> idle (the numbers are tid)

    int newTskTid0 = createTsk(myTskSJF0); // its tid will be 1
    setTskPara(ARRV_TIME, 0, tcbPool[newTskTid0]->para);
    setTskPara(EXEC_TIME, 2, tcbPool[newTskTid0]->para); 

    int newTskTid1 = createTsk(myTskSJF1); // its tid will be 2
    setTskPara(ARRV_TIME, 0, tcbPool[newTskTid1]->para);
    setTskPara(EXEC_TIME, 5, tcbPool[newTskTid1]->para); 

    int newTskTid2 = createTsk(myTskSJF2); // its tid will be 3
    setTskPara(ARRV_TIME, 0, tcbPool[newTskTid2]->para);
    setTskPara(EXEC_TIME, 4, tcbPool[newTskTid2]->para); 

    int newTskTid3 = createTsk(myTskSJF3); // its tid will be 4
    setTskPara(ARRV_TIME, 10, tcbPool[newTskTid3]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid3]->para);

    int newTskTid4 = createTsk(myTskSJF4); // its tid will be 5
    setTskPara(ARRV_TIME, 12, tcbPool[newTskTid4]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid4]->para);

    int newTskTid5 = createTsk(myTskSJF5); // its tid will be 6
    setTskPara(ARRV_TIME, 11, tcbPool[newTskTid5]->para);
    setTskPara(EXEC_TIME, 3, tcbPool[newTskTid5]->para);

    int newTskTid6 = createTsk(myTskSJF6); // its tid will be 7
    setTskPara(ARRV_TIME, 0, tcbPool[newTskTid6]->para);
    setTskPara(EXEC_TIME, 10, tcbPool[newTskTid6]->para); 

    enableTask(newTskTid5);
    enableTask(newTskTid1);
    enableTask(newTskTid2);
    enableTask(newTskTid4);
    enableTask(newTskTid3);
    enableTask(newTskTid0);
    enableTask(newTskTid6);
}
