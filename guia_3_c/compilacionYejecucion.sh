clear

echo "Borramos la carpeta anteriormente generada..."
rm -r output

echo "Compilando...."
gcc -c  ziggurat/ziggurat.c -o ziggurat/ziggurat.o
gcc main.c ziggurat/ziggurat.o -o main -lm # El lm es para enlazar bibliotecas como math.h

# Creamos las carpetas
mkdir output

# Hacemos un bucle por cada T
for ((i=1; i<=20; i++)); do
    
    T=$(LC_NUMERIC=C printf "%.4f" $(echo "scale=4; 5/$i" | bc))
    echo "Contador: T = $T"

    mkdir "output/T_$T"

    # Escribimos el valor de T al archivo
    echo "$T" > input/T

    echo "Ejecutando código...."
    ./main
done


#echo "Graficando...."
python3 graficador.py
