#include "file_sys.h"
#include "systemcalls.h"
//#include "kernel.c"


/*
file_init(uint32_t file_start)
Description: initializes the file system through the file system pointer given in kernel.c
Inputs: UINT32_T file_start location, a UINT32 that shows there the start of filesys
Outputs: None
Side effects: initializes FS
 */
void file_init(uint32_t file_start){
    boot = (boot_block_t*) file_start;
}
/*
read_dentry_by_name
Description: reads and passes information to the given data entry
Inputs: filename and data entry
Outputs: 0 if success, -1 if failure
Side effects: Data entry value passed in will be modified
 */
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
    // uint32_t length = strlen((int8_t*)fname);
    // if(length > 32) {
    //     length = 32; 
    // }
    int i;
    for(i = 0; i < NUMBER_OF_FILES; i++){
        if(strncmp((int8_t*)(fname), (int8_t*)(boot->directory[i].filename), FILENAME_LEN) == 0){
            
            return read_dentry_by_index(i, dentry);
        }
    }
    return -1;
}
/*
read_dentry_by_index
Description: reads and passes information to the given data entry
Inputs: index and data entry
Outputs: 0 if success, -1 if failure
Side effects: Data entry value passed in will be modified
 */
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){

    // printf("%d\n", (boot->directory[index]).filetype);

    if(boot->directory[index].inode_num < 0 || boot->directory[index].inode_num > NUMBER_OF_FILES){            // fix this check
        return -1;
    }

    // printf(boot->directory[index].filename);

    // printf("%d",strlen((int8_t*)boot->directory[index].filename));
    // printf(boot->directory[index].filetype);

    memcpy((dentry),&(boot->directory[index]), sizeof(dentry_t));


    return 0;
}
/*
read data
Description: read length bytes into buffer at an offset from the file in a specific inode
Inputs: inode value, offset from start of file, buffer, length
Outputs: Number of bytes read
Side effects: buffer passed in will be filled
 */
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    if(inode > boot->inode_count){
        printf("inode invalid");
        return -1;
    }
// inode start is the pointer representation of the starting inode this corresponds to
    i_node_start = (inode_t*)(boot + inode + 1);
// length, offset and other things are unsigned int, I'm pretty sure they can't be negative
    if (length <= 0 || offset < 0 || offset > i_node_start->length) {
        
        printf("invalid bounds \n");
        return -1;
    }

    int i;
    // uint32_t count;
    // which number datablock we start from
    uint32_t dataBlockOffset = offset / 4096;
    // which byte of the starting datablock we start from
    uint32_t dataBlockStart = offset % 4096;


    uint32_t filelen = i_node_start->length;

    if(offset > filelen) {
        return 0; 
    }
    else if(offset + length > filelen) {
        length = filelen - offset; 
    }

    // printf("length of file: %d \n readlength: %d", filelen, length);

    uint32_t dataBlockIndex;
    // index of the starting datablock
    dataBlockIndex = i_node_start->data_block_num[dataBlockOffset];
    // printf("datablockIdx: %d", dataBlockIndex);
    // count = offset % sizeof(uint32_t);
    // uint32_t read; 
    // dataBlock is the pointer to the start of the datablock, start of returned data is dataBlock + dataBlockStart
    dataBlock = (uint8_t*)(boot + (boot->inode_count + 1 + dataBlockIndex));
// i effectively says how many bytes we have read
    for(i = 0; i < length;) {

        // printf(i);
        // This is a 
        if (4096 - dataBlockStart >= length - i) {
            memcpy((uint8_t*)buf + i, (uint8_t*)dataBlock + dataBlockStart, length - i);
            // printf("actual bytes read: %d", read); 
            i = length;
        }
        // else, data crosses datablocks, so we do a little funny 
        else {
            memcpy((uint8_t*)buf + i, (uint8_t*)dataBlock + dataBlockStart, 4096 - dataBlockStart);
            
            // update which datablock is accessed
            dataBlockIndex = i_node_start->data_block_num[++dataBlockOffset];
            dataBlock = (uint8_t*)(boot + (boot->inode_count + 1 + dataBlockIndex));

            i += (4096 - dataBlockStart);
            dataBlockStart = 0;
        }


    }

    // if(i == filelen){
    //     return 0;
    // }
    return i;
}

/*
read file
Description: reads the active file from the start
Inputs: file descriptor, number of bytes you want to read, buffer
Outputs: Number of bytes read
Side effects: buffer passed in will be filled
 */
int32_t read_file (int32_t fd, void* buf, int32_t nbytes) {
    pcb_t* pcb = current_pcb();
    int32_t bytes_read = read_data(pcb->file_array[fd].inode, pcb->file_array[fd].file_pos, buf, nbytes);
    return bytes_read;
}
/*
read file
Description: does nothing
Inputs:nothing
Outputs: -1
Side effects: NONE
 */
int32_t write_file (int32_t fd, const void* buf, int32_t nbytes){
    return -1;
}

/*
open file
Description: sets the desired filename as active
Inputs:nothing
Outputs: 0 on success
Side effects: NONE
 */
int32_t open_file (const uint8_t* filename) {
    // if(filename == NULL) {
    //     return -1; 
    // }
    // dentry_t data; 
    // read_dentry_by_name(filename, &data);
    // fd_node.inode = data.inode_num;
    // fd_node.file_position = 0;
    // fd_node.flags = 1; 
    return 0;
}
/*
open file
Description: sets the desired filename as inactive
Inputs:nothing
Outputs: 0 on success
Side effects: NONE
 */
int32_t close_file (int32_t fd) {
    // fd_node.inode = 0; 
    // fd_node.flags = 0; 
    return 0;
}
/*
write_dir
Description: nothing
Inputs:nothing
Outputs: -1
Side effects: NONE
 */
int32_t write_dir(int32_t fd, const void* buf, int32_t nbytes) {
    return -1;
}
/*
close_dir
Description: closes the directory
Inputs:nothing
Outputs: 0
Side effects: NONE
 */
int32_t close_dir(int32_t fd) {
    return 0;
}
/*
open directory
Description: sets the directory as active and makes the dir_index pointer 0
Inputs: filename
Outputs: -1 if null, 0 if success
Side effects: NONE
 */
int32_t open_dir(const uint8_t* filename) {
    if(filename == NULL) {
        return -1;
    }

    dir_index = 0; 
    return 0;
}
/*
read directory
Description: reads and prints the name and size of the file indexed by dir_index
Inputs: buffer
Outputs: -1 if fail, 0 if success
Side effects: NONE
 */
int32_t read_dir(int32_t fd, uint8_t* buf, int32_t nbytes) {
    // int strlength = strlen(boot->directory[dir_index].filename);
    // int i; 
    // copy into buffer
    strncpy((int8_t*)buf, (int8_t*)boot->directory[dir_index].filename, FILENAME_LEN); 
    // for(i = 0; i < FILENAME_LEN; i++) {
    //     // print file
    //     putc((uint8_t)buf[i]);
    // putc('\n');
    // }
    // inode_t* target_inode = (inode_t*)(boot + boot->directory[dir_index].inode_num + 1);
    // uint32_t size = target_inode->length; 
    // print byte size
    //printf(" number of bytes: %d \n", size);

    // printf("number of inodes %d ", boot->inode_count);
    dir_index++; 
    if(dir_index > boot->dir_count) {
        dir_index = 0;
        return 0; 
    }
    return FILENAME_LEN; 
    // dir_index = dir_index % boot->dir_count; 
}

