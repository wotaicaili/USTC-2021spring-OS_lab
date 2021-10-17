#define MAX_HOOK_FUNC_NUM 32

int kernel_tick_times = 0;

int hook_func_num = 0;
void (*hook_list[MAX_HOOK_FUNC_NUM])(void); // hook function list

void setWallClockHook(void (*func)(void)) {
    hook_list[hook_func_num++] = func;
}

// when a time interrupt takes place, this function is called
void tick(void) {

    kernel_tick_times++;

    // call all the hook functions
    for (int i = 0; i < hook_func_num; i++)
    	hook_list[i]();
}
