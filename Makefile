CC = gcc
CFLAGS = -pthread -Wall

all: philosophers prod_cons readers_writers

philosophers: philosophers.c
	$(CC) $(CFLAGS) -o philosophers philosophers.c

prod_cons: prod_cons.c
	$(CC) $(CFLAGS) -o prod_cons prod_cons.c

readers_writers: readers_writers.c
	$(CC) $(CFLAGS) -o readers_writers readers_writers.c

clean:
	rm -f philosophers prod_cons readers_writers

perf:
	./perf.sh

plot:
	python3 plot.py

run_all: run_perf plot