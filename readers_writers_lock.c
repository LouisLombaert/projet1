#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "locker.h"  // Include the custom lock

#define NUM_READS 2560
#define NUM_WRITES 640
#define TOTAL_ACCESS 6400

int readers_count = 0;
int writers_count = 0;
int write_count = 0;
int read_count = 0;

volatile int custom_lock = 0;  // Use the custom lock

void simulateAccess() {
    for (int i = 0; i < 10000; ++i) {}
}

void* reader(void* arg) {
    //int reader_id = *((int*)arg);

    for (int i = 0; i < NUM_READS; i++) {
        locker_lock(&custom_lock);

        readers_count++;
        if (readers_count == 1) {
            // First reader, block writers
            locker_lock(&custom_lock);
        }

        locker_unlock(&custom_lock);

        read_count++;

        // Critical Section: READ
        simulateAccess();
        //printf("Reader %d read data: %d\n", reader_id, read_count);
        // End Critical Section

        locker_lock(&custom_lock);

        readers_count--;

        if (readers_count == 0) {
            // Last reader, unblock writers
            locker_unlock(&custom_lock);
        }

        locker_unlock(&custom_lock);
    }

    return NULL;
}

void* writer(void* arg) {
    //int writer_id = *((int*)arg);

    for (int i = 0; i < NUM_WRITES; i++) {
        locker_lock(&custom_lock);

        writers_count++;
        if (writers_count == 1) {
            // First writer, block readers
            locker_lock(&custom_lock);
        }

        locker_unlock(&custom_lock);

        // Critical Section: WRITE
        write_count++;
        simulateAccess();
        //printf("Writer %d wrote data: %d\n", writer_id, write_count);
        // End Critical Section

        locker_lock(&custom_lock);

        writers_count--;

        if (writers_count == 0) {
            // Last writer, unblock readers
            locker_unlock(&custom_lock);
        }

        locker_unlock(&custom_lock);
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

    // Initialize the custom lock
    locker_unlock(&custom_lock);

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

    return 0;
}
