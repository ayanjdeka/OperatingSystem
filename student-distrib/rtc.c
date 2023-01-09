#include "rtc.h"
#include "types.h"
#include "terminal.h"
/* RTC Initializatoin
 
 
Initializes RTC, code found on https://wiki.osdev.org/RTC, and sets frequency up to 1024 hz
Inputs: NONE
Outputs: NONE
Side Effects: Intializes an RTC interrupt that happens 1024 times per second 
 */

volatile uint32_t counter[3] = {0,0,0};
static int frequencies[3] = {1024,1024,1024};
volatile int flag[3] = {0, 0, 0};
void rtc_init() {

    uint8_t rate; 
    // rate corresponds to 1024 hz
    rate = 0x06;
    char prev = inb(RTC_PORT + 1);

    enable_irq(IRQ_PORT);

    outb(0x8B, RTC_PORT); //select reg B, disable interrupts
    prev = inb(RTC_PORT + 1); //read current value of reg B
    outb(0x8B, RTC_PORT); //set the index again 
    outb((prev & TWO_HRTZ_FREQUENCY) | 0x40, RTC_PORT + 1); //write previous value or'd with 0x40

    outb(0x8A, RTC_PORT);		// set index to register A, disable NMI
    prev=inb(RTC_PORT + 1);	// get initial value of register A
    outb(0x8A, RTC_PORT);		// reset index to A
    outb((prev & BIT_MASK) | rate, RTC_PORT + 1); //write only our rate to A. Note, rate is the bottom 4 bits.

    // sti();

}
/* RTC Handler
 * 
Handles RTC interrupt, reads register C so that another interrupt can take place
Inputs: NONE
Outputs: NONE

Side Effects: Will continue RTC interrupting for a bit

 */
void rtc_handler(){
// reads register C and discards it
    uint32_t eflags;
    cli_and_save(eflags);

    outb(0x8C, RTC_PORT);
    inb(RTC_PORT + 1);
    flag[get_current_terminal()] = 1;
    send_eoi(8);
    // test_interrupts();
    //printf("RTC IS WORKING");
    // send_eoi(2);
    restore_flags(eflags);
    asm volatile ("                   \n\
        leave                       \n\
        iret"
    );
}

/* Open
 * 
This handles initializing the RTC frequency to 2 hz
Inputs: filename
Outputs: 0

Side Effects: Will intialize the frequency to the correct 2 hz

 */
int32_t rtc_open (const uint8_t* filename) {
    uint32_t eflags;
    cli_and_save(eflags);
    char prev;
    frequencies[get_current_terminal()] = 0x0F; 
    outb(0x8A, RTC_PORT); //select reg B, disable interrupts
    prev = inb(RTC_PORT + 1); //read current value of reg B
    outb(0x8A, RTC_PORT); //set the index again 
    outb((prev & BIT_MASK) | TWO_HRTZ_FREQUENCY, RTC_PORT + 1); //write previous value or'd with 0x40
    restore_flags(eflags);
    return 0;
}

/* Close
 * 
This handles closing the RTC
Inputs: file directory
Outputs: 0

Side Effects: Will disable the RTC port

 */
int32_t rtc_close (int32_t fd) {
    return 0;
}

/* Read
 * 
This handles reading the RTC, and making sure to check the interrupt flag
Inputs: file directory, buffer of the frequency, number of bytes
Outputs: 0

Side Effects: Will wait for the flag to be set to 0

 */
int32_t rtc_read (int32_t fd, void* buf, int32_t nbytes) {
    sti();
    
    while (!flag[get_current_terminal()]) {
        //do nothing
    }
    flag[get_current_terminal()] = 0;
    return 0;
}
/* Write
 * 
This handles writing the RTC with the new frequency, 
Inputs: file directory, buffer of the frequency, number of bytes
Outputs: 0

Side Effects: Will set the RTC with the new frequency

 */
int32_t rtc_write (int32_t fd, const void* buf, int32_t nbytes) {
    // uint32_t frequency;
    // frequency = *((uint32_t*)buf);

    //frequencies[get_current_terminal()] = frequency; 

    cli();
    char prev;
    uint32_t frequency;

    uint32_t firstFrequency;
    uint32_t secondFrequency;
    uint32_t thirdFrequency;
    uint32_t fourthFrequency;
    uint32_t fifthrequency;
    uint32_t sixthFrequency;
    uint32_t seventhFrequency;
    uint32_t eigthFrequency;
    uint32_t ninthFrequency;
    uint32_t tenthFrequency;

    firstFrequency = 2;
    secondFrequency = 4;
    thirdFrequency = 8;
    fourthFrequency = 16;
    fifthrequency = 32;
    sixthFrequency = 64;
    seventhFrequency = 128;
    eigthFrequency = 256;
    ninthFrequency = 512;
    tenthFrequency = 1024;

    frequency = *((uint32_t*)buf);

    //check the current state of the frequency, and get the new | parameter of the frequency
    if (frequency == firstFrequency) {
        frequency = TWO_HRTZ_FREQUENCY;
    } else if (frequency == secondFrequency) {
        frequency = FOUR_HRTZ_FREQUENCY;
    } else if (frequency == thirdFrequency) {
        frequency = EIGHT_HRTZ_FREQUENCY;
    } else if (frequency == fourthFrequency) {
        frequency = SIXTEEN_HRTZ_FREQUENCY;
    } else if (frequency == fifthrequency) {
        frequency = THIRTYTWO_HRTZ_FREQUENCY;
    } else if (frequency == sixthFrequency) {
        frequency = SIXTYFOUR_HRTZ_FREQUENCY;
    } else if (frequency == seventhFrequency) {
        frequency = ONEHUNDRED28_HRTZ_FREQUENCY;
    } else if (frequency == eigthFrequency) {
        frequency = TWOHUNDRED56_HRTZ_FREQUENCY;
    } else if (frequency == ninthFrequency) {
        frequency = FIVEHUNDRED12_HRTZ_FREQUENCY;
    } else if (frequency >= tenthFrequency) {
        frequency = TENTWENTYFOUR_HRTZ_FREQUENCY;
    }


    outb(0x8A, RTC_PORT); //select reg B, disable interrupts
    prev = inb(RTC_PORT + 1); //read current value of reg B
    outb(0x8A, RTC_PORT); //set the index again 
    outb((prev & BIT_MASK) | frequency, RTC_PORT + 1); //write previous value and'd with 0xF0

    frequencies[get_current_terminal()] = frequency;

    sti();
    
    return nbytes;
}

