clear
echo "Compilando...."
gcc -c arrays_dinamicos.c -o arrays_dinamicos.o
gcc ejercicio-1.c arrays_dinamicos.o -o ejercicio-1
echo "Ejecutando...."
./ejercicio-1
echo "Ejecutamos el código python..."
python3 graficador.py
