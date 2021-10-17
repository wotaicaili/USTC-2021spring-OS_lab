#ifndef __I386_IO_H__
#define __I386_IO_H__

unsigned char inb(unsigned short int port_from);
void outb(unsigned short int port_to, unsigned char value);

#endif
