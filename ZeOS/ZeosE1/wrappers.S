#include <asm.h>

ENTRY(write)

	pushl %ebp;
	movl %esp, %ebp;
	pushl %ebx;
	pushl %edi;
	movl 8(%ebp), %ebx;
	movl 12(%ebp), %ecx;
	movl 16(%ebp), %edx;

	movl $0x04, %eax;
	
	pushl %ecx;
	pushl %edx;
	
	leal errorchk, %edi;
	pushl %edi;		

	pushl %ebp;
	movl %esp, %ebp;
	
	sysenter;

errorchk:
	cmpl $0, %eax;
	jge tornada;
	neg %eax;
	movl %eax, errno;
	movl $-1, %eax; 	

tornada:	
	popl %ebp;
	popl %edi;
	popl %edx;
	popl %ecx;
	popl %edi;
	popl %ebx;
	popl %ebp;


	ret;


ENTRY(gettime)

	pushl %ebp;
	movl %esp, %ebp;

	pushl %edi;
	
	movl $0x0a, %eax;
	
	pushl %ecx;
	pushl %edx;
		

	leal errorchk2, %edi;
	pushl %edi;		

	pushl %ebp;
	movl %esp, %ebp;
	
	sysenter;

errorchk2:
	cmpl $0, %eax;
	jge tornada2;
	neg %eax;
	movl %eax, errno;
	movl $-1, %eax; 	



tornada2:
	popl %ebp;
	popl %edi;
	popl %edx;
	popl %ecx;
	popl %edi;
	popl %ebp;


	ret;


ENTRY(getpid)

	pushl %ebp;
	movl %esp, %ebp;

	pushl %edi;
	
	movl $0x14, %eax;

	pushl %ecx
	pushl %edx

	leal figetpid, %edi 

	pushl %edi

	push %ebp
	movl %esp, %ebp

	sysenter


figetpid:
	popl %ebp;
	popl %edi;
	popl %edx;
	popl %ecx;
	popl %edi;
	popl %ebp;
	
	ret

ENTRY(fork)
	pushl %ebp
	movl %esp, %ebp
	pushl %ecx
	pushl %edx
	pushl %edi
	leal tornada3, %edi //punto de retorno
	pushl %edi
	pushl %ebp //fake dynamic link
	movl %esp, %ebp
	movl $2, %eax //sys_number del fork
	sysenter
tornada3:
    popl %ebp
	addl $4, %esp
	popl %edi
	popl %edx
	popl %ecx
	popl %ebp
	ret


ENTRY(exit)

	pushl %ebp;
	movl %esp, %ebp;

	pushl %edi;
	
	movl $0x01, %eax;

	pushl %ecx
	pushl %edx

	leal fiexit, %edi 

	pushl %edi

	push %ebp
	movl %esp, %ebp

	sysenter


fiexit:
	popl %ebp;
	popl %edi;
	popl %edx;
	popl %ecx;
	popl %edi;
	popl %ebp;
	
	ret
