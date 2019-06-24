/*
 * libc.c 
 */

#include <libc.h>
#include <errno.h>
#include <types.h>

int errno;


void perror(void) {
	switch(errno) {
		case 9 :
			write(1, "fd invalid men", strlen("fd invalid men"));
			break;
		case 13 : 
			write(1, "permís denegat calling the police", strlen("permís denegat calling the police"));
			break;
	}
}

void itoa(int a, char *b)
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}

int strlen(char *a)
{
  int i;
  
  i=0;
  
  while (a[i]!=0) i++;
  
  return i;
}

