#ifndef LOCKER_H
#define LOCKER_H

#include <stdlib.h>

typedef struct _locker {
    int lock;
} locker_t;

locker_t *init_lock();
void destroy_lock(locker_t *locker);
int lock(locker_t *locker);
int unlock(locker_t *locker);

#endif // LOCKER_H