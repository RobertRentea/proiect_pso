#include "../threads/synch.h"
#include "threads/thread.h"


#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#define USER_VADDR_BOTTOM ((void *) 0x08048000)

#define ERROR -1

// struct child_proc{
//     tid_t pit;
//     struct semaphore parent_sema;
// };

void syscall_init (void);
struct thread* child_process(tid_t child_tid);

#endif /* userprog/syscall.h */
