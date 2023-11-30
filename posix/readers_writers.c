#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_READS 2560
#define NUM_WRITES 640

int readers_count = 0;
int writers_count = 0;
int write_count = 0;
int read_count = 0;

sem_t write_sem, read_sem; // 3 sems ??
pthread_mutex_t read_mutex, write_mutex, z;

void simulateAccess() {
    for (int i = 0; i < 10000; ++i);
}

void* reader(void* arg) {
    //int reader_id = *((int*)arg);

    for (int i=0; i<NUM_READS; i++) {
        pthread_mutex_lock(&z);
        sem_wait(&read_sem);
        pthread_mutex_lock(&read_mutex);
        readers_count++;

        if (readers_count == 1) {
            sem_wait(&write_sem);
        }

        pthread_mutex_unlock(&read_mutex);
        sem_post(&read_sem);
        pthread_mutex_unlock(&z);
        read_count++;

        ///////////////////////////////
        // READ()

        simulateAccess();
        //printf("Reader %d read data: %d\n", reader_id, read_count);

        // Fin READ
        //////////////////////////////

        pthread_mutex_lock(&read_mutex);
        readers_count--;

        if (readers_count == 0) {
            sem_post(&write_sem);
        }

        pthread_mutex_unlock(&read_mutex);
    }

    return NULL;
}

void* writer(void* arg) {
    //int writer_id = *((int*)arg);

    for (int i=0; i<NUM_WRITES; i++) {
        pthread_mutex_lock(&write_mutex);
        writers_count++;
        if (writers_count == 1){
            sem_wait(&read_sem);
        }
        pthread_mutex_unlock(&write_mutex);
        sem_wait(&write_sem);
        write_count++;

        ///////////////////////////////////
        //  WRITE()

        simulateAccess();
        //printf("Writer %d wrote data: %d\n", writer_id, write_count);

        //  FIN WRITE
        //////////////////////////////////

        sem_post(&write_sem);
        pthread_mutex_lock(&write_mutex);
        writers_count--;
        if (writers_count == 0){
            sem_post(&read_sem);
        }
        pthread_mutex_unlock(&write_mutex);
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

    //sem_init(&mutex, 0, 1);
    sem_init(&write_sem, 0, 1);
    sem_init(&read_sem, 0, 1);
    
    if (pthread_mutex_init(&read_mutex, NULL) != 0) {
        // Gestion de l'erreur d'initialisation
        printf("erreur lors de l'init du read mutex");
        return 1;
    }

    if (pthread_mutex_init(&write_mutex, NULL) != 0) {
        // Gestion de l'erreur d'initialisation
        printf("erreur lors de l'init du write mutex");
        return 1;
    }

    if (pthread_mutex_init(&z, NULL) != 0) {
        // Gestion de l'erreur d'initialisation
        printf("erreur lors de l'init du mutex z");
        return 1;
    }

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

    //sem_destroy(&mutex);
    sem_destroy(&write_sem);
    sem_destroy(&read_sem);
    pthread_mutex_destroy(&read_mutex);
    pthread_mutex_destroy(&write_mutex);
    pthread_mutex_destroy(&z);

    return 0;
}