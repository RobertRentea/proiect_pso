#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "process.h"

#define MAX_ARGS 3

static void syscall_handler (struct intr_frame *);
static void syscall_handler (struct intr_frame *);
void validate_ptr (const void* vaddr);
void validate_str (const void* str);
void validate_buffer (const void* buf, unsigned byte_size);
void get_args (struct intr_frame *f, int *arg, int num_of_args);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  int syscall_no = ((int*)f->esp)[0];
	int fd, no;
	char *buf;
	int waited_tid;
	int *status, th_status, i;
  int args[MAX_ARGS];


	switch (syscall_no) {
    case SYS_HALT:
      break;
    case SYS_EXIT:{

      f->eax = syscall_exit();
      break;
    }
    case SYS_EXEC:{

      char *filename = ((char*)f->esp)[1];

      validate_ptr((const void*)filename);

      f->eax = syscall_exec(filename);

      break;
    }
    case SYS_WAIT:{

      tid_t pid = ((tid_t*)f->esp)[1];

      validate_ptr((const void*)pid);

      f->eax = syscall_wait(pid);

      break;
    }
    case SYS_CREATE:
      break;
    case SYS_REMOVE:
      break;
    case SYS_OPEN:
      break;
    case SYS_FILESIZE:
      break;
    case SYS_READ:
      break;
    case SYS_WRITE:
      fd = ((int*)f->esp)[1];
      buf = (char*)((int*)f->esp)[2];
      no = ((int*)f->esp)[3];
      break;
    case SYS_SEEK:
      break;
    case SYS_TELL:
      break;
    case SYS_CLOSE:
      break;
  }
  thread_exit ();
}


tid_t syscall_exec(const char* filename){
  tid_t child = process_execute(filename);

  struct thread *child_proc = child_process(child);
  if (child_proc == NULL){
    return ERROR;
  }

  struct thread *current = thread_current();

  sema_down(&current->sema);

  if (child_proc->load_status == LOAD_FAIL){
    return ERROR;
  }
  return child;
}


int syscall_wait(tid_t pid){
  thread_current()->wait = WAITING;
  process_wait(pid);
}

int syscall_exit(){
  int status = thread_current()->status;
  process_exit();
  return status;
}

void remove_child(struct thread *child){
  list_remove(&child->parent_elem);
}


void
get_args (struct intr_frame *f, int *args, int num_of_args)
{
  int i;
  int *ptr;
  for (i = 0; i < num_of_args; i++)
  {
    ptr = (int *) f->esp + i + 1;
    validate_ptr((const void *) ptr);
    args[i] = *ptr;
  }
}

void
validate_ptr (const void *vaddr)
{
    if (vaddr < USER_VADDR_BOTTOM || !is_user_vaddr(vaddr))
    {
      // virtual memory address is not reserved for us (out of bound)
      //syscall_exit(ERROR);
	  return -1;
    }
}

void
validate_str (const void* str)
{
    for (; * (char *) getpage_ptr(str) != 0; str = (char *) str + 1);
}

void
validate_buffer(const void* buf, unsigned byte_size)
{
  unsigned i = 0;
  char* local_buffer = (char *)buf;
  for (; i < byte_size; i++)
  {
    validate_ptr((const void*)local_buffer);
    local_buffer++;
  }
}

int
getpage_ptr(const void *vaddr)
{
  void *ptr = pagedir_get_page(thread_current()->pagedir, vaddr);
  if (!ptr)
  {
    //syscall_exit(ERROR);
	return -1;
  }
  return (int)ptr;
}

struct thread* child_process(tid_t child_tid){
  struct thread *cTh = thread_current();
  struct list_elem *t;

  for (t = list_begin(&cTh->child_list); t != list_end(&cTh->child_list); t = list_next(t)){
    struct thread *child = list_entry(t, struct thread, elem);
    if (child->tid = child_tid){
      return child;
    }
  }
  return NULL;
}


