#ifndef __SEHLL_H__
#define __SEHLL_H__

void initShell(void);
void startShell(void);
void addNewCmd(unsigned char *cmd, 
		int (*func)(int argc, unsigned char **argv), 
		void (*help_func)(void), 
		unsigned char* desc);

#endif
