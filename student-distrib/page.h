#ifndef _PAGE_H
#define _PAGE_H

#include "lib.h"

#define FISH_VIDEO_MEMORY 33


//directory entry when using 4K
typedef struct __attribute__((packed)) page_directory_entry_4k{
    uint8_t present : 1;
    uint8_t read_write : 1;
    uint8_t user_sup : 1;
    uint8_t write_through : 1;
    uint8_t cache_dis : 1;
    uint8_t accessed : 1;
    uint8_t reserved0 : 1;
    uint8_t page_size : 1;
    uint8_t global : 1;
    uint8_t available : 3;
    uint32_t address : 20;


}page_directory_entry_4k;

//directory entry when using 4M
typedef struct  __attribute__ ((packed)) page_directory_entry_4M{
    uint8_t present : 1;
    uint8_t read_write : 1;
    uint8_t user_sup : 1;
    uint8_t write_through : 1;
    uint8_t cache_dis : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t page_size : 1;
    uint8_t global : 1;
    uint8_t available : 3;
    uint8_t attribute_table : 1;
    uint32_t reserved0 : 9;
    uint32_t address : 10;


}page_directory_entry_4M;

//Directory
typedef union page_directory_entry {
        struct page_directory_entry_4k entry_4k;
        struct page_directory_entry_4M entry_4M;
}page_directory_entry;


//Table entry 
typedef struct __attribute__ ((packed))page_table_entry {
    uint8_t present : 1;
    uint8_t read_write : 1;
    uint8_t user_sup : 1;
    uint8_t write_through : 1;
    uint8_t cache_dis : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t page_attribute : 1;
    uint8_t global : 1;
    uint8_t available : 3;
    uint32_t address : 20;


}page_table_entry;


// directory and table
page_directory_entry page_directory[1024] __attribute__((aligned(4096)));
page_table_entry page_table[1024] __attribute__((aligned(4096)));
page_table_entry user_page_table[1024] __attribute__((aligned(4096)));

// initializes directory and tables
void page_init();

// enables paging in assembly
extern void enable_paging();


#endif

