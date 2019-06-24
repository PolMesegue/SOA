#include <libc.h>


char buff[24];

int pid;
extern int zeos_ticks;
long inner (long n) 
{
	int i;
	long suma;
	suma = 0;
	for (i = 0; i<n; i++) suma = suma + i;
	return suma;
}

long outer (long n) 
{
	int i;
	long acum;
	acum = 0;
	for (i=0; i<n; i++) acum = acum + inner(i);
	return acum;
}

int add(int par1, int par2) {
	return  par1 + par2;

}


int __attribute__ ((__section__(".text.main")))
  main(void)
{

    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
	

    	if (-1 == (write (2, "aqui hauria de haver un error", strlen("aqui hauria de haver un error")))) perror();	
 	
	if (-1 == (write (1, "\nticks del rellotge: ", strlen("ticks del rellotge:  ")))) perror();	
	
char buff[16];	

int i = 0;
/*
int i2 = 0;	
	while(i2 < 5) {
		while(i < 90000) {
		itoa(gettime(), buff);		
		i++;
		}
	write(1, buff, strlen(buff));
	i2++;
	}*/
	int aux = 0;
	while(1) {

		itoa(getpid(), buff);
		write(1, buff, strlen(buff));
	//	aux = gettime();
/*
		while (i < 10000) {
		if (aux != gettime()) {
			itoa(gettime(), buff);
			write(1, buff, strlen(buff));
			aux = gettime();
		}
		i++;
		}
		*/
	}


	return 0;
}
