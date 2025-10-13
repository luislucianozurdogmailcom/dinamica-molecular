import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os

carpetas = os.listdir('output');
carpetas.sort()

# Recorremos las carpetas por cada T creado
for T in carpetas:
    
    # Es el archivo de los datos promedio
    if '.dat' in T:
        
        data = np.loadtxt(f'output/{T}');
        
        # Crear un DataFrame para facilitar el manejo de los datos según el tipo de dato que tengamos
        df = pd.DataFrame(data, columns=['Temperatura','Energia','Magnetización Normalizada', 'Fracción de Aceptación', 'Calor Específico', 'Susceptibilidad']);
        df['Magnetización Normalizada'] = df['Magnetización Normalizada'].abs();

        # Gráficos de dispersión
        for column in df.columns:
            
            # Si es la propia columna de temperatura, la salteamos
            if column in 'Temperatura':
                continue;

            # Crear la gráfica con su tamaño correspondiente
            plt.figure(figsize=(10, 6))
            
            # Configuración 
            sns.scatterplot(data      = df, 
                            x         = 'Temperatura', 
                            y         = column, 
                            alpha     = 0.7,
                            s         = 50,
                            color     = "blue",
                            edgecolor = 'black',
                            linewidth = 0.5);
            
            plt.title(f"Gráfica de {column} vs Temperatura", fontsize=22)
            plt.xlabel("Temperatura", fontsize=18)
            plt.ylabel(f"{column}", fontsize=18)
            plt.tick_params(axis='x', labelsize=12)  # Solo eje X
            plt.tick_params(axis='y', labelsize=12)  # Solo eje Y
            plt.grid(alpha = 0.3);
            
            # Guardar la gráfica como PDF con 300 DPI
            output_path = f'output/Gráfica de {column} vs Temperatura.pdf'
            plt.savefig(output_path, format="pdf", dpi=300)
            plt.close();
    
        continue;
    

    # Recorremos cada archivo creado
    for archivo in os.listdir(f'output/{T}'):

        if '.pdf' in archivo:
            continue;

        print(f'graficos de {T}/{archivo}')
        data = np.loadtxt(f'output/{T}/{archivo}')

        # Distinción entre tipos de datos a procesar
        if 'energia' in archivo:
            magnitud_medida = 'Energía';
        
        if 'magnetizacion' in archivo:
            magnitud_medida = 'Magnetización';

        # Crear un DataFrame para facilitar el manejo de los datos según el tipo de dato que tengamos
        df = pd.DataFrame(data, columns=[magnitud_medida]);

        # Crear la gráfica
        plt.figure(figsize=(10, 6))
        sns.lineplot(data=df, x=df.index, y=magnitud_medida, marker=",", color="blue")
        plt.title(f"Gráfica de {magnitud_medida}", fontsize=16)
        plt.xlabel("Índice", fontsize=12)
        plt.ylabel(f"{magnitud_medida}", fontsize=12)

        # Guardar la gráfica como PDF con 300 DPI
        output_path = f'output/{T}/{archivo}.pdf'
        plt.savefig(output_path, format="pdf", dpi=300)
        plt.close();
    
        
    # Histogramas
    for archivo in os.listdir(f'output/{T}'):
        
        if '.pdf' in archivo:
            continue;

        print(f'Histogramas de {T}/{archivo}')
        data = np.loadtxt(f'output/{T}/{archivo}')

        # Distinción entre tipos de datos a procesar
        if 'energia' in archivo:
            magnitud_medida = 'Energía';
        
        if 'magnetizacion' in archivo:
            magnitud_medida = 'Magnetización';

        # Crear un DataFrame para facilitar el manejo de los datos según el tipo de dato que tengamos
        df = pd.DataFrame(data, columns=[magnitud_medida]);

        # Crear la gráfica
        plt.figure(figsize=(10, 6))
        sns.histplot(data  = df, 
                     x     = magnitud_medida, 
                     bins  = 30,
                     kde   = True,
                     alpha = 0.7,
                     color = 'skyblue')
        plt.title(f"Frecuencias de {magnitud_medida}", fontsize=22)
        plt.xlabel(magnitud_medida, fontsize=18)
        plt.ylabel("Frecuencia", fontsize=18)
        plt.grid(alpha=0.3)

        # Guardar la gráfica como PDF con 300 DPI
        output_path = f'output/{T}/histograma_{archivo}.pdf'
        plt.savefig(output_path, format="pdf", dpi=300)
        plt.close();
    

# Analisis para Tc a partir de la curva de magnetización
for T in carpetas:
    
    # Es el archivo de los datos promedio
    if '.dat' in T:
        
        data = np.loadtxt(f'output/{T}');
        
        # Crear un DataFrame para facilitar el manejo de los datos según el tipo de dato que tengamos
        df = pd.DataFrame(data, columns=['Temperatura','Energia','Magnetización Normalizada', 'Fracción de Aceptación', 'Calor Específico', 'Susceptibilidad']);
        df['Magnetización Normalizada'] = df['Magnetización Normalizada'].abs();

        # Calculamos la derivada de la magnetización respecto a T de manera numérica:
        df['Diff_dM_dT'] = df['Magnetización Normalizada'].diff().abs() / df['Temperatura'].diff().abs();

        # Obtenemos la temperatura para la cual tenemos la dM/dT mas gránde.
        T_critica = df.loc[df['Diff_dM_dT'].idxmax(), 'Temperatura'];

        # Crear la gráfica con su tamaño correspondiente
        plt.figure(figsize=(10, 6))
        
        # Configuración 
        sns.lineplot(data      = df, 
                     x         = 'Temperatura', 
                     y         = 'Diff_dM_dT', 
                     linestyle = '-',
                     color     = "blue",
                     linewidth = 1.5,
                     alpha     = 0.7);
        
        plt.title(f"Derivada de la Magnetización respecto T vs Temperatura", fontsize=22)
        plt.xlabel("Temperatura", fontsize=18)
        plt.ylabel(f"dM/dT", fontsize=18)
        plt.tick_params(axis='x', labelsize=12)  # Solo eje X
        plt.tick_params(axis='y', labelsize=12)  # Solo eje Y
        plt.grid(alpha = 0.3);

        # Resaltar el punto crítico
        plt.scatter(T_critica, 
                    df.loc[df['Diff_dM_dT'].idxmax(), 'Diff_dM_dT'], 
                    color='red', 
                    s=50, 
                    zorder=5, 
                    label=f'T_crítica = {T_critica:.3f}')

        # Líneas punteadas para marcar la posición del punto crítico
        plt.axvline(x=T_critica, 
                    color='red', 
                    linestyle='--', 
                    alpha=0.9,
                    zorder=4,
                    linewidth=1.3)
        
        # Agregar leyenda
        plt.legend(fontsize=14)

        # Guardar la gráfica como PDF con 300 DPI
        output_path = f'output/dM|dT vs Temperatura.pdf'
        plt.savefig(output_path, format="pdf", dpi=300)
        plt.close();
