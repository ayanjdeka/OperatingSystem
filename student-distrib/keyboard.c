#include "keyboard.h"
#include "types.h"
#include "terminal.h"
/* Structure of Keyboard Buffer */


/* int get_kbuf_size()
 * Functionality: Get keyboard_buffer size.
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 */
int get_kbuf_size() {
  printf("buf n:%d.",kbuffer.n_kbuf);
  return kbuffer.n_kbuf;
}

/* void print_kbuf_char()
 * Functionality: Echo all chars in keyboard_buffer onto the screen.
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 */
void print_kbuf_char() {
  int i;
  for (i=0; i<kbuffer.n_kbuf; i++) {
    printf("%c",kbuffer.kbuf[i]);
  }
}

/* void char_to_kbuf(unsigned char inputchar)
 * Functionality: Load a chars into the keyboard_buffer.
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 */
void char_to_kbuf(unsigned char inputchar) {
  if (kbuffer.n_kbuf==127) {
    // if it's the 127th char to be written to the buffer
    // If you hit 127, keyboard should stop taking inputs unless it’s a backspace or enter.
    if (inputchar == '\n') {
      kbuffer.kbuf[kbuffer.n_kbuf] = inputchar;
      putc(inputchar);          // echo on screen

    } else {
      // do nothing and return
    }
    return;

  } else if (kbuffer.n_kbuf<127) {

    kbuffer.kbuf[kbuffer.n_kbuf] = inputchar;
    // printf("char:%c.",kbuffer.kbuf[kbuffer.n_kbuf]);
    kbuffer.n_kbuf += 1;
    // printf("add to buf n:%d.",kbuffer.n_kbuf);

    putc(inputchar);          // echo on screen
  }
  return;
}

/* void char_delete_kbuf(unsigned char inputchar)
 * Functionality: Delete char in keyboard_buffer (when backspace hitted).
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 */
void char_delete_kbuf(unsigned char inputchar) {
  if (kbuffer.n_kbuf>0) {

    kbuffer.kbuf[kbuffer.n_kbuf] = 0;
    // printf("last char deleted.");
    kbuffer.n_kbuf -= 1;
    // printf("delete buf n:%d.",kbuffer.n_kbuf);

    putc(inputchar);          // delete the previous stored char on display

  } else {
    return;
  }
}

// KBDUS - US Keyboard Layout.
// Reference: http://www.osdever.net/bkerndev/Docs/keyboard.htm
// capslock state: 0
// scancode is 8 bits, while the lower 7 bits represents the key pressed or released
// and the msb is 1 when released, 0 when pressed
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* 14 - Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 , L-38*/
 '\'', '`',   
    
    0,		/* 2a - Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
    'm', ',', '.', '/',   
    0,				/* 36 - Right shift */
    '*',
    0,	/* Alt */
    ' ',	/* Space bar */
    0,	/* 3a - Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow ################ */
    0,	/* Page Up */
    '-',
    0,	/* Left Arrow ################ */
    0,
    0,	/* Right Arrow ################ */
    '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow ################ */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

// Modifier key conditions:
// if shift is being pressed
// capslock state: 1
unsigned char kbdus_shift_on[128] =
{
    // Modified for shift
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	/* 9 */
  '(', ')', '_', '+', 
  
  '\b',	/* Backspace */
  '\t',			/* Tab */

  // When shift is on:
  //    if capslock on, should map to lowercase
  //    if capslock off, should ampa to uppercase
  'Q', 'W', 'E', 'R',	/* 19 */
  'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n',	/* ENTER KEY */
    0,			/* 29   - CONTROL */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',	/* 39 */
 '\"', '`',   0,		/* LEFT SHIFT */
 '|', 'Z', 'X', 'C', 'V', 'B', 'N',			/* 49 */
  'M', 
  
  '<', '>', '?',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};
/* void keyboard_init()
 * Functionality: Initialize the keyboard.
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 */
void keyboard_init(){
  int i;
    // outb(0x00, STATUS_REG);  // Initialize Status Register
    // clear();  // clear the screen (??)
  enable_irq(1);

  // Initialize the modifier flags
  modifier.shift_flag = 0;
  modifier.ctrl_flag = 0;
  modifier.alt_flag = 0;
  modifier.capslock_flag = 0;
  modifier.enter_flag = 0;
  modifier.terminal_flag = 1;


  // initialize the keyboard buffer
  printf("Initialize the buffer.");


  kbuffer.n_kbuf = 0;
  kbuffer_terminal1.n_kbuf = 0;
  kbuffer_terminal2.n_kbuf = 0;
  kbuffer_terminal3.n_kbuf = 0;
  kbuffer_terminal_temp.n_kbuf = 0;
  for (i=0; i<MAX_KEY_BUFFER; i++) {
    kbuffer.kbuf[i] = 0;
    kbuffer_terminal1.kbuf[i] = 0;
    kbuffer_terminal2.kbuf[i] = 0;
    kbuffer_terminal3.kbuf[i] = 0;
    kbuffer_terminal_temp.kbuf[i] = 0;
  }
}
/* void change_keyboard()
 * Functionality: Changes the terminal and keyboard buffer
 * Inputs: new and old terminal
 * Outputs: NONE
 * Side Effects: Changes the terminal and keyboard buffer
 */
