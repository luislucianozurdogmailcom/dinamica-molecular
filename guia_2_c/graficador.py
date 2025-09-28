import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import math

data = np.loadtxt('numeros.dat');

# Creamos los bins
hist, bin_edges = np.histogram(data[:,0], bins=50);

# Create a bar chart
plt.figure(figsize=(10, 6))
plt.hist(data[:,0], bins=bin_edges, color='skyblue', edgecolor='navy', density=True)

# Add labels and title
plt.xlabel('Index')
plt.ylabel('Value')
plt.title('Bar Chart of Data from numeros.dat')

# Add grid for better readability
plt.grid(axis='y', linestyle='--', alpha=0.7)

# Improve aesthetics
plt.tight_layout()

# Show the plot
plt.savefig("grafico.pdf");