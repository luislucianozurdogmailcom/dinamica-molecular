import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

contenido   = 'output.dat'
contenido_d = 'output_D.dat'

def graficador(file):

    with open(file, 'r') as archivo:

        diccionario = {};
        datos       = [];
        for n,linea in enumerate(archivo):

            if n == 0:
                columnas = linea.strip().split();
            else:
                dato = linea.strip().split();
                datos.append(dato);

        for n,i in enumerate(columnas):
            diccionario[f'{i}'] = [float(j[n]) for j in datos];

    df = pd.DataFrame(diccionario);
    print(df['X'], df['Y'])

    plt.figure(figsize=(10, 6))
    plt.scatter(df['X'].values, df['Y'].values)
    plt.savefig(f'{file}.pdf')

graficador(contenido);
graficador(contenido_d);