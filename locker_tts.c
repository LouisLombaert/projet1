#include "locker_tts.h"

locker_t *init_lock() {
    locker_t *locker = malloc(sizeof(locker_t));
    if (locker == NULL) { return NULL; }

    locker->lock = 0;
    return locker;
}

void destroy_lock(locker_t *locker) {
    locker->lock = 0;
    free(locker);
}

int lock(locker_t *locker) {
    int set = 1;
    while (set == 1) {
        if(locker->lock==0) { // Test before TS
            asm("xchgl  %0, %1" : "+q" (set), "+m" (locker->lock));
        }
    }
    return EXIT_SUCCESS;
}

int unlock(locker_t *locker) {
    locker->lock = 0;
    return EXIT_SUCCESS;
}