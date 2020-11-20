#include "../threads/synch.h"
#include "threads/thread.h"


#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

struct child_proc{
    tid_t pit;
    struct semaphore parent_sema;
};

void syscall_init (void);

#endif /* userprog/syscall.h */
