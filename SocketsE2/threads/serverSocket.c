#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>

void *doService(void *fd) {

int i = 0;
char buff[80];
char buff2[80];
int ret;
int *socket_fd = (int*) fd;
	

	ret = read(*socket_fd, buff, sizeof(buff));

	while(ret > 0) {
		buff[ret]='\0';

		sprintf(buff2, "Server [%d] received: %s\n", getpid(), buff);
		write(1, buff2, strlen(buff2));
			  sleep(2);

		ret = write(*socket_fd, "caracola ", 8);

		if (ret < 0) {
			perror ("Error writing to socket");
			exit(1);
		}
		ret = read(*socket_fd, buff, sizeof(buff));
		//write(1, "he llegit hola2 \n", sizeof("he llegit hola2 \n"));
	}
	if (ret < 0) {
			perror ("Error reading from socket");

	}
	sprintf(buff2, "Server [%d] ends service\n", getpid());
	write(1, buff2, strlen(buff2));

    pthread_exit(NULL);

}

doServiceFork(int fd) {

	pthread_t thread;
	
    int ret = pthread_create(&thread, NULL, doService, &fd);
    if(ret > 0) {
	exit(EXIT_FAILURE);
    }
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

	  doServiceFork(connectionFD);

  }

 

}
