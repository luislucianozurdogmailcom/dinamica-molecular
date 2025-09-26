clear
echo "Compilando...."
gcc -c  ziggurat/ziggurat.c -o ziggurat/ziggurat.o
gcc problema_1.c ziggurat/ziggurat.o -o problema_1 -lm # El lm es para enlazar bibliotecas como math.h
echo "Ejecutando...."
./problema_1
echo "Ejecutamos el código en C..."

