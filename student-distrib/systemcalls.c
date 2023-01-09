#include "systemcalls.h"


// static int curr;
// this is the current process we are on
//the maximum amount of processes that can happen
static int proccesses_state[6] = {0, 0, 0, 0, 0, 0};


//int shell_counter = 0;
/*
execute
Description: read in the command specified in the parameter, and act upon it
Inputs: command
Outputs: returns 0 if command executed correctly, -1 otherwise
Side effects: parses the command, checks validility, sets up paging, and sets up iret for execution
 */
int32_t execute(const uint8_t * command) {

    //simple base case check
    if (command == NULL) {
        return -1;
    }

    //command can be a max 32 characters long
    uint8_t command_input[MAX_CMD_CHARACTER_COUNT];
    uint8_t command_argument[MAX_CMD_CHARACTER_COUNT];

    //get the most significant 4 bytes of the varitual address
    uint32_t virtualAddress[BYTE_LENGTH];    
    //set up temporary dentry, pid , length of command string, and iterator number        
    dentry_t temporaryDentry;
    int i;
    int j;
    int k;
    int length;
    int pid;
    length = strlen((char*)command);

    //parse the command function
    for (i = 0; i < length; i++) {
        if (command[i] == ' ' || command[i] == '\0') break;
        command_input[i] = command[i];

    }
    //parse this function for the cat command
    k = 0;
    for (j = i + 1; j < length; j++) {
        if (command[i] == '\0') {
            break;
        }
        if (command[j] == ' ') {
            continue;
        }
        command_argument[k] = command[j];
        k++;
    }

    //set the end to this character
    command_input[i] = '\0';

    //check dentry and command
    if (read_dentry_by_name(command_input, &temporaryDentry) == -1) {
        return -1;
    }

    //check the elf magic numbers 
    char magicNumbersCheckBuf[4];
    if (read_data(temporaryDentry.inode_num, 0, (unsigned char*) magicNumbersCheckBuf, 4) == -1) {
        return -1;
    }
    // Check the magnic numbers buffer with the four ELF provided magic numbers, if any one of the four don't match we immedietly kill exe
    if((magicNumbersCheckBuf[0] != MAGIC_NUMBER_1_ELF && magicNumbersCheckBuf[1] != MAGIC_NUMBER_2_ELF &&
         magicNumbersCheckBuf[2] != MAGIC_NUMBER_3_ELF && magicNumbersCheckBuf[3] != MAGIC_NUMBER_4_ELF)) {
        return -1; 
    }

    //calculate the virtual address
    dentry_t dentry = temporaryDentry;
    // ELF Bytes are at 24-27, which is 4 bytes tarting at 24
    read_data(dentry.inode_num, 24, (uint8_t*)virtualAddress, 4);

    //uint32_t prog_loc = ((virtualAddress[3]) << 24) | ((virtualAddress[2]) << 16) | ((virtualAddress[1]) << 8)  | (virtualAddress[0]);


    //calculate the next available process
    int pidIterator;
    int processFlag;
    processFlag = 0;
    for (pidIterator = 0; pidIterator < 6; pidIterator++) {
        //if the process at the current index is equal to 1, then set the pid equal to this index
        if (proccesses_state[pidIterator] == 0) {
            pid = pidIterator;
            processFlag = 1;
            break;
        }
    }

    if (processFlag == 1){
        //if an available process was found, then update the state array and current counter
        proccesses_state[pid] = 1;
       
    } else {
        return -1;
    }
    if(terminals[get_current_terminal()].shell_flag == -1) {
        terminals[get_current_terminal()].shell_flag = pid; 
    }
    //set up paging
    page_directory[PROGRAM_PAGING].entry_4M.present = 1;                //changes pointer to user program
    page_directory[PROGRAM_PAGING].entry_4M.page_size = 1;
    page_directory[PROGRAM_PAGING].entry_4M.user_sup = 1;
    page_directory[PROGRAM_PAGING].entry_4M.read_write = 1;
    page_directory[PROGRAM_PAGING].entry_4M.address = ((KSTACK_START + (FOUR_MB * pid)))>>22;

    


    //get the pcb basied on the current address on the pid calculation
    pcb_t* pcb = (pcb_t *)(KSTACK_START - PCB_SIZE * (pid + 1));
    //make sure to save the pid to the pcb's pid
    pcb->pid=pid;

    pcb->terminal = get_current_terminal();

    //copy the arguments with the current pcb with the command_argument
    strcpy((int8_t*)pcb->arguments, (int8_t*)command_argument);

    //set up terminal read and terminal write
    pcb->file_array[0].read  = (void*)terminal_read;  
    pcb->file_array[0].write = NULL;
    pcb->file_array[0].open  = NULL;
    pcb->file_array[0].close = NULL;
    pcb->file_array[0].flag  = 1;

    pcb->file_array[1].read  = NULL;
    pcb->file_array[1].write = (void*)terminal_write;
    pcb->file_array[1].open  = NULL;
    pcb->file_array[1].close = NULL;
    pcb->file_array[1].flag  = 1;

    //set all files except stdin stdout to 0, has to start at 2
    for (i = 2; i < FDESC_SIZE; i++) {
        pcb->file_array[i].flag = 0;
    }


    // Base shell has parent of itself
    if (pid == terminals[get_current_terminal()].shell_flag) {
        pcb->parent = (KSTACK_START - PCB_SIZE * (terminals[get_current_terminal()].shell_flag + 1));
    }
    // Else Parent is the previous thing before
    else {
        pcb->parent = (KSTACK_START - (PCB_SIZE * (terminals[get_current_terminal()].pid + 1)));
    }
    terminals[get_current_terminal()].pid = pid;
    // asm volatile("          \n
    //             movl %%ebp, %%eax   \n
    //             movl %%esp, %%ebx   \n
    //         "
    //         :"=a"(pcb->parent_ebp), "=b"(pcb->parent_esp)
    //         :
    //         : "eax", "ebx"
    //         );



    // page_directory[32].entry_4M.present = 1;
    // page_directory[32].entry_4M.page_size = 1;
    // page_directory[32].entry_4M.user_sup = 1;
    // page_directory[32].entry_4M.read_write = 1;
    // page_directory[32].entry_4M.address = ((pid*(0x100000*4)) + (0x100000*4))>>22;

// TLB Flush
    asm volatile(
                 "movl %%cr3, %%eax;"
                 "movl %%eax, %%cr3;"
                 :                      
                 :                      
                 :"%eax"                
                 );

// Copy the Program Data
    dentry_t d;
    read_dentry_by_name(command_input, &d);
    read_data(d.inode_num, 0, (uint8_t *)(PROG_IMG), FOUR_MB);        // change magic number
// TSS switch
    tss.ss0 = KERNEL_DS;
    tss.esp0 = KSTACK_START - (PCB_SIZE * pid) - 4;
// Save ESP and Save EBP of the parent within the PCB
    uint32_t saved_ebp = 0; 
    uint32_t saved_esp = 0;

    asm volatile(
                "movl %%ebp, %0;"
                "movl %%esp, %1;"
                :"=g"(saved_ebp), "=g"(saved_esp)
                );
    
    pcb->parent_ebp = saved_ebp;
    pcb->parent_esp = saved_esp;

    // if(shell_counter < 2){
    //     memcpy(uint32_t* 0xB8000, const uint32_t* vidMap[shell_counter], uint32_t FOUR_KB);
    //     shell_counter++;
    //     execute("shell");
    // }

    // uint32_t temp = (0x00048000 + 0x8048000);
// Context Switch
    asm volatile(
            "mov $0x2B, %%ax;"
            "mov %%ax, %%ds;"
            "movl $0x83FFFFC, %%eax;"
            "pushl %1;"
            "pushl $0x2B;"
            "pushl %%eax;"
            "pushfl;"
            "popl %%eax;"
            "orl $0x200, %%eax;"
            "push %%eax;"
            "pushl $0x23;"
            "pushl %0;"
            "iret;"
            // "RETURN_FROM_IRET:;"
            // "LEAVE;"
            // "RET;"
            :               /* no outputs         */
            :"r"(virtualAddress[0]), "r"(pcb)    /* input              */
            :"%edx","%eax"  /* clobbered register */
            );

    return 0;

    
}
/*
Read
Description: Calls either directory read, terminal read, or file read depnding on passed fd
Inputs: fd file in file array, buffer to write into, nbytes to copy
Outputs: -1 if no bytes can be read, number of bytes
Side effects: Fills up file buffer
 */
