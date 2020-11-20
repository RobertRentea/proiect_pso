#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  int syscall_no = ((int*)f->esp)[0];

  switch (syscall_no)
  {
  case SYS_EXEC:
  {

    char* executable = ((char*)f->esp)[1];
    
    f->eax = syscall_exec(executable);

    break;
  }
  
  default:
    break;
  }
  thread_exit ();
}

syscall_exec(const char* filename){
  tid_t child = process_execute(filename);

  struct thread *current = thread_current();
  
  

  sema_down(&current->sema);
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

