
/*
 * lotteryTest.c
 * by Gavin Osborn 10/13/2023
 *
 * This user program tests lottery scheduling for an appropriate 3:2:1 ratio for 3 processes with 3, 2, 1 tickets respectively.
 * 
 * functions:
 * int main()
 * - initiates test_processes
 * int test_processes(int procnum)
 * - runs procnum of processes. Can't exceed number of free processes in process list.
 */

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/pstat.h"

int main() {
	struct pstat pinfo;
    getpinfo(&pinfo);
    for (int i = 0; i < NPROC; i++){
      if (pinfo.inuse[i]){
          printf("--PROCESS (PID): %d--\n", pinfo.pid[i]);
	      printf("ticks: %d\n", pinfo.ticks[i]);
          printf("tickets: %d\n", pinfo.tickets[i]);
      }
    }
    exit(0);
}
