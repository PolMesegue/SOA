#include <libc.h>

char buff[24];

int pid;

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */
  
  
  set_sched_policy(0); //0=RR - 1=FCSF

  int i;
  int proc = 0;
  int PID; 

  char buff[16];
  //creem 2 fills, 3 processos en total
  for (i = 1; i < 3; i++) {
    PID = fork();
    if (PID == 0) {
      proc = i+1;
      i = 3;
    }
    
  }
  
  
  int j, k;

  int useless = 0;

  //CPU:

  for (j = 1; j < 32000; ++j) {
    for (k = 1; k < 2048; ++k) {

          if (j % 2 == 0 && k % 2 == 0) ++useless;
      }
  }
 
  char buff2[16];
  itoa(useless,buff2);

  write(0, buff2,sizeof(buff2)); //no escriu per pantalla pero necesari perque el compilador faci els bucles i no optimitzi
  

  //IO:
  useless = 0;
  int vector[1024];  
  for (j = 0; j < 1024; ++j) {
    vector[j] = j; 
    useless += vector[j]; 
    read(0,buff,10);
  }
  read(0,buff,500);


  itoa(useless,buff2);

  write(0, buff2,sizeof(buff2));


if (proc == 1) { //seleccionar process que vols veure



char bufferUSER[16];
char bufferSYSTEM[16];
char bufferBLOCKED[16];
char bufferREADY[16];




  struct stats p;
  get_stats(getpid(), &p);

  itoa(p.user_ticks, bufferUSER);
  write(1, bufferUSER, sizeof(bufferUSER));

  itoa(p.blocked_ticks, bufferBLOCKED);
  write(1, bufferBLOCKED, sizeof(bufferBLOCKED));

  itoa(p.ready_ticks, bufferREADY);
  write(1, bufferREADY, sizeof(bufferREADY));

  itoa(p.system_ticks, bufferSYSTEM);
  write(1, bufferSYSTEM, sizeof(bufferSYSTEM));

  

}

read(0,buff,1);


  while(1){};

}
