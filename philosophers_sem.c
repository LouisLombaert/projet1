#include "locker.h"
#include "sem.h"
#include <stdio.h>
#include <pthread.h>

#define NUM_CYCLES 1000000

int cycle_counter = 0;
locker_t* cycle_counter_lock;
sem_t** forks;
int num_philosophers;

void* philosopher(void* arg) {
    int id = *((int*)arg);
    int left_fork = id;
    int right_fork = (id + 1) % num_philosophers;

    while (1) {
        lock(cycle_counter_lock);
        if (cycle_counter >= NUM_CYCLES) {
            unlock(cycle_counter_lock);
            break;
        }
        unlock(cycle_counter_lock);

        // Grab left fork
        sem_wait(forks[left_fork]);

        // Grab right fork
        sem_wait(forks[right_fork]);

        // Release forks
        sem_post(forks[left_fork]);
        sem_post(forks[right_fork]);

        // Increment cycle counter
        lock(cycle_counter_lock);
        cycle_counter++;
        unlock(cycle_counter_lock);
    }

    return NULL;
}

int main(int argc, char const *argv[]) {
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
    forks = malloc(num_philosophers * sizeof(sem_t*));

    for (int i = 0; i < num_philosophers; ++i) {
        forks[i] = sem_init(1);
    }

    cycle_counter_lock = init_lock();

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
    for (int i = 0; i < num_philosophers; ++i) {
        sem_destroy(forks[i]);
    }

    free(philosophers);
    free(ids);
    free(forks);

    destroy_lock(cycle_counter_lock);

    return 0;
}
