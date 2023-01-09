/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

#define MASK_REGISTER_WIDTH 8
#define MASK_EOI_NUMBER 2


/* Initialize the 8259 PIC */
void i8259_init(void) {

    //make sure to clear both interrupt masks
    master_mask = 0xFF;
    slave_mask = 0xFF;

    //make sure to start the initialization with the first cascade
    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);

    //assign the vector to the proper data ports
    outb(ICW2_MASTER, MASTER_DATA_PORT);
    outb(ICW2_SLAVE, SLAVE_DATA_PORT);

    //assign the proper wiring to the proper data ports
    outb(ICW3_MASTER, MASTER_DATA_PORT);
    outb(ICW3_SLAVE, SLAVE_DATA_PORT);

    //assign the additional info the proper wiring to the proper data ports
    outb(ICW4, MASTER_DATA_PORT);
    outb(ICW4, SLAVE_DATA_PORT);

    outb(master_mask, MASTER_DATA_PORT);
    outb(slave_mask, SLAVE_DATA_PORT);
    
    enable_irq(2);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    uint16_t port;
    uint8_t value;

    // if the number is less than 8 bits, then set the port to the master
    if (irq_num < MASK_REGISTER_WIDTH) {
        port = MASTER_DATA_PORT;
    } else {
        //otherwise, make sure to decrement the number passed in and set the port to the slave
        port = SLAVE_DATA_PORT;
        irq_num -= MASK_REGISTER_WIDTH;
    }
    //set this value to set the mask with an and statement
    value = inb(port) & ~(1 << irq_num);
    outb(value, port);
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    uint16_t port;
    uint8_t value;

    // if the number is less than 8 bits, then set the port to the master
    if (irq_num < MASK_REGISTER_WIDTH) {
        port = MASTER_DATA_PORT;
    } else {
        //otherwise, make sure to decrement the number passed in and set the port to the slave
        port = SLAVE_DATA_PORT;
        irq_num -= MASK_REGISTER_WIDTH;
    }
    //set this value to clear the mask with an or statement
    value = inb(port) | ~(1 << irq_num);
    outb(value, port);
}


/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {

    if (irq_num >= MASK_REGISTER_WIDTH) {
        //if the number is greater than 8, decrement it by the width
        irq_num -= MASK_REGISTER_WIDTH;

        //make sure to or both the number and EOI address to the slave port 
        outb(EOI | irq_num, SLAVE_8259_PORT);
        //make sure to or both the number 2 and EOI address to the master port
        outb(EOI | MASK_EOI_NUMBER, MASTER_8259_PORT);
    }
    else{
        outb(EOI | irq_num, MASTER_8259_PORT);
    }
    
    //send this signal to the master port
    

}
