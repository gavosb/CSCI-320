#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "pstat.h"
#include "random.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
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
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Added for Project 3
/*
 * sys_settickets
 * This system call assigns tickets to the calling process.
 * It only takes the number of tickets as a parameter.
 *
 */
uint64
sys_settickets(void)
{
  int number;
  struct proc *p = myproc();
  argint(0, &number);
  acquire(&p->lock);
  p->tickets = number;
  release(&p->lock);
  
  return 0;
}

// Added for Project 3
/*
 * This function uses a return parameter, so a pstat struct must be 
 * defined prior to calling.
 *
 * This call fills the pstat struct with information of ALL running processes.
 *
 */
uint64
sys_getpinfo(void)
{
  struct pstat *process_info;
  uint64 process_info_addr;
  argaddr(0, &process_info_addr);
  process_info = (struct pstat *) process_info_addr;
  return getpinfo(process_info);
}

uint64
sys_rand_init(void)
{
  int seed;
  argint(0, &seed);
  rand_init(seed);
  return 0;
}

uint64
sys_scaled_random(void)
{
  int low;
  int high;
  argint(0, &low);
  argint(1, &high);
  return scaled_random(low,high);
}
