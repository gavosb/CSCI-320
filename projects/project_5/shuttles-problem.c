
/*
 * The Shuttle Problem
 * 
 * There is a shuttle that takes attendees from point A to point B.
 * The shuttle has a limit of attendees able to board at a given time.
 * The shuttle takes time to travel between stops.
 * The shuttle is a shared resource, and the attendees are threads using the shuttle.
 *
 * In this implementation, the shuttle is actually a family of individual resources that are shared. That is,
 * the shuttle is composed of a door, a starter, and an arrival notification.
 *
 * There are auxiliary shared resources such as the passenger count and the waiting counters for the bus stops.
 * The bus stops themselves, while semaphores, are not "really" shared resources. They are just a mechanism serving as a queue.
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

Zem_t start_shuttle;
Zem_t waiting_mutex_A;
Zem_t waiting_mutex_B;
Zem_t passengers_mutex;
Zem_t door_mutex;
Zem_t bus_stop_A;
Zem_t bus_stop_B;
Zem_t arrived_A;
Zem_t arrived_B;
int waiting_at_A = 0;
int waiting_at_B = 0;
int passengers = 0;
int shuttle_capacity = 30;
int door = 0; // 0 = open, 1 = closed
int total_attendees = 100;

/*
 * The attendee will wait at the bus stop, shuttle,
 * wait at the bus stop again then be immediately picked up, shuttle, and repeat forever.
 *
 *
 */
void *attendee(void *arg) {

    long long int value = (long long int) arg;
    int id = (int) value;

    while (1) {
        
       /* wait at A */
        Zem_wait(&waiting_mutex_A);
        waiting_at_A++;
        Zem_post(&waiting_mutex_A);
		Zem_wait(&bus_stop_A);
		
        Zem_wait(&passengers_mutex);
        Zem_wait(&door_mutex);
        Zem_wait(&waiting_mutex_A);
        if ((waiting_at_A == 0 || passengers == shuttle_capacity) && door == 0) { // if last passenger, tell shuttle to leave
            door = 1;
            Zem_post(&start_shuttle);
            printf("\n %d telling shuttle to leave\n", id);
        }
        Zem_post(&waiting_mutex_A);
        Zem_post(&door_mutex);
        Zem_post(&passengers_mutex);
        Zem_wait(&arrived_B); // wait to arrive at B
        
        /* wait at B */
        Zem_wait(&waiting_mutex_B);
        waiting_at_B++;
        Zem_post(&waiting_mutex_B);
		Zem_wait(&bus_stop_B);

        Zem_wait(&passengers_mutex);
        Zem_wait(&door_mutex);
        Zem_wait(&waiting_mutex_B);
        //passengers++;
        if ((waiting_at_A == 0 || passengers == shuttle_capacity) && door == 0) { // if last passenger, tell shuttle to leave
            // technically this is unnecessary, but we dont want to post more than once to this
            door = 1;            
            Zem_post(&start_shuttle);
        }
        Zem_post(&waiting_mutex_B);
        Zem_post(&door_mutex);
        Zem_post(&passengers_mutex);
        Zem_wait(&arrived_A); // wait to arrive at A
        
    }
    
    return NULL;
    
}

