#ifndef _TERMINAL_H
#define _TERMINAL_H
#include "lib.h"
#include "keyboard.h"
#include "types.h"

#define VIDEO_PAGE 0xB8000
#define fourK 0x1000

typedef struct terminal_t{
    int pid; 
    int terminal_number;
    int active; 
    unsigned char buffer[MAX_KEY_BUFFER];
    int32_t buf_loc;
    // volatile struct KEYBOARD_BUFFER buffer; 
    // volatile struct MODIFIER_FLAGS modifiers;
    int x_pos, y_pos;
    uint8_t *vidmem; 
    int shell_flag; 
} terminal_t;

terminal_t terminals[3];

int32_t terminal_init(); 
// int32_t terminal_launch(int terminal_number); 
int32_t terminal_switch(int to);
int32_t save_terminal();
int32_t load_terminal(int number); 

/* Initialize the terminal */ 
int32_t terminal_open();

/* clear any terminal specific variables */
int32_t terminal_close();

/* reads from the keyboard buffer into buf, until a newline '\n'*/
int32_t terminal_read(int32_t fd, void * buf, int32_t n);

/* writes n bytes from buf to screen */
int32_t terminal_write(int32_t fd, void * buf, int32_t n);

int get_current_terminal(); 
void set_current_terminal(int a);
#endif
