// 不用修改
extern int myPrintk(int color,const char *format, ...);
extern void clear_screen(void);
extern void myMain(void);

void osStart(void){
	clear_screen();
	myPrintk(0x2,"START RUNNING......\n");
	myMain();
	myPrintk(0x2, "STOP RUNNING......ShutDown\n");
	while(1);
}