int32_t read(int32_t fd, void * buf, int32_t nbytes){
// You can read from stdin, so fd can equal 0, but not 1
    if (fd < 0 || fd >= FDESC_SIZE || fd == 1) {
        return -1;
    }
// If we don't have a buffer 
    if (buf == NULL) {
        return -1;
    }
// We can't have number of bytes be less than 0
    if (nbytes < 0) {
        return -1;
    }
// Find PCB, find the flags, if the file is in use we can read from it and call read
    pcb_t * temp_pointer = (pcb_t*)(KSTACK_START - ((terminals[get_current_terminal()].pid + 1) * PCB_SIZE));

    //int offset = temp_pointer->file_array[fd].file_pos;
    if (temp_pointer->file_array[fd].flag == 1) {
        // int inode = temp_pointer->file_array[fd].inode;
        int count = temp_pointer->file_array[fd].read(fd, buf, nbytes); // figure out offset, point to file system read and terminal read
        if (count < 0) {
            return -1;
        }
        temp_pointer->file_array[fd].file_pos += count;
// return bytes read
        return count;
    }
// otherwise file is not in use
    return -1;
}
/*
Write
Description: Calls either directory write, terminal write, or file write depnding on passed fd
Inputs: fd file in file array, buffer to write into, nbytes to copy
Outputs: -1 if no bytes can be read, number of bytes written to
Side effects: NONE
 */
