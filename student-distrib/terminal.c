
#include "terminal.h"

#include "systemcalls.h"
/* Structure of Keyboard Buffer */

// struct MODIFIER_FLAGS {
//   int shift_flag, ctrl_flag, alt_flag, capslock_flag, enter_flag, terminal_flag;
// };

// struct KEYBOARD_BUFFER {
//   unsigned char kbuf[MAX_KEY_BUFFER];
//   int32_t n_kbuf;
// };
// extern struct KEYBOARD_BUFFER kbuffer; 
// extern struct MODIFIER_FLAGS modifier;

int current_terminal; 
/* int32_t terminal_init()
 * Functionality: Initialize the terminal.
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 */
int32_t terminal_init() {
    int i; 
    int j;

    for(i = 0; i < 3; i++) {
        terminals[i].pid = -1;
        terminals[i].active = 0; 
        terminals[i].terminal_number = i;
        terminals[i].x_pos = 0;
        terminals[i].y_pos = 0; 
        // terminals[i].modifiers.enter_flag = 0;
        terminals[i].vidmem = (uint8_t*) 0xb8000 + (i + 1)*0x1000;
        for(j = 0; j < 128; j++) {
             terminals[i].buffer[j] = '\0';
        }
        terminals[i].shell_flag = -1;
        terminals[i].buf_loc = 0; 
    }
    return 0;

}
/* int32_t terminal_switch()
 * Functionality: Switches the terminal to the one in the input
 * Inputs: To terminal
 * Outputs: NONE
 * Side Effects: NONE
 */
int32_t terminal_switch(int to) {
    if(to == current_terminal) {
        return 0;
    }
    if(to >= 3 || to < 0) {
        return -1;
    }
    // Save the terminal
    save_terminal();
    current_terminal = to; 

// set current terminal to the intended destination
    if((terminals[to].active) == 0) {
        // set terminal as active
        // DO SOME MEMCPY SHIT HERE
        sti();
        send_eoi(1);
        terminals[to].active = 1;
        load_terminal(to);
        // execute shell
        clear();
        

        terminals[to].shell_flag = terminals[to].pid; 
        execute((uint8_t*)"shell");
        return 0; 
    }
    // If the terminal is active, save and load, update current terminal 
    load_terminal(to);
    current_terminal = to; 

    // Save EBP ESP
    sti();
    return 0;
}
/* int32_t terminal_switch()
 * Functionality: Switches the terminal to the one in the input
 * Inputs: To terminal
 * Outputs: NONE
 * Side Effects: NONE
 */
int32_t save_terminal() {
    int cursor_position;
    cursor_position = get_cursor_position();
    terminals[current_terminal].x_pos = cursor_position % NUM_COLS;
    terminals[current_terminal].y_pos = cursor_position / NUM_COLS;
    terminals[current_terminal].buf_loc = kbuffer.n_kbuf;
    // Get current X position and Y position and save them to the struct

    // load everything in vidmem to a different location
    memcpy((void*)VIDEO_PAGE + (current_terminal + 1)*fourK,(const void*) VIDEO_PAGE,(uint32_t) fourK);
    // Save the current buffer 
    strcpy((int8_t*)terminals[current_terminal].buffer, (const int8_t*)kbuffer.kbuf);
    return 0;
}

/* int32_t load_terminal()
 * Functionality: loads the correct terminal
 * Inputs: To terminal
 * Outputs: NONE
 * Side Effects: NONE
 */
int32_t load_terminal(int number) {
    update_cursor(terminals[number].x_pos, terminals[number].y_pos);
    // update X,Y coordinates
    // restore buffer

    // THIS DOES NOT COPY PROPERLY SINCE TERMINALS BUFFER IS ALL NULL ONLY COPIES FIRST NULL CHARACTER
    //strcpy(kbuffer.kbuf,terminals[number].buffer);
    strcpy((int8_t*)kbuffer.kbuf, (const int8_t*)terminals[0].buffer);
    kbuffer.n_kbuf = terminals[number].buf_loc;
    // load this into video memory
    memcpy((void*)VIDEO_PAGE,(const void*) VIDEO_PAGE + (number + 1)*fourK,(uint32_t) fourK);
    return 0;
}

/* int32_t terminal_open()
 * Functionality: Initialize the keyboard.
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 */
int32_t terminal_open() {
    return 0;
}

/* int32_t terminal_open()
 * Functionality: Clear any terminal specific variables.
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 */
int32_t terminal_close() {
    return 0;
}

/* int32_t terminal_read(int32_t fd, void * buf, int32_t n)
 * Functionality: Reads from the keyboard buffer into buf, until a newline '\n'.
 e.g.
    n is the number of chars to be read
    CALL read(0,buf,n);
    after user presses enter buf contains "frame0.txt\n"
 * Inputs: NONE
 * Outputs: the number of bytes read, or -1 if failed
 * Side Effects: NONE
 */
int32_t terminal_read(int32_t fd, void * buf, int32_t n) {
    char * char_buf = (char *) buf;
    int n_read = 0;
    // int i;
    int readed[128] = {0};

    sti();

	//printf("Terminal read Function called\n");

    while (1) {
        // if (n_read == n) return -1;
        if(modifier.enter_flag == 1){
            modifier.enter_flag = 0;
            while(kbuffer.kbuf[n_read] != '\n' && kbuffer.kbuf[n_read] != NULL && n_read < 128){
                if (readed[n_read] == 0) {
                    readed[n_read] = 1;
                    char_buf[n_read] = kbuffer.kbuf[n_read];
                    // fill in the other char
                    n_read++;
                }
            }
            break;
        }
    }

    // load the last char ('\n')
    char_buf[n_read] = kbuffer.kbuf[n_read];
    // printf("load %c", kbuffer.kbuf[n_read]);
    n_read++;

    // printf("Terminal read Function encountered enter\n");
    int i;
    kbuffer.n_kbuf = 0;
    for (i=0; i<128; i++) {
        kbuffer.kbuf[i] = 0;
    }
    // for (i=0; i<n; i++) {
    //     // newline encountered, break
    //     if (kbuffer.kbuf[i] == '\n') {
    //         char_buf[i] = kbuffer.kbuf[i];
    //         n_read++;
    //         break;
    //     }

    //return terminal_write(0, char_buf, n);
    return n_read;

}

/* int32_t terminal_write(int32_t fd, void * buf, int32_t n)
 * Functionality: Writes n bytes from buf to screen.
  e.g.
    char* buf = "391OS> "
    write(1, buf, 7);
    "391OS> " will be printed to the screens
 * Inputs: NONE
 * Outputs: the number of bytes written, or -1 if failed
 * Side Effects: NONE
 */
int32_t terminal_write(int32_t fd, void * buf, int32_t n) {
    char * char_buf = buf;
    int i = 0;
    // if(n == 0){
    //     return;
    // }
    for( i = 0; i < n; i ++) {
        if(char_buf[i] != '\0') {
            putc(char_buf[i]);
        }
    }
    // while(char_buf[i] != '\0') {
    //     // print the content of buffer onto screen
    //     putc(char_buf[i]);
    //     n_written++;
    //     i++;
    //     if(i == n){
    //         break;
    //     }
    // }

    // if(char_buf[i] == NULL){
    //     putc('\n');
    //     // n_written++;
    // }
    

    // if(n_written > n){
    //     putc(' ');
    //     return -1;
    // }else{
        return n;
}

int get_current_terminal() {
    return current_terminal;
}

void set_current_terminal(int a) {
    current_terminal = a;
}
