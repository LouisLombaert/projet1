# Makefile
CC = gcc
CFLAGS = -pthread

all: philosophers prod_cons readers_writers

philosophers: philosophers.c
	$(CC) $(CFLAGS) philosophers.c -o philosophers

prod_cons: prod_cons.c
	$(CC) $(CFLAGS) prod_cons.c -o prod_cons

readers_writers: readers_writers.c
	$(CC) $(CFLAGS) readers_writers.c -o readers_writers

clean:
	rm -f philosophers prod_cons readers_writers
