#!/bin/bash

NB_RUN=5
CSV_FILE="philo_time.csv"

[ -e "$CSV_FILE" ] && rm "$CSV_FILE"

for nb_threads in 2 4 8 16 32 64; do 

    echo "Running philosophers with $nb_threads threads..."

    total_time=0
    for i in $(seq 1 $NB_RUN); do
        time_used=$( { /usr/bin/time -f "%e" ./philosophers $nb_threads; } 2>&1 )

        total_time=$(echo "$total_time + $time_used" | bc)
    done

    avg=$(echo "scale=3; $total_time / $NB_RUN" | bc)

    echo "Total_time: $total_time, avg time: $avg"

    echo "$nb_threads,$avg" >> "$CSV_FILE"

done