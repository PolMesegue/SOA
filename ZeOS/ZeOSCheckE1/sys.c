/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <errno.h>
extern int zeos_ticks;
char buff[256];

#define LECTURA 0
#define ESCRIPTURA 1

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}

int sys_gettime()
{
return zeos_ticks;
}

int sys_write(int fd, char *buffer, int size) {
	int error;
	if ((error = check_fd(fd, ESCRIPTURA))<0) return error;
	if (buffer == NULL) return -EFAULT;
	if (size < 0) return -EINVAL;
	
	int i = size;
	int ret;

	while(i > 256) {
		copy_from_user(buffer, buff, 256);
		ret = sys_write_console (buff, 256);
		buffer += 256;
		i -= ret;
	}
	
	if (i>0) {
		copy_from_user(buffer, buff, i);
		ret = sys_write_console(buff, i);
		i -= ret;
	}
	return size - i; 
	
}




















