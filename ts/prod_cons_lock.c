#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../locker.h"

#define NUM_CYCLES 1000000
#define BUFFER_SIZE 8

int cycle_counter = 0;

locker_t cycle_counter_lock;
locker_t buffer_lock;

volatile int* buffer;
volatile int in = 0, out = 0;

void simulateProcessing() {
    for (int i = 0; i < 10000; ++i) {}
}

void* producer(void* arg) {
    int num_threads = *((int*)arg);
    int data_count = (6400 / num_threads);

    for (int i = 0; i < data_count; ++i) {
        lock(&buffer_lock);

        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;

        simulateProcessing();

        unlock(&buffer_lock);
    }

    return NULL;
}

void* consumer(void* arg) {
    int num_threads = *((int*)arg);
    int data_count = (6400 / num_threads);

    for (int i = 0; i < data_count; ++i) {
        lock(&buffer_lock);

        out = (out + 1) % BUFFER_SIZE;

        simulateProcessing();

        unlock(&buffer_lock);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_producers> <num_consumers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);

    if (num_producers <= 0 || num_consumers <= 0) {
        fprintf(stderr, "Number of producers and consumers must be greater than 0.\n");
        return EXIT_FAILURE;
    }

    pthread_t* producer_threads = malloc(num_producers * sizeof(pthread_t));
    pthread_t* consumer_threads = malloc(num_consumers * sizeof(pthread_t));
    int* producer_ids = malloc(num_producers * sizeof(int));
    int* consumer_ids = malloc(num_consumers * sizeof(int));

    buffer = malloc(BUFFER_SIZE * sizeof(volatile int));
    locker_t* buffer_lock = init_lock();
    locker_t* cycle_counter_lock = init_lock();

    // Create producer threads
    for (int i = 0; i < num_producers; ++i) {
        producer_ids[i] = i;
        pthread_create(&producer_threads[i], NULL, producer, &num_producers);
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; ++i) {
        consumer_ids[i] = i;
        pthread_create(&consumer_threads[i], NULL, consumer, &num_consumers);
    }

    // Join producer threads
    for (int i = 0; i < num_producers; ++i) {
        pthread_join(producer_threads[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < num_consumers; ++i) {
        pthread_join(consumer_threads[i], NULL);
    }

    destroy_lock(buffer_lock);
    destroy_lock(cycle_counter_lock);

    free(producer_threads);
    free(consumer_threads);
    free(producer_ids);
    free(consumer_ids);
    free((void*)buffer);

    return 0;
}
