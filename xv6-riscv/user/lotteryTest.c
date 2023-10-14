
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
#include "kernel/random.h"
#include "kernel/pstat.h"

// procnum can't exceed the total number of free process slots available
// or else we won't have accurate data in the process table
int test_processes(int procnum){
	int total_tickets = (procnum * (procnum+1))/2; // closed form for ticket count sum
	total_tickets = total_tickets * 10; // remove if not scaling
	int total_ticks = 0;
	int main_process = 0;
	printf("Running %d test processes: \n", procnum);
	for (int i = 0; i < procnum; i++){
		int pid = fork();
		if (pid == 0) { // child
		  settickets((procnum - i) * 10); // proc1 gets 30 tickets, 2 gets 20, 3 gets 10 etc
		  int looper = 0;
		  while (looper < (procnum - i) * 3000){
              looper += 1;
          }
		  exit(0);
		}
		if ((pid != 0) && i == procnum-1){
			main_process = 1;
		}
	}
	
	if (main_process == 1){
	    sleep(40); // wait some time for the processes to gather time slices
	    printf("* Printing out pstat info *\n");
	    printf("TOTAL TICKETS: %d\n", total_tickets);
		struct pstat pinfo;
        getpinfo(&pinfo);
        for (int i = procnum; i < NPROC; i++){
          if (pinfo.inuse[i]){
            total_ticks += pinfo.ticks[i];
          }
        }
        for (int i = procnum; i < NPROC; i++){
          if (pinfo.inuse[i]){
              printf("-- PROCESS (PID) %d--\n", pinfo.pid[i]);
	          printf("ticks: %d\n", pinfo.ticks[i]);
              printf("tickets: %d\n", pinfo.tickets[i]);
              printf("desired ratio: (%d / %d)\n", pinfo.tickets[i], total_tickets);
              printf("actual ratio: (%d / %d)\n", pinfo.ticks[i], total_ticks);
          }
        }
	}
	
	return 0;
}

int main() {
	printf("Running lotteryTest\n");
	if (test_processes(3)){
	  printf("success: three processes");
	}
    exit(0);
}
