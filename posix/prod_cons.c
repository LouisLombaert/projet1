#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 8
#define DATA_COUNT 8192

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty, full, mutex;

void simulateProcessing() { 
    for (int i = 0; i < 10000; ++i);
}

void* producer(void* arg) {
    for (int i = 0; i < DATA_COUNT; ++i) {
        simulateProcessing();

        sem_wait(&empty);
        sem_wait(&mutex);

        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);
    }

    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < DATA_COUNT; ++i) {
        sem_wait(&full);
        sem_wait(&mutex);

        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&empty);

        simulateProcessing();
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

    pthread_t producers[num_producers], consumers[num_consumers];
    int producer_ids[num_producers], consumer_ids[num_consumers];

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    // Create producer threads
    for (int i = 0; i < num_producers; ++i) {
        producer_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    // Create consumer threads
    for (int i = 0; i < num_consumers; ++i) {
        consumer_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    // Join producer threads
    for (int i = 0; i < num_producers; ++i) {
        pthread_join(producers[i], NULL);
    }

    // Join consumer threads
    for (int i = 0; i < num_consumers; ++i) {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}