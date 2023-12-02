
/*
 * The Dining Philosophers Problem
 *  
 *  There are n philosophers and n forks. They sit at a circular table, with the forks adjacent to each philosopher.
 *  Each philosopher eats and sleeps, but in order to eat, the two adjacent forks must be acquired.
 *  
 * Algorithm 2
 *  
 *  Both forks must be available at the same time, and then both forks are grabbed at the same time.
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
Zem_t mutex;
int fork_bitmap = 0; // max 32 forks, locked by mutex
int bitmap_updated_counter = 0; //tracks total accesses to bitmap, locked by mutex
int *eating_distribution; // locked by mutex, array of integers to track how many times a philosopher ate, slayyy
int n; // number of philosophers

// get the left fork's index
// works for both index to forks array and bit index for bitmap
int left(int p) {

  return p;
  
}

// get the right fork's index
// works for both index to forks array and bit index for bitmap
int right(int p) {

  return (p + 1) % n;
  
}

// acquire the forks semaphores
void get_forks(int p) {

  Zem_wait(&forks[right(p)]);
  Zem_wait(&forks[left(p)]);
  printf("\n both forks acquired: %d\n", p);

}

// post on the forks semaphores
void put_forks(int p) {

  Zem_post(&forks[right(p)]);
  Zem_post(&forks[left(p)]);
  printf("\n forks returned: %d\n", p);
  
}

// stole this off stack overflow, modified
// prints bits in an unsigned integer; tailored specifically for the philosopher bitmap
void printBits(unsigned int num)
{
   for(int bit=0;bit<n; bit++)
   {
      printf("%i ", num & 0x01);
      num = num >> 1;
   }
   printf("\n");
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
        
		// only get forks if both forks available
		
		Zem_wait(&mutex);
		int right_available = (fork_bitmap & (1<<right(p))) == 0; // if bit not set == false, this will be true (its available)
        int left_available = (fork_bitmap & (1<<left(p))) == 0;
        
		if (right_available && left_available){
			fork_bitmap = fork_bitmap | (1<<right(p));
			fork_bitmap = fork_bitmap | (1<<left(p));
			
            /* statistics v */
			bitmap_updated_counter += 2; // 1 eat = 2 accesses
			eating_distribution[right(p)] += 1;
			eating_distribution[left(p)] += 1;
            /* statistics ^ */
            
            /* debugging */
			printBits(fork_bitmap);
            /* debugging */
            
            Zem_post(&mutex); // no longer need bitmap, get forks
			get_forks(p);
            
			sleep(.02); // eat
			Zem_wait(&mutex);
			put_forks(p); // put forks back
			fork_bitmap = fork_bitmap ^ (1<<right(p));
			fork_bitmap = fork_bitmap ^ (1<<left(p));
            
            /* debugging */
			printf("bit should be unset: \n");
			printBits(fork_bitmap);
			/* debugging */
			
		}
		
		Zem_post(&mutex);
		sleep(.02); // think
		
    }
    
    return NULL;
    
}

/*
 * print_statistics
 * 
 * This function prints relevant statistics to aid in identifying starvation and deadlock.
 *
 */
void print_statistics(){
    printf("\n * RUN STATISTICS * \n");
    printf("fork bitmap accessed %d times for %d philosophers\n", bitmap_updated_counter, n);
    printf("\n eating distribution: \n");
    for (int i=0; i < n; i++){
        printf("%d ate %d times\n", i, eating_distribution[i]);
    }
    
}

/*
 * main
 *
 * Driver for The Dining Philosophers Problem
 * Initialized semaphores. Creates threads and sets them to be philosophers.
 * Waits for 20 seconds, then prints out the statistics of the algorithm's functioning.
 *
 */
int main(int argc, char *argv[]) {
	Zem_init(&mutex, 1);
    if (argc < 2){ printf("provide the number of philosophers"); return -1;}
    n = atoi(argv[1]);
	if (n > 32) { printf("max philosophers 32"); return -1;}
    
    forks = (Zem_t *)malloc(n * sizeof(Zem_t));
    eating_distribution = (int *)malloc(n * sizeof(int));
    printf("\n Dining Philosophers Problem Algorithm 2 \n");
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
    
    /*
    for (int i = 0; i < n; ++i) {
        pthread_join(threads[i], NULL);
    }
    */    

    sleep(20);
    print_statistics();

    return 0;
    
}

