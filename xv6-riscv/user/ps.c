
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

int main() {
	struct pstat pinfo_arr[NPROC];
    struct pstat *pinfo; //malloc an empty pstat?
    getpinfo(pinfo_arr);
    pinfo = pinfo_arr;
    printf("%d\n", &pinfo);
    /*
	for (pinfo = pinfo_arr; pinfo < &pinfo_arr[NPROC]; pinfo++){ // any way to do this procnum rather than NPROC?
		printf("--PROCESS: %d--", pinfo->pid);
		printf("ticks: %d\n", pinfo->ticks);
		printf("tickets: %d\n", pinfo->tickets);
	}
    */
    exit(0);
}
