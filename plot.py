import pandas as pd
import matplotlib.pyplot as plt

# Fonction pour lire un fichier CSV et renvoyer un DataFrame
def read_csv(file_path):
    return pd.read_csv(file_path, header=None, names=['N', 'S'])

# Liste des fichiers CSV pour chaque version (avec et sans lock)
files_without_lock = ['philo_time.csv', 'prod_cons_time.csv', 'readers_writers_time.csv']
files_with_lock = ['philo_time_lock.csv', 'prod_cons_time_lock.csv', 'readers_writers_time_lock.csv']

# Fonction pour créer un graphique à partir des données
def plot_graph(ax, dataframes, labels, title):
    for df, label in zip(dataframes, labels):
        ax.plot(df['N'], df['S'], label=label)

    ax.set_xlabel('Nombre de Threads')
    ax.set_ylabel('Temps Moyen (Sec)')
    ax.set_title(title)
    ax.legend()
    ax.grid(True)

# Lire les fichiers CSV
dataframes_without_lock = [read_csv(file) for file in files_without_lock]
dataframes_with_lock = [read_csv(file) for file in files_with_lock]

# Labels pour les graphiques
labels_without_lock = ['Philosophes', 'Producteurs/Consommateur', 'Readers/Writers']
labels_with_lock = ['Philosophes avec Lock', 'Producteurs/Consommateur avec Lock', 'Readers/Writers avec Lock']

# Créer une seule figure
fig, axs = plt.subplots(2, 1, figsize=(12, 10))

# Créer les graphiques
plot_graph(axs[0], dataframes_without_lock, labels_without_lock, 'Performance')
plot_graph(axs[1], dataframes_with_lock, labels_with_lock, 'Performance avec Lock')

# Ajuster l'espacement entre les sous-graphiques
plt.tight_layout()

# Afficher la figure
plt.show()
