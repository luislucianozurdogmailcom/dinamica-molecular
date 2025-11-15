'''
Scripts de automatización de procesos
'''
import os
import shutil
import math

densidades = [0.001, 0.01, 0.1] \
           + [((0.8-0.1)/10)*(i+1) + 0.1 for i in range(10)] \
           + [0.9, 1];

particulas = 200;
sigma      = 1;

iteraciones = 10;

# Para borrar todas las carpetas generadas, descomenta la siguiente línea:
for densidad in densidades:
    shutil.rmtree(str(densidad), ignore_errors=True)

# Creamos nuevas simulaciones
for densidad in densidades:
    for iteracion in range(iteraciones):
        
        # Creamos la carpeta con sus padres si no existe
        carpeta = f'{densidad}' 
        os.makedirs(carpeta, exist_ok=True)

        # Copiamos el caso_main dentro
        shutil.copytree('caso_main', f'{carpeta}/{iteracion}')

        # Ejecutamos la simulación
        print(f'Ejecutando simulación para densidad={densidad}, iteración={iteracion}')
        os.chdir(f'{carpeta}/{iteracion}')
        
        # Modificamos el lado L de la caja
        archivo = 'input/L'
        with open(archivo, 'w') as f:
            f.writelines(f'{math.pow( ( particulas ) / ( densidad*(sigma**3) ) , 1/3)}')

        os.system('bash compilacionYejecucion.sh')
        os.chdir('../..')  # Volvemos al directorio raíz

        exit()
        
