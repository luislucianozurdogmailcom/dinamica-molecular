import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import math

data = np.loadtxt('cociente_puntos.dat');
X    = np.arange(1, len(data) + 1) * 1000;

sns.lineplot(x=X, y=data)
sns.lineplot(x=[-1000*1000,9000*1000], y=[math.pi/4, math.pi/4], label='Valor real')
plt.savefig('grafico_punto_2.pdf', format='pdf')