#include "lib.h"
#include "i8259.h"

#define RTC_PORT 0x70
#define IRQ_PORT 0x08
#define TWO_HRTZ_FREQUENCY 0x0F
#define FOUR_HRTZ_FREQUENCY 0x0E
#define EIGHT_HRTZ_FREQUENCY 0x0D
#define SIXTEEN_HRTZ_FREQUENCY 0x0C
#define THIRTYTWO_HRTZ_FREQUENCY 0x0B
#define SIXTYFOUR_HRTZ_FREQUENCY 0x0A
#define ONEHUNDRED28_HRTZ_FREQUENCY 0x09
#define TWOHUNDRED56_HRTZ_FREQUENCY 0x08
#define FIVEHUNDRED12_HRTZ_FREQUENCY 0x07
#define TENTWENTYFOUR_HRTZ_FREQUENCY 0x06
#define BIT_MASK 0xF0
void rtc_init();
void rtc_handler();

int32_t rtc_read (int32_t fd, void* buf, int32_t nbytes);
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes);
int32_t rtc_open (const uint8_t* filename);
int32_t rtc_close (int32_t fd);
// extern void rtc_interrupt_asm();