void change_keyboard(int new, int old){
  //changing from 1 to 2
  if(new == 2 && old == 1){
    kbuffer_terminal_temp = kbuffer;
    kbuffer = kbuffer_terminal2;
    kbuffer_terminal1 = kbuffer_terminal_temp;
  }
  //changing from 1 to 3
  if(new == 3 && old == 1){
    kbuffer_terminal_temp = kbuffer;
    kbuffer = kbuffer_terminal3;
    kbuffer_terminal1 = kbuffer_terminal_temp;
  }
  //changing from 2 to 1
  if(new == 1 && old == 2){
    kbuffer_terminal_temp = kbuffer;
    kbuffer = kbuffer_terminal1;
    kbuffer_terminal2 = kbuffer_terminal_temp;
  }
  //changing from 1 to 2
  if(new == 3 && old == 2){
    kbuffer_terminal_temp = kbuffer;
    kbuffer = kbuffer_terminal3;
    kbuffer_terminal2 = kbuffer_terminal_temp;
  }
  //changing from 3 to 2
  if(new == 2 && old == 3){
    kbuffer_terminal_temp = kbuffer;
    kbuffer = kbuffer_terminal2;
    kbuffer_terminal3 = kbuffer_terminal_temp;
  }
  //changing from 3 to 1
  if(new == 1 && old == 3){
    kbuffer_terminal_temp = kbuffer;
    kbuffer = kbuffer_terminal1;
    kbuffer_terminal3 = kbuffer_terminal_temp;
  }
}

/* void keyboard_handler()
 * Functionality: Handle the input from keyboard.
 *          Stores/delete the corresponding char in kbuffer.
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 * Reference: http://www.osdever.net/bkerndev/Docs/keyboard.htm
 */
