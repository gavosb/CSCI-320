
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/random.h"
#include "kernel/pstat.h"

int test_processes(int procnum){
	//int total_tickets = (procnum * (procnum+1))/2; // closed form for ticket count sum
	int main_process = 0;
	
	for (int i = 0; i < procnum; i++){
		int pid = fork();
		
		if (pid == 0) { // child
		  settickets(procnum); // proc1 gets 1 ticket, 2 gets 2, 3 gets 3 etc
		  sleep(2);
		}
		//printf("i: %d, procnum: %d\n", i, procnum);
		if ((pid == 1) && i == procnum-1){
			main_process = 1;
		}
	}
	
	if (main_process == 1){
		struct pstat pinfo_arr[NPROC];
		struct pstat *pinfo;
		getpinfo(pinfo);
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
    if(test_processes(3)){
		printf("three process success");
	}
    exit(0);
}
