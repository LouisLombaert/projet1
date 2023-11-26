#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "locker.h"

#define NUM_CYCLES 1000000

int cycle_counter = 0;
pthread_mutex_t cycle_counter_mutex = PTHREAD_MUTEX_INITIALIZER;
volatile int* forks; // Changed to volatile int

void* philosopher(void* arg) {
    int id = *((int*)arg);
    int left_fork = id;
    int right_fork = (id + 1) % *((int*)arg);

    while (1) {
        pthread_mutex_lock(&cycle_counter_mutex);
        if (cycle_counter >= NUM_CYCLES) {
            pthread_mutex_unlock(&cycle_counter_mutex);
            break;
        }
        pthread_mutex_unlock(&cycle_counter_mutex);

        // Grab left fork
        locker_lock(&forks[left_fork]);

        // Grab right fork
        locker_lock(&forks[right_fork]);

        // Release forks
        locker_unlock(&forks[left_fork]);
        locker_unlock(&forks[right_fork]);

        // Increment cycle counter
        pthread_mutex_lock(&cycle_counter_mutex);
        cycle_counter++;
        pthread_mutex_unlock(&cycle_counter_mutex);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_philosophers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_philosophers = atoi(argv[1]);

    if (num_philosophers < 1) {
        fprintf(stderr, "Number of philosophers must be greater than 1.\n");
        return EXIT_FAILURE;
    }

    pthread_t* philosophers = malloc(num_philosophers * sizeof(pthread_t));
    int* ids = malloc(num_philosophers * sizeof(int));
    forks = malloc(num_philosophers * sizeof(volatile int)); // Changed to volatile int

    // Initialize forks
    for (int i = 0; i < num_philosophers; ++i) {
        forks[i] = 0; // Initialize forks to 0
    }

    // Create philosopher threads
    for (int i = 0; i < num_philosophers; ++i) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &num_philosophers);
    }

    // Join philosopher threads
    for (int i = 0; i < num_philosophers; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    // Cleanup
    free(philosophers);
    free(ids);
    free((void*)forks);

    return 0;
}
