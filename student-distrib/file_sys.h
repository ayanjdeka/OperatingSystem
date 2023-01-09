#ifndef _FILE_SYS
#define _FILE_SYS

#include "lib.h"
#include "types.h"

#define FILENAME_LEN 32
#define NUMBER_OF_FILES 63
#define BOOT_RESERVE 52
#define DENTRY_RESERVE 24
#define NUMBER_OF_DATA_BLOCKS 1023
#define FD_LENGTH 8

typedef struct __attribute__((packed)) dentry_t {

    int8_t filename[FILENAME_LEN];
    int32_t filetype;
    int32_t inode_num;
    int8_t reserved_24[DENTRY_RESERVE];

} dentry_t;

typedef struct __attribute__((packed)) boot_block_t {
    int32_t dir_count;
    int32_t inode_count;
    int32_t data_count;
    int8_t reserved_52[BOOT_RESERVE];
    dentry_t directory[NUMBER_OF_FILES];
}boot_block_t;

typedef struct __attribute__((packed)) inode_t {
    int32_t length;
    int32_t data_block_num [NUMBER_OF_DATA_BLOCKS];
}inode_t;

boot_block_t* boot;
inode_t*  i_node_start;
uint8_t* dataBlock;
dentry_t* data_entry; 

// typedef struct fd_array_node_t {
//     int32_t file_operations;
//     int32_t inode;
//     int32_t file_position;
//     int32_t flags;
// } fd_array_node_t;

// fd_array_node_t fd_node;

uint32_t dir_index; 


int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);

int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);


int32_t read_file (int32_t fd, void* buf, int32_t nbytes);

int32_t write_file (int32_t fd, const void* buf, int32_t nbytes);

int32_t open_file (const uint8_t* filename);

int32_t close_file (int32_t fd);

int32_t read_dir(int32_t fd, uint8_t* buf, int32_t nbytes);

int32_t write_dir(int32_t fd, const void* buf, int32_t nbytes);

int32_t open_dir(const uint8_t* filename);

int32_t close_dir(int32_t fd);

extern void file_init(uint32_t file_start);


#endif


