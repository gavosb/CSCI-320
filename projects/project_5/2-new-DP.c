#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t *forks;
Zem_t mutex;
int fork_bitmap = 0; // max 32 forks
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
  Zem_wait(&forks[left(p)]);
  Zem_wait(&forks[right(p)]);
  printf("\n both forks acquired: %d\n", p);

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
		// only get forks if both forks available
		
		Zem_wait(&mutex);
		int right_available = ((fork_bitmap & (1<<right(p)))!=0); // 1 available 0 unavailable
		int left_available = ((fork_bitmap & (1<<left(p)))!=0);
		if (right_available && left_available){
			fork_bitmap = fork_bitmap | (1<<right(p));
			fork_bitmap = fork_bitmap | (1<<left(p));
			printf("%d \n", fork_bitmap);
			get_forks(p);
			sleep(.02); // eat
		}else{
			//printf("couldnt get both forks");
		}
		Zem_post(&mutex);
    }
    
    return NULL;
    
}

// this algorithm will cause starvation
int main(int argc, char *argv[]) {
	Zem_init(&mutex, 1);
    if (argc < 2){ printf("provide the number of philosophers"); return -1;}
    n = atoi(argv[1]);
	if (n > 32) { printf("max philosophers 32"); return -1;}
    
    forks = (Zem_t *)malloc(n * sizeof(Zem_t));
    printf("\n Dining Philosophers Problem Algorithm 2 \n");
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

