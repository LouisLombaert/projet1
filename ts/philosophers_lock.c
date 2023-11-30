#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../locker.h"

#define NUM_CYCLES 1000000

int cycle_counter = 0;
locker_t cycle_counter_lock;
locker_t** forks;
int num_philosophers;

void* philosophers(void* arg) {
    int id = *((int*)arg);
    int left_fork = id;
    int right_fork = (id + 1) % num_philosophers;

    while (1) {
        lock(&cycle_counter_lock);
        if (cycle_counter >= NUM_CYCLES) {
            unlock(&cycle_counter_lock);
            break;
        }
        unlock(&cycle_counter_lock);

        // Grab left fork and right fork
        lock(forks[left_fork]);
        lock(forks[right_fork]);

        // Release forks
        unlock(forks[left_fork]);
        unlock(forks[right_fork]);

        // Increment cycle counter
        lock(&cycle_counter_lock);
        cycle_counter++;
        unlock(&cycle_counter_lock);
    }

    return NULL;
}

void *philosopher(void *arg) {

    int *id = (int *) arg;
    int left_stick = *id;
    int right_stick = (*id + 1) % num_philosophers;


    for (int i = 0; i < NUM_CYCLES; ++i) {

        if (left_stick < right_stick) {
            lock(forks[left_stick]);
            lock(forks[right_stick]);
        } else {
            lock(forks[right_stick]);
            lock(forks[left_stick]);
        }

        unlock(forks[left_stick]);
        unlock(forks[right_stick]);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_philosophers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    num_philosophers = atoi(argv[1]);

    if (num_philosophers <= 1) {
        fprintf(stderr, "Number of philosophers must be greater than 1.\n");
        return EXIT_FAILURE;
    }

    pthread_t* philosophers = malloc(num_philosophers * sizeof(pthread_t));
    int* ids = malloc(num_philosophers * sizeof(int));
    forks = malloc(num_philosophers * sizeof(locker_t*));

    // Initialize forks
    for (int i = 0; i < num_philosophers; ++i) {
        forks[i] = init_lock();
    }

    locker_t* cycle_counter_lock = init_lock();

    // Create philosopher threads
    for (int i = 0; i < num_philosophers; ++i) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Join philosopher threads
    for (int i = 0; i < num_philosophers; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    // Cleanup
    free(philosophers);
    free(ids);

    for (int i = 0; i < num_philosophers; ++i) {
        destroy_lock(forks[i]);
    }
    
    destroy_lock(cycle_counter_lock);

    return 0;
}
