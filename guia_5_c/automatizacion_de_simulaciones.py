'''
Scripts de automatización de procesos
'''
import os
import shutil
import math
import numpy as np
from concurrent.futures import ThreadPoolExecutor, as_completed

densidades = np.concatenate([     
                np.array([0.001, 0.01]),
                np.arange(0.1, 0.8 + 0.07, 0.07),
                np.array([0.9, 1])
            ]);

deltaT     = np.concatenate([
                np.array([0.002, 0.002, 0.002]),
                np.arange(0.002, 0, -0.0002),
                np.array([0.0001, 0.0001])
            ]);

particulas = 200;
sigma      = 1;

iteraciones = 10;

# Número máximo de simulaciones simultáneas
MAX_THREADS = 8  # Ajusta según los núcleos de tu CPU

def ejecutar_simulacion(densidad, n, iteracion, base_dir):
    """
    Función que ejecuta una simulación individual
    """
    try:
        # Creamos la carpeta con sus padres si no existe
        carpeta = os.path.join(base_dir, f'{densidad}')
        os.makedirs(carpeta, exist_ok=True)

        # Copiamos el caso_main dentro
        destino = os.path.join(carpeta, str(iteracion))
        shutil.copytree(os.path.join(base_dir, 'caso_main'), destino)

        # Ejecutamos la simulación
        print(f'[Thread-{iteracion}] Ejecutando simulación para densidad={densidad}, iteración={iteracion}')
        
        # Modificamos el lado L de la caja
        archivo = os.path.join(destino, 'input/L')
        archivoDeltaT = os.path.join(destino, 'input/deltaT')
        
        with open(archivo, 'w') as f:
            f.write(f'{math.pow((particulas) / (densidad*(sigma**3)), 1/3)}')
        
        with open(archivoDeltaT, 'w') as f:
            f.write(f'{deltaT[n]}')

        # Ejecutamos el script desde el directorio correcto
        os.chdir(destino)
        resultado = os.system('bash run.sh')
        os.chdir(base_dir)  # Volvemos al directorio raíz
        
        print(f'[Thread-{iteracion}] Simulación completada para densidad={densidad}, iteración={iteracion}')
        return (densidad, iteracion, resultado == 0)
    
    except Exception as e:
        print(f'[Thread-{iteracion}] Error en densidad={densidad}, iteración={iteracion}: {str(e)}')
        return (densidad, iteracion, False)

# Para borrar todas las carpetas generadas, descomenta la siguiente línea:
for densidad in densidades:
    shutil.rmtree(str(densidad), ignore_errors=True)

# Directorio base
base_dir = os.getcwd()

# Creamos una lista de todas las tareas
tareas = []
for n, densidad in enumerate(densidades):
    for iteracion in range(iteraciones):
        tareas.append((densidad, n, iteracion, base_dir))

# Ejecutamos las simulaciones en paralelo
print(f'Iniciando {len(tareas)} simulaciones con {MAX_THREADS} threads simultáneos...')
resultados = []

with ThreadPoolExecutor(max_workers=MAX_THREADS) as executor:
    # Enviamos todas las tareas
    futuros = {
        executor.submit(ejecutar_simulacion, densidad, n, iteracion, base_dir): (densidad, iteracion)
        for densidad, n, iteracion, base_dir in tareas
    }
    
    # Procesamos los resultados a medida que se completan
    for futuro in as_completed(futuros):
        densidad, iteracion = futuros[futuro]
        try:
            resultado = futuro.result()
            resultados.append(resultado)
        except Exception as e:
            print(f'Excepción en simulación densidad={densidad}, iteración={iteracion}: {str(e)}')

# Resumen de resultados
print('\n' + '='*60)
print('RESUMEN DE SIMULACIONES')
print('='*60)
exitosas = sum(1 for _, _, exito in resultados if exito)
print(f'Total: {len(resultados)} simulaciones')
print(f'Exitosas: {exitosas}')
print(f'Fallidas: {len(resultados) - exitosas}')
        
