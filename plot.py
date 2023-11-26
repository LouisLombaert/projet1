import matplotlib.pyplot as plt
import csv
from locale import atof
import numpy as np

def plot_csv(csv_file, title, subplot_index):
    threads = []
    avg_times = []
    std_devs = []

    with open(csv_file, 'r') as file:
        reader = csv.reader(file)
        next(reader)  # Skip headers
        for row in reader:
            threads.append(int(row[0]))
            avg_times.append(atof(row[1]))

    # Calculate standard deviation
    std_dev = np.std(avg_times)

    # Create a subplot
    plt.subplot(3, 2, subplot_index)  # Changed to 3 rows, 2 columns

    plt.errorbar(threads, avg_times, yerr=std_dev, label=title, marker='o', linestyle='-')
    
    plt.xlabel('Number of Threads')
    plt.ylabel('Average Time (sec)')
    plt.title(f'Performance - {title}')
    plt.legend()
    plt.grid(True)
    plt.ylim(bottom=0)  # Ensure y-axis starts at 0

# Plot for Philosophers
plot_csv('philo_time.csv', 'Philosophers', 1)

# Plot for Prod/Cons
plot_csv('prod_cons_time.csv', 'Producers/Consumers', 3)

# Plot for Readers/Writers
plot_csv('readers_writers_time.csv', 'Readers/Writers', 5)

# Plot for Philosophers
plot_csv('philo_time_lock.csv', 'Philosophers Lock', 2)

# Plot for Prod/Cons
plot_csv('prod_cons_time_lock.csv', 'Producers/Consumers Lock', 4)

# Plot for Readers/Writers Locker
plot_csv('readers_writers_time_lock.csv', 'Readers/Writers', 6)

plt.tight_layout()  # Adjust layout to prevent overlapping
plt.show()
