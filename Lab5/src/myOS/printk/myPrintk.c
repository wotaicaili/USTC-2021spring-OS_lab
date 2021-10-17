#include <stdarg.h>
#include "vga.h"
#include "uart.h"
#include "vsprintf.h"

int vsprintf(char *buf, const char *fmt, va_list args);

char kBuf[400];
int myPrintk(int color, const char *format, ...) {
    va_list args;
    
    va_start(args, format);
    int cnt = vsprintf(kBuf, format, args);
    va_end(args);
    append2screen(kBuf, color);
    uart_put_chars(kBuf);
    
    return cnt;
}

char uBuf[400];
int myPrintf(int color, const char *format, ...) {
    va_list args;
    
    va_start(args, format);
    int cnt = vsprintf(uBuf, format, args);
    va_end(args);
    append2screen(uBuf, color);
    uart_put_chars(uBuf);
    
    return cnt;
}