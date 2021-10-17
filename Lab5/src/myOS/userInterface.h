#ifndef __USERINTERFACE_H__
#define __USERINTERFACE_H__

// interfaces to user
#include "vga.h"
#include "uart.h"
#include "string.h"
#include "myPrintk.h"
#include "malloc.h"
#include "types.h"
#include "task.h"

// generally, these ones should not be exposed to the user, but we need to test them in the shell
#include "kmalloc.h"
#include "mem.h"

#endif