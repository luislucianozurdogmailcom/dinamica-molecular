clear
echo "Compilando...."
gcc -c  ziggurat/ziggurat.c -o ziggurat/ziggurat.o
gcc main.c ziggurat/ziggurat.o -o main -lm # El lm es para enlazar bibliotecas como math.h
echo "Ejecutando...."
./main
echo "Ejecutamos el código en C..."

