#ifndef __TASK_PARA_H__
#define __TASK_PARA_H__

#define MAX_PRIORITY 5
#define MAX_EXEC_TIME 20

#define PRIORITY 0
#define EXEC_TIME 1 
#define ARRV_TIME 2

typedef struct tskPara {
    int priority;
    int exec_time;
    int arrv_time;
} tskPara;

void initTskPara(tskPara **para);
void setTskPara(unsigned int option, unsigned int value, tskPara *para);
unsigned int getTskPara(unsigned option, tskPara *para);

void showCurrentTskParaInfo_hook(void);

void task_execute(unsigned int wait_time);

#endif