void *shuttle_driver(void *arg) {
    
    while (1) {
        Zem_wait(&waiting_mutex_A);
        Zem_wait(&waiting_mutex_B);
        Zem_wait(&passengers_mutex);
        printf("\n beginning loop... \n ");
        printf("\npassengers: %d, waitingA: %d, waitingB: %d\n", passengers, waiting_at_A, waiting_at_B);
        printf("\n...\n");
        Zem_post(&waiting_mutex_A);
        Zem_post(&waiting_mutex_B);
        Zem_post(&passengers_mutex);
        
        /* handle bus stop A */
        Zem_wait(&door_mutex);
        door = 0;
        Zem_post(&door_mutex);
        Zem_wait(&waiting_mutex_A);
        Zem_wait(&waiting_mutex_B); // causing it to just pick everyone up from B, then everyone from A, etc.
        Zem_wait(&passengers_mutex);
        if (waiting_at_A != 0){ // pick up passengers at A
            while (waiting_at_A != 0 && passengers < shuttle_capacity) {
                waiting_at_A--;
                Zem_post(&bus_stop_A); // wake up passengers
                passengers++;
            }
        } else {
            Zem_post(&start_shuttle); // just leave
        }
        printf("\n leaving for B with %d passengers \n", passengers);
        Zem_post(&passengers_mutex);
        Zem_post(&waiting_mutex_A);
        Zem_post(&waiting_mutex_B);
        

        Zem_wait(&start_shuttle);
        sleep(5); // travel to B
        Zem_wait(&waiting_mutex_B);
        Zem_wait(&passengers_mutex);
        // drop off passengers at B
        printf("\n arrived at B with %d passengers \n", passengers);
        passengers = 0;
        for (int i = 0; i < shuttle_capacity; i++) {
            Zem_post(&arrived_B); // wake up all passengers
        }
        Zem_post(&passengers_mutex);
        Zem_post(&waiting_mutex_B);
        /* finished with travel to B */

        //
        //
        //
        
        /* handle bus stop B */
        Zem_wait(&door_mutex);
        door = 0;
        Zem_post(&door_mutex);
        Zem_wait(&waiting_mutex_B);
        Zem_wait(&waiting_mutex_A); // ???
        Zem_wait(&passengers_mutex);
        if (waiting_at_B != 0){ // pick up passengers at A
            while (waiting_at_B != 0 && passengers < shuttle_capacity) {
                waiting_at_B--;                
                Zem_post(&bus_stop_B); // wake up passengers                
                passengers++;
            }
        } else {
            Zem_post(&start_shuttle); // just leave
        }
        printf("\n leaving for A with %d passengers \n", passengers);
        Zem_post(&passengers_mutex);
        Zem_post(&waiting_mutex_B);
        Zem_post(&waiting_mutex_A);
        

        Zem_wait(&start_shuttle);
        sleep(5); // travel to B
        Zem_wait(&waiting_mutex_A);
        Zem_wait(&passengers_mutex);
        // drop off passengers at B
        printf("\n arrived at A with %d passengers \n", passengers);
        passengers = 0;
        for (int i = 0; i < shuttle_capacity; i++) {
            Zem_post(&arrived_A); // wake up all passengers
        }
        Zem_post(&passengers_mutex);
        Zem_post(&waiting_mutex_A);
        /* finished with travel to B */
    
    }

    return NULL;
}


int main(int argc, char *argv[]) {

    printf("\n Shuttle Problem \n");
    
    /*
        So basically.
        Shuttle is a semaphore that launches at 0.
        When at 30, signals all waiting on the left/right condition to wake up.
        They then pile into the semaphore, then go to sleep again if not chosen.
    */
    Zem_init(&waiting_mutex_A, 1);
    Zem_init(&waiting_mutex_B, 1);
    Zem_init(&passengers_mutex, 1);
    Zem_init(&door_mutex, 1);
    Zem_init(&start_shuttle, 0);
    Zem_init(&bus_stop_A, 0);
    Zem_init(&bus_stop_B, 0);
    Zem_init(&arrived_A, 0);
    Zem_init(&arrived_B, 0);
    
    // spawn attendees & shuttle driver
    pthread_t c;
    long long int me = (long long int) -1;
    pthread_create(&c, NULL, shuttle_driver, (void *) me);
    for (int i = 0; i < total_attendees; ++i)
    {
        pthread_t c;
        long long int me = (long long int) i;
        pthread_create(&c, NULL, attendee, (void *)me);
    }
    
    while (1) {
        sleep(1000);
    }

    return 0;
    
}

