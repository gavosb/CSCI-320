#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t shuttle;
Zem_t waiting_mutex_A;
Zem_t waiting_mutex_B;
Zem_t passengers_mutex;
Zem_t bus_stop_A;
Zem_t bus_stop_B;
Zem_t arrived_A;
Zem_t arrived_B;
int waiting_at_A = 0;
int waiting_at_B = 0;
int passengers = 0;
int shuttle_capacity = 30;

/*
 * The attendee will wait at the bus stop, shuttle, then sleep for awhile, and wait at the bus stop again, forever.
 *
 *
 */
void *attendee(void *arg) {
    printf("\n attendee initialized \n");
    
    while (1) {
        
        /* wait at stop A */
        Zem_wait(&waiting_mutex_A);
        waiting_at_A++;
        Zem_post(&waiting_mutex_A);
		Zem_wait(&bus_stop_A);
        Zem_post(&shuttle);
        Zem_wait(&arrived_B); // wait to arrive at B
        
        /* wait at stop B */
        
        Zem_wait(&waiting_mutex_B);
        waiting_at_B++;
        Zem_post(&waiting_mutex_B);
		Zem_wait(&bus_stop_B);
        Zem_post(&shuttle);
        Zem_wait(&arrived_A); // wait to arrive at B
        
    }
    
    return NULL;
    
}

void *shuttle_driver(void *arg) {
    long long int capacity_long = (long long int) arg;
    int capacity = (int) capacity_long;
    
    while (1) {
        

        // shuttle signals people to get on at bus stop (post to stop)
        // people wait on bus as this happens
        // as people get on, check if bus full & if anyone else waiting, if so, post to start_bus
        // bus sleeps
        // people wait on arrived_A
        // people get off and wait on bus stop
        // repeat steps for B on both shuttle and attendee
        

        /* handle bus stop A */
        Zem_wait(&waiting_mutex_A);
        Zem_wait(&passengers_mutex);
        if (waiting_at_A != 0){ // pick up passengers at A
            while (waiting_at_A != 0 && passengers < capacity) { // passengers < capacity || waiting_at_A == 0
                Zem_post(&bus_stop_A);
                waiting_at_A--;
                passengers++;
            }
            
        }//else{
        //    Zem_post(&shuttle); // shuttle should be 0 here, make sure
        //}
        Zem_post(&shuttle); // set shuttle to 30+1?
        
        printf("\n leaving for A with %d passengers \n", passengers);
        Zem_post(&passengers_mutex);
        Zem_post(&waiting_mutex_A);
        Zem_wait(&shuttle); // leave
        sleep(2); // travel to B
        Zem_wait(&waiting_mutex_B);
        Zem_wait(&passengers_mutex);
        waiting_at_B += passengers; // drop off passengers at B
        passengers = 0;
        for (int i = 0; i < shuttle_capacity; i++) {
            Zem_post(&arrived_B); // wake up all passengers
        }
        printf("\n arrived at A with %d passengers \n", passengers);
        Zem_post(&passengers_mutex);
        Zem_post(&waiting_mutex_B);
        /* finished with travel to B */
        
        /* handle bus stop B */

        Zem_wait(&waiting_mutex_B);
        Zem_wait(&passengers_mutex);
        if (waiting_at_B != 0){ // pick up passengers at A
            while (waiting_at_B != 0 && passengers < capacity) {
                Zem_post(&bus_stop_B);
                waiting_at_B--;
                passengers++;
            }
            
        }else{
            Zem_post(&shuttle); // shuttle should be 0 here, make sure
        }
        printf("\n leaving for B with %d passengers \n", passengers);
        Zem_post(&passengers_mutex);
        Zem_post(&waiting_mutex_B);
        Zem_wait(&shuttle); // leave
        sleep(2); // travel to B
        Zem_wait(&waiting_mutex_A);
        Zem_wait(&passengers_mutex);
        waiting_at_A += passengers; // drop off passengers at A
        passengers = 0;
        for (int i = 0; i < shuttle_capacity; i++) {
            Zem_post(&arrived_A); // wake up all passengers
        }
        printf("\n arrived at B with %d passengers \n", passengers);
        Zem_post(&passengers_mutex);
        Zem_post(&waiting_mutex_A);
        
    }    
    
    return NULL;
}


int main(int argc, char *argv[]) {

    printf("\n Shuttle Problem \n");
    int total_attendees = 100;
    
    /*
        So basically.
        Shuttle is a semaphore that launches at 0.
        When at 30, signals all waiting on the left/right condition to wake up.
        They then pile into the semaphore, then go to sleep again if not chosen.
    */
    Zem_init(&shuttle, -shuttle_capacity);
    Zem_init(&waiting_mutex_A, 1);
    Zem_init(&waiting_mutex_B, 1);
    Zem_init(&passengers_mutex, 1);
    Zem_init(&bus_stop_A, 0);
    Zem_init(&bus_stop_B, 0);
    Zem_init(&arrived_A, 0);
    Zem_init(&arrived_B, 0);
    
    // spawn attendees & shuttle driver
    pthread_t c;
    long long int capacity = (long long int) shuttle_capacity;
    pthread_create(&c, NULL, shuttle_driver, (void *)capacity);
    for (int i = 0; i < total_attendees; ++i)
    {
        pthread_t c;
        long long int me = (long long int) me;
        pthread_create(&c, NULL, attendee, (void *)me);
    }
    
    while (1) {
        sleep(1000);
    }

    return 0;
    
}

