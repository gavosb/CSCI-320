
/*
 * The Dining Philosophers Problem
 *  
 *  There are n philosophers and n forks. They sit at a circular table, with the forks adjacent to each philosopher.
 *  Each philosopher eats and sleeps, but in order to eat, the two adjacent forks must be acquired.
 *  
 * Algorithm 1
 *  
 *  Each philosopher acquires their left fork, and then acquires their right fork.
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
int n; // number of philosophers

// get the left fork's index
int left(int p) {

  return p;
  
}

// get the right fork's index
int right(int p) {

  return (p + 1) % 5;
  
}

// acquire the forks semaphores
void get_forks(int p) {
  Zem_wait(&forks[left(p)]);
  Zem_wait(&forks[right(p)]);
  printf("\n both forks acquired: %d\n", p);
  
}

// post on the forks semaphores
void put_forks(int p) {
  Zem_post(&forks[left(p)]);
  Zem_post(&forks[right(p)]);
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
        sleep(.02); // think
        get_forks(p);
        sleep(.02); // eat
        put_forks(p);
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
	if (n > 32) { printf("max philosophers 32"); return -1;}
    
    forks = (Zem_t *)malloc(n * sizeof(Zem_t));
    
    printf("\n Dining Philosophers Problem Algorithm 1 \n");
    
    for (int i = 0; i < n; ++i){
      Zem_init(&forks[i], 1);
    }
    
    pthread_t threads[n];
    
    for (int i = 0; i < n; ++i)
    {
        //pthread_t c;
        long long int me = i;
        pthread_create(&threads[i], NULL, philosopher, (void *)me);
    }

    for (int i = 0; i < n; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
    
}

