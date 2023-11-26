CC = gcc
CFLAGS = -pthread -Wall

all: locker philosophers prod_cons readers_writers philosophers_lock prod_cons_lock readers_writers_lock

philosophers: philosophers.c
	$(CC) $(CFLAGS) -o philosophers philosophers.c

prod_cons: prod_cons.c
	$(CC) $(CFLAGS) -o prod_cons prod_cons.c

readers_writers: readers_writers.c
	$(CC) $(CFLAGS) -o readers_writers readers_writers.c

locker: locker.c
	$(CC) -c locker.c -o locker 

philosophers_lock: philosophers_lock.c
	$(CC) $(CFLAGS) -o philosophers_lock philosophers_lock.c locker.c

prod_cons_lock: prod_cons_lock.c
	$(CC) $(CFLAGS) -o prod_cons_lock prod_cons_lock.c locker.c

readers_writers_lock: readers_writers_lock.c
	$(CC) $(CFLAGS) -o readers_writers_lock readers_writers_lock.c locker.c

clean:
	rm -f philosophers philosophers_lock prod_cons prod_cons_lock readers_writers readers_writers_lock *.csv

perf:
	./perf.sh

plot:
	python3 plot.py

run_all: perf plot