#include "lib.h"
#include "idt.h"
#include "x86_desc.h"
//#include "interrupt_service_wrapper.h"

extern void sys_call();


/*
 *void exception_0()
 *     DESCRIPTION: divide error handler 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle exception divide error
 */
void exception_0(){
	clear();
	
	printf(" Divide Error Exception ");
	while(1);
}

/*
 *void exception_1()
 *     DESCRIPTION: Reserved for intel use only
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: reserved interrupt
 */
void exception_1(){
	clear();
	
	printf(" RESERVED ");
	while(1);
}

/*
 *void exception_2()
 *     DESCRIPTION: nonmaskable external interrupt 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle NMI
 */
void exception_2(){
	clear();
	
	printf(" NMI Interrupt ");
	while(1);
}

/*
 *void exception_3()
 *     DESCRIPTION: Breakpoint 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle INT 3 instruction
 */
void exception_3(){
	clear();
	
	printf(" Breakpoint ");
	while(1);
}

/*
 *void exception_4()
 *     DESCRIPTION: Overflow 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle INTO intruction
 */
void exception_4(){
	clear();
	
	printf(" Overflow ");
	while(1);
}

/*
 *void exception_5()
 *     DESCRIPTION: BOUND range exceeded 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle BOUND instruction
 */
void exception_5(){
	clear();
	
	printf(" BOUND range exceeded ");
	while(1);
}

/*
 *void exception_6()
 *     DESCRIPTION: Invalid Opcode 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle UD2 instruction or reserved opcode
 */
void exception_6(){
	clear();
	
	printf(" Invalid Opcode ");
	while(1);
}

/*
 *void exception_7()
 *     DESCRIPTION: Device not availible 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle Floating-Point or WAIT/FWAIT instruction
 */
void exception_7(){
	clear();
	
	printf(" Device not availible ");
	while(1);
}

/*
 *void exception_8()
 *     DESCRIPTION: Double Fault 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle any instruction that can generate an exception, an NMI, or an INTR
 */
void exception_8(){
	clear();
	
	printf(" Double Fault ");
	while(1);
}

/*
 *void exception_9()
 *     DESCRIPTION: Coprocessor Segment Overrun
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle Floating-point instruction
 */
void exception_9(){
	clear();
	
	printf(" Coprocessor Segment Overrun ");
	while(1);
}

/*
 *void exception_10()
 *     DESCRIPTION: Invalid TSS 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle Task switch or TSS access
 */
void exception_10(){
	clear();
	
	printf(" Invalid TSS ");
	while(1);
}

/*
 *void exception_11()
 *     DESCRIPTION: Segment not found
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle loading segment registers or accessing system segments
 */
void exception_11(){
	clear();
	
	printf(" Segment not found ");
	while(1);
}

/*
 *void exception_12()
 *     DESCRIPTION: Stack segment fault
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle Stack operations and SS register loads
 */
void exception_12(){
	clear();
	
	printf(" Stack segment fault ");
	while(1);
}

/*
 *void exception_13()
 *     DESCRIPTION: Geenral Protection 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle Any memory checks and other protection checks
 */
void exception_13(){
	clear();
	
	printf(" General Protection ");
	while(1);
}

/*
 *void exception_14()
 *     DESCRIPTION: Page Fault 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle Any memory referance
 */
void exception_14(){
	clear();
	
	printf(" Page Fault ");
	// halt(15);
	while(1);
}

/*
 *void exception_15()
 *     DESCRIPTION: Intel reserved (Do not use) 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: intel reserved
 */
void exception_15(){
	clear();
	
	printf(" Intel Reserved ");
	while(1);
}

/*
 *void exception_16()
 *     DESCRIPTION: x86 FPU Floating-point error
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle x86 FPU floating-point or WAIT/FWAIT instruction
 */
void exception_16(){
	clear();
	
	printf(" x86 Floating-point error ");
	while(1);
}

/*
 *void exception_17()
 *     DESCRIPTION: Alignment check 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle Any data reference in memory
 */
void exception_17(){
	clear();
	
	printf(" Alignment check ");
	while(1);
}

/*
 *void exception_18()
 *     DESCRIPTION: Machine Check 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle Error codes and source are model dependent
 */
void exception_18(){
	clear();
	
	printf(" Machine check ");
	while(1);
}

/*
 *void exception_19()
 *     DESCRIPTION: SIMD Floating-point Exception 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle SSE/SSE2/SSE3 floating-point instructions
 */
void exception_19(){
	clear();
	
	printf(" SIMD floating-point exception ");
	while(1);
}

/*
 *void exception_19()
 *     DESCRIPTION: SIMD Floating-point Exception 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: handle SSE/SSE2/SSE3 floating-point instructions
 */
// void sys_call(){
// 	clear();
	
// 	printf(" System Call Made ");
// 	while(1);
// }