int32_t write(int32_t fd, const void * buf, int32_t nbytes){
    // FD can be 1
    if (fd < 1 || fd >= FDESC_SIZE) {
        return -1;
    }
// Null Checks
    if (buf == NULL) {
        return -1;
    }

    if (nbytes < 0) {
        return -1;
    }
    // Find PCB we need to write to 
    pcb_t * temp_pointer = (pcb_t*)(KSTACK_START - ((terminals[get_current_terminal()].pid + 1) * PCB_SIZE));

    // int inode = temp_pointer->file_array[fd].inode;
    int count = temp_pointer->file_array[fd].write(fd, (void *)buf, nbytes);

    if (count < 0){ 
        return -1;
    }

    return count;

}
/*
Open
Description: Open a file in the filearray
Inputs: filename, name of file 
Outputs: Index in the filearray that it is passed into
Side effects: FD array is modified if a filename exists and the FD array is not full
 */
int32_t open(const uint8_t * filename) {
// Cretes Dentry and finds the Dentry within the directory
    dentry_t dentry;

    if(strlen((char*)filename) == NULL || read_dentry_by_name(filename, &dentry) == -1){
        return -1;  /* empty string*/
    }

    pcb_t * temp_pointer;

    int i;
// Finds the PCB for the current pocess
    temp_pointer = (pcb_t*)(KSTACK_START - ((terminals[get_current_terminal()].pid + 1) * PCB_SIZE));
// Finds an open space in the file array, if one cannot be found, return -1
    for (i = 2; i < FDESC_SIZE; i++) {
        if (temp_pointer->file_array[i].flag == 0) {
            temp_pointer->file_array[i].flag = 1;
            break;
        }
    }

    if (i == FDESC_SIZE) return -1;
// If the filetype is 0, that means it is RTC
    if (dentry.filetype == 0) {
        temp_pointer->file_array[i].read = (void*)rtc_read;
        temp_pointer->file_array[i].write = (void*)rtc_write;
        temp_pointer->file_array[i].open = rtc_open;
        temp_pointer->file_array[i].close = rtc_close;
    }
// Filetype of 1 means that it is a directory
    if (dentry.filetype == 1) {
        temp_pointer->file_array[i].read = (void*)read_dir;
        temp_pointer->file_array[i].write = (void*)write_dir;
        temp_pointer->file_array[i].open = open_dir;
        temp_pointer->file_array[i].close = close_dir;
    }
// Filetype of 2 means that it is a file
    if (dentry.filetype == 2) {
        temp_pointer->file_array[i].read = (void*)read_file;
        temp_pointer->file_array[i].write = (void*)write_file;
        temp_pointer->file_array[i].open = open_file;
        temp_pointer->file_array[i].close = close_file;
    }
// Set the Inode number, flag, and reset file position to 0
    temp_pointer->file_array[i].inode = dentry.inode_num;
    temp_pointer->file_array[i].flag = 1;
    temp_pointer->file_array[i].file_pos = 0;
    temp_pointer->file_array[i].open(filename);

// return FD array
    return i;

}
/*
Close
Description: Closes a file in the filearray
Inputs: index of the fd array that we wish to close  
Outputs: 0 on successful close, -1 otherwise
Side effects: FD array is modified if fd exists and is currently open 
 */
int32_t close(int32_t fd){ 
// Cannot close StdinStdout
    if (fd < 2 || fd >= FDESC_SIZE) {
        return -1;
    }
// Finds PCB for current process, and sets flag to 0 if it is not already closed, otherwise error
    pcb_t * temp_pointer = (pcb_t*)(KSTACK_START - ((terminals[get_current_terminal()].pid + 1) * PCB_SIZE));
    if(temp_pointer->file_array[fd].flag == 0) {
        return -1; 
    }
    temp_pointer->file_array[fd].flag = 0;
    temp_pointer->bitmap[fd] = 0;

    return 0;
}
/*
Halt
Description: Halts a process that is currently running
Inputs: status
Outputs: 0 on successful close, -1 otherwise
Side effects: Stack is returned to that of userspace, shell may be restarted on a halt on the original shell 
 */
