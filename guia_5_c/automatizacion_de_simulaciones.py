'''
Scripts de automatización de procesos
'''
import os
import shutil

densidades = [0.001, 0.01, 0.1] \
           + [((0.8-0.1)/10)*(i+1) + 0.1 for i in range(10)] \
           + [0.9, 1];

iteraciones = 10;

# Para borrar todas las carpetas generadas, descomenta la siguiente línea:
for densidad in densidades:
    shutil.rmtree(str(densidad), ignore_errors=True)

# Creamos nuevas simulaciones
for densidad in densidades:
    for iteracion in range(iteraciones):
        
        # Creamos la carpeta con sus padres si no existe
        carpeta = f'{densidad}/{iteracion}' 
        os.makedirs(carpeta, exist_ok=True)

        # Copiamos el caso_main dentro
        shutil.copytree('caso_main', f'{carpeta}/caso_main')

        # Ejecutamos la simulación
        print(f'Ejecutando simulación para densidad={densidad}, iteración={iteracion}')
        os.chdir(f'{carpeta}/caso_main')
        os.system('bash compilacionYejecucion.sh')
        os.chdir('../..')  # Volvemos al directorio raíz
        
