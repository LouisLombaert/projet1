#ifndef LOCKER_H
#define LOCKER_H

void locker_lock(volatile int* lock);
void locker_unlock(volatile int* lock);

#endif