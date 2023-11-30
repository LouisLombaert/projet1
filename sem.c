#include <stdlib.h>
#include "sem.h"
#include "locker_tts.h"

typedef struct sem_t {
    long int val;
    locker_t *locker;
} sem_t;

sem_t *sem_init(unsigned int value) {
    sem_t *sem = malloc(sizeof(sem_t));
    if (sem == NULL) return NULL;

    sem->locker = init_lock();
    if (sem->locker == NULL) { free(sem); return NULL; }

    sem->val = value;
    return sem;
}

int sem_destroy(sem_t *sem) {
    destroy_lock(sem->locker);
    free(sem);
    return EXIT_SUCCESS;
}

int sem_wait(sem_t *sem) {
    lock(sem->locker);
    sem->val--;
    unlock(sem->locker);
    return EXIT_SUCCESS;
}

int sem_post(sem_t *sem) {
    lock(sem->locker);
    sem->val++;
    unlock(sem->locker);
    return EXIT_SUCCESS;
}