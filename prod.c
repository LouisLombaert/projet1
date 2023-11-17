#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 8
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t mutex, empty, full;

void *producer(void *arg) {
    int item;

    for (int i = 0; i < 10; i++) {
        item = i; // Produce item

        sem_wait(&empty); // Wait if buffer is full
        sem_wait(&mutex); // Enter critical section

        // Add item to the buffer
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;

        printf("Produced: %d\n", item);

        sem_post(&mutex); // Exit critical section
        sem_post(&full);  // Signal that the buffer is not empty
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int item;

    for (int i = 0; i < 10; i++) {
        sem_wait(&full);  // Wait if buffer is empty
        sem_wait(&mutex); // Enter critical section

        // Remove item from the buffer
        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;

        printf("Consumed: %d\n", item);

        sem_post(&mutex); // Exit critical section
        sem_post(&empty); // Signal that the buffer is not full
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // Create producer threads
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_create(&producers[i], NULL, producer, NULL);
    }

    // Create consumer threads
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_create(&consumers[i], NULL, consumer, NULL);
    }

    // Wait for producer threads to finish
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    // Wait for consumer threads to finish
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
