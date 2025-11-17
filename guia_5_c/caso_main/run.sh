clear

echo "Borramos la carpeta anteriormente generada..."
rm -r output 

echo "Compilando con optimizaciones para código científico...."

# Flags de optimización científica
OPTIMIZE_FLAGS="-O3 -march=native -mtune=native -flto -funroll-loops -ffast-math -finline-functions -fomit-frame-pointer -DNDEBUG"

# Warnings útiles
WARNING_FLAGS="-Wall -Wextra -Wno-unused-parameter"

#gcc -c ziggurat/ziggurat.c -o ziggurat/ziggurat.o $OPTIMIZE_FLAGS $WARNING_FLAGS
gcc main.c utils/fisica.c ziggurat/ziggurat.o -o main $OPTIMIZE_FLAGS $WARNING_FLAGS -lm -fopenmp

# Creamos las carpetas
mkdir output

# Corremos el programa
./main

# Graficamos
python3 graficador.py

echo "Finaliza el programa"
