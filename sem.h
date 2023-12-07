#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef struct sem_t sem_t;

sem_t *sem_init(unsigned int value);
int sem_destroy(sem_t *sem);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);

#endif