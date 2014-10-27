#include "types.h"
#include "x86.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
//Turn of the computer
int sys_halt(void){
  outw (0xB004, 0x0 | 0x2000);
  return 0;
}

//run the signal function
int sys_killsignal(int pid, int signum) {
    return killsignal(pid, signum);
}

int sys_signal(int signum,sighandler_t * handler){
  return signal(signum, handler);

}

int sys_getppid(void){
  return getppid();
}

//Processes || Copy elements from the kernel ptable to the user space
extern struct proc * getptable_proc(void);

int sys_getptable(void){
  int size;
  char *buf;
  char *s;
  struct proc *p = '\0';
  
  if (argint(0, &size) <0){
    return -1;
  }
  if (argptr(1, &buf,size) <0){
    return -1;
  }
  
  s = buf;
  p = getptable_proc();
  
  while(buf + size > s && p->state != UNUSED){
    *(int *)s = p->state;
    s+=4;
    *(int *)s = p -> pid;
    s+=4;
    *(int *)s = p->parent->pid;
    s+=4;
    memmove(s,p->name,16);
    s+=16;
    p++;
  } 
  return 0;
}


