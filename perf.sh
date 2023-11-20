#!/bin/bash

make

# Function to run a benchmark for a given program and configuration
run_benchmark() {
    PROGRAM="$1"
    NB_THREADS="$2"
    OUTPUT_FILE="$3"

    total_time=0
    for i in $(seq 1 $NB_RUN); do
        if [ "$PROGRAM" == "philosophers" ]; then
            time_used=$( { /usr/bin/time -f "%e" ./$PROGRAM "$NB_THREADS"; } 2>&1 )
        else
            time_used=$( { /usr/bin/time -f "%e" ./$PROGRAM "$((NB_THREADS/2))" "$((NB_THREADS/2))"; } 2>&1 )
        fi
        total_time=$(echo "$total_time + $time_used" | bc)
    done

    avg=$(echo "scale=3; $total_time / $NB_RUN" | bc)

    echo "Total_time for $PROGRAM with $NB_THREADS threads: $total_time, avg time: $avg"
    echo "$NB_THREADS,$avg" >> "$OUTPUT_FILE"
}

NB_RUN=5
CSV_FILE_PHILOSOPHERS="philo_time.csv"
CSV_FILE_PROD_CONS="prod_cons_time.csv"
CSV_FILE_READERS_WRITERS="readers_writers_time.csv"

# Philosophers
[ -e "$CSV_FILE_PHILOSOPHERS" ] && rm "$CSV_FILE_PHILOSOPHERS"
for nb_threads in 2 4 8 16 32 64; do 
    echo "Running philosophers with $nb_threads threads..."
    run_benchmark "philosophers" "$nb_threads" "$CSV_FILE_PHILOSOPHERS"
done

# Prod/Cons
[ -e "$CSV_FILE_PROD_CONS" ] && rm "$CSV_FILE_PROD_CONS"
for nb_threads in 2 4 8 16 32 64; do 
    echo "Running prod/cons with $nb_threads threads..."
    run_benchmark "prod_cons" "$nb_threads" "$CSV_FILE_PROD_CONS"
done

# Readers/Writers
#[ -e "$CSV_FILE_READERS_WRITERS" ] && rm "$CSV_FILE_READERS_WRITERS"
#for nb_threads in 2 4 8 16 32 64; do 
#    echo "Running readers/writers with $nb_threads threads..."
#    run_benchmark "readers_writers" "$nb_threads" "$CSV_FILE_READERS_WRITERS"
#done
