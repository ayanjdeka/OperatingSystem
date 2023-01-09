
# refereanced https://wiki.osdev.org/Setting_Up_Paging
# enable paging: loads and enables paging
#            C3: address to directory
#           C0: enables paging
#           C4: enables 4M pages
#   input: none
#   Output: none
#   Effect: loads and enables paging
#

.global	 enable_paging

enable_paging:

pushl %ebp                   
movl %esp, %ebp              
movl 8(%esp), %eax           
movl  %eax, %cr3             
movl %cr4, %eax              
orl $0x00000010, %eax         
movl %eax, %cr4              
movl %cr0, %eax              
orl $0x80000000, %eax         
movl %eax, %cr0              
            
leave                   
ret              

