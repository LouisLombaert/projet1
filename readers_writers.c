#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_READS 2560
#define NUM_WRITES 640

int shared_data = 0;
int readers_count = 0;

sem_t mutex, write_mutex, read_mutex;

void simulateAccess() {
    for (int i = 0; i < 10000; ++i);
}

void* reader(void* arg) {
    int reader_id = *((int*)arg);

    while (1) {
        sem_wait(&read_mutex);
        sem_wait(&mutex);
        readers_count++;

        if (readers_count == 1) {
            sem_wait(&write_mutex);
        }

        sem_post(&mutex);
        sem_post(&read_mutex);

        // Read shared data
        //simulateAccess();
        printf("Reader %d read data: %d\n", reader_id, shared_data);

        sem_wait(&mutex);
        readers_count--;

        if (readers_count == 0) {
            sem_post(&write_mutex);
        }

        sem_post(&mutex);
    }

    return NULL;
}

void* writer(void* arg) {
    int writer_id = *((int*)arg);

    while (1) {
        sem_wait(&write_mutex);

        // Write to shared data
        //simulateAccess();
        shared_data++;
        printf("Writer %d wrote data: %d\n", writer_id, shared_data);

        sem_post(&write_mutex);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_readers> <num_writers>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_readers = atoi(argv[1]);
    int num_writers = atoi(argv[2]);

    if (num_readers <= 0 || num_writers <= 0) {
        fprintf(stderr, "Number of readers and writers must be greater than 0.\n");
        return EXIT_FAILURE;
    }

    pthread_t readers[num_readers], writers[num_writers];
    int reader_ids[num_readers], writer_ids[num_writers];

    sem_init(&mutex, 0, 1);
    sem_init(&write_mutex, 0, 1);
    sem_init(&read_mutex, 0, 1);

    // Create reader threads
    for (int i = 0; i < num_readers; ++i) {
        reader_ids[i] = i;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < num_writers; ++i) {
        writer_ids[i] = i;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Join reader threads
    for (int i = 0; i < num_readers; ++i) {
        pthread_join(readers[i], NULL);
    }

    // Join writer threads
    for (int i = 0; i < num_writers; ++i) {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&write_mutex);
    sem_destroy(&read_mutex);

    return 0;
}
