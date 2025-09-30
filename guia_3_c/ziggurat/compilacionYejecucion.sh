clear
echo "Compilando...."
gcc -c  ziggurat.c -o ziggurat.o
gcc test.c ziggurat.o -o test -lm # El lm es para enlazar bibliotecas como math.h
echo "Ejecutando...."
./test
echo "Código finalizado"

