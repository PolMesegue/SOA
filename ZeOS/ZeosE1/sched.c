/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

#if 1
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;

struct list_head freequeue;
struct list_head readyqueue;
struct task_struct *idle_task;
struct task_struct *init_task;

unsigned int ticks_restants;

/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{
	struct list_head *l = list_first(&freequeue);
	list_del(l);
	idle_task = list_head_to_task_struct(l);
	idle_task -> PID = 0;
	allocate_DIR(idle_task);
	idle_task -> kernel_esp = ((unsigned int *)idle_task)+KERNEL_STACK_SIZE-1;
	(*(idle_task -> kernel_esp)) = (unsigned int)&cpu_idle;
	idle_task -> kernel_esp = (idle_task -> kernel_esp)-1;
	(*(idle_task -> kernel_esp)) = 0;
}

void init_task1(void)
{
	struct list_head *l = list_first(&freequeue);
	list_del(l);
	init_task = list_head_to_task_struct(l);
	init_task -> PID = 1;
	allocate_DIR(init_task);
	set_user_pages(init_task);
	tss.esp0 = (DWord)(((unsigned int *)init_task)+KERNEL_STACK_SIZE);
	writeMsr(0x175, (int)tss.esp0);
	set_cr3(init_task->dir_pages_baseAddr);
}


void init_sched(){

	

}

void init_freequeue() {
	INIT_LIST_HEAD(&freequeue);
	int i;
	for (i = 0; i < NR_TASKS; i++) {
		task[i].task.PID=-1;
		list_add_tail(&(task[i].task.list), &freequeue);
	}
}

void update_sched_data_rr (void){
	
	--ticks_restants;
}

int needs_sched_rr (void){
	if (ticks_restants == 0) return 1;
	else return 0;
}

void update_process_state_rr (struct task_struct * t, struct list_head * dst_queue){
    
    if (t->estat!=ST_RUN) list_del(&(t->list));
    if (dst_queue!=NULL) list_add_tail(&(t->list), dst_queue);

	if (dst_queue==&readyqueue){
		t->estat=ST_READY;
	}else if (dst_queue==NULL){
		t->estat=ST_RUN;
	}else{
		t->estat=ST_BLOCKED; 
	}
	

}

void sched_next_rr (void){

	struct task_struct * next;
	if (!list_empty(&readyqueue)) {
		struct list_head * h = list_first(&readyqueue); 
		next = list_head_to_task_struct(h); 
	}
	else {
		next = idle_task;
		task_switch((union task_union *)next);
		return;
	}
	update_process_state_rr(next, NULL); 
	
	ticks_restants = get_quantum(next);
	task_switch((union task_union *)next);
	
}

void schedule() {
  update_sched_data_rr();
  if (needs_sched_rr())
  {
    update_process_state_rr(current(), &readyqueue);
    sched_next_rr();
  }
}


int get_quantum (struct task_struct *t){
	return t->quantum;
}
void set_quantum(struct task_struct *t, int new_quantum){
	t->quantum = new_quantum;
}

void init_readyqueue() {
	INIT_LIST_HEAD(&readyqueue);
}

void task_switch(union task_union *t);


struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

