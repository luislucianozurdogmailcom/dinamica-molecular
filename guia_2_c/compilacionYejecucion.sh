clear
echo "Compilando...."
gcc -c  ziggurat/ziggurat.c -o ziggurat/ziggurat.o
gcc problema_2.c ziggurat/ziggurat.o -o problema_2 -lm # El lm es para enlazar bibliotecas como math.h
echo "Ejecutando...."
./problema_2
echo "Ejecutamos el código en C..."

