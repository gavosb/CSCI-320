
/*
 * lotteryTest.c
 * by Gavin Osborn 10/20/2023
 *
 * This user program tests lottery scheduling for an appropriate ratio.
 * A 3:2:1 ratio would be desired for three processes with 30, 20, and 10 tickets respectively.
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
	int total_tickets = 0;
	/* only works while we still have all the initial processes (left for broken dreams):
    int total_tickets = (procnum * (procnum+1))/2; // closed form for ticket count sum
	total_tickets = total_tickets * 10; // scaling
	*/
	struct pstat pinfo;
    getpinfo(&pinfo);
	
    printf("- pstat info - \n");
    printf("TOTAL TICKETS: %d\n", total_tickets);
    
    // get total time slices spent on processes and count total tickets
    for (int i = 0; i < NPROC; i++){
      if (pinfo.inuse[i]){
        total_ticks += pinfo.ticks[i];
		total_tickets += pinfo.tickets[i]; 
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
          printf("proportionality to tickets: (desired / actual)\n");
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
              double calculation = 0.0; // spin CPU cycles
              for (int k = 0; k < 20000; k++){
                calculation = calculation * calculation / 2 + 2;
              }
              
              // every 100k loops print out pinfo
              // i = procnun; lowest ticket process. i.e., least frequently scheduled
              if (i == procnum-1 && looper % 100000 == 0){
                printf("\n*\n*\n*\n*\n*\n");
                printf("Printing out pstat info: %d\n", looper % 11111);
                print_pinfo(procnum);
              }
              
		    }
		}
		i++;
	}
	
	// main process waits for children
    for (int i = 0; i < procnum; i++){
      wait(0);
    }
	return 0;
}

int main() {
    int num_of_procs = 6;
	printf("Running lotteryTest\n");
	if (test_processes(num_of_procs)){
	  printf("success: three processes");
	}
    
    exit(0);
}
