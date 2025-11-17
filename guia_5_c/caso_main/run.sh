clear

echo "Borramos la carpeta anteriormente generada..."
rm -r output 

echo "Compilando con optimizaciones para código científico...."

# Flags de optimización científica
OPTIMIZE_FLAGS="-O3 -march=native -mtune=native -flto -funroll-loops -ffast-math -finline-functions -fomit-frame-pointer -DNDEBUG"

# Warnings útiles
WARNING_FLAGS="-Wall -Wextra -Wno-unused-parameter"

# Recompilar ziggurat.o en el cluster para evitar incompatibilidades de versión LTO
echo "Recompilando ziggurat.o..."
gcc -c ziggurat/ziggurat.c -o ziggurat/ziggurat.o $OPTIMIZE_FLAGS $WARNING_FLAGS

# Compilar el programa principal
gcc main.c utils/fisica.c ziggurat/ziggurat.o -o main $OPTIMIZE_FLAGS $WARNING_FLAGS -lm -fopenmp

# Creamos las carpetas
mkdir output

# Corremos el programa
./main

# Graficamos (comentado por falta de módulo ovito en el cluster)
python3 graficador.py
# Si necesitas ovito, instala con: pip3 install --user ovito

echo "Finaliza el programa"