/*
 *void idt_init()
 *     DESCRIPTION: initialize idt 
 *     INPUTS: none
 *     OUTPUTS: none
 *     RETURN VALUE: none
 *     SIDE EFFECTS: makes sure to initialize the IDT table
 */

void idt_init(){
    int i = 0;
    for(i = 0; i < 20; i++){
        idt[i].present = 1;
        idt[i].dpl = 0x00;
        idt[i].reserved0 = 0;
        idt[i].size = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 0;
        idt[i].reserved4 = 0;
        idt[i].seg_selector = KERNEL_CS;
		if(i == 15){
			idt[i].present = 0;
		}
    }

	// Populating IDT with exceptions
	SET_IDT_ENTRY(idt[0], exception_0);
	SET_IDT_ENTRY(idt[1], exception_1);
	SET_IDT_ENTRY(idt[2], exception_2);
	SET_IDT_ENTRY(idt[3], exception_3);
	SET_IDT_ENTRY(idt[4], exception_4);
	SET_IDT_ENTRY(idt[5], exception_5);
	SET_IDT_ENTRY(idt[6], exception_6);
	SET_IDT_ENTRY(idt[7], exception_7);
	SET_IDT_ENTRY(idt[8], exception_8);
	SET_IDT_ENTRY(idt[9], exception_9);
	SET_IDT_ENTRY(idt[10], exception_10);
	SET_IDT_ENTRY(idt[11], exception_11);
	SET_IDT_ENTRY(idt[12], exception_12);
	SET_IDT_ENTRY(idt[13], exception_13);
	SET_IDT_ENTRY(idt[14], exception_14);
	SET_IDT_ENTRY(idt[15], exception_15);
	SET_IDT_ENTRY(idt[16], exception_16);
	SET_IDT_ENTRY(idt[17], exception_17);
	SET_IDT_ENTRY(idt[18], exception_18);
	SET_IDT_ENTRY(idt[19], exception_19);

	// System call
	idt[SYSCALL_INTERRUPT_NUM].present = 1;
	idt[SYSCALL_INTERRUPT_NUM].dpl = 3;
	idt[SYSCALL_INTERRUPT_NUM].reserved0 = 0;
	idt[SYSCALL_INTERRUPT_NUM].size = 1;
	idt[SYSCALL_INTERRUPT_NUM].reserved1 = 1;
	idt[SYSCALL_INTERRUPT_NUM].reserved2 = 1;
	idt[SYSCALL_INTERRUPT_NUM].reserved3 = 0;
	idt[SYSCALL_INTERRUPT_NUM].reserved4 = 0;
	idt[SYSCALL_INTERRUPT_NUM].seg_selector = KERNEL_CS;
	SET_IDT_ENTRY(idt[SYSCALL_INTERRUPT_NUM], sys_call);	
	// keyboard - IRQ1
	idt[KEYBOARD_INTERRUPT_NUM].present = 1;
	idt[KEYBOARD_INTERRUPT_NUM].dpl = 0;
	idt[KEYBOARD_INTERRUPT_NUM].reserved0 = 0;
	idt[KEYBOARD_INTERRUPT_NUM].size = 1;
	idt[KEYBOARD_INTERRUPT_NUM].reserved1 = 1;
	idt[KEYBOARD_INTERRUPT_NUM].reserved2 = 1;
	idt[KEYBOARD_INTERRUPT_NUM].reserved3 = 0;
	idt[KEYBOARD_INTERRUPT_NUM].reserved4 = 0;
	idt[KEYBOARD_INTERRUPT_NUM].seg_selector = KERNEL_CS;
	SET_IDT_ENTRY(idt[KEYBOARD_INTERRUPT_NUM], keyboard_handler);

// RTC handler - IRQ8
	idt[RTC_INTERRUPT_NUM].present = 1;
	idt[RTC_INTERRUPT_NUM].dpl = 0;
	idt[RTC_INTERRUPT_NUM].reserved0 = 0;
	idt[RTC_INTERRUPT_NUM].size = 1;
	idt[RTC_INTERRUPT_NUM].reserved1 = 1;
	idt[RTC_INTERRUPT_NUM].reserved2 = 1;
	idt[RTC_INTERRUPT_NUM].reserved3 = 0;
	idt[RTC_INTERRUPT_NUM].reserved4 = 0;
	idt[RTC_INTERRUPT_NUM].seg_selector = KERNEL_CS;
	SET_IDT_ENTRY(idt[RTC_INTERRUPT_NUM], rtc_handler);

//PIT
	idt[PIT_INT].present = 1;
	idt[PIT_INT].dpl = 0;
	idt[PIT_INT].reserved0 = 0;
	idt[PIT_INT].size = 1;
	idt[PIT_INT].reserved1 = 1;
	idt[PIT_INT].reserved2 = 1;
	idt[PIT_INT].reserved3 = 0;
	idt[PIT_INT].reserved4 = 0;
	idt[PIT_INT].seg_selector = KERNEL_CS;
	SET_IDT_ENTRY(idt[PIT_INT], pit_int_handler);

}

