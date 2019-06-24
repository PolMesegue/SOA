#include <asm.h>
#include <segment.h>

ENTRY(add2)

pushl   %ebp
movl    %esp,%ebp

movl 8(%ebp), %ecx
movl 12(%ebp), %eax

addl %ecx, %eax

movl %ebp, %esp
popl %ebp
ret    

