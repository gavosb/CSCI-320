#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"
pthread_cond_t  dir_right; // wait to go right
pthread_cond_t  dir_left; // wait to go left
Zem_t shuttle;

void *attendee(void *arg) {
    long long int value = (long long int) arg;
    int dir = (int) value; // 0 dest 1 origin
    printf("\n %d attendee initialized \n", dir);
    
    while (1) {
		Zem_wait(&direction);
		
        Zem_wait(&shuttle); //wait on table
		get_forks(p);
		sleep(1); // eat
		put_forks(p);
		Zem_post(&table);// post on table
		sleep(1); // think
    }
    
    return NULL;
    
}

// my idea here is that this is logically equivalent to just setting one to pick the opposite fork order
int main(int argc, char *argv[]) {
    if (argc < 2){ printf("provide the number of philosophers"); return -1;}
    n = atoi(argv[1]);
    
    forks = (Zem_t *)malloc(n * sizeof(Zem_t));
	Zem_init(&shuttle, 30);
	Cond_init(dir_right);
	Cond_init(dir_left);
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

