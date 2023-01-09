#include "lib.h"
#include "page.h"
#include "x86_desc.h"

#define video_mem_address0 0xB8000
#define video_mem_address1 0xB9000
#define video_mem_address2 0xBA000
#define video_mem_address3 0xBB000
#define kernel_mem_address 0x400000



/* Page_init(): create a page directory and table and map video memeory 
*               and kernel to the tables.
*   input: none
*   Output: none
*   Effect: creates directory and table
*/

void page_init(){

    int i;
    for( i = 0; i < 1024; i++){                     // inititalize all values of entries to zero
    page_directory[i].entry_4k.present = 0;
    page_directory[i].entry_4k.read_write = 0;
    page_directory[i].entry_4k.user_sup = 0;
    page_directory[i].entry_4k.write_through = 0;
    page_directory[i].entry_4k.cache_dis = 0;
    page_directory[i].entry_4k.accessed = 0;
    page_directory[i].entry_4k.reserved0 = 0;
    page_directory[i].entry_4k.page_size = 0;
    page_directory[i].entry_4k.global = 0;
    page_directory[i].entry_4k.available = 0;
    page_directory[i].entry_4k.address = 0;

    page_directory[i].entry_4M.present = 0;
    page_directory[i].entry_4M.read_write = 0;
    page_directory[i].entry_4M.user_sup = 0;
    page_directory[i].entry_4M.write_through = 0;
    page_directory[i].entry_4M.cache_dis = 0;
    page_directory[i].entry_4M.accessed = 0;
    page_directory[i].entry_4M.dirty = 0;
    page_directory[i].entry_4M.page_size = 0;
    page_directory[i].entry_4M.global = 0;
    page_directory[i].entry_4M.available = 0;
    page_directory[i].entry_4M.attribute_table = 0;
    page_directory[i].entry_4M.reserved0 = 0;
    page_directory[i].entry_4M.address = 0;

    page_table[i].present = 0 ;
    page_table[i].read_write = 0;
    page_table[i].user_sup = 0;
    page_table[i].write_through = 0;
    page_table[i].cache_dis = 0;
    page_table[i].accessed = 0;
    page_table[i].dirty = 0;
    page_table[i].page_attribute = 0;
    page_table[i].global = 0;
    page_table[i].available = 0;
    page_table[i].address = 0;
        
    }

    //set video memory
    page_directory[0].entry_4k.present = 1;
    page_directory[0].entry_4k.read_write = 1;
    page_directory[0].entry_4k.user_sup = 0;
    page_directory[0].entry_4k.write_through = 0;
    page_directory[0].entry_4k.cache_dis = 0;
    page_directory[0].entry_4k.accessed = 0;
    page_directory[0].entry_4k.reserved0 = 0;
    page_directory[0].entry_4k.page_size = 0;
    page_directory[0].entry_4k.global = 0;
    page_directory[0].entry_4k.available = 0;
    page_directory[0].entry_4k.address = (int)page_table >> 12;

    page_table[(video_mem_address0 >> 12)].present = 1 ;
    page_table[(video_mem_address0 >> 12)].read_write = 1;
    page_table[(video_mem_address0 >> 12)].user_sup = 0;
    page_table[(video_mem_address0 >> 12)].write_through = 0;
    page_table[(video_mem_address0 >> 12)].cache_dis = 0;
    page_table[(video_mem_address0 >> 12)].accessed = 0;
    page_table[(video_mem_address0 >> 12)].dirty = 0;
    page_table[(video_mem_address0 >> 12)].page_attribute = 0;
    page_table[(video_mem_address0 >> 12)].global = 1;
    page_table[(video_mem_address0 >> 12)].available = 0;
    page_table[(video_mem_address0 >> 12)].address = (video_mem_address0 >> 12);

    page_table[(video_mem_address1 >> 12)].present = 1 ;
    page_table[(video_mem_address1 >> 12)].read_write = 1;
    page_table[(video_mem_address1 >> 12)].user_sup = 0;
    page_table[(video_mem_address1 >> 12)].write_through = 0;
    page_table[(video_mem_address1 >> 12)].cache_dis = 0;
    page_table[(video_mem_address1 >> 12)].accessed = 0;
    page_table[(video_mem_address1 >> 12)].dirty = 0;
    page_table[(video_mem_address1 >> 12)].page_attribute = 0;
    page_table[(video_mem_address1 >> 12)].global = 1;
    page_table[(video_mem_address1 >> 12)].available = 0;
    page_table[(video_mem_address1 >> 12)].address = (video_mem_address1 >> 12);

    page_table[(video_mem_address2 >> 12)].present = 1 ;
    page_table[(video_mem_address2 >> 12)].read_write = 1;
    page_table[(video_mem_address2 >> 12)].user_sup = 0;
    page_table[(video_mem_address2 >> 12)].write_through = 0;
    page_table[(video_mem_address2 >> 12)].cache_dis = 0;
    page_table[(video_mem_address2 >> 12)].accessed = 0;
    page_table[(video_mem_address2 >> 12)].dirty = 0;
    page_table[(video_mem_address2 >> 12)].page_attribute = 0;
    page_table[(video_mem_address2 >> 12)].global = 1;
    page_table[(video_mem_address2 >> 12)].available = 0;
    page_table[(video_mem_address2 >> 12)].address = (video_mem_address2 >> 12);

    page_table[(video_mem_address3 >> 12)].present = 1 ;
    page_table[(video_mem_address3 >> 12)].read_write = 1;
    page_table[(video_mem_address3 >> 12)].user_sup = 0;
    page_table[(video_mem_address3 >> 12)].write_through = 0;
    page_table[(video_mem_address3 >> 12)].cache_dis = 0;
    page_table[(video_mem_address3 >> 12)].accessed = 0;
    page_table[(video_mem_address3 >> 12)].dirty = 0;
    page_table[(video_mem_address3 >> 12)].page_attribute = 0;
    page_table[(video_mem_address3 >> 12)].global = 1;
    page_table[(video_mem_address3 >> 12)].available = 0;
    page_table[(video_mem_address3 >> 12)].address = (video_mem_address3 >> 12);

    // set kernel memory
    page_directory[1].entry_4M.present = 1;
    page_directory[1].entry_4M.read_write = 1;
    page_directory[1].entry_4M.user_sup = 0;
    page_directory[1].entry_4M.write_through = 0;
    page_directory[1].entry_4M.cache_dis = 0;
    page_directory[1].entry_4M.accessed = 0;
    page_directory[1].entry_4M.page_size = 1;
    page_directory[1].entry_4M.global = 1;
    page_directory[1].entry_4M.available = 0;
    page_directory[1].entry_4M.attribute_table = 0;
    page_directory[1].entry_4M.reserved0 = 0;
    page_directory[1].entry_4M.address = (kernel_mem_address >> 22);

    page_directory[FISH_VIDEO_MEMORY].entry_4k.present = 1;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.read_write = 1;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.user_sup = 1;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.write_through = 0;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.cache_dis = 0;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.accessed = 0;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.page_size = 0;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.global = 1;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.available = 0;
    // page_directory[33].entry_4k.attribute_table = 0;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.reserved0 = 0;
    page_directory[FISH_VIDEO_MEMORY].entry_4k.address = ((unsigned int)user_page_table >> 12);

    //set the second video memory
    user_page_table[0].present = 1 ;
    user_page_table[0].read_write = 1;
    user_page_table[0].user_sup = 1;
    user_page_table[0].write_through = 0;
    user_page_table[0].cache_dis = 0;
    user_page_table[0].accessed = 0;
    user_page_table[0].dirty = 0;
    user_page_table[0].page_attribute = 0;
    user_page_table[0].global = 1;
    user_page_table[0].available = 0;
    user_page_table[0].address = (video_mem_address0 >> 12);



    enable_paging(page_directory);              // load directory and enable paging
    printf("Paging initialized  ");
}