void keyboard_handler(){
    // cli();
    unsigned char scancode;
    unsigned char c;

    // read from keyboard data port
    scancode = inb(DATA_PORT);

    /* For test use only - comment it when HANDIN! */
    // if (!(scancode & 0x80) && scancode == 2) {
    //   print_kbuf_char();

    //   send_eoi(1);    // keyboard IRQ = 1
    //   // sti();

    //   asm volatile ("                   \n
    //       leave                       \n
    //       iret"
    //   );
    //   return;
    // }

    // printf("%x", scancode);
    if(scancode == ENTER){
      modifier.enter_flag = 1;
    }

    // flags == 1 when shift/ctrl/alt has been pressed (active)
    // 1. Capslock state should only change once it is pressed
    if (scancode == CAPSLOCK_PRESSED) {
      if (modifier.capslock_flag == 0) modifier.capslock_flag = 1;
      else if (modifier.capslock_flag == 1) modifier.capslock_flag = 0;
    }
    // if (scancode == CAPSLOCK_RELEASED)
    // 2. Left-shift and Right-shift are equavalent
    if (scancode == LEFT_SHIFT_PRESSED || scancode == RIGHT_SHIFT_PRESSED) {
      modifier.shift_flag = 1;
    } else if (scancode == LEFT_SHIFT_RELEASED || scancode == RIGHT_SHIFT_RELEASED) {
      modifier.shift_flag = 0;
    }

    // 3. Control state on when pressed, off when released
    if (scancode == CONTROL_PRESSED) {
      modifier.ctrl_flag = 1;
    } else if (scancode == CONTROL_RELEASED) {
      modifier.ctrl_flag = 0;
    }

    // 4. ALT state on when pressed, off when released
    if (scancode == ALT_PRESSED) {
      modifier.alt_flag = 1;
    } else if (scancode == ALT_RELEASED) {
      modifier.alt_flag = 0;
    }

    // see what code is pressed or released in hex
    // printf("scancode:%x.",scancode);

    // clear screen - ctrl+L
    if ((scancode == 38) && (modifier.ctrl_flag == 1)) {
      clear();
      int i;
      kbuffer.n_kbuf = 0;
      for (i=0; i<128; i++) {
        kbuffer.kbuf[i] = 0;
      }
    } else if ((scancode == UP_PRESSED) || (scancode == DOWN_PRESSED) 
      || (scancode == LEFT_PRESSED) || (scancode == RIGHT_PRESSED)) {
        // Ignore the arrow keys
        // Don't put them into the buffer & print them out for CP2
        // do nothing

    // Switch to 3 terminals - cp 3.5
    } else if ((scancode == F1_PRESSED) && (modifier.alt_flag == 1)) {
      terminal_switch(0);
      // if(modifier.terminal_flag == 2){
      //   terminal_switch(0,1);
      // }
      // if(modifier.terminal_flag == 3){
      //   terminal_switch(0,2);

      // }
      modifier.terminal_flag = 1;
    } else if ((scancode == F2_PRESSED) && (modifier.alt_flag == 1)) {
      terminal_switch(1);
      // if(modifier.terminal_flag == 1){
      //   terminal_switch(1,0);
      // }
      // if(modifier.terminal_flag == 3){
      //   terminal_switch(1,2);
      // }
      modifier.terminal_flag = 2;
    } else if ((scancode == F3_PRESSED) && (modifier.alt_flag == 1)) {
      terminal_switch(2);
      // if(modifier.terminal_flag == 1){
      //   terminal_switch(2,0);
      // }
      // if(modifier.terminal_flag == 2){
      //   terminal_switch(2,1);
      // }
      modifier.terminal_flag = 3;
    }
    // 0. Only echo to the display when it is not a modifier key
    else if ((scancode != CAPSLOCK_PRESSED) && (scancode != CAPSLOCK_RELEASED) 
        && (scancode != LEFT_SHIFT_PRESSED) && (scancode != LEFT_SHIFT_RELEASED) 
        && (scancode != RIGHT_SHIFT_PRESSED) && (scancode != RIGHT_SHIFT_RELEASED)
        && (scancode != CONTROL_PRESSED) && (scancode != CONTROL_RELEASED)
        && (scancode != ALT_PRESSED) && (scancode != ALT_RELEASED)) 
    {

      // 1. When A key is pressed (bit7 - 1 means a key is released))
      if (!(scancode & 0x80)) {

        // CASE 1. When Capslock is ON
        if (modifier.capslock_flag == 1) {
          if (modifier.shift_flag == 1) {
            // Caps on, shift on
            // digits - kbdus_shift_on
            // letters - kbdus (lowercase)
            if (get_char_type(scancode) == DIGIT) {
              c = kbdus_shift_on[scancode];
            } else {
              c = kbdus[scancode];
            }
          } else {
            // Caps on, shift off
            // digits - kbdus
            // letters - kbdus_shift_on (uppercase)
            if (get_char_type(scancode) == DIGIT) {
              c = kbdus[scancode];
            } else {
              c = kbdus_shift_on[scancode];
            }
          }
        } // CASE 2. When Capslock is OFF
        else if (modifier.capslock_flag == 0) {
          if (modifier.shift_flag == 1) {
            // Caps off, shift on
            // digits - kbdus_shift_on
            // letters - kbdus_shift_on (uppercase)
            c = kbdus_shift_on[scancode];          
          } else {
            // Caps off, shift off
            // digits - kbdus
            // letters - kbdus (lowercase)
            c = kbdus[scancode];
          }
        }

        // check if the char should be put (by hitting normal char)
        // or should remove the last char in the keyboard buffer
        if (scancode == BACKSPACE_PRESSED) {
          char_delete_kbuf(c);   // delete last element in buffer
        } else {
          char_to_kbuf(c);       // write to keyboard buffer
        }
      }
    } else {
      // do nothing for now - for other modifier keys or not shown on screen
    }

    send_eoi(1);    // keyboard IRQ = 1
    // sti();

    asm volatile ("                   \n\
        leave                       \n\
        iret"
    );
}

/* int get_char_type(unsigned char scancode)
 * Functionality: Initialize the keyboard.
  Judge the type of the char by scancode lower 7 bits.
  returns an int that represents the type of the char
    - 0   char is a digit
    - 1   char is a letter
    - 2   others
 * Inputs: NONE
 * Outputs: NONE
 * Side Effects: NONE
 */
int get_char_type(unsigned char scancode) {
  scancode = scancode & 0x7F; // 0b 111 1111 only take a look at the lower 7 bits
  if (scancode >= 2 && scancode <= 11) {
    return DIGIT;
  } else if ((scancode >= 16 && scancode <= 25) || 
  (scancode >= 30 && scancode <= 38) || 
  (scancode >= 44 && scancode <= 50)) {
    return LETTER;
  }
  return OTHER;
}

