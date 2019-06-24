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

int global_PID= 2033; //numero random

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

int ret_from_fork(){
  return 0;
}

int sys_fork()
{
  int PID=-1;

struct list_head * l = list_first(&freequeue);
list_del(l);

struct task_struct* child = list_head_to_task_struct(l);
copy_data((void *)current(), (void *)child, (int)sizeof(union task_union));

allocate_DIR(child);
page_table_entry * father_PT = get_PT(current()); //tabla de paginas del padre
page_table_entry * child_PT = get_PT(child); //tabla de paginas del hijo

int chckerr;
int i, j;

//mapeamos paginas de DATA+STACK
for (i=0; i<NUM_PAG_DATA; i++) {
    chckerr=alloc_frame();
    if (chckerr != -1) set_ss_pag(child_PT, PAG_LOG_INIT_DATA+i,chckerr);
    else {
      for (j=0; j<i; j++) {
        free_frame(get_frame(child_PT, PAG_LOG_INIT_DATA+j));
        del_ss_pag(child_PT, PAG_LOG_INIT_DATA+j);
      }
      list_add_tail(l, &freequeue);
      return -EAGAIN; 
    }
  }

//mapeamos las paginas del codigo dle hijo igual que las del padre
for (i=0;i<NUM_PAG_CODE;++i){
    set_ss_pag(child_PT,PAG_LOG_INIT_CODE + i, get_frame(father_PT, PAG_LOG_INIT_CODE+i));
} 
//inicializamos las paginas de kernel del hijo igual que las del padre
for (i=0;i<NUM_PAG_KERNEL;++i){
    set_ss_pag(child_PT,i, get_frame(father_PT, i));
}
set_cr3(get_DIR(current()));

//PCB DEL HIJO

	child->PID = ++global_PID;
	child->estat = ST_READY;

	//mapear el ebp del padre en el hijo
	int ebp_reg =((int) get_ebp() - (int)current()) + (int)(child);
	child -> kernel_esp = ebp_reg + sizeof(DWord);

	DWord temp_ebp_reg =*(DWord*)ebp_reg;

	//dejamos al hijo preparado para el context switch

	child->kernel_esp-=sizeof(DWord);
  	*(DWord*)(child->kernel_esp)=(DWord)&ret_from_fork;
  	child->kernel_esp-=sizeof(DWord);
  	*(DWord*)(child->kernel_esp) = temp_ebp_reg;


  	child->estat=ST_READY;
  	list_add_tail(&(child->list), &readyqueue);
  
  	PID = child->PID;
  
	return PID;
}

void sys_exit()
{  
 	int i;

 	page_table_entry *process_PT = get_PT(current());

  for (i=0; i<NUM_PAG_DATA; ++i) {
    free_frame(get_frame(process_PT, PAG_LOG_INIT_DATA+i));
    del_ss_pag(process_PT, PAG_LOG_INIT_DATA+i);
  }
  
  list_add_tail(&(current()->list), &freequeue);
  
  current()->PID=-1;
  
  
  sched_next_rr();


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




















