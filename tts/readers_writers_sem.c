#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../sem.h"
#include "../locker_tts.h"

#define NUM_READS 2560
#define NUM_WRITES 640

int readers_count = 0;
int writers_count = 0;
int write_count = 0;
int read_count = 0;

locker_t* read_lock;
locker_t* write_lock;
locker_t* z_lock;

sem_t* read_sem;
sem_t* write_sem;

void simulateAccess() {
    for (int i = 0; i < 10000; ++i);
}

void* reader() {
    for (int i=0; i<NUM_READS; i++) {
        sem_wait(read_sem);

        lock(z_lock);
        lock(read_lock);
        readers_count++;

        if (readers_count == 1) {
            lock(write_lock);
        }

        unlock(read_lock);
        unlock(z_lock);
        
        // READ()
        simulateAccess();
        read_count++;
        // Fin READ

        lock(read_lock);
        readers_count--;

        if (readers_count == 0) {
            unlock(write_lock);
        }

        unlock(read_lock);

        sem_post(read_sem);
    }

    return NULL;
}

void* writer() {
    for (int i=0; i<NUM_WRITES; i++) {
        sem_wait(write_sem);
        lock(write_lock);
        writers_count++;

        if (writers_count == 1){
            sem_wait(read_sem);
        }

        unlock(write_lock);

        //  WRITE()
        simulateAccess();
        write_count++;
        //  FIN WRITE

        lock(write_lock);
        writers_count--;

        if (writers_count == 0){
            sem_post(read_sem);
        }
        unlock(write_lock);

        sem_post(write_sem);
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

    //locker
    read_lock = init_lock();
    write_lock = init_lock();
    z_lock = init_lock();

    read_sem = sem_init(1);
    write_sem = sem_init(1);

    // Create reader threads
    for (int i = 0; i < num_readers; ++i) {
        pthread_create(&readers[i], NULL, reader, NULL);
    }

    // Create writer threads
    for (int i = 0; i < num_writers; ++i) {
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    // Join reader threads
    for (int i = 0; i < num_readers; ++i) {
        pthread_join(readers[i], NULL);
    }

    // Join writer threads
    for (int i = 0; i < num_writers; ++i) {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(write_sem);
    sem_destroy(read_sem);
    destroy_lock(read_lock);
    destroy_lock(write_lock);
    destroy_lock(z_lock);
    return 0;
}