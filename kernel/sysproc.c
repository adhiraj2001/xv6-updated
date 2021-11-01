#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  int n;
  if(argint(0, &n) < 0)
    return -1;
  
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  return p->pid;
}

uint64
sys_fork(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  return fork();
}

uint64
sys_wait(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  uint64 addr;
  if(argaddr(0, &addr) < 0)
    return -1;
  
  return wait(addr);
}

// Handler for waitx custom syscall
// All arguments are stored in registers (p->trapframe)
// Maximum amount of arguments that are supported in xv6 is 5
// We cannot directly use user space to get arguments
// Use utility function to get arguments as integers (argint) or addresses (argaddr) or strings (argstr)
uint64
sys_waitx(void)
{ 
  uint64 addr, addr1, addr2;
  uint64 rtime, wtime;

  struct proc* p = myproc();
  p->argc = 0;

  // argument addresses from function call syscall function declared in syscall.c

  if(argaddr(0, &addr) < 0)
    return -1;
  if(argaddr(1, &addr1) < 0) // addresses are from user virtual memory
    return -1;
  if(argaddr(2, &addr2) < 0)
    return -1;

  // get approproate page from the page table
  // and then write our content in that page table

  int ret = waitx(addr, &rtime, &wtime);

  if(copyout(p->pagetable, addr1, (char*) &rtime, sizeof(int)) < 0)
    return -1;
  if(copyout(p->pagetable, addr2, (char*) &wtime, sizeof(int)) < 0)
    return -1;


  return ret;
}

//Handler for trace cusom syscall
uint64
sys_trace(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  int mask;
  if(argint(0, &mask) < 0)
    return -1;

  p->mask = mask;

  return 0;
}

//Handler for trace cusom syscall
uint64
sys_set_priority(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  int new_priority, pid;
  if(argint(0, &new_priority) < 0)
    return -1;

  if(argint(1, &pid) < 0)
    return -1;

  return set_priority(new_priority, pid);
}

uint64
sys_sbrk(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;

  addr = p->sz;

  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;

  
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  int pid;

  if(argint(0, &pid) < 0)
    return -1;

  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  struct proc* p = myproc();
  p->argc = 0;

  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
