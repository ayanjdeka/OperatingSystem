

#ifndef _SYSTEMCALLS_H
#define _SYSTEMCALLS_H

#include "lib.h"
#include "rtc.h"
#include "file_sys.h"
#include "keyboard.h"
#include "page.h"
#include "x86_desc.h"
#include "terminal.h"

//define pcb size, the stack start, the maximum desc size, and the program address
#define PCB_SIZE    0x2000
#define KSTACK_START  0x800000
#define FDESC_SIZE  8
#define program_addr 0x8000000
#define MAX_CMD_CHARACTER_COUNT 32
#define BYTE_LENGTH 4
#define PROG_IMG 0x8048000
#define FOUR_MB 0x400000
#define PROGRAM_PAGING 32
#define FOUR_KB 0x1000

#define MAGIC_NUMBER_1_ELF 0x7F
#define MAGIC_NUMBER_2_ELF 0x45
#define MAGIC_NUMBER_3_ELF 0x4C
#define MAGIC_NUMBER_4_ELF 0x46

/* https://stackoverflow.com/questions/9932212/jump-table-examples-in-c */
typedef struct file_desc_t { 
    int (*open)(const uint8_t *); 
    int (*close)(int32_t); 
    int (*read)(uint32_t,  uint8_t* , uint32_t);
    int (*write)(uint32_t, uint8_t* , uint32_t);
    int32_t inode, file_pos, flag;
} file_desc_t;

typedef struct pcb_t{
    int32_t parent;
    int32_t parent_esp;
    int32_t parent_ebp;
    file_desc_t file_array[FDESC_SIZE];
    int8_t arguments[1024];
    int32_t bitmap[FDESC_SIZE];
    int32_t pid;
    int32_t terminal;
} pcb_t;

//uint32_t vidMap[3] = {0xB9000, 0xBA000, 0xBB000};
int32_t halt(uint8_t status);
int32_t execute(const uint8_t * command);
int32_t read(int32_t fd, void * buf, int32_t nbytes);
int32_t write(int32_t fd, const void * buf, int32_t nbytes);
int32_t open(const uint8_t * filename);
int32_t close(int32_t fd);
int32_t getargs(uint8_t * buf, int32_t nbytes);
int32_t vidmap(uint8_t ** start_screen);
int32_t set_handler(int32_t signum, void * handler_address);
int32_t sigreturn(void);
pcb_t* current_pcb();

int32_t curr;
#endif /* _SYSCALL_H */

