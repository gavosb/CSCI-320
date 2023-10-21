
/*
 * lotteryTest.c
 * by Gavin Osborn 10/13/2023
 *
 * This user program prints out data on all processes without UNUSED state.
 * 
 * functions:
 * int main()
 * - outputs process information to system out
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
