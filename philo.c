#include <stdio.h>
#include <pthread.h>

#define NUM_PHILOSOPHERS 500
#define NUM_CYCLES 1000000

pthread_mutex_t forks[NUM_PHILOSOPHERS];
int cycle_counter = 0;
pthread_mutex_t cycle_counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *philosopher(void *arg) {
    int id = *((int *)arg);
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    while (1) {
        pthread_mutex_lock(&cycle_counter_mutex);
        if (cycle_counter >= NUM_CYCLES) {
            pthread_mutex_unlock(&cycle_counter_mutex);
            break;
        }
        pthread_mutex_unlock(&cycle_counter_mutex);

        // Thinking
        //printf("Philosopher %d is thinking.\n", id);

        // Grab left fork
        pthread_mutex_lock(&forks[left_fork]);
        //printf("Philosopher %d picked up left fork (%d).\n", id, left_fork);

        // Grab right fork
        pthread_mutex_lock(&forks[right_fork]);
        //printf("Philosopher %d picked up right fork (%d).\n", id, right_fork);

        // Eating
        //printf("Philosopher %d is eating.\n", id);

        // Release forks
        pthread_mutex_unlock(&forks[left_fork]);
        //printf("Philosopher %d put down left fork (%d).\n", id, left_fork);
        pthread_mutex_unlock(&forks[right_fork]);
        //printf("Philosopher %d put down right fork (%d).\n", id, right_fork);

        // Increment cycle counter
        pthread_mutex_lock(&cycle_counter_mutex);
        cycle_counter++;
        pthread_mutex_unlock(&cycle_counter_mutex);
    }

    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for philosopher threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
