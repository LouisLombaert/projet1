#ifndef SEMAPHORE_H
#define SEMAPHORE_H
/**
 * The semaphore structure with the value of the semaphore and the lock_t structure
 */
typedef struct sem_t sem_t;

/**
 * Initialize the semaphore
 * @param value: The initiale value of the semaphore
 * @return A semaphore active_sem_t on the HEAP
 */
sem_t *sem_init(unsigned int value);

/**
 * Destroy the semaphore
 * @param sem: The semaphore active_sem_t on the HEAP
 * @return EXIT_FAILURE if an error occurs or EXIT_SUCCESS otherwise
 */
int sem_destroy(sem_t *sem);

/**
 * Decrease the semaphore value and wait if it's <= 0
 * @param sem: The semaphore active_sem_t on the HEAP
 * @return EXIT_FAILURE if an error occurs or EXIT_SUCCESS otherwise
 */
int sem_wait(sem_t *sem);

/**
 * Increase the semaphore value
 * @param sem: The semaphore active_sem_t on the HEAP
 * @return EXIT_FAILURE if an error occurs or EXIT_SUCCESS otherwise
 */
int sem_post(sem_t *sem);
#endif