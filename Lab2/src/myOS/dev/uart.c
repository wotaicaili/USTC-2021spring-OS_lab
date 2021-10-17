extern unsigned char inb(unsigned short int port_from);
extern void outb(unsigned short int port_to, unsigned char value);

#define UART_BASE 0x3F8

// put a char to uart
void uart_put_char(unsigned char c) {
    outb(UART_BASE, c);
}

// read a char from uart
unsigned char uart_get_char(void) {
    unsigned char c;
    while(!(c = inb(UART_BASE)));
    return c;
}

// put a string to uart
void uart_put_chars(char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        outb(UART_BASE, str[i]);
    } 
}
