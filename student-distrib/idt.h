#ifndef _IDT_H
#define _IDT_H

#include "lib.h"
#include "keyboard.h"
#include "rtc.h"
#include "timer.h"
//#include "syscall.h"
//#include "timer.h"
#define SYSCALL_INTERRUPT_NUM 0x80
#define KEYBOARD_INTERRUPT_NUM 0x21
#define RTC_INTERRUPT_NUM 0x28
#define PIT_INT 0x20

extern void idt_init();

#endif /* _IDT_H */

