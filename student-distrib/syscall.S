.global sys_call

sys_call: 

pushfl

pushl %ebx
pushl %ecx
pushl %edx
pushl %esp
pushl %ebp
pushl %esi
pushl %edi

pushl %edx
pushl %ecx
pushl %ebx

cmpl $0, %eax
jbe invalid
cmpl $10, %eax
ja invalid 

sub $1, %eax

call *system_calls(, %eax, 4)

jmp end_syscall
invalid: 
movl $-1, %eax

end_syscall:

addl $12, %esp

popl %edi
popl %esi
popl %ebp
popl %esp
popl %edx
popl %ecx
popl %ebx
popfl
iret            # causes page fault


system_calls: 
.long halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn  

