#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>


doService(int fd) {
int i = 0;
char buff[80];
char buff2[80];
int ret, ret2;
int socket_fd = (int) fd;

int fdfitxer;

	ret = read(socket_fd, buff, strlen(buff));
	fdfitxer = open(buff,O_CREAT|O_TRUNC|O_WRONLY, 0600);
	if (fdfitxer < 0) {
    perror("error al obrir fitxer");
    exit(1);
 	}

	while(ret > 0) {
		
		ret = read(socket_fd, buff, strlen(buff));
		
		ret2 = write(fdfitxer, buff, strlen(buff));

		if (ret2 < 0) {
			perror ("error al escriure al fitxer");
			exit(1);
		}
	}
	if (ret < 0) {
			perror ("Error reading from socket");

	}
	sprintf(buff2, "Server [%d] ends service\n", getpid());
	write(1, buff2, strlen(buff2));

}


main (int argc, char *argv[])
{
  int socketFD;
  int connectionFD;
  char buffer[80];
  int ret;
  int port;


  if (argc != 2)
    {
      strcpy (buffer, "Usage: ServerSocket PortNumber\n");
      write (2, buffer, strlen (buffer));
      exit (1);
    }

  port = atoi(argv[1]);
  socketFD = createServerSocket (port);
  if (socketFD < 0)
    {
      perror ("Error creating socket\n");
      exit (1);
    }

  while (1) {
	  connectionFD = acceptNewConnections (socketFD);
	  if (connectionFD < 0)
	  {
		  perror ("Error establishing connection \n");
		  deleteSocket(socketFD);
		  exit (1);
	  }

	  doService(connectionFD);
  }

}
