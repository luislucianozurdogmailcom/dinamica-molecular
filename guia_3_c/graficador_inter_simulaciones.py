import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

carpetas = os.listdir();
carpetas.sort()

dataframes = {};

# Recorremos las carpetas por cada T creado
for carpeta in carpetas:

    if '.py' in carpeta or '.pdf' in carpeta or '50' in carpeta:
        continue;

    # Leemos 
    data = np.loadtxt(f'{carpeta}/output/T_E_M_PAcept_Cv_sus.dat');
        
    # Crear un DataFrame para facilitar el manejo de los datos según el tipo de dato que tengamos
    df = pd.DataFrame(data, columns=['Temperatura','Energia','Magnetización Normalizada', 'Fracción de Aceptación', 'Calor Específico', 'Susceptibilidad']);
    
    df['Magnetización Normalizada'] = df['Magnetización Normalizada'].abs();

    dataframes[carpeta.replace('guia_3_c_','Simulación: ')] = df;

# Gŕaficos comparativos para tres simulaciones
for column in df.columns:

    # Si es la propia columna de temperatura, la salteamos
    if column in 'Temperatura':
        continue;
    
    # Crear la gráfica con su tamaño correspondiente
    plt.figure(figsize=(10, 6))

    colores_cientificos = [
        'green',         # Verde
        'orange',        # Naranja
        'purple',        # Púrpura
        'brown',         # Marrón
        'pink',          # Rosa
        'gray',          # Gris
        'olive',         # Oliva
        'cyan',          # Cyan
        'darkred',       # Rojo oscuro
        'darkblue',      # Azul oscuro
        'darkgreen',     # Verde oscuro
        'darkorange',    # Naranja oscuro
        'darkviolet'     # Violeta oscuro
    ]
    
    for n,df in enumerate(dataframes):

        # Configuración 
        sns.scatterplot(data      = dataframes[df], 
                        x         = 'Temperatura', 
                        y         = column, 
                        alpha     = 0.7,
                        s         = 20,
                        color     = colores_cientificos[0],
                        label     = df,
                        edgecolor = colores_cientificos[0],
                        linewidth = 0.5);
        
        # Borramos un color de la posición 0
        colores_cientificos.pop(0)
    
    plt.title(f"Gráfica de {column} vs Temperatura", fontsize=22)
    plt.xlabel("Temperatura", fontsize=16)
    plt.ylabel(f"{column}", fontsize=16)
    plt.tick_params(axis='x', labelsize=12)  # Solo eje X
    plt.tick_params(axis='y', labelsize=12)  # Solo eje Y
    plt.grid(alpha=0.3);
    # Guardar la gráfica como PDF con 300 DPI
    output_path = f'Gráfica de {column} vs Temperatura.pdf'
    plt.savefig(output_path, format="pdf", dpi=300)
    plt.close();