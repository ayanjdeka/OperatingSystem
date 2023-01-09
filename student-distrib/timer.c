#include "timer.h"
#include "terminal.h"

// extern int32_t terminal_current[3];

/*
pit_init()
Description: initializes the PIT
Inputs: command
Outputs: none
Side effects: initializes the PIT and the correct terminals
 */
void pit_init() {
    /* Calculate frequency */
    int32_t divisor;
    divisor = RATE / RATE_DIVISOR;

    /* Enable PIT IRQs */

    /* Send command byte to command port and frequency to channel 0 */
    outb(INPUT, OUTPUT);
    outb(divisor & 0xFF, PIT_PORT);
    outb(divisor >> 8, 0x40);

    //enable_irq(0);

    strcpy((int8_t*) kbuffer.kbuf, (const int8_t*) terminals[0].buffer);
    load_terminal(0);
    set_current_terminal(0);
    terminals[0].active = 1; 
    clear();
    terminals[0].shell_flag = 0;

    execute((uint8_t*)"shell");

}
/*
pit_init()
Description: creates the pit handler to be called in the IDT entries
Inputs: command
Outputs: none
Side effects: creates the handler for the PIT and handles terminal logic
 */
void pit_int_handler() {
    int32_t next_terminal;
    cli();

    //sends EOI
    send_eoi(0);

    //get current pcb
    pcb_t* pcb= (pcb_t *)(KSTACK_START - PCB_SIZE * (terminals[get_active_terminal()].pid + 1));

    //gets the next terminal
    next_terminal = ((get_active_terminal()+ 1) % 3);
    if((terminals[0].active | terminals[1].active | terminals[2].active) == 0) {
        sti();
        return; 
    }
    while (terminals[next_terminal].active == 0) {
        next_terminal = ((next_terminal+ 1) % 3);
    }

    //get the next pcv
    pcb_t* next_pcb= (pcb_t *)(KSTACK_START - PCB_SIZE * (terminals[next_terminal].pid + 1));

    //set up paging
    page_directory[PROGRAM_PAGING].entry_4M.present = 1;
    page_directory[PROGRAM_PAGING].entry_4M.page_size = 1;
    page_directory[PROGRAM_PAGING].entry_4M.user_sup = 1;
    page_directory[PROGRAM_PAGING].entry_4M.read_write = 1;
    page_directory[PROGRAM_PAGING].entry_4M.address = ((KSTACK_START + (FOUR_MB * next_pcb->pid)))>>22;

    asm volatile(
                "mov %%cr3, %%eax;"
                "mov %%eax, %%cr3;"
                :                      /* no outputs */
                :                      /* no inputs */
                :"%eax"                /* clobbered register */
                );

    //set up video memory
    page_directory[0].entry_4k.present = 1;
    page_directory[0].entry_4k.read_write = 1;
    page_directory[0].entry_4k.user_sup = 0;
    page_directory[0].entry_4k.write_through = 0;
    page_directory[0].entry_4k.cache_dis = 0;
    page_directory[0].entry_4k.accessed = 0;
    page_directory[0].entry_4k.reserved0 = 0;
    page_directory[0].entry_4k.page_size = 0;
    page_directory[0].entry_4k.global = 0;
    page_directory[0].entry_4k.available = 0;
    page_directory[0].entry_4k.address = (int)page_table >> 12;

    page_table[(0xB8000 >> 12)].present = 1 ;
    page_table[(0xB8000 >> 12)].read_write = 1;
    page_table[(0xB8000 >> 12)].user_sup = 0;
    page_table[(0xB8000 >> 12)].write_through = 0;
    page_table[(0xB8000 >> 12)].cache_dis = 0;
    page_table[(0xB8000 >> 12)].accessed = 0;
    page_table[(0xB8000 >> 12)].dirty = 0;
    page_table[(0xB8000 >> 12)].page_attribute = 0;
    page_table[(0xB8000 >> 12)].global = 1;
    page_table[(0xB8000 >> 12)].available = 0;
    page_table[(0xB8000 >> 12)].address = (0xB8000 >> 12);

    tss.ss0 = KERNEL_DS;
    tss.esp0 = (KSTACK_START - (PCB_SIZE * (next_pcb->pid)) - 0x10);

    //get the next terminal and set it to the correct PID
    terminals[next_terminal].pid = next_pcb->pid;
    set_active_terminal(next_terminal);

    sti();

    /* Save currently running process's ebp and esp */
	asm volatile(
        "movl   %%ebp, %0   ;"
        "movl   %%esp, %1   ;"
        :"=r"(pcb->parent_ebp), "=r"(pcb->parent_esp)
    );

    /* Load in next process's ebp and esp to start running it */
    asm volatile(
        "movl   %0, %%esp   ;"
        "movl   %1, %%ebp   ;"
        "LEAVE;"
        "RET;"
        : :"r"(next_pcb->parent_esp), "r"(next_pcb->parent_ebp) 
    );

    return;

}
