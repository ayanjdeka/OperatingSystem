#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "lib.h"
#include "i8259.h"

#define KEYBOARD_PORT 0x21
#define DATA_PORT   0x60
#define STATUS_REG  0x64
#define COMMAND_REG 0x64
#define ENTER 0x1C

#define MAX_KEY_BUFFER 128

struct KEYBOARD_BUFFER {
  unsigned char kbuf[MAX_KEY_BUFFER];
  int32_t n_kbuf;
};

struct KEYBOARD_BUFFER kbuffer;
struct KEYBOARD_BUFFER kbuffer_terminal1;
struct KEYBOARD_BUFFER kbuffer_terminal2;
struct KEYBOARD_BUFFER kbuffer_terminal3;
struct KEYBOARD_BUFFER kbuffer_terminal_temp;

/* Structure of Modifier Flags */
/* Save the status of modifier keys. */
struct MODIFIER_FLAGS {
  int shift_flag, ctrl_flag, alt_flag, capslock_flag, enter_flag, terminal_flag;
};

volatile struct MODIFIER_FLAGS modifier;

void keyboard_init();
void change_keyboard(int new, int old);
void keyboard_handler();
int get_char_type(unsigned char scancode);

/* Modifier Keys - Scancodes */
/* Should not be printed to the screen when pressed. */
#define     CAPSLOCK_PRESSED        0x3a
#define     LEFT_SHIFT_PRESSED      0x2a
#define     RIGHT_SHIFT_PRESSED     0x36
#define     CONTROL_PRESSED         0x1d
#define     BACKSPACE_PRESSED       0x0e
#define     ALT_PRESSED             0x38

#define     CAPSLOCK_RELEASED       0xba
#define     LEFT_SHIFT_RELEASED     0xaa
#define     RIGHT_SHIFT_RELEASED    0xb6
#define     CONTROL_RELEASED        0x9d
#define     BACKSPACE_RELEASED      0x8e
#define     ALT_RELEASED            0xB8

/* Prevent arrow keys be written to buffers */
#define     UP_PRESSED              0x48
#define     UP_RELEASED             0xC8
#define     DOWN_PRESSED              0x50
#define     DOWN_RELEASED             0xD0
#define     LEFT_PRESSED              0x4B
#define     LEFT_RELEASED             0xCB
#define     RIGHT_PRESSED              0x4D
#define     RIGHT_RELEASED             0xCD

/* Judge the type of the char by scancode */
#define     DIGIT       0
#define     LETTER      1
#define     OTHER       2

/* scancode for F1, F2 and F3*/
#define     F1_PRESSED              0x3B
#define     F1_RELEASED             0xBB

#define     F2_PRESSED              0x3C
#define     F2_RELEASED             0xBC

#define     F3_PRESSED              0x3D
#define     F3_RELEASED             0xBD

#endif
