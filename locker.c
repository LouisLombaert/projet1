#include "locker.h"

void locker_lock(volatile int* lock) {
    int tmp;
    while (tmp) {
        __asm__(
            "xchg %0, %1\n\t"
            : "=r" (tmp), "=m" (*lock)
            : "0" (1), "m" (*lock)
            : "memory"
        );
    }
}

void locker_unlock(volatile int* lock) {
    *lock = 0; // Unlock, set to 0
}