clear

echo "Borramos la carpeta anteriormente generada..."
rm -r output

echo "Compilando con optimizaciones para código científico...."

# Flags de optimización científica
OPTIMIZE_FLAGS="-O3 -march=native -mtune=native -flto -funroll-loops -ffast-math -finline-functions -fomit-frame-pointer -DNDEBUG"

# Warnings útiles
WARNING_FLAGS="-Wall -Wextra -Wno-unused-parameter"

gcc -c ziggurat/ziggurat.c -o ziggurat/ziggurat.o $OPTIMIZE_FLAGS $WARNING_FLAGS
gcc main.c ziggurat/ziggurat.o -o main $OPTIMIZE_FLAGS $WARNING_FLAGS -lm -fopenmp

# gcc -c  ziggurat/ziggurat.c -o ziggurat/ziggurat.o
# gcc main.c ziggurat/ziggurat.o -o main -lm # El lm es para enlazar bibliotecas como math.h

# Creamos las carpetas
mkdir output

# Corremos el programa
./main

echo "Finaliza el programa"
