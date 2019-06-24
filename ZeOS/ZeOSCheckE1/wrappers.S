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
