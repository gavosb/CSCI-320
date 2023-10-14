
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
	//int total_tickets = (procnum * (procnum+1))/2; // closed form for ticket count sum
	int main_process = 0;
	printf("running %d test processes\n", procnum);
	for (int i = 0; i < procnum; i++){
		int pid = fork();
		if (pid == 0) { // child
		  settickets(procnum - i); // proc1 gets 3 tickets, 2 gets 2, 3 gets 1 etc
		  sleep(5);
		}
		if ((pid != 0) && i == procnum-1){
		    printf("main process");
			main_process = 1;
		}
	}
	
	if (main_process == 1){
	    printf("printing out pstat info");
		struct pstat pinfo_arr[NPROC];
		struct pstat *pinfo;
		getpinfo(pinfo_arr);
		for (pinfo = pinfo_arr; pinfo < &pinfo_arr[NPROC]; pinfo++){ // any way to do this procnum rather than NPROC?
			printf("--PROCESS--");
			printf("ticks: %d\n", pinfo->ticks);
			printf("tickets: %d\n", pinfo->tickets);
			//printf("ratio: %d\n", ((pinfo->ticks)/(total_tickets)));
		}
	}
	
	return 0;
}

int main() {
	printf("Running lotteryTest\n");
	test_processes(3);
    exit(0);
}
