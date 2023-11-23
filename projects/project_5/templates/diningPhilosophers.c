#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t forks[5];

int left(int p) {

  return p;
  
}

int right(int p) {

  return (p + 1) % 5;
  
}

void get_forks(int p) {
  printf("getting fork");
  if (p == 4) {
  
    Zem_wait(&forks[right(p)]);
    Zem_wait(&forks[left(p)]);
    
  } else {
  
    Zem_wait(&forks[left(p)]);
    Zem_wait(&forks[right(p)]);
    
  }
  printf("fork acquired");
  
}

void put_forks(int p) {
  printf("putting fork");
  Zem_post(&forks[left(p)]);
  Zem_post(&forks[right(p)]);
  printf("fork returned");
}

void *philosopher(void *arg) {

    long long int value = (long long int) arg;
    int p = (int) value; // philosopher's position
    printf("\n philosopher initialized \n");
    
    while (1) {
        sleep(2); // think
        get_forks(p);
        sleep(2); // eat
        put_forks(p);
    }
    
    return NULL;
    
}

int main(int argc, char *argv[]) {

    printf("\n Dining Philosophers Problem \n");
    for (int i = 0; i < 5; ++i){
      Zem_init(&forks[i], 1);
    }
    
    for (int i = 0; i < 5; ++i)
    {
        pthread_t c;
        long long int me = i;
        Pthread_create(&c, NULL, philosopher, (void *)me);
    }

    while (1){ // too lazy to make it wait on child threads
      sleep(1000);
    }

    return 0;
    
}

