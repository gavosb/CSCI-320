#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t *forks;
int n; // number of philosophers

int left(int p) {

  return p;
  
}

int right(int p) {

  return (p + 1) % n;
  
}

void get_forks(int p) {
  Zem_t *rightFork = &forks[right(p)];
  Zem_t *leftFork = &forks[left(p)];
  
  Mutex_lock(&rightFork->lock);
  if (rightFork->value > 0){
	  // can try leftFork now
	  Mutex_lock(&leftFork->lock);
	  if (leftFork->value > 0){
		  Zem_wait(&forks[left(p)]);
		  Zem_wait(&forks[right(p)]);
		  printf("\n both forks acquired: %d\n", p);
	  } else {
		  printf("\n could NOT acquire forks: %d\n", p);
	  }
  }
  
  Mutex_unlock(&rightFork->lock);
  Mutex_unlock(&leftFork->lock);
  
  
}

void put_forks(int p) {
  Zem_post(&forks[left(p)]);
  Zem_post(&forks[right(p)]);
  printf("\nforks returned: %d\n", p);
}

void *philosopher(void *arg) {
    long long int value = (long long int) arg;
    int p = (int) value; // philosopher's position
    printf("\n philosopher initialized \n");
    
    while (1) {
        sleep(.02); // think
		//only get forks if both forks available
		// we have to just mutex both forks
		Zem_t *rightFork = &forks[right(sp)];
		Zem_t *leftFork = &forks[left(p)];
		int forksAcquired = 0;
		Mutex_lock(&rightFork->lock);
		if (rightFork->value > 0){
			// can try leftFork now
			Mutex_lock(&leftFork->lock);
			if (leftFork->value > 0){
				printf("\n both forks acquired: %d\n", p);
				forksAcquired = 1;
				sleep(.02); // eat
			} else {
				printf("\n could NOT acquire forks: %d\n", p);
			}
			Mutex_unlock(&rightFork->lock);
			Mutex_unlock(&leftFork->lock);
		}else{
			Mutex_unlock(&rightFork->lock);
		}
    }
    
    return NULL;
    
}

// my idea here is that this is logically equivalent to just setting one to pick the opposite fork order
int main(int argc, char *argv[]) {
    if (argc < 2){ printf("provide the number of philosophers"); return -1;}
    n = atoi(argv[1]);
    
    forks = (Zem_t *)malloc(n * sizeof(Zem_t));
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