int32_t halt(uint8_t status) {
    // restore parent data
    // retore parent paging
    // Close FDs
    // Jump To Execute return
    int i;
    // Get the PCB for the current Process
    pcb_t *current_pcb = (pcb_t*)(KSTACK_START - ((terminals[get_current_terminal()].pid + 1) * PCB_SIZE));
    uint32_t stat_ret_val;
    pcb_t* new_pcb;
    int32_t parent_pcb;
    int32_t parent_esp;
    int32_t parent_ebp;
    // If the status isn't 15 than it did not error out and we can just return 0
    if (status == 15) {
        stat_ret_val = 256;
    }
    
    else {
        stat_ret_val = (uint32_t)status;
    }
// This program is the base shell, restart the shell
    if (current_pcb -> pid == terminals[get_current_terminal()].shell_flag) {
        proccesses_state[current_pcb -> pid] = 0;
        execute((uint8_t*)"shell");

    }

    else{
    // return to parent
        proccesses_state[terminals[get_current_terminal()].pid] = 0;  
        parent_pcb = current_pcb->parent;
        parent_esp = current_pcb->parent_esp; 
        parent_ebp = current_pcb->parent_ebp;
        new_pcb = (pcb_t*) parent_pcb;
        terminals[get_current_terminal()].pid = new_pcb -> pid;
    }
    // retore Parent Paging??????

    page_directory[32].entry_4M.address = ((KSTACK_START + (FOUR_MB * terminals[get_current_terminal()].pid))) >> 22;
 
    //putc(0x22);
//  Flush the TLB
        asm volatile(
                 "movl %%cr3, %%eax;"
                 "movl %%eax, %%cr3;"
                 :                      
                 :                      
                 :"%eax"                
                 );
    // close Fds
    for(i = 2; i < FDESC_SIZE; i++) {
        current_pcb->file_array[i].flag = 0;
    }
//    Reset the TSS
    tss.ss0 = KERNEL_DS;
    tss.esp0 = KSTACK_START - (PCB_SIZE * terminals[get_current_terminal()].pid) - 4;
    // Set up ESP and EBP and then
    // fuck this shit I'm asking a TA
    // putc('\n');

// Restore parent esp and ebp and write status return value into eax
    asm volatile("          \n\
            movl %0, %%ebp   \n\
            movl %1, %%esp   \n\
            movl %2, %%eax   \n\
            leave               \n\
            ret                 \n\
        "
        :
        :"r"(parent_ebp), "r"(parent_esp), "r"(stat_ret_val)
        : "cc", "%eax", "%esp", "%ebp"
        );
    return 0;
}
/*
Get Arguments
Description: Gets the arguments for the executables
Inputs: Buffer to put arguments into, number of bytes in argument
Outputs: 0 on successful arguments, -1 otherwise
Side effects: NONE
 */
int32_t getargs(uint8_t * buf, int32_t nbytes){
    //get the current pcb 
    pcb_t* temp_pointer = (pcb_t*)(KSTACK_START - ((terminals[get_current_terminal()].pid + 1) * PCB_SIZE));
    int i = 0;

    if (buf == NULL) return -1;

    //check the bounds
    if (nbytes < 0 || nbytes > 1024) {
        return -1;
    }
    if( temp_pointer->arguments[0] == NULL) {
        return -1; 
    }
    //fill the buf with the current pcb's arguments array
    for (i = 0; i < nbytes; i++) {
        buf[i] = temp_pointer->arguments[i];
    }

    return 0;
}
/*
VIDMAP
Description: Sets the Videomap
Inputs: Pointer to a pointer to where we should start the screen
Outputs: -1 if the pointer ot the pointer is out of the correct location, 0 otherwise
Side effects: Screen video memory will be messed up
 */
int32_t vidmap(uint8_t ** start_screen){
    // check if start screen is in the correct location and not null, otherwise return -1, 4*32 is 128 MB and 2*33 is 136, start
    // and end of video memory
    if((uint32_t) start_screen < (FOUR_MB * 32) || (uint32_t)start_screen > (FOUR_MB * 33) || start_screen == NULL){
        return -1;
    }
    // TLB is flushed
    asm volatile(
            "movl %%cr3, %%eax;"
            "movl %%eax, %%cr3;"
            :                      
            :                      
            :"%eax"                
    );
    // 4 MB *33 = 132 MB, start of video memory
   *start_screen = (uint8_t*) (FOUR_MB * 33);

    return 0;
}

int32_t set_handler(int32_t signum, void * handler_address){
    return 0;
}

int32_t sigreturn(void){
    return 0;
}
/*
VIDMAP
Description: Returns a pointer to the current PCB
Inputs: Nothing
Outputs: Pointer to the current pCB
Side effects: NONE
 */
pcb_t* current_pcb() {
    pcb_t* pcb = (pcb_t*)(KSTACK_START - ((terminals[get_current_terminal()].pid + 1) * PCB_SIZE));
    return pcb; 
}


