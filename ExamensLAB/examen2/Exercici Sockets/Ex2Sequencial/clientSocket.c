#include <sys/resource.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>




main (int argc, char *argv[])
{
  int connectionFD;
  int ret, ret2;
  int r;
  char buff[1024];
  char buff2[80];
  char *FileName;
  char *hostname;
  int port;
  int i;
  int msec_elapsed;
  struct timeval init_t, end_t;

  gettimeofday(&init_t, NULL);

  if (argc != 4)
    {
      sprintf (buff, "Usage: %s hostname port file\n", argv[0]);
      write (2, buff, strlen (buff));
      exit (1);
    }

  hostname = argv[1];
  port = atoi (argv[2]);
  FileName = argv[3];

  int fdfitxer;

  fdfitxer = open(argv[3], O_RDWR, 0600);
  
  if (fdfitxer < 0) {
    perror("error al obrir fitxer");
    exit(1);
  }


  connectionFD = clientConnection (hostname, port);
  if (connectionFD < 0)
    {
      perror ("Error establishing connection\n");
      exit (1);
    }

    ret = write (connectionFD, argv[3],strlen(argv[3]));
    if (ret < 0)
    {
      perror ("Error writing to connection\n");
      exit (1);
    }
  
  while ( (ret2 = read(fdfitxer, buff, strlen(buff))) > 0) {
    ret = write(connectionFD, buff, strlen(buff)); 

    if (ret < 0)
    {
      perror ("Error writing to connection\n");
      exit (1);
    }

  } 	  

	  if (ret2 < 0)
	  {
		  perror ("Error reading on file\n");
		  exit (1);
	  }


  

  gettimeofday(&end_t, NULL);

  sprintf (buff2, "\nClient [%d] finishes\n", getpid());
  write(1,buff2,strlen(buff2));
  struct timeval res_t;
  timersub(&end_t, &init_t, &res_t);
  msec_elapsed = (res_t.tv_sec*1000)+(res_t.tv_usec/1000);
  sprintf(buff2, "Time %d msec\n",msec_elapsed);
  write(2,buff2,strlen(buff2));
  deleteSocket (connectionFD);

}
