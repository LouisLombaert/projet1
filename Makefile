CC = gcc
CFLAGS = -pthread -Wall

all: locker locker_tts sem philosophers prod_cons readers_writers philosophers_lock prod_cons_lock readers_writers_lock philosophers_sem prod_cons_sem readers_writers_sem

# Lockers & semaphore
locker: locker.c
	$(CC) -c locker.c -o locker.o

locker_tts: locker_tts.c
	$(CC) -c locker_tts.c -o locker_tts.o

sem: sem.c
	$(CC) -pthread -c sem.c -o sem.o

# POSIX 
philosophers: posix/philosophers.c
	$(CC) $(CFLAGS) -o philosophers posix/philosophers.c

prod_cons: posix/prod_cons.c
	$(CC) $(CFLAGS) -o prod_cons posix/prod_cons.c

readers_writers: posix/readers_writers.c
	$(CC) $(CFLAGS) -o readers_writers posix/readers_writers.c

# LOCKER TS
philosophers_lock: ts/philosophers_lock.c
	$(CC) $(CFLAGS) -o philosophers_lock ts/philosophers_lock.c locker.o

prod_cons_lock: ts/prod_cons_lock.c
	$(CC) $(CFLAGS) -o prod_cons_lock ts/prod_cons_lock.c locker.o

readers_writers_lock: ts/readers_writers_lock.c
	$(CC) $(CFLAGS) -o readers_writers_lock ts/readers_writers_lock.c locker.o

# SEMAPHORE TTS
philosophers_sem: tts/philosophers_sem.c
	$(CC) $(CFLAGS) -o philosophers_sem tts/philosophers_sem.c locker_tts.o sem.o

prod_cons_sem: tts/prod_cons_sem.c
	$(CC) $(CFLAGS) -o prod_cons_sem tts/prod_cons_sem.c locker_tts.o sem.o

readers_writers_sem: tts/readers_writers_sem.c
	$(CC) $(CFLAGS) -o readers_writers_sem tts/readers_writers_sem.c locker_tts.o sem.o

# OTHER
clean:
	rm -f philosophers philosophers_lock philosophers_sem prod_cons prod_cons_lock prod_cons_sem readers_writers readers_writers_lock readers_writers_sem *.o *.csv *.png

perf:
	./perf.sh

plot:
	python3 plot.py

run_all: perf plot