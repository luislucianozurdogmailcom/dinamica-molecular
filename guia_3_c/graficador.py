import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

# Configuraciones de Seaborn para los gráficos
sns.set(style="whitegrid")

# Recorremos las carpetas por cada T creado
for T in os.listdir('output'):

    # Recorremos cada archivo creado
    for archivo in os.listdir(f'output/{T}'):
        
        data = np.loadtxt(f'output/{T}/{archivo}')
        
        if 'energia' in archivo:
            magnitud_medida = 'Energía';
        
        if 'magnetizacion' in archivo:
            magnitud_medida = 'Magnetización';
            
        # Crear un DataFrame para facilitar el manejo de los datos según el tipo de dato que tengamos
        df = pd.DataFrame(data, columns=[magnitud_medida]);
        
        # Crear la gráfica
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df, x=df.index, y=magnitud_medida, marker="o", color="blue")
        plt.title(f"Gráfica de {magnitud_medida}", fontsize=16)
        plt.xlabel("Índice", fontsize=12)
        plt.ylabel(f"{magnitud_medida}", fontsize=12)

        # Guardar la gráfica como PDF con 300 DPI
        output_path = f'output/{T}/{archivo}.pdf'
        plt.savefig(output_path, format="pdf", dpi=300)