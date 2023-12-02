
/*
 * The Dining Philosophers Problem
 *  
 *  There are n philosophers and n forks. They sit at a circular table, with the forks adjacent to each philosopher.
 *  Each philosopher eats and sleeps, but in order to eat, the two adjacent forks must be acquired.
 *  
 * Algorithm 3
 *  
 *  There is a table, and all eating must be done at the table, and no thinking may take place at the table.
 *  The table seats n-1 philosophers. There are n forks on the table.
 *  
 * By Gavin Osborn, 12/02/23
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t *forks;
Zem_t table;
int n; // number of philosophers

// get the left fork's index
int left(int p) {

  return p;
  
}

// get the right fork's index
int right(int p) {

  return (p + 1) % n;
  
}

// acquire the forks semaphores
void get_forks(int p) {
  Zem_wait(&forks[right(p)]);
  Zem_wait(&forks[left(p)]);
}

// post on the forks semaphores
void put_forks(int p) {
  Zem_post(&forks[right(p)]);
  Zem_post(&forks[left(p)]);
  printf("\nforks returned: %d\n", p);
}

/*
 * philosopher
 * 
 * This philosopher thinks, and eats. Represented as sleeps.
 * In order to eat, the philosopher must grab both fork semaphores adjacent to it, given by the philosopher's index.
 *
*/
void *philosopher(void *arg) {
    long long int value = (long long int) arg;
    int p = (int) value; // philosopher's position
    printf("\n philosopher initialized \n");
    
    while (1) {
        Zem_wait(&table); //wait on table, implicitly "thinking"
		get_forks(p);
		sleep(1); // eat
		put_forks(p);
		Zem_post(&table);// post on table
		sleep(1); // think, kind of redundant since waiting could be thinking, but this is independent from table
    }
    
    return NULL;
    
}

/*
 * main
 *
 * Driver for The Dining Philosophers Problem
 * Initialized semaphores. Creates threads and sets them to be philosophers.
 * Waits (indefinitely) for the philosophers to finish their processing.
 *
 */
int main(int argc, char *argv[]) {
    if (argc < 2){ printf("provide the number of philosophers"); return -1;}
    n = atoi(argv[1]);
    
    forks = (Zem_t *)malloc(n * sizeof(Zem_t));
	Zem_init(&table, n-1);
    printf("\n Dining Philosophers Problem \n");
    for (int i = 0; i < n; ++i){
      Zem_init(&forks[i], 1);
    }
    
    for (int i = 0; i < n; ++i)
    {
        pthread_t c;
        long long int me = i;
        pthread_create(&c, NULL, philosopher, (void *)me);
    }

    while (1){ // too lazy to make it wait on child threads
      sleep(1000);
    }

    return 0;
    
}

