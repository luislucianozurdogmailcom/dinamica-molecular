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

# Hacemos un bucle por cada T desde 4 a 3
for ((i=50; i>=30; i--)); do
    
    T=$(LC_NUMERIC=C printf "%.4f" $(echo "scale=4; $i/10" | bc))
    echo "Contador: T = $T"

    # Cremoas la carpeta
    mkdir "output/T_$T"

    # Escribimos el valor de T y T_anterior al archivo
    echo "$T" > input/T

    echo "Ejecutando código...."
    ./main
done

# Hacemos un bucle por cada T desde 3 a 2
for ((i=99; i>=1; i--)); do
    
    T=$(LC_NUMERIC=C printf "%.4f" $(echo "scale=4; ($i/100)+2" | bc))
    echo "Contador: T = $T"

    # Cremoas la carpeta
    mkdir "output/T_$T"

    # Escribimos el valor de T y T_anterior al archivo
    echo "$T" > input/T

    echo "Ejecutando código...."
    ./main
done

# Hacemos un bucle por cada T desde 2 a 0.1
for ((i=20; i>=1; i--)); do
    
    T=$(LC_NUMERIC=C printf "%.4f" $(echo "scale=4; $i/10" | bc))
    echo "Contador: T = $T"

    # Cremoas la carpeta
    mkdir "output/T_$T"

    # Escribimos el valor de T y T_anterior al archivo
    echo "$T" > input/T

    echo "Ejecutando código...."
    ./main
done

# Borramos archivo de ultima matriz
# rm input/ultimoDominio

#echo "Graficando...."
python3 graficador.py
