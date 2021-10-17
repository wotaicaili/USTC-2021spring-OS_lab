#include <stdarg.h>

extern void append2screen(char *str, int color);
extern void uart_put_chars(char *str);
extern int vsprintf(char *buf, const char *fmt, va_list args);

char kBuf[400];
int myPrintk(int color, const char *format, ...) {
    int i;
    va_list args;
    
    va_start(args, format);
    i = vsprintf(kBuf, format, args);
    va_end(args);
    append2screen(kBuf, color);
    uart_put_chars(kBuf);
    
    return i;
}

char uBuf[400];
int myPrintf(int color, const char *format, ...) {
    int i;
    va_list args;
    
    va_start(args, format);
    i = vsprintf(uBuf, format, args);
    va_end(args);
    append2screen(uBuf, color);
    uart_put_chars(uBuf);
    
    return i;
}
