
/*
 * lotteryTest.c
 * by Gavin Osborn 10/13/2023
 *
 * This user program tests lottery scheduling for an appropriate 3:2:1 ratio
 * for 3 processes with 30, 20, 10 tickets respectively.
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

/*
 * print_pinfo()
 * - Prints pstat struct, given number of processes
 *
 * Parameters:
 * - int procnum: number of processes
 *
 * Return:
 * - None
 *
 */
void print_pinfo(int procnum){

    int total_ticks = 0;
    int total_tickets = (procnum * (procnum+1))/2; // closed form for ticket count sum
	total_tickets = total_tickets * 10; // remove if not scaling
	struct pstat pinfo;
    getpinfo(&pinfo);
	
    printf("- pstat info - \n");
    printf("TOTAL TICKETS: %d\n", total_tickets);
    
    // get total time slices spent on processes
    // ISSUE: concurrency creates a race condition?
    // -> solution: make syscall that turns off interrupts?
    // Can we start at i = procnum, or does that assume no other runinng processes on system?
    for (int i = 0; i < NPROC; i++){
      if (pinfo.inuse[i]){
        total_ticks += pinfo.ticks[i];
      }
    }
    
    // print process info
    for (int i = 0; i < NPROC; i++){
      if (pinfo.inuse[i]){
          printf("-- PROCESS (PID) %d--\n", pinfo.pid[i]);
          printf("ticks: %d\n", pinfo.ticks[i]);
          printf("tickets: %d\n", pinfo.tickets[i]);
          printf("desired ratio: (%d / %d)\n", pinfo.tickets[i], total_tickets);
          printf("actual ratio: (%d / %d)\n", pinfo.ticks[i], total_ticks);
          printf("proportionality to tickets: (%d / %d)\n", pinfo.ticks[i], pinfo.tickets[i]);
      }
    }
}

/*
 * int test_processes()
 * - Tests procnum amount of processes, printing pstat struct info for debugging purposes at 100k intervals.
 *
 * Parameters:
 * - int procnum: number of processes
 *
 * Return:
 * - 0: Success, no errors
 *
 */
int test_processes(int procnum){
	int pid = -1;
	printf("Running %d test processes: \n", procnum);
	int i = 0;
	while (i < procnum && pid != 0){
		pid = fork();
		if (pid == 0){
		  settickets(((procnum - i)) * 10); // proc1 gets 30 tickets, 2 gets 20, 3 gets 10 etc
		  int looper = 0;
	      // loop every child process 1mil times
	      while (looper < 1000000){
              looper += 1;
              // every 100k loops print out pinfo
              // i = procnun; lowest ticket process. i.e., least frequently scheduled
              // note: this only gives us the pinfo from the process list of the current child, we need it from parent
              if (i == procnum-1 && looper % 100000 == 0){
                printf("\n*\n*\n*\n*\n*\n");
                printf("Printing out pstat info: %d\n", looper % 11111);
                print_pinfo(procnum);
              }
		    }
		}
		i++;
	}
    for (int i = 0; i < procnum; i++){
      wait(0);
    }
	return 0;
}

int main() {
    int num_of_procs = 3;
	printf("Running lotteryTest\n");
	if (test_processes(num_of_procs)){
	  printf("success: three processes");
	}
    
    exit(0);
}
