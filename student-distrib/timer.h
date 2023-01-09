#ifndef _TIMER_H
#define _TIMER_H

#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "systemcalls.h"
#include "page.h"

#define RATE 1193180
#define RATE_DIVISOR 50

#define INPUT 0x36
#define OUTPUT 0x43
#define PIT_PORT 0x40

/* PIT initialization and interrupt handler routines */
void pit_init();
void pit_int_handler();


#endif  /*_TIMER_H */
