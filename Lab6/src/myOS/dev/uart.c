#include "io.h"

#define UART_BASE 0x3F8

// put a char to uart
void uart_put_char(unsigned char c) {
    outb(UART_BASE, c);
}

// read a char from uart
unsigned char uart_get_char(void) {
    while(!(inb(UART_BASE + 5) & 1)); // loop until the input is available
    return inb(UART_BASE);
}

// put a string to uart
void uart_put_chars(char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        outb(UART_BASE, str[i]);
    } 
}
