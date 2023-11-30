import pandas as pd
import matplotlib.pyplot as plt

def plot_experiment(ax, csv_files, title, labels, linestyles):
    for i, file in enumerate(csv_files):
        df = pd.read_csv(file, header=None, names=['X-axis', 'Y-axis'])
        ax.plot(df['X-axis'], df['Y-axis'], label=labels[i], linestyle=linestyles[i])

# Philosopher experiment
philo_csv_files = ['philo_time.csv', 'philo_time_ts.csv', 'philo_time_tts.csv']
philo_labels = ['Posix', 'test-and-set', 'test-and-test-and-set']
philo_linestyles = ['-', '--', '-.']

# Prod/Cons experiment
prod_cons_csv_files = ['prod_cons_time.csv', 'prod_cons_time_ts.csv', 'prod_cons_time_tts.csv']
prod_cons_labels = ['Posix', 'test-and-set', 'test-and-test-and-set']
prod_cons_linestyles = ['-', '--', '-.']

# Reader/Writer experiment
readers_writers_csv_files = ['readers_writers_time.csv', 'readers_writers_time_ts.csv', 'readers_writers_time_tts.csv']
readers_writers_labels = ['Posix', 'test-and-set', 'test-and-test-and-set']
readers_writers_linestyles = ['-', '--', '-.']

fig, axs = plt.subplots(3, 1, figsize=(10, 18))

# Plot Philosopher Experiment
plot_experiment(axs[0], philo_csv_files, 'Philosopher Experiment', philo_labels, philo_linestyles)

# Plot Producer/Consumer Experiment
plot_experiment(axs[1], prod_cons_csv_files, 'Producer/Consumer Experiment', prod_cons_labels, prod_cons_linestyles)

# Plot Reader/Writer Experiment
plot_experiment(axs[2], readers_writers_csv_files, 'Reader/Writer Experiment', readers_writers_labels, readers_writers_linestyles)

# Add labels and legend
for ax in axs:
    ax.set_xlabel('Number of Threads')
    ax.set_ylabel('Execution Time (seconds)')
    ax.legend()

# Save or show the plot
plt.tight_layout()
plt.savefig('combined_plots.png')
plt.show()