#include "tests.h"
#include "x86_desc.h"
#include "rtc.h"
#include "lib.h"
#include "file_sys.h"

// modified
#include "terminal.h"
#include "keyboard.h"


#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}

// add more tests here

int idt_divide_zero(){
	TEST_HEADER;

	int i;
	int j = 1;
	for(i = 0; i < 69; i++) {
		j = j/i;
	}
	return FAIL;

}

/* RTC Test - Example
 * 
 * Tests the functionality
 * of the read, write, and open 
 * functions.
 */
int rtc_read_write_test() {
    TEST_HEADER;
    uint32_t i;
    uint32_t j;
    int32_t retval = 0;
	uint32_t maxFrequency = 1024;

    retval += rtc_open(NULL);
    for(i = 2; i <= maxFrequency; i*=2) {
        retval += rtc_write(NULL, &i, sizeof(uint32_t));
        printf("Testing: %d Hz\n[", i);
        for(j = 0; j < 10; j++) {
            retval += rtc_read(NULL, NULL, NULL);
            printf("#");
        }
        printf("]\n");
    }
	return PASS;
}

/* Checkpoint 2 tests */
int terminal_driver_test() {
	clear();
    // int n_r, n_w;
    // char* write_buf = ;
    // char buf[256];

	// Initialize the terminal
	terminal_open();
	
	// printf("Terminal write Test!\n");

	// TEST 1.1. Write the exact size of the buffer
	if (terminal_write(0, (uint8_t*)"Hello\n", 6) == -1) {
		printf("Error in terminal write Test 1!\n");
	}
	// TEST 1.2. Write the size smaller than the buffer
	if (terminal_write(0, (uint8_t*)"Hey World\n", 3) == -1) {
		printf("Error in terminal write Test 1.2!\n");
	}
	// TEST 1.3. Should not stop writting at a NULL byte
	if (terminal_write(0, (uint8_t*)"NULL Byte?\n", 20) == -1) {
		printf("Error in terminal write Test 1.3!\n");
	}

	// printf("Terminal read Test!\n");
	char buff[129];
	// TEST 2.1. Write the exact size of the buffer
	if (terminal_read(0, (uint8_t*)buff, 9) == -1) {
		printf("Error in terminal write Test 2.1!");
	}
	// printf("######## Terminal Read Test - 1 ########\n");
	// printf("> Step 1. Call terminal_read() that reads from the keyboard input: \n");
	// n_r = terminal_read(0, buf, 20);

	// // check if read has correctly loaded 
	// printf("> Step 2.Check if terminal_read() has loaded to the buffer: \n");
	// printf(">        by calling terminal_write(0, buf, 20)\n");
	// n_w = terminal_write(0, buf, 20);

	// printf("\n");
	// printf("> Step 3.Check if correct return value is correct: \n");
	// printf(">       The return value of terminal_read() is: %d\n", n_r);
	// printf(">       The return value of terminal_write() is: %d\n", n_w);
	// printf("########################################\n");

	terminal_close();
    return FAIL;
}

int file_system_name() {
	TEST_HEADER;
	dentry_t d_entry; 
	clear();
	if (read_dentry_by_name ((uint8_t*)"frame0.txt", &d_entry) == -1) {
		return FAIL;
	}
	
	printf("filetype:%d inode: %d", d_entry.filetype, d_entry.inode_num);
	return PASS; 
}

int file_system_long_name() {
	TEST_HEADER;
	dentry_t d_entry; 
	clear();
	// int i;
	if (read_dentry_by_name ((uint8_t*)"verylargetextwithverylongname.txt", &d_entry) == -1) {
		return FAIL;
	}
	
	printf("filetype:%d inode: %d \n", d_entry.filetype, d_entry.inode_num);
	// 32 is file length
	// for(i = 0; i < 32; i++) {
	// 	putc[d_entry.filename[i]];
	// }
	return PASS; 
}
int file_system_test() {
	TEST_HEADER;
	// cat is 5445 bytes long
	uint8_t buffer[5450];
	clear(); 
	dentry_t d_entry; 
	int i;
	int bytes_read;
	// printf("number of inodes:%d",boot->inode_count );
	read_dentry_by_name ((uint8_t*)"cat", &d_entry);
	printf("InodeNum: %d", d_entry.inode_num);
	// cat is 5445 bytes long
	bytes_read = read_data(d_entry.inode_num,0,buffer,5450);
	printf("bytes read %d \n", bytes_read);
	for(i = 0; i < bytes_read; i++) {
		if(buffer[i] != '\0') {
			//clear();
			putc(buffer[i]);
		}
	}

	if(bytes_read != 0) {
		return PASS;
	}
	// printf("%d",read_data(d_entry.inode_num,0,buffer,10));
	// printf("%d \n",strlen(buffer));
	// printf("Inode Num: %d",boot->inode_count);
	return FAIL; 
}
int file_system_test_two() {
	TEST_HEADER;
	clear();
	// Sample buffer of 4096
	uint8_t buffer[4096]; 
	dentry_t d_entry; 
	int i;
	int bytes_read;
	// printf("number of inodes:%d",boot->inode_count );
	read_dentry_by_name ((uint8_t*)"frame1.txt", &d_entry);
	printf("InodeNum: %d", d_entry.inode_num);
	bytes_read = read_data(d_entry.inode_num,0,buffer,4096);
	printf("bytes read %d \n", bytes_read);
	for(i = 0; i < bytes_read; i++) {
		if(buffer[i] != '\0') {
			putc(buffer[i]);
		}
	}

	if(bytes_read != 0) {
		return PASS;
	}
	// printf("%d",read_data(d_entry.inode_num,0,buffer,10));
	// printf("%d \n",strlen(buffer));
	// printf("Inode Num: %d",boot->inode_count);
	return FAIL; 
}
int file_system_list() {
	int ti; 
	clear();
	// read_dir(0,file,0);
	// printf(file);
	// printf("\n");
	open_dir((uint8_t*)".");
	// 17 is the number of directories in the boot block. 
	for(ti = 0; ti < 17; ti++) {
		// FILELEN is max 32
		uint8_t file[32];
		read_dir(0,file,0);
	}

	return PASS; 
}

int open_and_read_file() {
		// Sample buffer of 4096

	uint8_t buffer[4096]; 
	clear();
	int i;
	open_file((uint8_t*)"verylargetextwithverylongname.txt");
		// Sample buffer of 400

	int32_t maxlen = read_file(1,buffer, 4096); 
	if (maxlen == -1) {
		return FAIL;
	}
	for(i = 0; i < maxlen; i++) {
		if(buffer[i] != '\0') {
			putc(buffer[i]);
		}
	}
	return PASS; 
}

/* Checkpoint 3 tests */
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	//TEST_OUTPUT("idt_test", idt_test());
	// TEST_OUTPUT("divide zero", idt_divide_zero());

	// TEST_OUTPUT("RTC TEST", rtc_read_write_test());
	// while(1);

	

	// launch your tests here'

	//TEST_OUTPUT("bytes written by terminal test", terminal_driver_test());

	// TEST_OUTPUT("file system name test", file_system_name());
	//TEST_OUTPUT("file system long name test", file_system_long_name());
	//TEST_OUTPUT("file system read long", file_system_test());
	// TEST_OUTPUT("read_directory", file_system_list());
	//TEST_OUTPUT("file system fish test", file_system_test_two());
	// TEST_OUTPUT("file system open and read", open_and_read_file());
}
