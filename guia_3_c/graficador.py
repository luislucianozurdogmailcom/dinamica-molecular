import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

# Configuraciones de Seaborn para los gráficos
sns.set(style="whitegrid");


# Recorremos las carpetas por cada T creado
for T in os.listdir('output'):
    
    # Promedios y errores estándar
    magnetizacion_promedio   = [];
    energia_promedio         = [];
    magnetizacion_desviacion = [];
    energia_desviacion       = [];

    # Recorremos cada archivo creado
    for archivo in os.listdir(f'output/{T}'):
        
        data = np.loadtxt(f'output/{T}/{archivo}')
        
        # Distinción entre tipos de datos a procesar
        if 'energia' in archivo:
            magnitud_medida = 'Energía';
        
        if 'magnetizacion' in archivo:
            magnitud_medida = 'Magnetización';

        # Calculo de promedios
        if 'termalizacion' not in archivo:
            if 'energia' in archivo:
                energia_promedio.append(data.mean());
                energia_desviacion.append(data.std());
        
            if 'magnetizacion' in archivo:
                magnetizacion_promedio.append(data.mean());
                magnetizacion_desviacion.append(data.std());

        # Crear un DataFrame para facilitar el manejo de los datos según el tipo de dato que tengamos
        df_energia = pd.DataFrame(data, columns=[magnitud_medida]);
        df_magneti = pd.DataFrame()

        # Crear la gráfica
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df, x=df.index, y=magnitud_medida, marker="o", color="blue")
        plt.title(f"Gráfica de {magnitud_medida}", fontsize=16)
        plt.xlabel("Índice", fontsize=12)
        plt.ylabel(f"{magnitud_medida}", fontsize=12)

        # Guardar la gráfica como PDF con 300 DPI
        output_path = f'output/{T}/{archivo}.pdf'
        plt.savefig(output_path, format="pdf", dpi=300)

    
    # Procesamos datos para los promedios
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